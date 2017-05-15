//**********************************************************************//
//*
//*	说明：本方案以xml文本格式保存获取的平台/设备信息到*.xml文件，告知
//*      其路径，让Java端解析*.xml文件即可。这种方案存在优缺点：
//*      1. (缺)获取信息速度慢，读写.xml文件耗时多；
//*      2. (优)实现容易，C++与Java端解耦合设计，易于单独维护；
//*      3. (优)问题定位容易，容易划分问题出处（C++还是Java端）。
//*      
//*      XH_NetSDKForJava.cpp方案是以内存存储获取的平台/设备信息，Java
//*      端按照约定的内存格式读取相关信息。这种方案存在优缺点：
//*      1. (缺)一旦出现问题，C++与Java端定位问题困难；
//*      2. (缺)当遇到平台下是设备数目量级过大(几万)，存储这些信息的内存过大，
//*         容易引发问题。
//*      3. (优)获取信息速度快。
//*
//* 作者：(C++)D.M.H   (Java)W.J
//* 日期：2017.05.05
//* 补充：本方案中仅此loginAndGetEquip函数有用，其余函数因Java端定义接口过多，纯属多余
//**********************************************************************//


#include "stdafx.h"

#include "com_xh_ts_common_domain_EquipJNI.h"
#include "XHNetSDKLayer.h"
#include "XHCommonErrDefine.h"
#include "./tinyXML/tinyxml.h"  //包含tinyXML后，需要去掉预编译头文件功能

#include <stdio.h>
#include <string>
#include <ctime>

using namespace std;

#define XH_JAVA_PATH_LEN 256
char gJavaClassPath[XH_JAVA_PATH_LEN] = "com/xh/ts/common/domain/";

//xml元素类型
enum ELEMTYPE {
	ROOTDOMAIN = 0,        //根域
	CHILDDOMAIN = 1,       //下级域
	ROOTGROUP = 2,         //根组
	CHILDGROUP = 3,        //下级组
	GENERALIZEDGROUP = 4,  //广义组：将"ROOTDOMAIN/CHILDDOMAIN/ROOTGROUP/CHILDGROUP"定义为“广义组”——GENERALIZEDGROUP
	CAMEARITEM = 5,        //镜头
	UNKNOWNITEM = 0xffff   //未知类型
};  

struct RecordItem{
	string ID;			//设备编码
	string name;        //设备名称
	string parentID;    //父设备编码
	ELEMTYPE eleType;   //xml每条记录的类型
	string parentIP;    // eleType        parentIP                备注
						//  镜头       镜头归属的设备IP/组IP
						//  组         组归属的域IP/父组IP
						//  设备              空              以设备接入方式时使用
						//  下级域          父域的IP           以平台方式接入时使用
						//  根域              空                   同上
	string IP;			//由eleType决定：镜头/设备/组/下级域/根域 IP
	int cameraType;		//镜头类型：枪机、球机、半球机
};


XH_INT32 GetPaltformInfo(long loginHandle, const char*manufacturer, const char *sIP, TiXmlElement * root);
XH_INT32 GetDVRNVRInfo(long loginHandle, const char*manufacturer, const char *sIP, TiXmlElement * root);
bool WriteItem2XML(TiXmlElement *root, RecordItem &rdItem);
string CharactersAdapter(XH_CHAR* manufacturer, string &orgStr);
string ConvertGBK2UTF8(string& strGBK);
string jstring2Chars(JNIEnv  *env, jstring jstr);
jstring Chars2jstring(JNIEnv* env, char* str);

