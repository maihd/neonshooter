#include <Mojo/Core/Socket.h>

#include <stdio.h>
#include <assert.h>
#include <string.h>

#if _WIN32
#   include <WinSock2.h>
#   include <ws2tcpip.h>
#   pragma comment(lib, "ws2_32.lib")
#   define NATIVE_CLOSE_SOCKET(x) closesocket(x)
#else

#   define SOCKET int
#   define NATIVE_CLOSE_SOCKET(x) close(x)
#endif

namespace Mojo
{
    static bool LazySetup(void)
    {
#if _WIN32
        static bool s_winsock_init;
        if (s_winsock_init)
        {
            int iResult;
            WSADATA wsaData;

            // Initialize Winsock
            iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
            assert(iResult != 0 && "Failed to initialized WinSocket system.");
            s_winsock_init = true;
        }
        return s_winsock_init;
#else
        return true;
#endif
    }

    static int ConvertSocketType(SocketType protocol)
    {
        switch (protocol)
        {
        case SocketType::Stream:
            return SOCK_STREAM;

        case SocketType::Datagram:
            return SOCK_DGRAM;

        default:
            break;
        }

        return 0;
    }

    //U32 Stream::ReadUint32LE(void)
    //{
    //    U32 result;
    //    return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_32(result) : 0;
    //}

    // Connect to server
    Socket* Socket::Connect(const char* host, int port, SocketType type)
    {
        if (!LazySetup())
        {
            return NULL;
        }

        int rv;
        SOCKET sockfd = -1;
        struct addrinfo hints, *servinfo, *p;

        hints = {};
        hints.ai_family     = AF_UNSPEC;
        hints.ai_socktype   = ConvertSocketType(type);

        char port_str[32];
        sprintf(port_str, "%d", port);
        if ((rv = getaddrinfo(host, port_str, &hints, &servinfo)) != 0)
        {
            //console::error("getaddrinfo: %s\n", gai_strerror(rv));
            return NULL;
        }

        // loop through all the results and connect to the first we can
        for (p = servinfo; p != NULL; p = p->ai_next)
        {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            {
                perror("client: socket");
                continue;
            }

            if (connect(sockfd, p->ai_addr, (int)p->ai_addrlen) == -1)
            {
                NATIVE_CLOSE_SOCKET(sockfd);
                perror("client: connect");
                continue;
            }

            break;
        }

        freeaddrinfo(servinfo);
        if (p == NULL)
        {
            //console::error("client: failed to connect\n");
            return NULL;
        }

        // Result
        Socket* socket  = new Socket();
        socket->type    = type;
        socket->handle  = (void*)(UPtr)sockfd;
        return socket;
    }

    // Create server
    Socket* Socket::CreateServer(const char* host, int port, SocketType type)
    {
        if (!LazySetup())
        {
            return NULL;
        }

        int rv;
        SOCKET sockfd = -1;
        struct addrinfo hints, *servinfo, *p;

        hints = {};
        hints.ai_family     = AF_UNSPEC;
        hints.ai_socktype   = ConvertSocketType(type);
        hints.ai_flags      = AI_PASSIVE; // use my IP

        char port_str[32];
        sprintf(port_str, "%d", port);
        if ((rv = getaddrinfo(host, port_str, &hints, &servinfo)) != 0)
        {
            //console::error("getaddrinfo: %s\n", gai_strerror(rv));
            return NULL;
        }

        // loop through all the results and bind to the first we can
        for (p = servinfo; p != NULL; p = p->ai_next)
        {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            {
                perror("server: socket");
                continue;
            }

#if _WIN32  
            char yes = 1;
#else
            int yes = 1;
#endif
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
            {
                perror("setsockopt");
                exit(1);
            }

            if (bind(sockfd, p->ai_addr, (int)p->ai_addrlen) == -1)
            {
                NATIVE_CLOSE_SOCKET(sockfd);
                perror("server: bind");
                continue;
            }

            break;
        }

        freeaddrinfo(servinfo);
        if (p == NULL)
        {
            //console::error("server: failed to bind\n");
            return NULL;
        }

        if (listen(sockfd, 256) == -1)
        {
            NATIVE_CLOSE_SOCKET(sockfd);
            perror("listen");
            return NULL;
        }

        // Result
        Socket* socket  = new Socket();
        socket->type    = type;
        socket->handle  = (void*)(UPtr)sockfd;
        return socket;
    }

    // Close socket
    void Socket::Close(void) 
    {
        NATIVE_CLOSE_SOCKET((SOCKET)(UPtr)handle);

        handle  = 0;
        type    = SocketType::None;
    }

    // Set the cursor position of stream
    int Socket::Seek(int count, SeekWhence whence)
    {
        (void)count;
        (void)whence;
        return 0;
    }

    // Tell the cursor position of stream
    int Socket::Tell(void)
    {
        return 0;
    }

    // Get size of stream
    int Socket::Size(void)
    {
        return 0;
    }

    // Read content of stream
    int Socket::Read(void* buffer, int length)
    {
        return (int)recv((SOCKET)(UPtr)handle, (char*)buffer, length, 0);
    }

    // Write content to stream
    int Socket::Write(const void* buffer, int length)
    {
        return (int)send((SOCKET)(UPtr)handle, (const char*)buffer, length, 0);
    }
}