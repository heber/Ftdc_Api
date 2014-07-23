
#pragma once

#include "stdafx.h"

class MdApiImpl : public CThostFtdcMdApi,public ISocketClientHandler
{
	typedef SocketClientImpl<ISocketClientHandler> CSocketClient;
	friend CSocketClient;
public:
	MdApiImpl();
	~MdApiImpl();

	virtual void Release();
	
	virtual void Init();
	
	virtual int Join();

	virtual const char* GetTradingDay();
	
	virtual void RegisterFront(char *pszFrontAddress);
	
	virtual void RegisterNameServer(char *pszNsAddress);
	virtual void RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo);
	
	virtual void RegisterSpi(CThostFtdcMdSpi *pSpi);
	
	virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount);
	
	virtual int UnSubscribeMarketData(char *ppInstrumentID[], int nCount);
	
	virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);

	virtual int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID);

private:
	// SocketClient Interface handler
	virtual void OnThreadBegin(CSocketHandle* pSH);
	virtual void OnThreadExit(CSocketHandle* pSH);
	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	virtual void OnConnectionDropped(CSocketHandle* pSH);
	virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError);

public:
	bool	GetRandSvr();
	int	SendPkt(const BYTE* pbData, DWORD dwCount);
	void ParsePkt(const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	int SendPrePkt();
	int SendHBeatPkt();
	
public:
	TThostFtdcDateType			m_sTdday;
	std::vector<std::string>	m_SvrVec;
	std::vector<BYTE>			m_bPktSend;
	TThostFtdcMacAddressType	m_sMac;
	CThostFtdcRspInfoField		m_RspInf;
	std::string					m_sFront;
	std::string					m_sPort;
	int							m_iSockType;
private:
	CThostFtdcMdSpi				*m_pSpi;
	CSocketClient				m_SockClient;

};