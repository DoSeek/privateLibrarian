package com.xh.ts.common.domain;

import java.util.ArrayList;
import java.util.List;

public class XMLToObject {
	private String id;
	private String pId;
	private String name;
	private String type; // 类型， 4（域或者组） 5：镜头
	private String ip;
	private String pIp;
	
	private int channelType;
	
	private String fullname;
	
	private List<XMLToObject> children = new ArrayList<>(); // 子集合
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getpId() {
		return pId;
	}
	public void setpId(String pId) {
		this.pId = pId;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	public String getIp() {
		return ip;
	}
	public void setIp(String ip) {
		this.ip = ip;
	}
	public String getpIp() {
		return pIp;
	}
	public void setpIp(String pIp) {
		this.pIp = pIp;
	}
	public List<XMLToObject> getChildren() {
		return children;
	}
	public void setChildren(List<XMLToObject> children) {
		this.children = children;
	}
	public String getFullname() {
		return fullname;
	}
	public void setFullname(String fullname) {
		this.fullname = fullname;
	}
	public int getChannelType() {
		return channelType;
	}
	public void setChannelType(int channelType) {
		this.channelType = channelType;
	}
	
	
	
}
