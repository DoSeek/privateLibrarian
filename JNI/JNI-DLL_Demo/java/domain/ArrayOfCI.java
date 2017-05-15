package com.xh.ts.common.domain;


public class ArrayOfCI {
	private int realDevNum; //返回实际通道数目
	private ChannelInfoVO[] cInfoVO;
	
	private int errorCode;
	
	public int getRealDevNum() {
		return realDevNum;
	}
	public void setRealDevNum(int realDevNum) {
		this.realDevNum = realDevNum;
	}

	public int getErrorCode() {
		return errorCode;
	}
	public void setErrorCode(int errorCode) {
		this.errorCode = errorCode;
	}
	public ChannelInfoVO[] getcInfoVO() {
		return cInfoVO;
	}
	public void setcInfoVO(ChannelInfoVO[] cInfoVO) {
		this.cInfoVO = cInfoVO;
	}
	
	
	
	
}
