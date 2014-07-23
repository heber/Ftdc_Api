#include "stdafx.h"
#include "MdApiImpl.h"

static BYTE bUnkPkt[10] = {FTDTypeNone,6,0,0,7,4,0,0,0,0x27};
static BYTE bHBeatPkt[6] = {FTDTypeNone,2,0,0,FTDTagKeepAlive,0};
static BYTE bMdQuick[20]={0x10,1,0,6,0,1,0,0,0,0,0x10,1,0,6,0,4,0,0,0,0};
static BYTE bSubMdSign[4] = {0x24,0x41,0,0x1f};

void  MdApiImpl::Release()
{
	m_SockClient.Terminate();
	CSocketHandle::ReleaseLibrary();
	delete this;
}

MdApiImpl::MdApiImpl()
{
	CSocketHandle::InitLibrary(MAKEWORD(2,2));
	TCHAR szIPAddr[MAX_PATH] = { 0 };
	CSocketHandle::GetLocalAddress(szIPAddr, MAX_PATH, AF_INET);
	char szIP[MAX_PATH] = { 0 };
	#ifdef _UNICODE
		uni2ansi(CP_ACP,szIPAddr,szIP);
	#else
		StringCbCopyA(szIP, MAX_PATH, szIPAddr);
	#endif
	CSocketHandle::GetMacAddress(szIP, m_sMac,sizeof(TThostFtdcMacAddressType));

	m_SockClient.SetInterface(this);
}

bool MdApiImpl::GetRandSvr()
{
	srand((unsigned)time(NULL));
	size_t i=rand()% m_SvrVec.size();

	if (!m_SvrVec[i].empty())
	{
		std::string szPre = m_SvrVec[i].substr(0,3);
		
		if (!szPre.compare("tcp") || !szPre.compare("udp"))
		{
			if (!szPre.compare("tcp")) 
			{  m_iSockType = SOCK_STREAM; }
			else
			{  m_iSockType = SOCK_DGRAM; }
			
			size_t iPos = m_SvrVec[i].rfind(':');
			size_t iPos2 = m_SvrVec[i].find("//");

			m_sFront=m_SvrVec[i].substr(iPos2+2,iPos-iPos2-2);
			m_sPort=m_SvrVec[i].substr(iPos+1);

			return true;
		}
		else
		{
			m_RspInf.ErrorID = 1;
			StringCbCopyA(m_RspInf.ErrorMsg,81,"不支持的连接类型");
			m_pSpi->OnRspError(&m_RspInf,-1,true);
		}
	}	

	return false;
}

void  MdApiImpl::Init()
{
	//仅写了IPv4代码 可支持IPv6
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

int  MdApiImpl::Join()
{
	int iRet=WaitForSingleObject(m_SockClient.GetHandle(),INFINITE);
	return iRet; 
}
	
const char* MdApiImpl::GetTradingDay()
{

	return m_sTdday;
}
	

void  MdApiImpl::RegisterFront(char *pszFrontAddress)
{
	m_SvrVec.insert(m_SvrVec.end(),pszFrontAddress);
}
	
void  MdApiImpl::RegisterNameServer(char *pszNsAddress)
{

}

void  MdApiImpl::RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo)
{

}
	
void  MdApiImpl::RegisterSpi(CThostFtdcMdSpi *pSpi)
{
     InterlockedExchangePointer(reinterpret_cast<void**>(&m_pSpi), pSpi);
}
	
typedef struct OneSubPkt
{
	BYTE bSign[4];
	TThostFtdcInstrumentIDType sInst;
}ONEPKT;

int  MdApiImpl::SubscribeMarketData(char *ppInstrumentID[], int nCount)
{
	static BYTE bSubMdPkt[FTD_HDRLEN-4] = { FTDTypeFTDC,0,0,0x3d,1,0,8,CHAIN_LAST,0,0,0,0,0x44,1,0,0,0,0,0,1/*nCount*/,0,0x27,0,0,0,0/*,0x24,0x41,0,0x1f*/};

	int iOne = sizeof(TThostFtdcInstrumentIDType)+sizeof(bSubMdSign);
	DWORD iLen = FTD_HDRLEN-4+ nCount*iOne;
	m_bPktSend.resize(iLen);

	*(USHORT*)(bSubMdSign+2) = htons(sizeof(TThostFtdcInstrumentIDType));
	*(USHORT*)(bSubMdPkt+2) = htons(iLen-4); //分包?????
	*(USHORT*)(bSubMdPkt+0x12) = htons(nCount);
	*(USHORT*)(bSubMdPkt+0x14) = htons(nCount*iOne);

	memcpy(&m_bPktSend[0],bSubMdPkt,FTD_HDRLEN-4);

	ONEPKT pkt;
	ZeroMemory(&pkt, sizeof(pkt));
	memcpy(pkt.bSign,bSubMdSign,4);

	for (int i=0;i<nCount;i++)
	{
		StringCbCopyA(pkt.sInst,sizeof(TThostFtdcInstrumentIDType),ppInstrumentID[i]);
		memcpy(&m_bPktSend[FTD_HDRLEN-4+iOne*i],&pkt,iOne); 
	}
	
	return SendPkt(&m_bPktSend[0],iLen);  
}