/**
* 登陆并获取镜头（生成xml）
* @param ip				平台/设备IP地址
* @param port			平台/设备Port
* @param username		平台/设备用户名
* @param password       平台/设备密码
* @param manufacturer	平台/设备厂商名称
* @param packagePath	Java Class文件所在包路径 （com/xh/ts/common/domain/）
* @param devType		设备类型: 1-平台， 2-设备
* @return
*/
JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_loginAndGetEquip(JNIEnv *inEnv, jclass inClass, jstring ip, jint port, jstring username, jstring password, jstring manufacturer, jstring packagePath, jint devType)
{
	//1. 获取运行路径和Java Class路径
	//1.1 获取运行路径
	string strRunPath = "";
	char aFilePath[MAX_PATH + 1] = { 0 };
	::GetModuleFileNameA(NULL, aFilePath, MAX_PATH);
	strRunPath.append(aFilePath);
	int nFindLast = strRunPath.find_last_of("\\");
	strRunPath = strRunPath.substr(0, nFindLast + 1);
	printf(" \n********* run path: %s.\n\n", strRunPath.c_str());

	//1.2 获取Java Class路径
	const char *sJavaPath = inEnv->GetStringUTFChars(packagePath, 0);
	sprintf(gJavaClassPath, "%s", sJavaPath);

	//2. 准备Java类(LoginDvrVO)对象
	//2.1 定位LoginDvrVO类
	char sClassLoginDvrVO[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sClassLoginDvrVO, "%s%s", gJavaClassPath, "LoginDvrVO");
	printf("Java_com_xh_ts_common_domain_EquipJNI is '%s'.\n", sClassLoginDvrVO);
	jclass loginDvrVOClass = (inEnv)->FindClass(sClassLoginDvrVO);

	//2.2 获取类中每一个方法和属性
	jmethodID mhConstruct = inEnv->GetMethodID(loginDvrVOClass, "<init>", "()V");		//构造函数
	jfieldID fdErrorCode = (inEnv)->GetFieldID(loginDvrVOClass, "loginErrorCode", "I"); //错误码
	jfieldID fdLoginHandle = (inEnv)->GetFieldID(loginDvrVOClass, "loginHandle", "I");  //登陆句柄
	jfieldID fdFileName = (inEnv)->GetFieldID(loginDvrVOClass, "filename", "Ljava/lang/String;");  //生成xml的名称
	jfieldID fdFilePath = (inEnv)->GetFieldID(loginDvrVOClass, "filepath", "Ljava/lang/String;");  //xml文件保存路径
	jobject  objLoginDvrVo = inEnv->NewObject(loginDvrVOClass, mhConstruct);					   //生成LoginDvrVO对象

	//3. 登录平台/设备
	//3.1 Java字符串转C字符串
	printf("3.1 Java字符串转C字符串\n");
	const char *sIP = inEnv->GetStringUTFChars(ip, 0);
	const char *sUserName = inEnv->GetStringUTFChars(username, 0);
	const int iPort = port;
	const char *sPwd = inEnv->GetStringUTFChars(password, 0);
	const char *sManufacturer = inEnv->GetStringUTFChars(manufacturer, 0);


	//3.2 登录(可尝试登陆3次，保证登录成功率)
	printf("3.2 登录(可尝试登陆3次，保证登录成功率)\n");
	string xmlFileName, strXMLFilePath, absXMLFilePath;
	RecordItem rdItem;
	TiXmlDocument tinyXMLDoc;
	long loginHandle = -1;
	int nRet = XH_SDK_ConnectEquipment(const_cast<XH_CHAR*>(sIP), (XH_INT32)iPort, const_cast<XH_CHAR*>(sUserName), const_cast<XH_CHAR*>(sPwd), const_cast<XH_CHAR*>(sManufacturer), loginHandle);
	if (nRet != XH_SUCCEED)
		goto clean_up;

	//4. 准备xml文件
	//4.1 制定xml文件名 —— <IP>_YYYYmmddHHMMSS.xml
	printf("4.1 制定xml文件名\n");
	char tAll[255];
	std::memset(tAll, 0, sizeof(tAll));
	std::time_t tm = std::time(0);
	std::strftime(tAll, sizeof(tAll), "%Y%m%d%H%M%S", std::localtime(&tm));
	xmlFileName = string(sIP) + string("_") + string(tAll) + string(".xml");
	strXMLFilePath = strRunPath + string("xml");
	::CreateDirectoryA(strXMLFilePath.c_str(), NULL);
	inEnv->SetObjectField(objLoginDvrVo, fdFileName, Chars2jstring(inEnv, const_cast<char*>(xmlFileName.c_str())));  //设置xml文件名
	inEnv->SetObjectField(objLoginDvrVo, fdFilePath, Chars2jstring(inEnv, const_cast<char*>(strXMLFilePath.c_str())));   //设置xml文件路径

	//4.2 创建xml文件并做初始化
	printf("4.2 创建xml文件并做初始化\n");
	TiXmlElement * root = nullptr;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "utf-8", "");
	tinyXMLDoc.LinkEndChild(decl);
	root = new TiXmlElement("Root");
	tinyXMLDoc.LinkEndChild(root);

	//5. 获取设备信息并写入xml DOM对象	
	if (1 == devType){
		printf("5 请求获取平台信息：IP=%s\n", sIP);
		nRet = GetPaltformInfo(loginHandle, sManufacturer, sIP, root);
		printf("5 请求获取平台信息：nRet=%ld\n", nRet);
	}
	else if (2 == devType){
		printf("5 请求获取设备信息：IP=%s\n", sIP);
		nRet = GetDVRNVRInfo(loginHandle, sManufacturer, sIP, root);
		printf("5 请求获取设备信息：nRet=%ld\n", nRet);
	}
	else{
		printf("未知设备类型(不是平台/设备)，不予处理.\n");
		nRet = XH_FAIL;
		goto clean_up;
	}

	if (XH_SUCCEED != nRet)
		goto clean_up;

	//6. 保存xml文件
	printf("6. 保存xml文件\n");
	absXMLFilePath = strXMLFilePath + '\\' + xmlFileName;
	tinyXMLDoc.SaveFile(absXMLFilePath.c_str());

	//7. 登出平台/设备
	printf("7. 登出平台/设备\n");
	nRet = XH_SDK_DisconnectEquipment(loginHandle);


	//8. 善后
