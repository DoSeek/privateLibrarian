//**********************************************************************//
//*
//*	˵������������xml�ı���ʽ�����ȡ��ƽ̨/�豸��Ϣ��*.xml�ļ�����֪
//*      ��·������Java�˽���*.xml�ļ����ɡ����ַ���������ȱ�㣺
//*      1. (ȱ)��ȡ��Ϣ�ٶ�������д.xml�ļ���ʱ�ࣻ
//*      2. (��)ʵ�����ף�C++��Java�˽������ƣ����ڵ���ά����
//*      3. (��)���ⶨλ���ף����׻������������C++����Java�ˣ���
//*      
//*      XH_NetSDKForJava.cpp���������ڴ�洢��ȡ��ƽ̨/�豸��Ϣ��Java
//*      �˰���Լ�����ڴ��ʽ��ȡ�����Ϣ�����ַ���������ȱ�㣺
//*      1. (ȱ)һ���������⣬C++��Java�˶�λ�������ѣ�
//*      2. (ȱ)������ƽ̨�����豸��Ŀ��������(����)���洢��Щ��Ϣ���ڴ����
//*         �����������⡣
//*      3. (��)��ȡ��Ϣ�ٶȿ졣
//*
//* ���ߣ�(C++)D.M.H   (Java)W.J
//* ���ڣ�2017.05.05
//* ���䣺�������н���loginAndGetEquip�������ã����ຯ����Java�˶���ӿڹ��࣬��������
//**********************************************************************//


#include "stdafx.h"

#include "com_xh_ts_common_domain_EquipJNI.h"
#include "XHNetSDKLayer.h"
#include "XHCommonErrDefine.h"
#include "./tinyXML/tinyxml.h"  //����tinyXML����Ҫȥ��Ԥ����ͷ�ļ�����

#include <stdio.h>
#include <string>
#include <ctime>

using namespace std;

#define XH_JAVA_PATH_LEN 256
char gJavaClassPath[XH_JAVA_PATH_LEN] = "com/xh/ts/common/domain/";

//xmlԪ������
enum ELEMTYPE {
	ROOTDOMAIN = 0,        //����
	CHILDDOMAIN = 1,       //�¼���
	ROOTGROUP = 2,         //����
	CHILDGROUP = 3,        //�¼���
	GENERALIZEDGROUP = 4,  //�����飺��"ROOTDOMAIN/CHILDDOMAIN/ROOTGROUP/CHILDGROUP"����Ϊ�������顱����GENERALIZEDGROUP
	CAMEARITEM = 5,        //��ͷ
	UNKNOWNITEM = 0xffff   //δ֪����
};  

struct RecordItem{
	string ID;			//�豸����
	string name;        //�豸����
	string parentID;    //���豸����
	ELEMTYPE eleType;   //xmlÿ����¼������
	string parentIP;    // eleType        parentIP                ��ע
						//  ��ͷ       ��ͷ�������豸IP/��IP
						//  ��         ���������IP/����IP
						//  �豸              ��              ���豸���뷽ʽʱʹ��
						//  �¼���          �����IP           ��ƽ̨��ʽ����ʱʹ��
						//  ����              ��                   ͬ��
	string IP;			//��eleType��������ͷ/�豸/��/�¼���/���� IP
	int cameraType;		//��ͷ���ͣ�ǹ��������������
};


XH_INT32 GetPaltformInfo(long loginHandle, const char*manufacturer, const char *sIP, TiXmlElement * root);
XH_INT32 GetDVRNVRInfo(long loginHandle, const char*manufacturer, const char *sIP, TiXmlElement * root);
bool WriteItem2XML(TiXmlElement *root, RecordItem &rdItem);
string CharactersAdapter(XH_CHAR* manufacturer, string &orgStr);
string ConvertGBK2UTF8(string& strGBK);
string jstring2Chars(JNIEnv  *env, jstring jstr);
jstring Chars2jstring(JNIEnv* env, char* str);

