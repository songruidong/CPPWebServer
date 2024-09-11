#ifndef IO_URING_H
#define IO_URING_H

#include <liburing.h>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include "task.h"

constexpr size_t MAX_MESSAGE_LEN = 2048;
constexpr size_t BUFFERS_COUNT = 4096;

class io_uring_handler
{
public:
    io_uring_handler(unsigned entries, int sock_listen_fd);
    void event_loop(task func(int));
    void setup_first_buffer();
    ~io_uring_handler();
    void add_read_request(int fd, request &req);
    void add_write_request(int fd, size_t message_size, request &req);
    void add_accept_request(int fd, struct sockaddr *client_addr, socklen_t *client_len, unsigned flags);
    void add_buffer_request(request &req);
    void add_open_request();
    void add_close_request(int fd);

    char* get_buffer_pointer(int bid) {
        return buffer[bid];
    }

    int get_buffer_id(char* buffer) {
        return (buffer - (char*)this->buffer.get()) / MAX_MESSAGE_LEN;
    }

private:

};

io_uring_handler::io_uring_handler(unsigned entries, int sock_listen_fd)
{
    struct io_uring_params params;
    memset(&params, 0, sizeof(params));
    this->sock_listen_fd = sock_listen_fd;

    if (io_uring_queue_init_params(entries, &ring, &params) < 0)
    {
        perror("io_uring_init_failed...\n");
        exit(1);
    }

    // check if IORING_FEAT_FAST_POLL is supported
    if (!(params.features & IORING_FEAT_FAST_POLL))
    {
        printf("IORING_FEAT_FAST_POLL not available in the kernel, quiting...\n");
        exit(0);
    }

    struct io_uring_probe *probe;
    probe = io_uring_get_probe_ring(&ring);
    if (!probe || !io_uring_opcode_supported(probe, IORING_OP_PROVIDE_BUFFERS))
    {
        printf("Buffer select not supported, skipping...\n");
        exit(0);
    }
    free(probe);

    setup_first_buffer();
}

#endif