clean_up:
	//8.1 设置错误码
	printf("8.1 设置错误码, nRet = %ld\n", nRet);
	inEnv->SetIntField(objLoginDvrVo, fdErrorCode, nRet);

	printf("函数返回\n");
	return objLoginDvrVo;
}

//获取HW平台下的设备信息，其它平台需要各自处理
XH_INT32 GetPaltformInfo(long loginHandle, const char*manufacturer, const char *sIP, TiXmlElement * root)
{
	//5.1 获取域
	printf("5.1 获取域\n");

	RecordItem rdItem;
	XH_DOMAIN_INFO domainArray[100];
	memset(domainArray, 0, sizeof(XH_DOMAIN_INFO) * 100);
	int nDomainNum = 0;
	XH_INT32 nRet = XH_SDK_GetDevDomainList(loginHandle, domainArray, nDomainNum);
	if (XH_SUCCEED != nRet){
		return nRet;
	}

	//5.2 循环遍历根域下所有下级域
	printf("5.2 循环遍历根域下所有下级域\n");
	XH_DEV_GROUP_INFO *pGroupInfo = new XH_DEV_GROUP_INFO[5000];
	XH_CAMERA_INFO *pCameraInfo = new XH_CAMERA_INFO[10000];
	for (int k = 0; k < nDomainNum; ++k){
		//5.2.1 域信息写入xml
		printf("5.2.1 域信息写入xml\n");
		rdItem.ID = string(domainArray[k].sCode);
		rdItem.name = CharactersAdapter(const_cast<XH_CHAR*>(manufacturer), string(domainArray[k].sName));  //ConvertGBK2UTF8(string(domainArray[k].sName));
		rdItem.parentID = string(domainArray[k].sSuperDomain);
		rdItem.eleType = ELEMTYPE::GENERALIZEDGROUP;
		rdItem.parentIP = string(sIP);
		rdItem.IP = string(domainArray[k].sIP);
		rdItem.cameraType = 0xff;
		WriteItem2XML(root, rdItem);

		int nGetGroupNum = 0;
		memset(pGroupInfo, 0, 5000 * sizeof(XH_DEV_GROUP_INFO));
		//5.2.2 获取组
		printf("5.2.2 获取组\n");
		nRet = XH_SDK_GetDevGroupListByGroupCode(loginHandle, domainArray[k].sCode, 5000, pGroupInfo, nGetGroupNum);
		if (XH_SUCCEED == nRet)
		{
			for (int i = 0; i < nGetGroupNum; i++)
			{
				XH_DEV_GROUP_INFO * pInfo = pGroupInfo + i;
				//5.2.2.1 组信息写入xml文件
				//printf("5.2.2.1 组信息写入xml文件\n");
				rdItem.ID = string(pInfo->sCode) + string("#") + string(domainArray[k].sCode);  //根据业务需要将组ID设置为： 组ID+组归属的域ID
				rdItem.name = CharactersAdapter(const_cast<XH_CHAR*>(manufacturer), string(pInfo->sName));  //ConvertGBK2UTF8(string(pInfo->sName));
				//提取域下的直接组(此种类型组的sParentCode=="0")，这种类型组的parentID直接指定为其所属的域ID
				if (!strcmp("0", pInfo->sParentCode))
				{
					rdItem.parentID = string(domainArray[k].sCode);
				}
				else
				{
					rdItem.parentID = string(pInfo->sParentCode) + "#" + string(domainArray[k].sCode);
				}
				rdItem.eleType = ELEMTYPE::GENERALIZEDGROUP;
				rdItem.parentIP = string(domainArray[k].sIP);
				rdItem.IP = string("");
				rdItem.cameraType = 0xff;
				WriteItem2XML(root, rdItem);
			}
		}
		else
		{
			//("错误: %1:获取组信息失败-%2");
			continue;
		}


		//5.2.3 获取镜头
		printf("5.2.3 获取镜头\n");
		for (int j = 0; j < nGetGroupNum; ++j){
			memset(pCameraInfo, 0, 10000 * sizeof(XH_CAMERA_INFO));
			int index = 1;
			int nGetCameraNum = 0;
			nRet = XH_SDK_GetCameraListByGroupCode(loginHandle, pGroupInfo[j].sCode, index, 10000, pCameraInfo, nGetCameraNum);

			if (nRet == XH_SUCCEED)
			{
				for (int i = 0; i < nGetCameraNum; ++i)
				{
					XH_CAMERA_INFO * pInfo = pCameraInfo + i;
					//5.2.3.1 镜头信息写入xml文件
					//printf("5.2.3.1 镜头信息写入xml文件\n");
					rdItem.ID = string(pInfo->sCode);
					rdItem.name = CharactersAdapter(const_cast<XH_CHAR*>(manufacturer), string(pInfo->sName));  //ConvertGBK2UTF8(string(pInfo->sName));
					rdItem.parentID = string(pGroupInfo[j].sCode) + string("#") + string(domainArray[k].sCode);  //镜头归属的组ID必须是：组ID+组归属的域ID 
					rdItem.eleType = ELEMTYPE::CAMEARITEM;
					rdItem.parentIP = string("");
					rdItem.IP = string("");
					rdItem.cameraType = pInfo->nType;
					WriteItem2XML(root, rdItem);
				}
			}
			else
			{
				//("错误: %1:获取镜头信息失败-%2");
				continue;
			}
		}
	}//outer for loop

	//5.3 释放内存
	printf("5.3 释放内存\n");
	delete[] pGroupInfo;
	delete[] pCameraInfo;

	return nRet;
}


