/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class com_xh_ts_common_domain_EquipJNI */

#ifndef _Included_com_xh_ts_common_domain_EquipJNI
#define _Included_com_xh_ts_common_domain_EquipJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_xh_ts_common_domain_EquipJNI
 * Method:    loginEquip
 * Signature: (Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lcom/xh/ts/common/domain/LoginDvrVO;
 */
JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_loginEquip
  (JNIEnv *, jclass, jstring, jint, jstring, jstring, jstring, jstring);

/*
 * Class:     com_xh_ts_common_domain_EquipJNI
 * Method:    getDeviceList
 * Signature: (IIII)Lcom/xh/ts/common/domain/ArrayOfCI;
 */
JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDeviceList
  (JNIEnv *, jclass, jint, jint, jint, jint);

/*
 * Class:     com_xh_ts_common_domain_EquipJNI
 * Method:    getDevGroupsByCode
 * Signature: (ILjava/lang/String;I)Lcom/xh/ts/common/domain/ArrayOfThirdGroup;
 */
JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDevGroupsByCode
  (JNIEnv *, jclass, jint, jstring, jint);

/*
 * Class:     com_xh_ts_common_domain_EquipJNI
 * Method:    getDevicesByCode
 * Signature: (ILjava/lang/String;II)Lcom/xh/ts/common/domain/ArrayOfCI;
 */
JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDevicesByCode
  (JNIEnv *, jclass, jint, jstring, jint, jint);

/*
 * Class:     com_xh_ts_common_domain_EquipJNI
 * Method:    logoutEquip
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_xh_ts_common_domain_EquipJNI_logoutEquip
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_xh_ts_common_domain_EquipJNI
 * Method:    getDomain
 * Signature: (I)Lcom/xh/ts/common/domain/DomainVO;
 */
JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDomain
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_xh_ts_common_domain_EquipJNI
 * Method:    getDomainEx
 * Signature: (I)Lcom/xh/ts/common/domain/ArrayOfDomain;
 */
JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_getDomainEx
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_xh_ts_common_domain_EquipJNI
 * Method:    loginAndGetEquip
 * Signature: (Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)Lcom/xh/ts/common/domain/LoginDvrVO;
 */
JNIEXPORT jobject JNICALL Java_com_xh_ts_common_domain_EquipJNI_loginAndGetEquip
  (JNIEnv *, jclass, jstring, jint, jstring, jstring, jstring, jstring, jint);

#ifdef __cplusplus
}
#endif
#endif