package com.xh.ts.common.domain;



public class ArrayOfThirdGroup {
	private int realGroupNum; //返回实际通道数目
	//private List<ThirdGroupVO> tgVO;
	private ThirdGroupVO[] tgVO;
	private int errorCode;
	public int getRealGroupNum() {
		return realGroupNum;
	}
	public void setRealGroupNum(int realGroupNum) {
		this.realGroupNum = realGroupNum;
	}
	
	/*public List<ThirdGroupVO> getTgVO() {
		return tgVO;
	}
	public void setTgVO(List<ThirdGroupVO> tgVO) {
		this.tgVO = tgVO;
	}*/
	public int getErrorCode() {
		return errorCode;
	}
	public void setErrorCode(int errorCode) {
		this.errorCode = errorCode;
	}
	public ThirdGroupVO[] getTgVO() {
		return tgVO;
	}
	public void setTgVO(ThirdGroupVO[] tgVO) {
		this.tgVO = tgVO;
	}
	
	
}