//获取DVR/NVR设备的信息
XH_INT32 GetDVRNVRInfo(long loginHandle, const char*manufacturer, const char *sIP, TiXmlElement *root)
{
	printf("5.1 获取设备信息\n");
	XH_INT32 nRet = XH_SUCCEED;
	XH_INT32 outRealDevNum = 0;
	const XH_INT32 inDevNum = 200;
	XH_DEV_INFO outDevInfo[inDevNum];
	memset(outDevInfo, 0, sizeof(XH_DEV_INFO)*inDevNum);
	XH_UINT32 indexFrom = 1;
	RecordItem rdItem;

	//5.1 获取设备信息
	printf("5.1 调用XH_SDK_GetDeviceList() 开始\n");
	nRet = XH_SDK_GetDeviceList(loginHandle, XH_DEV_CLASS_CAMERA, outDevInfo, indexFrom, inDevNum, outRealDevNum);
	if (XH_SUCCEED != nRet)
	{
		printf("5.1 调用XH_SDK_GetDeviceList() 失败，errNum=%ld\n", nRet);
		return nRet;
	}
	printf("5.1 调用XH_SDK_GetDeviceList() 成功\n", nRet);

	//5.2 写入组信息到xml文件
	//rdItem.ID = string("");		//无设备ID
	//rdItem.name = string("");		//ConvertGBK2UTF8(string(sManufacturer));  //是否需要指定设备名称？
	//rdItem.parentID = string(""); //无父设备
	//rdItem.eleType = ELEMTYPE::GENERALIZEDGROUP;
	//rdItem.parentIP = string("");
	//rdItem.IP = string(sIP);
	//rdItem.cameraType = 0xff;
	//WriteItem2XML(root, rdItem);


	//5.3 镜头信息写入xml文件
	XH_DEV_INFO * pInfo = outDevInfo;
	for (int i = 0; i < outRealDevNum; ++i)
	{	
		printf("5.2 镜头信息写入xml文件\n");
		rdItem.ID = (pInfo + i)->cDevCode;				//镜头编码
		rdItem.name = CharactersAdapter(const_cast<XH_CHAR*>(manufacturer), string((pInfo + i)->cDevName));			//镜头名称
					  (pInfo + i)->cDevVendor;			//厂家名称
		rdItem.eleType = ELEMTYPE::CAMEARITEM;          //xml记录类型
		rdItem.parentID = string("");					//组/父设备编码
		rdItem.parentIP = string(sIP);					//父设备IP
		rdItem.IP = string((pInfo + i)->cCameraIp);		//镜头IP
		rdItem.cameraType = (pInfo + i)->nDevType;		//镜头类型：枪机、球机
		WriteItem2XML(root, rdItem);
	}

	return nRet;
}

