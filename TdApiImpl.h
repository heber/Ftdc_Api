
#pragma once

#include "stdafx.h"

class TdApiImpl : public CThostFtdcTraderApi,public ISocketClientHandler
{
	typedef SocketClientImpl<ISocketClientHandler> CSocketClient;
	friend CSocketClient;
public:
	TdApiImpl();
	~TdApiImpl();

public:
	virtual void Release();
	virtual void Init();
	virtual int Join();
	virtual const char *GetTradingDay();
	virtual void RegisterFront(char *pszFrontAddress);
	virtual void RegisterNameServer(char *pszNsAddress);
	virtual void RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo);
	virtual void RegisterSpi(CThostFtdcTraderSpi *pSpi);
	virtual void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType);
	virtual void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType);
	virtual int ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID);
	virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);
	
	virtual int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID);

	virtual int ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID);

	virtual int ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID);

	virtual int ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder, int nRequestID);

	virtual int ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID);

	virtual int ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID);

	virtual int ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID);

	virtual int ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID);

	virtual int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID);

	virtual int ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID);

	virtual int ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID);

	virtual int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID);

	virtual int ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID);

	virtual int ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID);

	virtual int ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID);

	virtual int ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID);

	virtual int ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID);

	virtual int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID);

	virtual int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID);

	virtual int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID);

	virtual int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID);

	virtual int ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID);

	virtual int ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID);

	virtual int ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID);

	virtual int ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID);

	virtual int ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID);

	virtual int ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID);

	virtual int ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID);

	virtual int ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID);

	virtual int ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID);

	virtual int ReqQryInvestorProductGroupMargin(CThostFtdcQryInvestorProductGroupMarginField *pQryInvestorProductGroupMargin, int nRequestID);

	virtual int ReqQryExchangeMarginRate(CThostFtdcQryExchangeMarginRateField *pQryExchangeMarginRate, int nRequestID);

	virtual int ReqQryExchangeMarginRateAdjust(CThostFtdcQryExchangeMarginRateAdjustField *pQryExchangeMarginRateAdjust, int nRequestID);

	virtual int ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID);

	virtual int ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID);

	virtual int ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID);

	virtual int ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID);

	///ÇëÇó²éÑ¯Ô¤Âñ³·µ¥
	virtual int ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID);

	virtual int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID);

	virtual int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID);

	virtual int ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID);

	virtual int ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID);
	virtual int ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID);

	virtual int ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID);

private:
	// SocketClient Interface handler
	virtual void OnThreadBegin(CSocketHandle* pSH);
	virtual void OnThreadExit(CSocketHandle* pSH);
	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	virtual void OnConnectionDropped(CSocketHandle* pSH);
	virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError);
public:
	bool GetRandSvr();
	int SendPkt(const BYTE* pbData, DWORD dwCount);
	void ParsePkt(const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	int SendPrePkt();
	int SendHBeatPkt();
	void FillReqHdr(PFTDHDREX pHdr,int iType,int iReqId);
	
public:
	TThostFtdcDateType			m_sTdday;
	CThostFtdcRspInfoField		m_RspInf;
	std::vector<std::string>	m_SvrVec;
	TThostFtdcMacAddressType	m_sMac;
	std::string					m_sFront;
	std::string					m_sPort;
	std::vector<BYTE>			m_bPktSend;
	int							m_iSockType;
	int							m_iOrdRef;
	PFTDHDREX					m_pHdrEx;				
private:
	CThostFtdcTraderSpi *m_pSpi;
	CSocketClient m_SockClient;
};