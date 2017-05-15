// XH_NetSDKForJava.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "com_xh_xEye_jni_EquipJNI.h"
#include "XHNetSDKLayer.h"
#include "XHCommonErrDefine.h"

#include <stdio.h>
#include <string>

using namespace std;

#define XH_JAVA_PATH_LEN 256
char gJavaPath[XH_JAVA_PATH_LEN] = "com/xh/xEye/domain/";

char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}


char* jstringToChar(JNIEnv  *env, jstring jstr)
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
	return rtn;
}
//将windows类型转换成jstring类型
jstring CharTojstring(JNIEnv* env, char* str)
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

JNIEXPORT jobject JNICALL Java_com_xh_xEye_jni_EquipJNI_loginEquip(JNIEnv *inEnv, jclass inClass, jstring inIp, jint inPort, jstring inUser, jstring inPass, jstring inSt, jstring inJavaPath)
{
	string sReturn = "";

	//获得程序路径
	char aFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, aFilePath, MAX_PATH);

	sReturn.append(aFilePath);
	int nFindLast = sReturn.find_last_of("\\");
	sReturn = sReturn.substr(0, nFindLast + 1);

	printf(" \n********* run path: %s.\n\n", sReturn.c_str());

	const char *sUIp = inEnv->GetStringUTFChars(inIp, 0);
	//char *sGIp = U2G(sUIp);

	const char *sUUser = inEnv->GetStringUTFChars(inUser, 0);
	//char *sGUser = U2G(sUUser);

	const char *sUPass = inEnv->GetStringUTFChars(inPass, 0);
	//char *sGPass = U2G(sUPass);

	const char *sUSt = inEnv->GetStringUTFChars(inSt, 0);
	//char *sGSt = U2G(sUSt);

	const char *sJavaPath = inEnv->GetStringUTFChars(inJavaPath, 0);
	if (strlen(sJavaPath) != 0)
	{
		sprintf(gJavaPath, "%s", sJavaPath);
	}

	printf("Java_com_xh_vms_jni_EquipJNI_loginEquip: gJavaPath is %s.\n", gJavaPath);

	int nPort = inPort;
	
	//FILE * fp = fopen("C:\\HKP_GetCHannelInfo.txt", "a+");
	//fprintf(fp, "ip : %s\n", sUIp);
	//fprintf(fp, "port: %d\n", inPort);
	//fprintf(fp, "user: %s\n", sUUser);
	//fprintf(fp, "pass: %s\n", sUPass);
	//fprintf(fp, "st: %s\n", sUSt);
	//fclose(fp);

	long lConnectHandle = -1;
	printf("login(%s-%s@%s:%d). st is %s.\n", sUUser, sUPass, sUIp, nPort, sUSt);
	int nRet = XH_SDK_ConnectEquipment((char *)sUIp, nPort, (char *)sUUser, (char *)sUPass, (char *)sUSt, lConnectHandle);
	if (nRet != XH_SUCCEED)
	{
		printf("login fail(%s-%s@%s:%d). st is %s.  err is %d.\n", sUUser, sUPass, sUIp, nPort, sUSt, nRet);
	}

	char sClassLoginDvrVO[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sClassLoginDvrVO, "%s%s", gJavaPath, "LoginDvrVO");
	printf("Java_com_xh_vms_jni_EquipJNI_loginEquip is '%s'.\n", sClassLoginDvrVO);
	jclass objectClass = (inEnv)->FindClass(sClassLoginDvrVO);
	//jclass objectClass = (inEnv)->FindClass("com/xh/vms/domain/LoginDvrVO");

	//获取类中每一个变量的定义
	//名字
	jmethodID m_mid = inEnv->GetMethodID(objectClass, "<init>", "()V");
	jfieldID nErr = (inEnv)->GetFieldID(objectClass, "loginErrorCode", "I");
	//序列号
	jfieldID nHandle = (inEnv)->GetFieldID(objectClass, "loginHandle", "I");

	jobject  oResultObj = inEnv->NewObject(objectClass, m_mid);

	//inEnv->SetObjectField(m_obj, nErr, inEnv->NewStringUTF("AP1"));]
	inEnv->SetIntField(oResultObj, nErr, nRet);
	inEnv->SetIntField(oResultObj, nHandle, lConnectHandle);
	
	
	inEnv->ReleaseStringUTFChars(inIp, sUIp);
	inEnv->ReleaseStringUTFChars(inUser, sUUser);
	inEnv->ReleaseStringUTFChars(inPass, sUPass);
	inEnv->ReleaseStringUTFChars(inSt, sUSt);
	inEnv->ReleaseStringUTFChars(inJavaPath, sJavaPath);

	printf("Java_com_xh_vms_jni_EquipJNI_loginEquip Complete.\n");
	return oResultObj;
	//给每一个实例的变量付值
	//(inEnv)->SetObjectField(inClass, str, (inEnv)->NewStringUTF("my name is D:"));
	/*(inEnv)->SetIntField(inClass, nErr, 6);
	(inEnv)->SetIntField(inClass, nHandle, 10);
*/


	/*jclass    m_cls = env->FindClass("com/ldq/ScanResult");

	jmethodID m_mid = env->GetMethodID(m_cls, "<init>", "()V");

	jfieldID  m_fid_1 = env->GetFieldID(m_cls, "ssid", "Ljava/lang/String;");
	jfieldID  m_fid_2 = env->GetFieldID(m_cls, "mac", "Ljava/lang/String;");
	jfieldID  m_fid_3 = env->GetFieldID(m_cls, "level", "I");

	jobject   m_obj = env->NewObject(m_cls, m_mid);

	env->SetObjectField(m_obj, m_fid_1, env->NewStringUTF("AP1"));
	env->SetObjectField(m_obj, m_fid_2, env->NewStringUTF("00-11-22-33-44-55"));
	env->SetIntField(m_obj, m_fid_3, -50);
	return m_obj;  返回自定义对象*/

}