/**
* ��½����ȡ��ͷ������xml��
* @param ip				ƽ̨/�豸IP��ַ
* @param port			ƽ̨/�豸Port
* @param username		ƽ̨/�豸�û���
* @param password       ƽ̨/�豸����
* @param manufacturer	ƽ̨/�豸��������
* @param packagePath	Java Class�ļ����ڰ�·�� ��com/xh/ts/common/domain/��
* @param devType		�豸����: 1-ƽ̨�� 2-�豸
* @return
*/
JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_loginAndGetEquip(JNIEnv *inEnv, jclass inClass, jstring ip, jint port, jstring username, jstring password, jstring manufacturer, jstring packagePath, jint devType)
{
	//1. ��ȡ����·����Java Class·��
	//1.1 ��ȡ����·��
	string strRunPath = "";
	char aFilePath[MAX_PATH + 1] = { 0 };
	::GetModuleFileNameA(NULL, aFilePath, MAX_PATH);
	strRunPath.append(aFilePath);
	int nFindLast = strRunPath.find_last_of("\\");
	strRunPath = strRunPath.substr(0, nFindLast + 1);
	printf(" \n********* run path: %s.\n\n", strRunPath.c_str());

	//1.2 ��ȡJava Class·��
	const char *sJavaPath = inEnv->GetStringUTFChars(packagePath, 0);
	sprintf(gJavaClassPath, "%s", sJavaPath);

	//2. ׼��Java��(LoginDvrVO)����
	//2.1 ��λLoginDvrVO��
	char sClassLoginDvrVO[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sClassLoginDvrVO, "%s%s", gJavaClassPath, "LoginDvrVO");
	printf("Java_com_xh_ts_common_domain_EquipJNI is '%s'.\n", sClassLoginDvrVO);
	jclass loginDvrVOClass = (inEnv)->FindClass(sClassLoginDvrVO);

	//2.2 ��ȡ����ÿһ������������
	jmethodID mhConstruct = inEnv->GetMethodID(loginDvrVOClass, "<init>", "()V");		//���캯��
	jfieldID fdErrorCode = (inEnv)->GetFieldID(loginDvrVOClass, "loginErrorCode", "I"); //������
	jfieldID fdLoginHandle = (inEnv)->GetFieldID(loginDvrVOClass, "loginHandle", "I");  //��½���
	jfieldID fdFileName = (inEnv)->GetFieldID(loginDvrVOClass, "filename", "Ljava/lang/String;");  //����xml������
	jfieldID fdFilePath = (inEnv)->GetFieldID(loginDvrVOClass, "filepath", "Ljava/lang/String;");  //xml�ļ�����·��
	jobject  objLoginDvrVo = inEnv->NewObject(loginDvrVOClass, mhConstruct);					   //����LoginDvrVO����

	//3. ��¼ƽ̨/�豸
	//3.1 Java�ַ���תC�ַ���
	printf("3.1 Java�ַ���תC�ַ���\n");
	const char *sIP = inEnv->GetStringUTFChars(ip, 0);
	const char *sUserName = inEnv->GetStringUTFChars(username, 0);
	const int iPort = port;
	const char *sPwd = inEnv->GetStringUTFChars(password, 0);
	const char *sManufacturer = inEnv->GetStringUTFChars(manufacturer, 0);


	//3.2 ��¼(�ɳ��Ե�½3�Σ���֤��¼�ɹ���)
	printf("3.2 ��¼(�ɳ��Ե�½3�Σ���֤��¼�ɹ���)\n");
	string xmlFileName, strXMLFilePath, absXMLFilePath;
	RecordItem rdItem;
	TiXmlDocument tinyXMLDoc;
	long loginHandle = -1;
	int nRet = XH_SDK_ConnectEquipment(const_cast<XH_CHAR*>(sIP), (XH_INT32)iPort, const_cast<XH_CHAR*>(sUserName), const_cast<XH_CHAR*>(sPwd), const_cast<XH_CHAR*>(sManufacturer), loginHandle);
	if (nRet != XH_SUCCEED)
		goto clean_up;

	//4. ׼��xml�ļ�
	//4.1 �ƶ�xml�ļ��� ���� <IP>_YYYYmmddHHMMSS.xml
	printf("4.1 �ƶ�xml�ļ���\n");
	char tAll[255];
	std::memset(tAll, 0, sizeof(tAll));
	std::time_t tm = std::time(0);
	std::strftime(tAll, sizeof(tAll), "%Y%m%d%H%M%S", std::localtime(&tm));
	xmlFileName = string(sIP) + string("_") + string(tAll) + string(".xml");
	strXMLFilePath = strRunPath + string("xml");
	::CreateDirectoryA(strXMLFilePath.c_str(), NULL);
	inEnv->SetObjectField(objLoginDvrVo, fdFileName, Chars2jstring(inEnv, const_cast<char*>(xmlFileName.c_str())));  //����xml�ļ���
	inEnv->SetObjectField(objLoginDvrVo, fdFilePath, Chars2jstring(inEnv, const_cast<char*>(strXMLFilePath.c_str())));   //����xml�ļ�·��

	//4.2 ����xml�ļ�������ʼ��
	printf("4.2 ����xml�ļ�������ʼ��\n");
	TiXmlElement * root = nullptr;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "utf-8", "");
	tinyXMLDoc.LinkEndChild(decl);
	root = new TiXmlElement("Root");
	tinyXMLDoc.LinkEndChild(root);

	//5. ��ȡ�豸��Ϣ��д��xml DOM����	
	if (1 == devType){
		printf("5 �����ȡƽ̨��Ϣ��IP=%s\n", sIP);
		nRet = GetPaltformInfo(loginHandle, sManufacturer, sIP, root);
		printf("5 �����ȡƽ̨��Ϣ��nRet=%ld\n", nRet);
	}
	else if (2 == devType){
		printf("5 �����ȡ�豸��Ϣ��IP=%s\n", sIP);
		nRet = GetDVRNVRInfo(loginHandle, sManufacturer, sIP, root);
		printf("5 �����ȡ�豸��Ϣ��nRet=%ld\n", nRet);
	}
	else{
		printf("δ֪�豸����(����ƽ̨/�豸)�����账��.\n");
		nRet = XH_FAIL;
		goto clean_up;
	}

	if (XH_SUCCEED != nRet)
		goto clean_up;

	//6. ����xml�ļ�
	printf("6. ����xml�ļ�\n");
	absXMLFilePath = strXMLFilePath + '\\' + xmlFileName;
	tinyXMLDoc.SaveFile(absXMLFilePath.c_str());

	//7. �ǳ�ƽ̨/�豸
	printf("7. �ǳ�ƽ̨/�豸\n");
	nRet = XH_SDK_DisconnectEquipment(loginHandle);


	//8. �ƺ�
clean_up:
	//8.1 ���ô�����
	printf("8.1 ���ô�����, nRet = %ld\n", nRet);
	inEnv->SetIntField(objLoginDvrVo, fdErrorCode, nRet);

	printf("��������\n");
	return objLoginDvrVo;
}

//��ȡHWƽ̨�µ��豸��Ϣ������ƽ̨��Ҫ���Դ���
XH_INT32 GetPaltformInfo(long loginHandle, const char*manufacturer, const char *sIP, TiXmlElement * root)
{
	//5.1 ��ȡ��
	printf("5.1 ��ȡ��\n");

	RecordItem rdItem;
	XH_DOMAIN_INFO domainArray[100];
	memset(domainArray, 0, sizeof(XH_DOMAIN_INFO) * 100);
	int nDomainNum = 0;
	XH_INT32 nRet = XH_SDK_GetDevDomainList(loginHandle, domainArray, nDomainNum);
	if (XH_SUCCEED != nRet){
		return nRet;
	}

	//5.2 ѭ�����������������¼���
	printf("5.2 ѭ�����������������¼���\n");
	XH_DEV_GROUP_INFO *pGroupInfo = new XH_DEV_GROUP_INFO[5000];
	XH_CAMERA_INFO *pCameraInfo = new XH_CAMERA_INFO[10000];
	for (int k = 0; k < nDomainNum; ++k){
		//5.2.1 ����Ϣд��xml
		printf("5.2.1 ����Ϣд��xml\n");
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
		//5.2.2 ��ȡ��
		printf("5.2.2 ��ȡ��\n");
		nRet = XH_SDK_GetDevGroupListByGroupCode(loginHandle, domainArray[k].sCode, 5000, pGroupInfo, nGetGroupNum);
		if (XH_SUCCEED == nRet)
		{
			for (int i = 0; i < nGetGroupNum; i++)
			{
				XH_DEV_GROUP_INFO * pInfo = pGroupInfo + i;
				//5.2.2.1 ����Ϣд��xml�ļ�
				//printf("5.2.2.1 ����Ϣд��xml�ļ�\n");
				rdItem.ID = string(pInfo->sCode) + string("#") + string(domainArray[k].sCode);  //����ҵ����Ҫ����ID����Ϊ�� ��ID+���������ID
				rdItem.name = CharactersAdapter(const_cast<XH_CHAR*>(manufacturer), string(pInfo->sName));  //ConvertGBK2UTF8(string(pInfo->sName));
				//��ȡ���µ�ֱ����(�����������sParentCode=="0")�������������parentIDֱ��ָ��Ϊ����������ID
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
			//("����: %1:��ȡ����Ϣʧ��-%2");
			continue;
		}


		//5.2.3 ��ȡ��ͷ
		printf("5.2.3 ��ȡ��ͷ\n");
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
					//5.2.3.1 ��ͷ��Ϣд��xml�ļ�
					//printf("5.2.3.1 ��ͷ��Ϣд��xml�ļ�\n");
					rdItem.ID = string(pInfo->sCode);
					rdItem.name = CharactersAdapter(const_cast<XH_CHAR*>(manufacturer), string(pInfo->sName));  //ConvertGBK2UTF8(string(pInfo->sName));
					rdItem.parentID = string(pGroupInfo[j].sCode) + string("#") + string(domainArray[k].sCode);  //��ͷ��������ID�����ǣ���ID+���������ID 
					rdItem.eleType = ELEMTYPE::CAMEARITEM;
					rdItem.parentIP = string("");
					rdItem.IP = string("");
					rdItem.cameraType = pInfo->nType;
					WriteItem2XML(root, rdItem);
				}
			}
			else
			{
				//("����: %1:��ȡ��ͷ��Ϣʧ��-%2");
				continue;
			}
		}
	}//outer for loop

	//5.3 �ͷ��ڴ�
	printf("5.3 �ͷ��ڴ�\n");
	delete[] pGroupInfo;
	delete[] pCameraInfo;

	return nRet;
}


