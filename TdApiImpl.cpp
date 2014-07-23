#include "stdafx.h"
#include "TdApiImpl.h"

static BYTE bUnkPkt[10] = {FTDTypeNone,6,0,0,7,4,0,0,0,0x27};
static BYTE bHBeatPkt[6] = {FTDTypeNone,2,0,0,FTDTagKeepAlive,0};

//其实这是4个10字节的结构体
static BYTE bQuickPkt[40] ={0x10,1,0,6,0,1,0,0,0,0,0x10,1,0,6,0,2,0xff,0xff,0xff,0xff,0x10,1,
0,6,0,3,0xff,0xff,0xff,0xff,0x10,1,0,6,0,4,0,0,0,0};
static BYTE bRestartPkt[40] ={0x10,1,0,6,0,1,0,0,0,0,0x10,1,0,6,0,2,0,0,0,0,0x10,1,
			0,6,0,3,0,0,0,0,0x10,1,0,6,0,4,0,0,0,0};

TdApiImpl::TdApiImpl()
{
	CSocketHandle::InitLibrary(MAKEWORD(2,2));

	TCHAR szIPAddr[MAX_PATH] = { 0 };
	CSocketHandle::GetLocalAddress(szIPAddr, MAX_PATH, AF_INET);
	char szIP[MAX_PATH] = { 0 };
	#ifdef _UNICODE
		uni2ansi(CP_ACP,szIPAddr,szIP);
	#else
		::StringCbCopyA(szIP, MAX_PATH, szIPAddr);
	#endif
	CSocketHandle::GetMacAddress(szIP, m_sMac,sizeof(TThostFtdcMacAddressType));

	m_SockClient.SetInterface(this);
	m_iSockType = SOCK_STREAM;
	m_iOrdRef = 1;
	m_pHdrEx = new FTDHDREX;
}

TdApiImpl::~TdApiImpl()
{
	delete m_pHdrEx;
}

void  TdApiImpl::Release()
{
	m_SockClient.Terminate();
	CSocketHandle::ReleaseLibrary();
	delete this;
}

bool TdApiImpl::GetRandSvr()
{
	srand((unsigned)time(NULL));
	size_t i=rand()% m_SvrVec.size();
	
	if (!m_SvrVec[i].empty())
	{
		std::string szPre = m_SvrVec[i].substr(0,3);
		
		if (!szPre.compare("tcp") || !szPre.compare("udp"))
		{
			m_iSockType = SOCK_STREAM;

			size_t iPos = m_SvrVec[i].rfind(':');
			size_t iPos2 = m_SvrVec[i].find("//");

			m_sFront=m_SvrVec[i].substr(iPos2+2,iPos-iPos2-2);
			m_sPort=m_SvrVec[i].substr(iPos+1);

			return true;
		}
		else
		{
			m_RspInf.ErrorID = 1;
			StringCbCopyA(m_RspInf.ErrorMsg,81,"不支持的连接类型!");
			m_pSpi->OnRspError(&m_RspInf,-1,true);
		}
	}
	
	return false;
}

void  TdApiImpl::Init()
{
	if (GetRandSvr() && m_SockClient.StartClient(NULL, m_sFront.c_str(), m_sPort.c_str(), AF_INET, m_iSockType))
	{
		SendPrePkt();
		m_pSpi->OnFrontConnected();
	}
	else
	{
		m_RspInf.ErrorID = 2;
		StringCbCopyA(m_RspInf.ErrorMsg,81,"建立连接失败!");
		m_pSpi->OnRspError(&m_RspInf,-1,true);
	}
	
}	

int TdApiImpl::Join()
{ 
	int iRet=WaitForSingleObject(m_SockClient.GetHandle(),INFINITE);
	return iRet; 
}

const char* TdApiImpl::GetTradingDay()
{

	return m_sTdday;
}

void TdApiImpl::RegisterFront(char *pszFrontAddress)
{
	m_SvrVec.insert(m_SvrVec.end(),pszFrontAddress);
}
void TdApiImpl::RegisterNameServer(char *pszNsAddress)
{

}