/*
* Class:     com_xh_vms_jni_EquipJNI
* Method:    getDeviceList
* Signature: (IIII)Lcom/xh/vms/domain/ArrayOfCI;
*/
JNIEXPORT jobject JNICALL Java_com_xh_xEye_jni_EquipJNI_getDeviceList(JNIEnv *inEnv, jclass inClass, jint inHandler, jint inDeviceType, jint inFromIndex, jint inDeviceNum)
{
	long lLoginHandle = inHandler;
	int nDevType = inDeviceType;
	int nFromIndex = inFromIndex;
	int nDevNum = inDeviceNum;
	if (nDevNum <= 0)
	{
		nDevNum = 30;
	}

	//printf("getDeviceList: handle=%ld, from=%d, num=%d.\n", inHandler, inFromIndex, inDeviceNum);

	//1-实例ChannelInfoVO 及 成员变量
	char sClassChannelInfoVO[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sClassChannelInfoVO, "%s%s", gJavaPath, "ChannelInfoVO");
	//printf("Java_com_xh_vms_jni_EquipJNI_getDeviceList: sClassChannelInfoVO is '%s'.\n", sClassChannelInfoVO);
	//jclass jfChannelInfoVO = inEnv->FindClass("com/xh/vms/domain/ChannelInfoVO");
	jclass jfChannelInfoVO = inEnv->FindClass(sClassChannelInfoVO);
	//实例变量
	jfieldID jnStatus = inEnv->GetFieldID(jfChannelInfoVO, "status", "I");
	jfieldID jfCameraType = inEnv->GetFieldID(jfChannelInfoVO, "channelType", "I");
	jfieldID jfCameraName = inEnv->GetFieldID(jfChannelInfoVO, "cDevName", "Ljava/lang/String;");
	jfieldID jfCameraCode = inEnv->GetFieldID(jfChannelInfoVO, "cDevCode", "Ljava/lang/String;");
	jfieldID jfCameraVendor = inEnv->GetFieldID(jfChannelInfoVO, "cDevVendor", "Ljava/lang/String;");
	jfieldID jfCameraGroup = inEnv->GetFieldID(jfChannelInfoVO, "groupCode", "Ljava/lang/String;");
	jfieldID jfCameraParent = inEnv->GetFieldID(jfChannelInfoVO, "parentDevCode", "Ljava/lang/String;");


	//2-实例ArrayOfCI 及 成员变量
	char sClassArrayOfCI[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sClassArrayOfCI, "%s%s", gJavaPath, "ArrayOfCI");
	jclass joArrayOfCI = inEnv->FindClass(sClassArrayOfCI);
	//实例变量
	jfieldID jfErr = (inEnv)->GetFieldID(joArrayOfCI, "errorCode", "I");
	jfieldID jfGetNum = (inEnv)->GetFieldID(joArrayOfCI, "realDevNum", "I");
	char sObjectChannelInfoVO[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sObjectChannelInfoVO, "[L%s;", sClassChannelInfoVO);
	//printf("Java_com_xh_vms_jni_EquipJNI_getDeviceList: sObjectChannelInfoVO is '%s'.\n", sObjectChannelInfoVO);
	//jfieldID jfChannel = inEnv->GetFieldID(joArrayOfCI, "cInfoVO", "[Lcom/xh/vms/domain/ChannelInfoVO;");
	jfieldID jfChannel = inEnv->GetFieldID(joArrayOfCI, "cInfoVO", sObjectChannelInfoVO);
	if (jfChannel == NULL)
	{
		printf("XH_NetSDKForJava : XH_SDK_GetDeviceList : oJChannelInfoVO is NULL.\n");
		return NULL;
	}
	else
	{
		//printf("XH_NetSDKForJava : XH_SDK_GetDeviceList : oJChannelInfoVO is %ld.\n", (long)jfChannel);
	}

	//3-新建ArrayOfCI object
	//jobject  joResultObj = inEnv->AllocObject(joArrayOfCI);
	jmethodID jMethod = inEnv->GetMethodID(joArrayOfCI, "<init>", "()V");
	jobject  joResultObj = inEnv->NewObject(joArrayOfCI, jMethod);

	//4-新建数组，容量为100，第一个数据为jfChannelInfoVO
	jobjectArray jArrChannelInfoVO = inEnv->NewObjectArray(nDevNum, jfChannelInfoVO, 0);


	//5-调用XH_SDK_GetDeviceList获取数据
	XH_DEV_INFO *pDevInfo = new XH_DEV_INFO[nDevNum];
	memset(pDevInfo, 0, nDevNum * sizeof(XH_DEV_INFO));
	int nGetNum = 0;
	int nRet = XH_SDK_GetDeviceList(inHandler, XH_DEV_CLASS_CAMERA, pDevInfo, nFromIndex, nDevNum, nGetNum);
	if (nRet == XH_SUCCEED)
	{
		jmethodID jChanMethod = inEnv->GetMethodID(jfChannelInfoVO, "<init>", "()V");
		for (int i = 0; i < nGetNum; ++i)
		{

			//6-新建jfChannelInfoVO对象
			//jobject  joChanObj = inEnv->AllocObject(jfChannelInfoVO);
			jobject  joChanObj = inEnv->NewObject(jfChannelInfoVO, jChanMethod);

			//7-变量赋值
			inEnv->SetIntField(joChanObj, jnStatus, (pDevInfo + i)->nDevStatus);
			inEnv->SetIntField(joChanObj, jfCameraType, (pDevInfo + i)->nDevType);
			inEnv->SetObjectField(joChanObj, jfCameraName, CharTojstring(inEnv, (pDevInfo + i)->cDevName));
			inEnv->SetObjectField(joChanObj, jfCameraCode, CharTojstring(inEnv, (pDevInfo + i)->cDevCode));
			inEnv->SetObjectField(joChanObj, jfCameraVendor, CharTojstring(inEnv, (pDevInfo + i)->cDevVendor));
			inEnv->SetObjectField(joChanObj, jfCameraGroup, CharTojstring(inEnv, (pDevInfo + i)->cDevParentCode));
			inEnv->SetObjectField(joChanObj, jfCameraParent, CharTojstring(inEnv, (pDevInfo + i)->cDevParentCode));

			//8-添加到objcet数组中
			inEnv->SetObjectArrayElement(jArrChannelInfoVO, i, joChanObj);
		}
		//9-将jArrChannelInfoVO数组添加到joResultObj中
		inEnv->SetObjectField(joResultObj, jfChannel, jArrChannelInfoVO);

		inEnv->SetIntField(joResultObj, jfGetNum, nGetNum);
	}

	inEnv->SetIntField(joResultObj, jfErr, nRet);

	delete pDevInfo;
	return joResultObj;
}

