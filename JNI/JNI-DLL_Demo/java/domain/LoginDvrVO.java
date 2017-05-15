package com.xh.ts.common.domain;

public class LoginDvrVO {
	private int loginErrorCode;
	private int loginHandle; //登陆句柄
	private String filename; //生成xml的名称
	private String filepath; //
	
	public int getLoginErrorCode() {
		return loginErrorCode;
	}
	public void setLoginErrorCode(int loginErrorCode) {
		this.loginErrorCode = loginErrorCode;
	}
	public int getLoginHandle() {
		return loginHandle;
	}
	public void setLoginHandle(int loginHandle) {
		this.loginHandle = loginHandle;
	}
	public String getFilename() {
		return filename;
	}
	public void setFilename(String filename) {
		this.filename = filename;
	}
	public String getFilepath() {
		return filepath;
	}
	public void setFilepath(String filepath) {
		this.filepath = filepath;
	}
	
	
}