void TdApiImpl::RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo)
{

}

void  TdApiImpl::RegisterSpi(CThostFtdcTraderSpi *pSpi)
{
     InterlockedExchangePointer(reinterpret_cast<void**>(&m_pSpi), pSpi);
}

void TdApiImpl::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType)
{

}

void TdApiImpl::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType)
{
}

int TdApiImpl::ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID) 
{ 
	int iRet=0; 
	return iRet; 
}

void TdApiImpl::FillReqHdr(PFTDHDREX pHdr,int iType,int iReqId)
{
	//此方法灵活的取代每个REQ时用到的报文数组,方便维护
	 switch (iType)
	 {
		 case T_REQ_LOGIN:
		{

 			break;
		}
		 case T_SETCONFIRM:
		{
				 
			break;
		}
		 case T_QRY_ORDER:
		{
				 
			break;
		}
		 case T_QRY_TRADE:
		{
				 
			break;
		}
		 case T_QRY_INVPOS:
		{
				 
			break;
		}
		 case T_QRY_TDACC:
		{
				 
			break;
		}
		 case T_QRY_INST:
		{
				 
			break;
		}
		 case T_QRY_MD:
		{
				 
			break;
		}
		 case T_QRY_TDCODE:
		{
				 
			break;
		}
		 case T_QRY_MGR:
		{
				 
			break;
		}
		 case T_QRY_FEE:
		{
				 
			break;
		}
		 case T_REQ_ORDINSERT:
		{
				 
			break;
		}
		 case T_REQ_CANCEL:
		{
				 
			break;
		}
		 case T_REQ_MODPASS:
		{
				 
			break;
		}
		 case T_QRY_CFMMC:
		{
				 
			break;
		}
		 case T_QRY_SMI:
		{
				 
			break;
		}
		 case T_REQ_BFT:
		{
				 
			break;
		}
		 case T_QRY_BKYE:
		{
				 
			break;
		}
		 case T_QRY_BFLOG:
		{
				 
			break;
		}
		 case T_QRY_ACCREG:
		{
				 
			break;
		}
		 default:
			 break;
	 }
}

int TdApiImpl::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) 
{ 
	static BYTE bLoginPkt[FTD_HDRLEN] = { FTDTypeFTDC,0,0,0xfe,1,0,8,CHAIN_LAST,0,0,0,0,0x30,0,0,0,0,0,0,5,0,0xe8,0,0,0,0,0x10,2,0,0xbc};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcReqUserLoginField)+sizeof(bQuickPkt);
	m_bPktSend.resize(iLen);
	
	*(DWORD*)(bLoginPkt+0x16) = htonl(nRequestID);
	GetToday(pReqUserLoginField->TradingDay,9);
	if (!strlen(pReqUserLoginField->UserProductInfo))
	{ StringCbCopyA(pReqUserLoginField->UserProductInfo,11,PROD_INFO); }
	if (!strlen(pReqUserLoginField->InterfaceProductInfo))
	{ StringCbCopyA(pReqUserLoginField->InterfaceProductInfo,11,API_INFO); }
	if (!strlen(pReqUserLoginField->ProtocolInfo))
	{ StringCbCopyA(pReqUserLoginField->ProtocolInfo,11,PROT_INFO); }
	
	StringCbCopyA(pReqUserLoginField->MacAddress,21,m_sMac);
	
	memcpy(&m_bPktSend[0],bLoginPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pReqUserLoginField,sizeof(CThostFtdcReqUserLoginField));
	memcpy(&m_bPktSend[FTD_HDRLEN+sizeof(CThostFtdcReqUserLoginField)],bQuickPkt,sizeof(bQuickPkt));
	
	return SendPkt(&m_bPktSend[0],iLen); 
}
	
int TdApiImpl::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) 
{ 
	int iRet=0; 
	
	return iRet; 
}