/*
* Class:     com_xh_vms_jni_EquipJNI
* Method:    getDevGroupsByCode
* Signature: (ILjava/lang/String;I)Lcom/xh/vms/domain/ArrayOfThirdGroup;
*/
JNIEXPORT jobject JNICALL Java_com_xh_xEye_jni_EquipJNI_getDevGroupsByCode(JNIEnv *inEnv, jclass inClass, jint inHandler, jstring inGroupCode, jint inGroupNum)
{
	//public class ArrayOfThirdGroup {
	//	private int realGroupNum; //返回实际通道数目
	//	private ThirdGroupVO[] tgVO;
	//	private int errorCode;
	//	}


	//}

	// -------------2
	/*public class ThirdGroupVO {
		private String name;
		private String code;
		private String parentCode;
		private String domainCode;
		private String sReserve;
	}*/

	const char *sGroupCode = inEnv->GetStringUTFChars(inGroupCode, 0);
	long lLoginHandle = inHandler;


	//1-实例ThirdGroupVO 及 成员变量
	char sClassThirdGroupVO[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sClassThirdGroupVO, "%s%s", gJavaPath, "ThirdGroupVO");
	//jclass joThirdGroupVO = inEnv->FindClass("com/xh/vms/domain/ThirdGroupVO");
	printf("Java_com_xh_vms_jni_EquipJNI_getDevGroupsByCode: sClassThirdGroupVO is '%s'.\n", sClassThirdGroupVO);
	jclass joThirdGroupVO = inEnv->FindClass(sClassThirdGroupVO);
	//实例变量
	jfieldID jfName = inEnv->GetFieldID(joThirdGroupVO, "name", "Ljava/lang/String;");
	jfieldID jfCode = inEnv->GetFieldID(joThirdGroupVO, "code", "Ljava/lang/String;");
	jfieldID jfDomainCode = inEnv->GetFieldID(joThirdGroupVO, "domainCode", "Ljava/lang/String;");
	jfieldID jfParentCode = inEnv->GetFieldID(joThirdGroupVO, "parentCode", "Ljava/lang/String;");
	jfieldID jfReserve = inEnv->GetFieldID(joThirdGroupVO, "sReserve", "Ljava/lang/String;");


	//2-实例ArrayOfThirdGroup 及 成员变量
	char sClassArrayOfThirdGroup[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sClassArrayOfThirdGroup, "%s%s", gJavaPath, "ArrayOfThirdGroup");
	printf("Java_com_xh_vms_jni_EquipJNI_getDevGroupsByCode: sClassArrayOfThirdGroup is '%s'.\n", sClassArrayOfThirdGroup);
	//jclass joArrayOfThirdGroup = inEnv->FindClass("com/xh/vms/domain/ArrayOfThirdGroup");
	jclass joArrayOfThirdGroup = inEnv->FindClass(sClassArrayOfThirdGroup);
	//实例变量
	jfieldID jfErr = (inEnv)->GetFieldID(joArrayOfThirdGroup, "errorCode", "I");
	jfieldID jfGetNum = (inEnv)->GetFieldID(joArrayOfThirdGroup, "realGroupNum", "I");
	char sObjectThirdGroupVO[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sObjectThirdGroupVO, "[L%s;", sClassThirdGroupVO);
	//jfieldID jfGroup = inEnv->GetFieldID(joArrayOfThirdGroup, "tgVO", "[Lcom/xh/vms/domain/ThirdGroupVO;");
	printf("Java_com_xh_vms_jni_EquipJNI_getDevGroupsByCode: sObjectThirdGroupVO is '%s'.\n", sObjectThirdGroupVO);
	jfieldID jfGroup = inEnv->GetFieldID(joArrayOfThirdGroup, "tgVO", sObjectThirdGroupVO);
	if (jfGroup == NULL)
	{
		printf("XH_NetSDKForJava : XH_SDK_GetDeviceList : jfGroup is NULL.\n");
		return NULL;
	}

	//3-新建ArrayOfCI object
	//jobject  joResultObj = inEnv->AllocObject(joArrayOfCI);
	jmethodID jMethod = inEnv->GetMethodID(joArrayOfThirdGroup, "<init>", "()V");
	jobject  joResultObj = inEnv->NewObject(joArrayOfThirdGroup, jMethod);


	//4-调用XH_SDK_GetDeviceList获取数据
	int nNeedNum = 1000;
	XH_DEV_GROUP_INFO *pGroupInfo = new XH_DEV_GROUP_INFO[nNeedNum];
	memset(pGroupInfo, 0, nNeedNum * sizeof(XH_DEV_GROUP_INFO));
	int nGetNum = 0;
	int nRet =XH_SDK_GetDevGroupListByGroupCode(lLoginHandle, (char *)sGroupCode, nNeedNum, pGroupInfo, nGetNum);
	if (nRet == XH_SUCCEED)
	{
		//5-新建数组，容量为100，第一个数据为jfChannelInfoVO
		jobjectArray jArrGroupInfoVO = inEnv->NewObjectArray(nGetNum, joThirdGroupVO, 0);

		jmethodID jGroupMethod = inEnv->GetMethodID(joThirdGroupVO, "<init>", "()V");
		for (int i = 0; i < nGetNum; ++i)
		{
			//6-新建jfChannelInfoVO对象
			//jobject  joChanObj = inEnv->AllocObject(jfChannelInfoVO);
			jobject  joGroupObj = inEnv->NewObject(joThirdGroupVO, jGroupMethod);

			//7-变量赋值
			//inEnv->SetIntField(joGroupObj, jfName, (pDevInfo + i)->nDevStatus);
			//inEnv->SetIntField(joGroupObj, jfCameraType, (pDevInfo + i)->nDevType);
			inEnv->SetObjectField(joGroupObj, jfName, CharTojstring(inEnv, (pGroupInfo + i)->sName));
			inEnv->SetObjectField(joGroupObj, jfCode, CharTojstring(inEnv, (pGroupInfo + i)->sCode));
			inEnv->SetObjectField(joGroupObj, jfDomainCode, CharTojstring(inEnv, (pGroupInfo + i)->sDomainCode));
			inEnv->SetObjectField(joGroupObj, jfParentCode, CharTojstring(inEnv, (pGroupInfo + i)->sParentCode));
			inEnv->SetObjectField(joGroupObj, jfReserve, CharTojstring(inEnv, (pGroupInfo + i)->sReserve));

			//8-添加到objcet数组中
			inEnv->SetObjectArrayElement(jArrGroupInfoVO, i, joGroupObj);

			//inEnv->DeleteLocalRef(joGroupObj);
		}
		//9-将jArrGroupInfoVO数组添加到joResultObj中
		inEnv->SetObjectField(joResultObj, jfGroup, jArrGroupInfoVO);

		inEnv->SetIntField(joResultObj, jfGetNum, nGetNum);


		//inEnv->DeleteLocalRef(jArrGroupInfoVO);
	}

	inEnv->SetIntField(joResultObj, jfErr, nRet);


	/*inEnv->DeleteLocalRef(joThirdGroupVO);
	inEnv->DeleteLocalRef(joArrayOfThirdGroup);
	inEnv->DeleteLocalRef(joResultObj);*/

	inEnv->ReleaseStringUTFChars(inGroupCode, sGroupCode);
	delete pGroupInfo;

	return joResultObj;
}

