
#pragma once
#pragma warning(push)
#pragma warning(disable:4995)
#include <windows.h>
#include <vector>
#include <crtdbg.h>
#pragma warning(pop)

#include "SocketHandle.h"

class ISocketClientHandler
{
public:
    virtual void OnThreadBegin(CSocketHandle* ) {}
    virtual void OnThreadExit(CSocketHandle* ) {}
    virtual void OnDataReceived(CSocketHandle* , const BYTE* , DWORD , const SockAddrIn& ) {}
    virtual void OnConnectionDropped(CSocketHandle* ) {}
    virtual void OnConnectionError(CSocketHandle* , DWORD ) {}
};

template <typename T, size_t tBufferSize = 1440>
class SocketClientImpl
{
    typedef SocketClientImpl<T, tBufferSize> thisClass;
public:
    SocketClientImpl()
    : _pInterface(0)
    , _thread(0)
    {
    }

    void SetInterface(T* pInterface)
    {
        InterlockedExchangePointer(reinterpret_cast<void**>(&_pInterface), pInterface);
    }

    operator CSocketHandle*() throw()
    {
        return( &_socket );
    }

    CSocketHandle* operator->() throw()
    {
        return( &_socket );
    }

	HANDLE GetHandle() const
    {
        return _thread;
    }

    bool IsOpen() const
    {
        return _socket.IsOpen();
    }

    bool CreateSocket(LPCTSTR pszHost, LPCTSTR pszServiceName, int nFamily, int nType, UINT uOptions = 0)
    {
        return _socket.CreateSocket(pszHost, pszServiceName, nFamily, nType, uOptions);
    }
    
    bool ConnectTo(LPCTSTR pszHostName, LPCTSTR pszRemote, LPCTSTR pszServiceName, int nFamily, int nType)
    {
        return _socket.ConnectTo(pszHostName, pszRemote, pszServiceName, nFamily, nType);
    }
    
    void Close()
    {
        _socket.Close();
    }

    DWORD Read(LPBYTE lpBuffer, DWORD dwSize, LPSOCKADDR lpAddrIn = NULL, DWORD dwTimeout = INFINITE)
    {
        return _socket.Read(lpBuffer, dwSize, lpAddrIn, dwTimeout);
    }

    DWORD Write(const LPBYTE lpBuffer, DWORD dwCount, const LPSOCKADDR lpAddrIn = NULL, DWORD dwTimeout = INFINITE)
    {
        return _socket.Write(lpBuffer, dwCount, lpAddrIn, dwTimeout);
    }

    bool StartClient(LPCTSTR pszHost, LPCTSTR pszRemote, LPCTSTR pszServiceName, int nFamily, int nType, UINT uOptions = 0);
    void Terminate(DWORD dwTimeout = INFINITE);

    static bool IsConnectionDropped(DWORD dwError);

protected:
    void Run();
    static DWORD WINAPI SocketClientProc(thisClass* _this);

    T*              _pInterface;
    HANDLE          _thread;
    CSocketHandle   _socket;
};


template <typename T, size_t tBufferSize>
bool SocketClientImpl<T, tBufferSize>::StartClient(LPCTSTR pszHost, LPCTSTR pszRemote, LPCTSTR pszServiceName, int nFamily, int nType, UINT uOptions)
{
    // must be closed first...
    if ( IsOpen() ) return false;
    bool result = false;
    if ( nType == SOCK_STREAM )
    {
        result = _socket.ConnectTo(pszHost, pszRemote, pszServiceName, nFamily, nType); // TCP
    }
    else
    {
        // let provider select a port for us
        result = _socket.CreateSocket(pszHost, TEXT("0"), nFamily, nType, uOptions); // UDP
    }
    if ( result )
    {
        _thread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)SocketClientProc, this,NULL,NULL);
        if ( _thread == NULL )
        {
            DWORD dwError = GetLastError();
            _socket.Close();
            SetLastError(dwError);
            result = false;
        }
    }
    return result;
}

template <typename T, size_t tBufferSize>
void SocketClientImpl<T, tBufferSize>::Run()
{
    SockAddrIn addrIn;
    std::vector<unsigned char> data;
    data.resize( tBufferSize );
    DWORD dwRead;
    DWORD dwError;

    _ASSERTE( _pInterface != NULL && "Need an interface to pass events");

    // Notification: OnThreadBegin
    if ( _pInterface != NULL ) {
        _pInterface->OnThreadBegin(*this);
    }

    int type = _socket.GetSocketType();
    if (type == SOCK_DGRAM) {
        _socket.GetPeerName( addrIn );
    }

    while ( _socket.IsOpen() )
    {
        if (type == SOCK_STREAM)
        {
            dwRead = _socket.Read(&data[0], tBufferSize, NULL, INFINITE);
        }
        else
        {
            dwRead = _socket.Read(&data[0], tBufferSize, addrIn, INFINITE);
        }
        if ( (dwRead != -1L) && (dwRead > 0))
        {
            // Notification: OnDataReceived
            if ( _pInterface != NULL ) {
                _pInterface->OnDataReceived(*this, &data[0], dwRead, addrIn);
            }
        }
        else if (type == SOCK_STREAM && dwRead == 0L )
        {
            // connection broken
            if ( _pInterface != NULL )
            {
                _pInterface->OnConnectionDropped(*this);
            }
            break;
        }
        else if ( dwRead == -1L )
        {
            dwError = GetLastError();
            if ( _pInterface != NULL )
            {
                if (IsConnectionDropped( dwError) ) {
                    // Notification: OnConnectionDropped
                    if (type == SOCK_STREAM || (dwError == WSAENOTSOCK || dwError == WSAENETDOWN))
                    {
                        _pInterface->OnConnectionDropped(*this);
                        break;
                    }
                }
                // Notification: OnConnectionError
                _pInterface->OnConnectionError(*this, dwError);
            }
            else {
                break;
            }
        }
    }

    data.clear();

    // Notification: OnThreadExit
    if ( _pInterface != NULL ) {
        _pInterface->OnThreadExit(*this);
    }
}

template <typename T, size_t tBufferSize>
void SocketClientImpl<T, tBufferSize>::Terminate(DWORD dwTimeout /*= INFINITE*/)
{
    _socket.Close();
    if ( _thread != NULL )
    {
        if ( WaitForSingleObject(_thread, dwTimeout) == WAIT_TIMEOUT ) {
            TerminateThread(_thread, 1);
        }
        CloseHandle(_thread);
        _thread = NULL;
    }
}

template <typename T, size_t tBufferSize>
DWORD WINAPI SocketClientImpl<T, tBufferSize>::SocketClientProc(thisClass* _this)
{
    if ( _this != NULL )
    {
        _this->Run();
    }
    return 0;
}

template <typename T, size_t tBufferSize>
bool SocketClientImpl<T, tBufferSize>::IsConnectionDropped(DWORD dwError)
{
    // see: winerror.h for definition
    switch( dwError )
    {
        case WSAENOTSOCK:
        case WSAENETDOWN:
        case WSAENETUNREACH:
        case WSAENETRESET:
        case WSAECONNABORTED:
        case WSAECONNRESET:
        case WSAESHUTDOWN:
        case WSAEHOSTDOWN:
        case WSAEHOSTUNREACH:
            return true;
        default:
            break;
    }
    return false;
}