//写入一条记录到xml DOM对象中
bool WriteItem2XML(TiXmlElement *root, RecordItem &rdItem)
{
	// <Item ID="3j35c83" Name="龙岗" ParentID="ce90f0" Type="0" ParentIP="190.114.130.4" IP="190.114.113.51" CameraType="1"> </Item>
	TiXmlElement *item = new TiXmlElement("Item");  //tinyXML自己释放节点对象占用的内存
	item->SetAttribute("ID", rdItem.ID.c_str());
	item->SetAttribute("Name", rdItem.name.c_str());
	item->SetAttribute("ParentID", rdItem.parentID.c_str());
	item->SetAttribute("Type", rdItem.eleType);
	item->SetAttribute("ParentIP", rdItem.parentIP.c_str());
	item->SetAttribute("IP", rdItem.IP.c_str());
	item->SetAttribute("CameraType", rdItem.cameraType);
	root->LinkEndChild(item);

	return true;
}

//不同厂家使用了不同的字符编码，需要适配到UTF-8
string CharactersAdapter(XH_CHAR* manufacturer, string &orgStr)
{
	//为了简单处理，此处用硬编码不使用灵活的配置文件
	if (!strcmp("HW", manufacturer))
	{
		return ConvertGBK2UTF8(orgStr);
	}
	else if (!strcmp("DH", manufacturer))
	{
		return ConvertGBK2UTF8(orgStr);
	}
	else if (!strcmp("HK", manufacturer))
	{
		return orgStr;
	}
	else if (!strcmp("HKP", manufacturer))
	{
		return orgStr;
	}
	else if (!strcmp("HS", manufacturer))
	{
		return orgStr;
	}
	else if (!strcmp("AIWS", manufacturer))
	{
		return orgStr;
	}
	else if (!strcmp("XYP", manufacturer))
	{
		return orgStr;
	}
	else
	{
		return orgStr;
	}

	return orgStr;
}

// GBK编码转UTF-8编码
string ConvertGBK2UTF8(string& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	WCHAR *wszUtf8 = new WCHAR[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL, NULL);

	string strUTF8(szUtf8);
	delete[] szUtf8;
	delete[] wszUtf8;

	return strUTF8;
}

//将jstring转换为C++ string
string jstring2Chars(JNIEnv  *env, jstring jstr)
{
	int length = (env)->GetStringLength(jstr);
	const jchar* jcstr = (env)->GetStringChars(jstr, 0);
	char* rtn = (char*)malloc(length * 2 + 1);
	int size = 0;
	size = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)jcstr, length, rtn, (length * 2 + 1), NULL, NULL);
	if (size <= 0)
		return NULL;
	(env)->ReleaseStringChars(jstr, jcstr);
	rtn[size] = 0;
	string str(rtn);
	free(rtn);

	return str;
}

//将windows类型转换成jstring类型
jstring Chars2jstring(JNIEnv* env, char* str)
{
	jstring rtn = 0;
	int slen = strlen(str);
	unsigned short * buffer = 0;
	if (slen == 0)
		rtn = (env)->NewStringUTF(str);
	else
	{
		int length = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str, slen, NULL, 0);
		buffer = (unsigned short *)malloc(length * 2 + 1);
		if (MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str, slen, (LPWSTR)buffer, length) >0)
			rtn = (env)->NewString((jchar*)buffer, length);
	}
	if (buffer)
		free(buffer);
	return rtn;
}


JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_loginEquip(JNIEnv *, jclass, jstring, jint, jstring, jstring, jstring, jstring)
{
	return NULL;
}


JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDeviceList(JNIEnv *, jclass, jint, jint, jint, jint)
{
	return NULL;
}


JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDevGroupsByCode(JNIEnv *, jclass, jint, jstring, jint)
{
	return NULL;
}


JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDevicesByCode(JNIEnv *, jclass, jint, jstring, jint, jint)
{
	return NULL;
}


JNIEXPORT jint JNICALL Java_com_xh_ts_common_domain_EquipJNI_logoutEquip(JNIEnv *, jclass, jint)
{
	return NULL;
}


JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDomain(JNIEnv *, jclass, jint)
{
	return NULL;
}


JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDomainEx(JNIEnv *, jclass, jint)
{
	return NULL;
}