int TdApiImpl::ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder, int nRequestID) 
{
	static BYTE bOrdInsPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0xc4,1,0,8,CHAIN_LAST,0,1,0,0,0x40,0,0,0,0,2,0,1,0,0xae,0,0,0,0,4,0,0,0xaa};
	DWORD iLen  = FTD_HDRLEN+sizeof(CThostFtdcInputOrderField);
	m_bPktSend.resize(iLen);

	*(DWORD*)(bOrdInsPkt+0x16) = htonl(nRequestID);
	
	SwapDouble(&pInputOrder->LimitPrice);
	pInputOrder->VolumeTotalOriginal = htonl(pInputOrder->VolumeTotalOriginal);

	StringCbPrintfA(pInputOrder->OrderRef,13,"%012d", ++m_iOrdRef);
	
	pInputOrder->CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	pInputOrder->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	pInputOrder->IsAutoSuspend = 0; 
	pInputOrder->UserForceClose = 0;
	
	memcpy(&m_bPktSend[0],bOrdInsPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pInputOrder,sizeof(CThostFtdcInputOrderField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID) 
{
	return 0;
}

int TdApiImpl::ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID) 
{
	// FrontID + SessionID + OrdRef + InstrumentID 或ExchangeID+OrdSysID
	static BYTE bCancelPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0xa9,1,0,8,CHAIN_LAST,0,1,0,0,0x40,6,0,0,0,0x12,0,1,0,0x93,0,0,0,0,4,4,0,0x8f};
	DWORD iLen  = FTD_HDRLEN+sizeof(CThostFtdcParkedOrderActionField);
	m_bPktSend.resize(iLen);
	*(DWORD*)(bCancelPkt+0x16) = htonl(nRequestID);

	//uncomp
	pInputOrderAction->FrontID = htonl(pInputOrderAction->FrontID);
	pInputOrderAction->SessionID = htonl(pInputOrderAction->SessionID);
	pInputOrderAction->ActionFlag = THOST_FTDC_AF_Delete;
	
	memcpy(&m_bPktSend[0],bCancelPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pInputOrderAction,sizeof(CThostFtdcInputOrderField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID) 
{ 
	static BYTE bConfPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0x44,1,0,8,CHAIN_LAST,0,1,0,0,0x40,0x13,0,0,0,1,0,1,0,0x2e,0,0,0,0,4,0x0f,0,0x2a};
	DWORD iLen  = FTD_HDRLEN+sizeof(CThostFtdcSettlementInfoConfirmField);
	m_bPktSend.resize(iLen);
	
	*(DWORD*)(bConfPkt+0x16) = htonl(nRequestID);
	
	memcpy(&m_bPktSend[0],bConfPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pSettlementInfoConfirm,sizeof(CThostFtdcSettlementInfoConfirmField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID) 
{
	static BYTE bQryOrd[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0x81,1,0,8,CHAIN_LAST,0,4,0,0,0x80,0,0,0,0,2,0,1,0,0x6b,0,0,0,0,7,0,0,0x67};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcQryOrderField);
	m_bPktSend.resize(iLen);

	*(DWORD*)(bQryOrd+0x16) = htonl(nRequestID);

	memcpy(&m_bPktSend[0],bQryOrd,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pQryOrder,sizeof(CThostFtdcQryOrderField));

	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID) 
{ 
	static BYTE bQryTd[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0x81,1,0,8,CHAIN_LAST,0,4,0,0,0x80,2,0,0,0,3,0,1,0,0x6b,0,0,0,0,7,1,0,0x67};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcQryTradeField);
	m_bPktSend.resize(iLen);

	*(DWORD*)(bQryTd+0x16) = htonl(nRequestID);
	
	memcpy(&m_bPktSend[0],bQryTd,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pQryTrade,sizeof(CThostFtdcQryTradingAccountField));

	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) 
{
	static BYTE bTdAccPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0x32,1,0,8,CHAIN_LAST,0,4,0,0,0x80,6,0,0,0,0,0,1,0,0x1c,0,0,0,0,7,3,0,0x18};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcQryTradingAccountField);
	m_bPktSend.resize(iLen);
	
	*(DWORD*)(bTdAccPkt+0x16) = htonl(nRequestID);
	
	memcpy(&m_bPktSend[0],bTdAccPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pQryTradingAccount,sizeof(CThostFtdcQryTradingAccountField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID) 
{	
	static BYTE bQryMgrPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0x52,1,0,8,CHAIN_LAST,0,4,0,0,0x80,0xe,0,0,0,8,0,1,0,0x3c,0,0,0,0,7,7,0,0x38};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcQryInstrumentMarginRateField);
	m_bPktSend.resize(iLen);
	*(DWORD*)(bQryMgrPkt+0x16) = htonl(nRequestID);

	memcpy(&m_bPktSend[0],bQryMgrPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pQryInstrumentMarginRate,sizeof(CThostFtdcQryInstrumentMarginRateField));
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID) 
{
	static BYTE bQryFeePkt[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0xa9,1,0,8,CHAIN_LAST,0,1,0,0,0x40,6,0,0,0,0x12,0,1,0,0x93,0,0,0,0,4,4,0,0x8f};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcQryInstrumentCommissionRateField);
	m_bPktSend.resize(iLen);
	*(DWORD*)(bQryFeePkt+0x16) = htonl(nRequestID);

	memcpy(&m_bPktSend[0],bQryFeePkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pQryInstrumentCommissionRate,sizeof(CThostFtdcQryInstrumentCommissionRateField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID) 
{
	static BYTE bQryInstPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0x80,1,0,8,CHAIN_LAST,0,4,0,0,0x80,0x2e,0,0,0,1,0,1,0,0x6a,0,0,0,0,7,0x17,0,0x66};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcQryInstrumentField);
	m_bPktSend.resize(iLen);
	*(DWORD*)(bQryInstPkt+0x16) = htonl(nRequestID);
	
	memcpy(&m_bPktSend[0],bQryInstPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pQryInstrument,sizeof(CThostFtdcQryInstrumentField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryInvestorProductGroupMargin(CThostFtdcQryInvestorProductGroupMarginField *pQryInvestorProductGroupMargin, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryExchangeMarginRate(CThostFtdcQryExchangeMarginRateField *pQryExchangeMarginRate, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryExchangeMarginRateAdjust(CThostFtdcQryExchangeMarginRateAdjustField *pQryExchangeMarginRateAdjust, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID) 
{ 
	static BYTE bQryTfs[FTD_HDRLEN] = {FTDTypeFTDC,0,2,0x36,1,0,8,CHAIN_LAST,0,1,0,1,0x82,0,0,0,0,0,0,1,0,0x20,0,0,0,0,0x30,0xd,0,0x1c};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcQryTransferSerialField);
	m_bPktSend.resize(iLen);
	
	*(DWORD*)(bQryTfs+0x16) = htonl(nRequestID);
	
	memcpy(&m_bPktSend[0],bQryTfs,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pQryTransferSerial,sizeof(CThostFtdcQryTransferSerialField));
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID) 
{
	static BYTE bAccregPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,0,0x36,1,0,8,CHAIN_LAST,0,4,0,0,0x82,2,0,0,0,0,0,1,0,0x20,0,0,0,0,0x30,0x11,0,0x1c};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcQryAccountregisterField);
	m_bPktSend.resize(iLen);
	
	*(DWORD*)(bAccregPkt+0x16) = htonl(nRequestID);
	
	memcpy(&m_bPktSend[0],bAccregPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pQryAccountregister,sizeof(CThostFtdcQryAccountregisterField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID) 
{ int iRet=0; return iRet; }

int TdApiImpl::ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) 
{ 
	static BYTE bBftPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,2,0xc3,1,0,8,CHAIN_LAST,0,1,0,1,0x80,0xb,0,0,0,0,0,1,2,0xad,0,0,0,0,0x28,7,2,0xa9};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcReqTransferField);
	m_bPktSend.resize(iLen);
	
	*(DWORD*)(bBftPkt+0x16) = htonl(nRequestID);

	StringCbCopyA(pReqTransfer->TradeCode,7,"202001");
	StringCbCopyA(pReqTransfer->BankBranchID,5,"0000");
	StringCbCopyA(pReqTransfer->CurrencyID,4,"RMB");

	pReqTransfer->SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;
	
	memcpy(&m_bPktSend[0],bBftPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pReqTransfer,sizeof(CThostFtdcReqTransferField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) 
{ 
	static BYTE bBftPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,2,0xc3,1,0,8,CHAIN_LAST,0,1,0,1,0x80,0xd,0,0,0,0,0,1,2,0xad,0,0,0,0,0x28,7,2,0xa9};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcReqTransferField);
	m_bPktSend.resize(iLen);
	
	*(DWORD*)(bBftPkt+0x16) = htonl(nRequestID);
	StringCbCopyA(pReqTransfer->TradeCode,7,"202002");
	StringCbCopyA(pReqTransfer->BankBranchID,5,"0000");
	StringCbCopyA(pReqTransfer->CurrencyID,4,"RMB");

	pReqTransfer->SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;
	
	memcpy(&m_bPktSend[0],bBftPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pReqTransfer,sizeof(CThostFtdcReqTransferField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

int TdApiImpl::ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) 
{
	static BYTE bQryBkPkt[FTD_HDRLEN] = {FTDTypeFTDC,0,2,0x20,1,0,8,CHAIN_LAST,0,1,0,1,0x80,0x19,0,0,0,0,0,1,2,0xa,0,0,0,0,0x28,0x19,2,6};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcReqQueryAccountField);
	m_bPktSend.resize(iLen);
	
	*(DWORD*)(bQryBkPkt+0x16) = htonl(nRequestID);
	
	StringCbCopyA(pReqQueryAccount->TradeCode,7,"204002");
	StringCbCopyA(pReqQueryAccount->BankBranchID,5,"0000");
	StringCbCopyA(pReqQueryAccount->CurrencyID,4,"RMB");
	
	pReqQueryAccount->SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;
	memcpy(&m_bPktSend[0],bQryBkPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pReqQueryAccount,sizeof(CThostFtdcReqQueryAccountField));
	
	return SendPkt(&m_bPktSend[0],iLen);
}

// SocketClient Interface handler
void TdApiImpl::OnThreadBegin(CSocketHandle* pSH)
{
	ASSERT(pSH == m_SockClient);
	(pSH);	
}

void TdApiImpl::OnThreadExit(CSocketHandle* pSH)
{
	ASSERT(pSH == m_SockClient);
	(pSH);	
}

void TdApiImpl::ParsePkt(const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr)
{
	//这里解析各种报文类型 按照报头的关键信息 (总长度和偏移qry的响应为多个记录时要仔细)
	//先解码并激发回调类的虚函数 每次读出的数据不为各结构体大小的整数倍时 需等继续读出形成整数倍
}

void TdApiImpl::OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr)
{
	ASSERT(pSH == m_SockClient);
	(pSH);	
	if (dwCount > 20)
	{
		ParsePkt(pbData, dwCount,addr);
	}
	else
	{
		if (!memcmp(bHBeatPkt, pbData, 6)) { SendHBeatPkt(); }
	}
}

void TdApiImpl::OnConnectionDropped(CSocketHandle* pSH)
{
	ASSERT(pSH == m_SockClient);
	(pSH);	
	m_pSpi->OnFrontDisconnected(0x1001);
}

void TdApiImpl::OnConnectionError(CSocketHandle* pSH, DWORD dwError)
{
	ASSERT(pSH == m_SockClient);
	(pSH);	
	_com_error err(dwError);
	m_pSpi->OnFrontDisconnected(0x1002);
}

int TdApiImpl::SendPrePkt()
{
	return SendPkt(bUnkPkt,10);	
}

int TdApiImpl::SendHBeatPkt()
{
	return SendPkt(bHBeatPkt,6);	
}

int TdApiImpl::SendPkt(const BYTE* pbData, DWORD dwCount)
{
	int iRet=0;
	DWORD dwWt = m_SockClient.Write((const LPBYTE)pbData, dwCount, NULL);
	if (dwWt!=dwCount) { iRet=-1; }
	
	return iRet;
}