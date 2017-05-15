package com.xh.ts.common.domain;


public class ArrayOfDomain {
	private int realDomianNum; //返回实际域个数
	private int errorCode;
	private DomainVO[] domains;
	
	public int getRealDomianNum() {
		return realDomianNum;
	}
	public void setRealDomianNum(int realDomianNum) {
		this.realDomianNum = realDomianNum;
	}
	public int getErrorCode() {
		return errorCode;
	}
	public void setErrorCode(int errorCode) {
		this.errorCode = errorCode;
	}
	public DomainVO[] getDomains() {
		return domains;
	}
	public void setDomains(DomainVO[] domains) {
		this.domains = domains;
	}
	
	
}