/*
* Class:     com_xh_vms_jni_EquipJNI
* Method:    getDevicesByCode
* Signature: (ILjava/lang/String;II)Lcom/xh/vms/domain/ArrayOfCI;
*/
JNIEXPORT jobject JNICALL Java_com_xh_xEye_jni_EquipJNI_getDevicesByCode(JNIEnv *inEnv, jclass inClass, jint inHandler, jstring inGroupCode, jint inFromIndex, jint inDeviceNum)
{
	// -------------1
	//public class ArrayOfCI {
	//	private int realDevNum; //返回实际通道数目
	//	private ChannelInfoVO[] cInfoVO;
	//	private int errorCode;

	//}

	// -------------2
	//public class ChannelInfoVO {
	//	private String cDevName;
	//	private String cDevCode; //sdk号
	//	private String cDevVendor; //厂商

	//	private String groupCode;
	//	private String parentDevCode;
	//	private int status; //在线状态
	//	private int channelType; //摄像机类型：球机、枪机等

	//}
	
	const char *sGroupCode = inEnv->GetStringUTFChars(inGroupCode, 0);

	long lLoginHandle = inHandler;
	int nFromIndex = inFromIndex;
	int nDevNum = inDeviceNum;
	if (nDevNum <= 0)
	{
		nDevNum = 30;
	}

	

	//1-实例ChannelInfoVO 及 成员变量
	//jclass jfChannelInfoVO = inEnv->FindClass("com/xh/vms/domain/ChannelInfoVO");
	char sClassChannelInfoVO[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sClassChannelInfoVO, "%s%s", gJavaPath, "ChannelInfoVO");
	printf("Java_com_xh_vms_jni_EquipJNI_getDevicesByCode: sClassChannelInfoVO is '%s'.\n", sClassChannelInfoVO);
	jclass jfChannelInfoVO = inEnv->FindClass(sClassChannelInfoVO);
	//实例变量
	jfieldID jnStatus = inEnv->GetFieldID(jfChannelInfoVO, "status", "I");
	jfieldID jfCameraType = inEnv->GetFieldID(jfChannelInfoVO, "channelType", "I");
	jfieldID jfCameraName = inEnv->GetFieldID(jfChannelInfoVO, "cDevName", "Ljava/lang/String;");
	jfieldID jfCameraCode = inEnv->GetFieldID(jfChannelInfoVO, "cDevCode", "Ljava/lang/String;");
	jfieldID jfCameraVendor = inEnv->GetFieldID(jfChannelInfoVO, "cDevVendor", "Ljava/lang/String;");
	jfieldID jfCameraGroup = inEnv->GetFieldID(jfChannelInfoVO, "groupCode", "Ljava/lang/String;");
	jfieldID jfCameraParent = inEnv->GetFieldID(jfChannelInfoVO, "parentDevCode", "Ljava/lang/String;");


	//2-实例ArrayOfCI 及 成员变量
	char sClassArrayOfCI[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sClassArrayOfCI, "%s%s", gJavaPath, "ArrayOfCI");
	//jclass joArrayOfCI = inEnv->FindClass("com/xh/vms/domain/ArrayOfCI");
	jclass joArrayOfCI = inEnv->FindClass(sClassArrayOfCI);
	//实例变量
	jfieldID jfErr = (inEnv)->GetFieldID(joArrayOfCI, "errorCode", "I");
	jfieldID jfGetNum = (inEnv)->GetFieldID(joArrayOfCI, "realDevNum", "I");
	char sObjectChanal[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sObjectChanal, "[L%s%s;", gJavaPath, "ChannelInfoVO");
	printf("Java_com_xh_vms_jni_EquipJNI_getDevicesByCode: sObjectChanal is '%s'.\n", sObjectChanal);
	//jfieldID jfChannel = inEnv->GetFieldID(joArrayOfCI, "cInfoVO", "[Lcom/xh/vms/domain/ChannelInfoVO;");
	jfieldID jfChannel = inEnv->GetFieldID(joArrayOfCI, "cInfoVO", sObjectChanal);
	if (jfChannel == NULL)
	{
		printf("XH_NetSDKForJava : XH_SDK_GetDeviceList : oJChannelInfoVO is NULL.\n");
		return NULL;
	}
	else
	{
		printf("XH_NetSDKForJava : XH_SDK_GetDeviceList : oJChannelInfoVO is %ld.\n", (long)jfChannel);
	}

	//3-新建ArrayOfCI object
	//jobject  joResultObj = inEnv->AllocObject(joArrayOfCI);
	jmethodID jMethod = inEnv->GetMethodID(joArrayOfCI, "<init>", "()V");
	jobject  joResultObj = inEnv->NewObject(joArrayOfCI, jMethod);



	//4-调用XH_SDK_GetDeviceList获取数据
	XH_CAMERA_INFO *pDevInfo = new XH_CAMERA_INFO[nDevNum];
	memset(pDevInfo, 0, nDevNum * sizeof(XH_CAMERA_INFO));
	int nGetNum = 0;
	int nRet = XH_SDK_GetCameraListByGroupCode(lLoginHandle, (char *)sGroupCode, nFromIndex, nDevNum, pDevInfo, nGetNum);
	if (nRet == XH_SUCCEED)
	{
		//5-新建数组，容量为100，第一个数据为jfChannelInfoVO
		jobjectArray jArrChannelInfoVO = inEnv->NewObjectArray(nGetNum, jfChannelInfoVO, 0);

		jmethodID jChanMethod = inEnv->GetMethodID(jfChannelInfoVO, "<init>", "()V");
		for (int i = 0; i < nGetNum; ++i)
		{
			//6-新建jfChannelInfoVO对象
			//jobject  joChanObj = inEnv->AllocObject(jfChannelInfoVO);
			jobject  joChanObj = inEnv->NewObject(jfChannelInfoVO, jChanMethod);

			//7-变量赋值
			inEnv->SetIntField(joChanObj, jnStatus, (pDevInfo + i)->nStatus);
			inEnv->SetIntField(joChanObj, jfCameraType, (pDevInfo + i)->nType);
			inEnv->SetObjectField(joChanObj, jfCameraName, CharTojstring(inEnv, (pDevInfo + i)->sName));
			inEnv->SetObjectField(joChanObj, jfCameraCode, CharTojstring(inEnv, (pDevInfo + i)->sCode));
			inEnv->SetObjectField(joChanObj, jfCameraVendor, CharTojstring(inEnv, (pDevInfo + i)->sVendor));
			inEnv->SetObjectField(joChanObj, jfCameraGroup, CharTojstring(inEnv, (pDevInfo + i)->sGroupCode));
			inEnv->SetObjectField(joChanObj, jfCameraParent, CharTojstring(inEnv, (pDevInfo + i)->sParentCode));

			//8-添加到objcet数组中
			inEnv->SetObjectArrayElement(jArrChannelInfoVO, i, joChanObj);
		}
		//9-将jArrChannelInfoVO数组添加到joResultObj中
		inEnv->SetObjectField(joResultObj, jfChannel, jArrChannelInfoVO);

		inEnv->SetIntField(joResultObj, jfGetNum, nGetNum);
	}

	inEnv->SetIntField(joResultObj, jfErr, nRet);

	inEnv->ReleaseStringUTFChars(inGroupCode, sGroupCode);
	delete pDevInfo;

	return joResultObj;
}


