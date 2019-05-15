#pragma once

#include <Mojo/Core/Stream.h>

inline namespace Mojo
{
    enum struct SocketType
    {
        None,
        Stream,
        Datagram,
    };

    struct Socket : public Stream
    {
        SocketType  type;
        int         handle;
        
        // Connect to server
        static Socket* Connect(const char* host, int port, SocketType type = SocketType::Stream);

        // Create server
        static Socket* CreateServer(const char* host, int port, SocketType type = SocketType::Stream);

        // Close socket
        virtual void Close(void);

        // Set the cursor position of stream
        virtual int Seek(int count, SeekWhence whence = SeekWhence::Current) override;

        // Tell the cursor position of stream
        virtual int Tell(void) override;

        // Get size of stream
        virtual int Size(void) override;

        // Read content of stream
        virtual int Read(void* buffer, int GetLength) override;

        // Write content to stream
        virtual int Write(const void* buffer, int GetLength) override;
    };
}