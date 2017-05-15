package com.xh.ts.common.domain;

public class ThirdGroupVO {
	private String name; 
	private String code;
	private String parentCode;
	private String domainCode;
	private String sReserve;
	private String pCode;
	private String fullname;
	
	
	public String getpCode() {
		return pCode;
	}
	public void setpCode(String pCode) {
		this.pCode = pCode;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getCode() {
		return code;
	}
	public void setCode(String code) {
		this.code = code;
	}
	public String getParentCode() {
		return parentCode;
	}
	public void setParentCode(String parentCode) {
		this.parentCode = parentCode;
	}
	public String getDomainCode() {
		return domainCode;
	}
	public void setDomainCode(String domainCode) {
		this.domainCode = domainCode;
	}
	public String getsReserve() {
		return sReserve;
	}
	public void setsReserve(String sReserve) {
		this.sReserve = sReserve;
	}
	@Override
	public String toString() {
		return "ThirdGroupVO [name=" + name + ", code=" + code + ", parentCode=" + parentCode + ", domainCode="
				+ domainCode + ", sReserve=" + sReserve + ", pCode=" + pCode + ", fullname=" + fullname + "]";
	}
	public String getFullname() {
		return fullname;
	}
	public void setFullname(String fullname) {
		this.fullname = fullname;
	}
	public ThirdGroupVO() {
	}
	public ThirdGroupVO(String name, String code, String domainCode, String pCode, String fullname) {
		this.name = name;
		this.code = code;
		this.domainCode = domainCode;
		this.pCode = pCode;
		this.fullname = fullname;
	}
	
	
	
	
}