JNIEXPORT jobject JNICALL Java_com_xh_xEye_jni_EquipJNI_getDomain(JNIEnv *inEnv, jclass inClass, jint inHandler)
{
	/*public class DomainVO {
		private int errorCode;
		private String code;
		private String name;
		private String superDomain;*/

	long lLoginHandle = inHandler;



	XH_DOMAIN_INFO oneDomainInfo;
	memset(&oneDomainInfo, 0, sizeof(oneDomainInfo));
	int nGetNum = 0;
	int nRet = XH_SDK_GetDevDomainList(lLoginHandle, &oneDomainInfo, nGetNum);
	if (nRet != XH_SUCCEED)
	{
		printf("XH_SDK_GetDevDomainList fail. err is %d.",nRet);
		
	}


	char sJavaClassDomainVO[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sJavaClassDomainVO, "%s%s", gJavaPath, "DomainVO");
	printf("Java_com_xh_vms_jni_EquipJNI_getDomain: sJavaClassDomainVO is '%s'.\n", sJavaClassDomainVO);
	jclass objectClass = (inEnv)->FindClass(sJavaClassDomainVO);

	//获取类中每一个变量的定义
	//名字
	jfieldID jfErr = (inEnv)->GetFieldID(objectClass, "errorCode", "I");
	jfieldID jfCode = (inEnv)->GetFieldID(objectClass, "code", "Ljava/lang/String;");
	jfieldID jfName = (inEnv)->GetFieldID(objectClass, "name", "Ljava/lang/String;");
	jfieldID jfSuperDomain = (inEnv)->GetFieldID(objectClass, "superDomain", "Ljava/lang/String;");

	jmethodID jmId = inEnv->GetMethodID(objectClass, "<init>", "()V");
	jobject  oResultObj = inEnv->NewObject(objectClass, jmId);

	//inEnv->SetObjectField(m_obj, nErr, inEnv->NewStringUTF("AP1"));]
	inEnv->SetIntField(oResultObj, jfErr, nRet);
	if (nRet == XH_SUCCEED)
	{
		inEnv->SetObjectField(oResultObj, jfCode, CharTojstring(inEnv, oneDomainInfo.sCode));
		inEnv->SetObjectField(oResultObj, jfName, CharTojstring(inEnv, oneDomainInfo.sName));
		inEnv->SetObjectField(oResultObj, jfSuperDomain, CharTojstring(inEnv, oneDomainInfo.sSuperDomain));
	}

	return oResultObj;
}

