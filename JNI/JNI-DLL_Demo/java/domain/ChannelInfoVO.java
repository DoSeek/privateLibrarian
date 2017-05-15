package com.xh.ts.common.domain;

public class ChannelInfoVO {
	private String cDevName; 
	private String cDevCode; //sdk号
	private String cDevVendor; //厂商
	private String cCode; //
	private String groupCode;
	private String parentDevCode;
	private int status; //在线状态
	private int channelType; //摄像机类型：球机、枪机等
	
	private Double px;
	private Double py;
	private String ip;
	private String fullGroupCode;
	private String fullGroupName;
	
	
	public String getcCode() {
		return cCode;
	}
	public void setcCode(String cCode) {
		this.cCode = cCode;
	}
	public String getcDevName() {
		return cDevName;
	}
	public void setcDevName(String cDevName) {
		this.cDevName = cDevName;
	}
	public String getcDevCode() {
		return cDevCode;
	}
	public void setcDevCode(String cDevCode) {
		this.cDevCode = cDevCode;
	}
	public String getcDevVendor() {
		return cDevVendor;
	}
	public void setcDevVendor(String cDevVendor) {
		this.cDevVendor = cDevVendor;
	}
	public String getGroupCode() {
		return groupCode;
	}
	public void setGroupCode(String groupCode) {
		this.groupCode = groupCode;
	}
	public String getParentDevCode() {
		return parentDevCode;
	}
	public void setParentDevCode(String parentDevCode) {
		this.parentDevCode = parentDevCode;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	public int getChannelType() {
		return channelType;
	}
	public void setChannelType(int channelType) {
		this.channelType = channelType;
	}
	public Double getPx() {
		return px;
	}
	public void setPx(Double px) {
		this.px = px;
	}
	public Double getPy() {
		return py;
	}
	public void setPy(Double py) {
		this.py = py;
	}
	public String getIp() {
		return ip;
	}
	public void setIp(String ip) {
		this.ip = ip;
	}
	public String getFullGroupCode() {
		return fullGroupCode;
	}
	public void setFullGroupCode(String fullGroupCode) {
		this.fullGroupCode = fullGroupCode;
	}
	public String getFullGroupName() {
		return fullGroupName;
	}
	public void setFullGroupName(String fullGroupName) {
		this.fullGroupName = fullGroupName;
	}
	
	public ChannelInfoVO() {
	}
	
	public ChannelInfoVO(String cDevName, String cDevCode, String groupCode, int channelType, Double px, Double py,
			String ip, String fullGroupCode, String fullGroupName) {
		this.cDevName = cDevName;
		this.cDevCode = cDevCode;
		this.groupCode = groupCode;
		this.channelType = channelType;
		this.px = px;
		this.py = py;
		this.ip = ip;
		this.fullGroupCode = fullGroupCode;
		this.fullGroupName = fullGroupName;
	}
}