int  MdApiImpl::UnSubscribeMarketData(char *ppInstrumentID[], int nCount)
{
	static BYTE bUnSubMdPkt[FTD_HDRLEN-4] = { FTDTypeFTDC,0,0,0x3d,1,0,8,CHAIN_LAST,0,0,0,0,0x44,3,0,0,0,0,0,1/*nCount*/,0,0x27,0,0,0,0/*,0x24,0x41,0,0x1f*/};

	int iOne = sizeof(TThostFtdcInstrumentIDType)+sizeof(bSubMdSign);
	DWORD iLen = FTD_HDRLEN-4+ nCount*iOne;
	m_bPktSend.resize(iLen);
	
	*(USHORT*)(bSubMdSign+2) = htons(sizeof(TThostFtdcInstrumentIDType));
	*(USHORT*)(bUnSubMdPkt+2) = htons(iLen-4);	//分包?????
	*(USHORT*)(bUnSubMdPkt+0x12) = htons(nCount);
	*(USHORT*)(bUnSubMdPkt+0x14) = htons(nCount*iOne);
	
	memcpy(&m_bPktSend[0],bUnSubMdPkt,FTD_HDRLEN-4);
	
	ONEPKT pkt;
	ZeroMemory(&pkt, sizeof(pkt));
	memcpy(pkt.bSign,bSubMdSign,4);
	
	for (int i=0;i<nCount;i++)
	{
		StringCbCopyA(pkt.sInst,sizeof(TThostFtdcInstrumentIDType),ppInstrumentID[i]);
		memcpy(&m_bPktSend[FTD_HDRLEN-4+iOne*i],&pkt,iOne); 
	}
	
	return SendPkt(&m_bPktSend[0],iLen);   
}	

int  MdApiImpl::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
	static BYTE bLoginPkt[FTD_HDRLEN] = { FTDTypeFTDC,0,0,0xea,1,0,8,CHAIN_LAST,0,0,0,0,0x30,0,0,0,0,0,0,3,0,0xd4,0,0,0,0,0x10,2,0,0xbc};
	DWORD iLen = FTD_HDRLEN+sizeof(CThostFtdcReqUserLoginField)+sizeof(bMdQuick);
	m_bPktSend.resize(iLen);

	*(DWORD*)(bLoginPkt+0x16) = htonl(nRequestID);
	
	GetToday(pReqUserLoginField->TradingDay,9);
	//strcpy(pReqUserLoginField->BrokerID, m_sBkrId);
	if (!strlen(pReqUserLoginField->UserProductInfo))
	{ StringCbCopyA(pReqUserLoginField->UserProductInfo,11,PROD_INFO); }
	if (!strlen(pReqUserLoginField->InterfaceProductInfo))
	{ StringCbCopyA(pReqUserLoginField->InterfaceProductInfo,11,API_INFO); }
	if (!strlen(pReqUserLoginField->ProtocolInfo))
	{ StringCbCopyA(pReqUserLoginField->ProtocolInfo,11,PROT_INFO); }
	
	StringCbCopyA(pReqUserLoginField->MacAddress,21,m_sMac);
	
	memcpy(&m_bPktSend[0],bLoginPkt,FTD_HDRLEN);
	memcpy(&m_bPktSend[FTD_HDRLEN],pReqUserLoginField,sizeof(CThostFtdcReqUserLoginField));
	memcpy(&m_bPktSend[FTD_HDRLEN+sizeof(CThostFtdcReqUserLoginField)],bMdQuick,sizeof(bMdQuick));
	
	return SendPkt(&m_bPktSend[0],iLen); 
}

int  MdApiImpl::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
	int iRet=0;
	
	return iRet;
}

MdApiImpl::~MdApiImpl()
{
	//Release();
}


// SocketClient Interface handler
void MdApiImpl::OnThreadBegin(CSocketHandle* pSH)
{
	ASSERT(pSH == m_SockClient);
	(pSH);	

	//m_pSpi->OnFrontConnected()
}

void MdApiImpl::OnThreadExit(CSocketHandle* pSH)
{
	ASSERT(pSH == m_SockClient);
	(pSH);	
}

void MdApiImpl::ParsePkt(const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr)
{
	//这里解析各种报文类型 按照报头的关键信息 (总长度和偏移qry的响应为多个记录时要仔细)
	//先解码并激发回调类的虚函数 每次读出的数据不为各结构体大小的整数倍时 需等继续读出形成整数倍

}

void MdApiImpl::OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr)
{
	ASSERT(pSH == m_SockClient);
	(pSH);

	if (dwCount > 20)
	{
		//SetStatusTxt(_T("登录成功"),1);
		ParsePkt(pbData, dwCount, addr);
	}
	else
	{
		if (!memcmp(bHBeatPkt, pbData, 6)) { SendHBeatPkt(); }
	}
}

void MdApiImpl::OnConnectionDropped(CSocketHandle* pSH)
{
	ASSERT(pSH == m_SockClient);
	(pSH);

	m_pSpi->OnFrontDisconnected(0x1001);
	//m_pSpi->OnFrontConnected();
}

void MdApiImpl::OnConnectionError(CSocketHandle* pSH, DWORD dwError)
{
	ASSERT(pSH == m_SockClient);
	(pSH);
	_com_error err(dwError);
	
	m_pSpi->OnFrontDisconnected(0x1002);
	//m_pSpi->OnFrontConnected();
}

int MdApiImpl::SendPrePkt()
{
	return SendPkt(bUnkPkt,10);	
}

int MdApiImpl::SendHBeatPkt()
{
	return SendPkt(bHBeatPkt,6);	
}

int MdApiImpl::SendPkt(const BYTE* pbData, DWORD dwCount)
{
	int iRet=0;
	DWORD dwWt = m_SockClient.Write((const LPBYTE)pbData, dwCount, NULL);
	if (dwWt!=dwCount) { iRet=-1; }

	return iRet;
}