/*
* Class:     com_xh_xEye_jni_EquipJNI
* Method:    getDomainEx
* Signature: (I)Lcom/xh/xEye/domain/ArrayOfDomain;
* Comment: 获取域数组
*/
JNIEXPORT jobject JNICALL Java_com_xh_xEye_jni_EquipJNI_getDomainEx(JNIEnv *inEnv, jclass inClass, jint inHandler)
{
	//心得: C/C++中使用Java类中的属性和方法有点类似于使用“动态加载的链接库中的属性和函数”。
	//     Java类中的方法在C/C++层看来都是回调函数，C/C++使用这些回调函数向Java层传递数据。

	/// 1-Java类获取
	char sJavaClassAbsPath[XH_JAVA_PATH_LEN] = { 0 };
	sprintf(sJavaClassAbsPath, "%s%s", gJavaPath, "DomainVO");
	printf("Java_com_xh_vms_jni_EquipJNI_getDomainEx: Java Class 'DomainVO' path is '%s'.\n", sJavaClassAbsPath);
	jclass objectDomainVOClass = (inEnv)->FindClass(sJavaClassAbsPath);  //DomainVO.java中的DomainVO class

	memset(sJavaClassAbsPath, 0, sizeof(sJavaClassAbsPath));
	sprintf(sJavaClassAbsPath, "%s%s", gJavaPath, "ArrayOfDomain");
	printf("Java_com_xh_vms_jni_EquipJNI_getDomainEx: Java Class 'ArrayOfDomain' path is '%s'.\n", sJavaClassAbsPath);
	jclass objectArrayOfDomainClass = inEnv->FindClass(sJavaClassAbsPath);

	/// 2-Java类属性、方法获取
	jfieldID jfDomainVOErr = (inEnv)->GetFieldID(objectDomainVOClass, "errorCode", "I");
	jfieldID jfDomainVOCode = (inEnv)->GetFieldID(objectDomainVOClass, "code", "Ljava/lang/String;");
	jfieldID jfDomainVOName = (inEnv)->GetFieldID(objectDomainVOClass, "name", "Ljava/lang/String;");
	jfieldID jfDomainVOSuperDomain = (inEnv)->GetFieldID(objectDomainVOClass, "superDomain", "Ljava/lang/String;");

	jfieldID jfArrayOfDomainRealDomainNum = inEnv->GetFieldID(objectArrayOfDomainClass, "realDomianNum", "I");
	jfieldID jfArrayOfDomainErrorCode = inEnv->GetFieldID(objectArrayOfDomainClass, "errorCode", "I");
	sprintf(sJavaClassAbsPath, "[L%s%s;", gJavaPath, "DomainVO");  //数组类型签名"[type"，其中type类型签名为"Lcom/xh/xEye/domain/DomainVO"
	jfieldID jfArrayOfDomainDomains = inEnv->GetFieldID(objectArrayOfDomainClass, "domains", sJavaClassAbsPath);

	//JNI中，Java类的默认构造函数可以和实例方法一样被调用，调用方式也相似。传入“<init>”作为方法名，“V”作为返回类型。
	//可以通过向JNI函数NewObject传入方法ID(jmethodID)来调用构造函数并返回一个此类的实体对象。
	jmethodID jmArrayOfDomainConstructId = inEnv->GetMethodID(objectArrayOfDomainClass, "<init>", "()V");
	jobject oArrayOfDoaminObj = inEnv->NewObject(objectArrayOfDomainClass, jmArrayOfDomainConstructId);  //创建Java类ArrayOfDomain的对象oArrayOfDoaminObj


	/// 3-获取所有域
	long lLoginHandle = inHandler;
	XH_DOMAIN_INFO domainArrayInfo[100];
	memset(domainArrayInfo, 0, sizeof(XH_DOMAIN_INFO)*100);
	int nGetNum = 0;
	int nRet = XH_SUCCEED;
	for (int i = 0; i < 3; ++i){  //尝试多次调用，避免网络异常引起获取失败
		nRet = XH_SDK_GetDevDomainList(lLoginHandle, domainArrayInfo, nGetNum);
		if (XH_SUCCEED == nRet)
			break;
	}
	if (nRet != XH_SUCCEED)
	{
		printf("Java_com_xh_xEye_jni_EquipJNI_getDomainEx: XH_SDK_GetDevDomainList fail. err is %d.", nRet);
	}
	//向Java层返回错误
	inEnv->SetIntField(oArrayOfDoaminObj, jfArrayOfDomainErrorCode, nRet);

	/// 4-新建数组，容量为nGetNum，类型为DomainVO
	jobjectArray jArrOfDomainInfo = inEnv->NewObjectArray(nGetNum, objectDomainVOClass, NULL);

	jmethodID jmDomainVOConsturctId = inEnv->GetMethodID(objectDomainVOClass, "<init>", "()V");
	for (int i = 0; i < nGetNum; ++i)
	{
		/// 5-创建建DomainVO对象
		jobject  oDomainVOObj = inEnv->NewObject(objectDomainVOClass, jmDomainVOConsturctId);

		/// 6-变量赋值
		inEnv->SetObjectField(oDomainVOObj, jfDomainVOCode, CharTojstring(inEnv, domainArrayInfo[i].sCode));
		inEnv->SetObjectField(oDomainVOObj, jfDomainVOName, CharTojstring(inEnv, domainArrayInfo[i].sName));
		inEnv->SetObjectField(oDomainVOObj, jfDomainVOSuperDomain, CharTojstring(inEnv, domainArrayInfo[i].sSuperDomain));

		/// 7-添加到objcet数组中
		inEnv->SetObjectArrayElement(jArrOfDomainInfo, i, oDomainVOObj);
	}

	/// 8-将jArrOfDomainInfo数组添加到oArrayOfDoaminObj中
	inEnv->SetObjectField(oArrayOfDoaminObj, jfArrayOfDomainDomains, jArrOfDomainInfo);
	inEnv->SetIntField(oArrayOfDoaminObj, jfArrayOfDomainRealDomainNum, nGetNum);

	return oArrayOfDoaminObj;  //返回ArrayOfDomain的对象oArrayOfDoaminObj
}

/*
* Class:     com_xh_vms_jni_EquipJNI
* Method:    loginoutEquip
* Signature: (I)I
*/
JNIEXPORT jint JNICALL Java_com_xh_xEye_jni_EquipJNI_logoutEquip(JNIEnv *inEnv, jclass inClass, jint inHandler)
{
	return XH_SDK_DisconnectEquipment(inHandler);
}

