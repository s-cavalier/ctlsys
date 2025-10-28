#ifndef __IO_LAYER_H__
#define __IO_LAYER_H__
#include <iostream>
#include <streambuf>
#include <array>
#include <linux/input.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

namespace IOLayer {

    class InputBuffer : public std::streambuf {
        static constexpr size_t buf_size = sizeof(input_event) * 64;
        std::array<char, buf_size> buffer;
        size_t fd;

    protected:
        int_type underflow() override {
            if (gptr() < egptr()) return traits_type::to_int_type(*gptr());

            size_t n = read(fd, buffer.data(), buffer.size());
            if (n == 0) return traits_type::eof();
            if (n == -1) throw std::runtime_error(std::string("Inputbuf failed read: ") + strerror(errno));

            char* base = buffer.data();
            setg(base, base, base + n);

            return traits_type::to_int_type(*gptr());
        };

    public:
        InputBuffer(const char* device) {
            char* buf = buffer.data();
            setg(buf, buf, buf);
            fd = open(device, O_RDONLY | O_NONBLOCK);

            if (fd == -1) throw std::runtime_error( std::string("failed to open device: ") + strerror(errno) );
        }
        
        ~InputBuffer() { close(fd); }

        InputBuffer(const InputBuffer& other) = delete;
        InputBuffer(InputBuffer&& other) = delete;
        InputBuffer& operator=(const InputBuffer& other) = delete;
        InputBuffer& operator=(InputBuffer&& other) = delete;
    };


    class InputStream : public std::istream {
        InputBuffer buf;

    public:
        InputStream(const char* device);

        friend InputStream& operator>>(InputStream& is, input_event& inp);
    };

    InputStream& operator>>(InputStream& is, input_event& inp);

}

#endif