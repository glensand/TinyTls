#include "TlsStream.h"
#include "TinyTls.h"
#include "Foundation.h"

namespace TinyTls
{

class FTlsStream final : public FStream
{
public:
    virtual bool Connect(const char* Ip, const char* Port) override
    {
        if (!ConnectTcp(Ip, Port))
        {
            UE_LOG(LogTinyTls, Error,
                TEXT("TCP error: Could not connect to the specified Host[%hs]Port[%hs]"), Ip, Port);
            return false;
        }

        if (!SetupContext())
        {
            UE_LOG(LogTinyTls, Error, TEXT("OpenSSL error: Could not initialize SSL context"));
            return false;
        }
        
        Ssl = SSL_new(SslContext);
        SSL_set_fd(Ssl, (int32)Socket);

        const int32 Result = SSL_connect(Ssl);

        UE_LOG(LogTinyTls, Display, TEXT("%s TLS connection Host[%hs]Port[%hs]"),
            Result > 0 ? TEXT("Establisted") : TEXT("Could not establish"), Ip, Port);
        
        return Result > 0;
    }

    virtual void Disconnect() override
    {
        if (Ssl != nullptr)
        {
            SSL_shutdown(Ssl);
            SSL_free(Ssl);
            SSL_CTX_free(SslContext);

            Ssl = nullptr;
        }

        DisconectTcp();
    }

    virtual bool SetReceiveTimeout(int32 TimeOut) override
    {
        const auto Time = ConvertTime(TimeOut);
        const auto Error = setsockopt(Socket,
            SOL_SOCKET, SO_RCVTIMEO, (char*)&Time, sizeof(struct timeval));

        UE_CLOG(Error != 0, LogTinyTls, Error, TEXT("TCP error: could not set receive timeout"));
        return Error == 0;
    }

    virtual bool SetSendTimeout(int32 TimeOut) override
    {
        const auto Time = ConvertTime(TimeOut);
        const auto Error = setsockopt(Socket,
            SOL_SOCKET, SO_SNDTIMEO, (char*)&Time, sizeof(struct timeval));

        UE_CLOG(Error != 0, LogTinyTls, Error, TEXT("TCP error: could not set send timeout"));
        return Error == 0;
    }

    virtual bool Send(const void* Data, int32 Size) const override
    {
        if (!bConnected)
        {
            UE_LOG(LogTinyTls, Error,
                TEXT("TCP error: an attempt was made to send data, but the connection is not establised"));
            return false;
        }

        int32 Total = 0;
        do
        {
            const int32 Sent = SSL_write(Ssl, (char*)Data + Total, Size - Total);
            Total = Sent < 0 ? -1 : Total + Sent;
        }
        while (Total < Size);

        return true;
    }

    virtual int32 Receive(void* Data, int32 Size) const override
    {
        if (!bConnected)
        {
            UE_LOG(LogTinyTls, Error,
                TEXT("TCP error: an attempt was made to receive data, but the connection is not establised"));
            return -1;
        }

        int32 Total = 0;
        do
        {
            const int32 Received = SSL_read(Ssl, (char*)Data + Total, Size - Total);
            if (Received <= 0)
            {
                UE_LOG(LogTinyTls, Warning,
                    TEXT("TCP or SSL error: lost of connection or timeout while reading"));
                break;
            }

            Total += Received;
        }
        while (Total < Size);

        return Total;
    }

private:
    bool SetupContext()
    {
        auto* ContextMethod = TLS_client_method();
        check(ContextMethod);
        SslContext = SSL_CTX_new(ContextMethod);
        return SslContext != nullptr;
    }

    bool ConnectTcp(const char* Ip, const char* Port)
    {
        if (bConnected)
        {
            UE_LOG(LogTinyTls, Warning, TEXT("TCP error: An attempt was made to esteblish conneciton,"
                " but another connection is still in live"));
            Disconnect();
        }

        addrinfo* ResultAddrInfo{ nullptr };
        addrinfo HintsAddrInfo{ };
        ZeroMemory(&HintsAddrInfo, sizeof(HintsAddrInfo));
        HintsAddrInfo.ai_family = AF_INET;
        HintsAddrInfo.ai_socktype = SOCK_STREAM;
        HintsAddrInfo.ai_protocol = IPPROTO_TCP;

        // Resolve the server address and port
        const int32 Result = getaddrinfo(Ip, Port, &HintsAddrInfo, &ResultAddrInfo);
        struct FFreeAddressInfo final  // NOLINT(cppcoreguidelines-special-member-functions)
        {
            ~FFreeAddressInfo()
            {
                if (AddressInfo)
                {
                    freeaddrinfo(AddressInfo);
                }
            }
            addrinfo* AddressInfo;
        } FreeAddressInfo{ ResultAddrInfo };

        if (Result != 0)
        {
            UE_LOG(LogTinyTls, Error, TEXT("TCP error: Could not resolve address[%hs]"), Ip);
            return false;
        }

        Socket = INVALID_SOCKET;
        // Attempt to connect to an address until one succeeds
        for (const auto* AddressInfo = ResultAddrInfo; 
            AddressInfo != nullptr && Socket == INVALID_SOCKET; AddressInfo = AddressInfo->ai_next)
        {
            Socket = socket(AddressInfo->ai_family, AddressInfo->ai_socktype, AddressInfo->ai_protocol);
            if (Socket != INVALID_SOCKET) 
            {
                int32 On = 1; // Nagle's algorithm 'cause we don need it here, tls is not for "big data"
                int32 Error = setsockopt(Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&On, sizeof(On));
                if (Error == 0)
                {
                    Error = connect(Socket, AddressInfo->ai_addr, (int)AddressInfo->ai_addrlen);
                }

                if (Error == SOCKET_ERROR)
                {
                    closesocket(Socket);
                    Socket = INVALID_SOCKET;
                }
            }
        }

        if (Socket == INVALID_SOCKET)
        {
            UE_LOG(LogTinyTls, Error, TEXT("TCP error: Could not connect socket[%hs]"), Ip);
            return false;
        }

        bConnected = Result != SOCKET_ERROR;

        return bConnected;
    }

    void DisconectTcp()
    {
        if (bConnected)
        {
            bConnected = false;
            closesocket(Socket);
            Socket = INVALID_SOCKET;
        }
    }

    static timeval ConvertTime(int32 Time)
    {
        timeval Converted;      
        Converted.tv_sec = 0;
        Converted.tv_usec = Time * 1000;
        return Converted;
    }
    
    SOCKET Socket{ INVALID_SOCKET };
    SSL* Ssl{ nullptr };
    SSL_CTX* SslContext{ nullptr };

    bool bConnected{ false };
};

FStream* CreateTlsStream()
{
    return new FTlsStream();
}
    
}