//��ȡDVR/NVR�豸����Ϣ
XH_INT32 GetDVRNVRInfo(long loginHandle, const char*manufacturer, const char *sIP, TiXmlElement *root)
{
	printf("5.1 ��ȡ�豸��Ϣ\n");
	XH_INT32 nRet = XH_SUCCEED;
	XH_INT32 outRealDevNum = 0;
	const XH_INT32 inDevNum = 200;
	XH_DEV_INFO outDevInfo[inDevNum];
	memset(outDevInfo, 0, sizeof(XH_DEV_INFO)*inDevNum);
	XH_UINT32 indexFrom = 1;
	RecordItem rdItem;

	//5.1 ��ȡ�豸��Ϣ
	printf("5.1 ����XH_SDK_GetDeviceList() ��ʼ\n");
	nRet = XH_SDK_GetDeviceList(loginHandle, XH_DEV_CLASS_CAMERA, outDevInfo, indexFrom, inDevNum, outRealDevNum);
	if (XH_SUCCEED != nRet)
	{
		printf("5.1 ����XH_SDK_GetDeviceList() ʧ�ܣ�errNum=%ld\n", nRet);
		return nRet;
	}
	printf("5.1 ����XH_SDK_GetDeviceList() �ɹ�\n", nRet);

	//5.2 д������Ϣ��xml�ļ�
	//rdItem.ID = string("");		//���豸ID
	//rdItem.name = string("");		//ConvertGBK2UTF8(string(sManufacturer));  //�Ƿ���Ҫָ���豸���ƣ�
	//rdItem.parentID = string(""); //�޸��豸
	//rdItem.eleType = ELEMTYPE::GENERALIZEDGROUP;
	//rdItem.parentIP = string("");
	//rdItem.IP = string(sIP);
	//rdItem.cameraType = 0xff;
	//WriteItem2XML(root, rdItem);


	//5.3 ��ͷ��Ϣд��xml�ļ�
	XH_DEV_INFO * pInfo = outDevInfo;
	for (int i = 0; i < outRealDevNum; ++i)
	{	
		printf("5.2 ��ͷ��Ϣд��xml�ļ�\n");
		rdItem.ID = (pInfo + i)->cDevCode;				//��ͷ����
		rdItem.name = CharactersAdapter(const_cast<XH_CHAR*>(manufacturer), string((pInfo + i)->cDevName));			//��ͷ����
					  (pInfo + i)->cDevVendor;			//��������
		rdItem.eleType = ELEMTYPE::CAMEARITEM;          //xml��¼����
		rdItem.parentID = string("");					//��/���豸����
		rdItem.parentIP = string(sIP);					//���豸IP
		rdItem.IP = string((pInfo + i)->cCameraIp);		//��ͷIP
		rdItem.cameraType = (pInfo + i)->nDevType;		//��ͷ���ͣ�ǹ�������
		WriteItem2XML(root, rdItem);
	}

	return nRet;
}

//д��һ����¼��xml DOM������
bool WriteItem2XML(TiXmlElement *root, RecordItem &rdItem)
{
	// <Item ID="3j35c83" Name="����" ParentID="ce90f0" Type="0" ParentIP="190.114.130.4" IP="190.114.113.51" CameraType="1"> </Item>
	TiXmlElement *item = new TiXmlElement("Item");  //tinyXML�Լ��ͷŽڵ����ռ�õ��ڴ�
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

//��ͬ����ʹ���˲�ͬ���ַ����룬��Ҫ���䵽UTF-8
string CharactersAdapter(XH_CHAR* manufacturer, string &orgStr)
{
	//Ϊ�˼򵥴����˴���Ӳ���벻ʹ�����������ļ�
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

// GBK����תUTF-8����
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

//��jstringת��ΪC++ string
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

//��windows����ת����jstring����
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

