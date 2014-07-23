#pragma once

#include "api/ThostFtdcMdApi.h"
#include "api/ThostFtdcTraderApi.h"
#include "SocketHandle.h"
#include "SocketClientImpl.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "IPHlpApi.lib")

//#define WM_LOGINOK_MSG (WM_USER+1)
#define PROD_INFO "HackCtp"
#define API_INFO "THOST User"
#define PROT_INFO "FTDC 1"

enum {FTDTypeNone=0,FTDTypeCompressed,FTDTypeFTDC};
enum {FTDTagNone=0,FTDTagDatetime,FTDTagCompressMethod,FTDTagSessionState,FTDTagTradedate,FTDTagKeepAlive,FTDTagTarget};
enum {T_REQ_LOGIN=0x1002,T_RSP_LOGIN=0x1003,T_SETCONFIRM=0x40f,T_QRY_ORDER=0x700,T_QRY_TRADE=0x701,T_QRY_INVPOS=0x702,
T_RSP_INVPOS=0xd,T_QRY_TDACC=0x703,T_RSP_TDACC=0xc,T_QRY_INST=0x717,T_RSP_INST=0x03,T_QRY_MD=0x718,T_RSP_MD=0x12,
T_QRY_TDCODE=0x705,T_QRY_MGR=0x707,T_RSP_MGR=0xe,T_QRY_FEE=0x708,T_RSP_TDCODE=0x7,T_REQ_CANCEL=0x404,T_REQ_ORDINSERT=0x400,
T_RTN_ORDER=0x401,T_RTN_TRADE=0x409,T_REQ_MODPASS=0x304,T_REQ_SUBMD=0x2441,T_QRY_CFMMC=0x2503,T_QRY_SMI=0x70c,
T_REQ_BFT=0x2807,T_RSP_BFT=0x2808,T_QRY_BKYE=0x2819,T_RSP_BKYE=0x300b,T_QRY_BFLOG=0x300d,T_RSP_BFLOG=0x100e,
T_QRY_ACCREG=0x3011,T_RSP_ACCREG=0x3012};


enum {T_SEQ_EXH = 0x30010000,T_SEQ_CTP=0xf1020000};
enum {CHAIN_SINGLE='S',CHAIN_FIRST='F',CHAIN_CENTER='C',CHAIN_LAST='L'}; 
enum {FTD_HDRLEN=0x1E};

#pragma pack(1)

typedef struct proto_hdr
{
	BYTE	bFtdtype;
	BYTE	bExLen;
	WORD	wFtdcLen;
}PROHDR,*PPROHDR;

typedef struct ftd_hdr
{
	PROHDR  proh;
	BYTE	bVersion;
	BYTE	bFtdctype;	//类型0为req,3为rsp
	BYTE	bUnEnc;
	BYTE	bChan;		//报文链ASCII
	WORD	wSeqType;	//类别0为req,4为qry
	DWORD	dSeqNo;		//序列号
	DWORD 	dCount;
	WORD 	dFiCounts;
	WORD	wLen0;		//包含(tid+len)长度
	DWORD	dReqId;
}FTDHDR,*PFTDHDR;

typedef struct ftdc_hdr
{
	WORD 	wTid;
	WORD	wLen;		//正文长度
}FTDCHDR,*PFTDCHDR;

typedef struct ftd_hdrEx
{
	FTDHDR 	hdr1;
	FTDCHDR hdr2;
}FTDHDREX,*PFTDHDREX;

#pragma pack()

union DoubleAndChar {
	double dval;
	BYTE ucval[8];
};

inline void SwapDouble(double * dv) {
	DoubleAndChar dac;
	BYTE ucTmp;
	dac.dval = *dv;
	for(int i=0;i<4;i++) {
		ucTmp = dac.ucval[i];
		dac.ucval[i] = dac.ucval[7-i];
		dac.ucval[7-i] = ucTmp;
	}
	*dv = dac.dval;
}


inline void GetToday(char* szDay,int nSize)
{
	SYSTEMTIME tm;
	::GetLocalTime(&tm);
	//if (tm.wHour>16){ tm.wDay +=1;}
	StringCbPrintfA(szDay,nSize,"%4d%02d%02d", tm.wYear, tm.wMonth, tm.wDay);
}

inline size_t uni2ansi(UINT uCodepage,WCHAR *unicodestr, char *ansistr )
{
    int result = 0;
    try
    {
        int needlen = WideCharToMultiByte( uCodepage, 0, unicodestr, -1, NULL, 0, NULL, NULL );
        if( needlen < 0 )
        {
            return needlen;
        }
		
        result = WideCharToMultiByte( uCodepage, 0, unicodestr, -1, ansistr, needlen + 1, NULL, NULL );
        if( result < 0 )
        {
            return result;
        }
        return strlen( ansistr );
    }
    catch( ... )
    {
        OutputDebugString(_T("unicode2ansi ERROR!"));
    }
    return result;
}

inline size_t ansi2uni(UINT uCodepage, char *ansistr,WCHAR *unicodestr)
{
	int result = 0;
	try
	{
		int needlen = MultiByteToWideChar( uCodepage, 0, ansistr, -1, NULL, 0);
		if( needlen < 0 )
		{
			return needlen;
		}
		
		result = MultiByteToWideChar( uCodepage, 0, ansistr, -1, unicodestr, needlen + 1);
		if( result < 0 )
		{
			return result;
		}
		return wcslen( unicodestr );
	}
	catch( ... )
	{
		OutputDebugString(_T("ansi2unicode ERROR!"));
	}
	return result;
}

inline void ProcRcvPkt(const BYTE* bRcv,int& iRcvLen,BYTE* bRet,int iRetLen)
{
	for (int i=0,j=0,k=0;i<iRcvLen,j<iRetLen;i++,j++,k++)
	{
		if (bRcv[i] >=0xe0 && bRcv[i] <=0xef) 
		{
			if (bRcv[i]==0xe0)
			{
				i++;
				if ((i<=iRcvLen-1)&&(bRcv[i]>=0xe0) && (bRcv[i]<=0xef))
				{ bRet[j] = bRcv[i]; k++;}
			}
			else
			{ 
				j += (bRcv[i]-0xe1);
				if (j>iRetLen-1) break;
			}
		}
		else
		{ bRet[j]= bRcv[i];}	
	}
	
	iRcvLen = k;
}