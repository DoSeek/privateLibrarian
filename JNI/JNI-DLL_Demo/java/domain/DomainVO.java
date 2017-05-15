package com.xh.ts.common.domain;

import java.util.ArrayList;
import java.util.List;

public class DomainVO {
	private int errorCode;
	private String code;
	private String name;
	private String superDomain;
	
	private String fullname; //全路径
	
	private List<DomainVO> cdomain = new ArrayList<DomainVO>(); //子域
	
	public String getCode() {
		return code;
	}
	public void setCode(String code) {
		this.code = code;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getSuperDomain() {
		return superDomain;
	}
	public void setSuperDomain(String superDomain) {
		this.superDomain = superDomain;
	}
	public int getErrorCode() {
		return errorCode;
	}
	public void setErrorCode(int errorCode) {
		this.errorCode = errorCode;
	}
	public List<DomainVO> getCdomain() {
		return cdomain;
	}
	public void setCdomain(List<DomainVO> cdomain) {
		this.cdomain = cdomain;
	}
	public String getFullname() {
		return fullname;
	}
	public void setFullname(String fullname) {
		this.fullname = fullname;
	}
	
	
}
