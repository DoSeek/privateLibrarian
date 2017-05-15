package com.xh.ts.common.domain;

import org.apache.log4j.Logger;


public class EquipJNI {
	private static Logger logger = Logger.getLogger(EquipJNI.class);
	static {
		loadlib();
	}
	
	public static void loadlib(){
		try {
			logger.info("path........"+System.getProperty("java.library.path"));
			System.loadLibrary("XH_NetSDKForJava");
			logger.info("加载动态库成功");
		} catch (Exception e) {
			//logger.info("加载动态库失败"+e.getMessage());
			logger.info("加载动态库失败",e);
		}
	}
	
	// 登陆dvr
	public static native LoginDvrVO loginEquip(String ip, int port, String username, String password,
			String Manufacturer, String packagePath);

	// 获取设备列表
	public static native ArrayOfCI getDeviceList(int handler, int deviceType, int fromIndex, int deviceNum);

	// 获取组下属的组信息
	public static native ArrayOfThirdGroup getDevGroupsByCode(int handler, String groupCode, int groupNum);

	// 获取组下属的摄像机信息
	public static native ArrayOfCI getDevicesByCode(int handler, String groupCode, int fromIndex, int deviceNum);

	// 注销dvr
	public static native int logoutEquip(int handler);

	// 获取域
	public static native DomainVO getDomain(int handler);
	
	// 获取所有域
	public static native ArrayOfDomain getDomainEx(int handler);
	
	/**
	 * 登陆并获取镜头（生成xml）
	 * @param ip 
	 * @param port
	 * @param username 
	 * @param password 
	 * @param Manufacturer 厂商
	 * @param packagePath 所在包路径 （com/xh/ts/common/domain/）
	 * @return
	 */
	public static native LoginDvrVO loginAndGetEquip(String ip, int port, String username, String password,
			String Manufacturer, String packagePath);

	
	public static void main(String[] args) {
		 System.out.println("path........"+System.getProperty("java.library.path"));
		/* LoginDvrVO lvo = loginEquip("190.224.5.201", 8000, "admin", "12345","HK","com/xh/xEye/domain/");
		 if(lvo.getLoginErrorCode()==0){
		 System.out.println("登陆成功，句柄为："+lvo.getLoginHandle());
		 }else{
		 System.out.println("登陆失败，错误代码："+lvo.getLoginErrorCode());
		 }
		 ArrayOfCI aoci = getDeviceList(lvo.getLoginHandle(), 0, 1, 90);
		 System.out.println("errorCode.."+aoci.getErrorCode());
		 System.out.println("size.."+aoci.getRealDevNum());*/
		//getDevGroupsByCode(handler, groupCode, groupNum)

		// 华为平台
//		LoginDvrVO lvo = loginEquip("10.253.66.102", 9900, "dby", "2015.dby", "HW", "com/xh/xEye/domain/");
//		ArrayOfThirdGroup atg = getDevGroupsByCode(lvo.getLoginHandle(), "0", 2000);
//		// LoginDvrVO lvo = loginEquip("190.224.5.201", 8000, "admin", "12345",
//		// "HK", "com/xh/xEye/domain/");
//		System.out.println("atg............" + atg.getRealGroupNum());
		// DomainVO domianvo = getDomain(lvo.getLoginHandle());
		// System.out.println(domianvo.getErrorCode());
		/*
		 * if(lvo.getLoginErrorCode()==0){
		 * System.out.println("登陆成功，句柄为："+lvo.getLoginHandle()); }else{
		 * System.out.println("登陆失败，错误代码："+lvo.getLoginErrorCode()); }
		 * //ArrayOfThirdGroup aotg = getDevGroupsByCode(lvo.getLoginHandle(),
		 * "0", 100); ArrayOfCI aoci = getDevicesByCode(lvo.getLoginHandle(),
		 * "4403060000131112000#13", 1, 2000);
		 */
		//System.out.println(".............." + lvo.getLoginErrorCode());
		// ArrayOfCI cis = getDeviceList(lvo.getLoginHandle(), 0, 1, 16);
		// System.out.println("获取通道数："+cis.getRealDevNum()+"错误码："+cis.getErrorCode());
		// System.out.println("errorCode.."+aotg.getErrorCode());
		// System.out.println("size.."+aotg.getTgVO().length);
		// System.out.println("ec....."+lvo.getLoginErrorCode()+"hd.."+lvo.getLoginHandle());
	}

}