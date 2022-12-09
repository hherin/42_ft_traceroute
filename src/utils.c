#include "../inc/traceroute.h"


void str_exit_error(const char *strerr, char *buf, t_trace *data, int exitcode)
{
    fprintf(stderr, "%s %s\n", buf, strerr);

    freeaddrinfo(data->send_addinfo);
    close(data->sendsock);
    close(data->recvsock);
    exit(exitcode);
}

char *get_ip_addr(struct sockaddr_in *sin)
{
    static char dst[INET_ADDRSTRLEN];
    ft_bzero(dst, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &sin->sin_addr, dst, INET_ADDRSTRLEN);

    return dst;
}

double diff_rtt(struct timeval stime, struct timeval recvtime)
{
    double s = (double)stime.tv_sec * 1000 + (double)stime.tv_usec / 1000;
    double r = (double)recvtime.tv_sec * 1000 + (double)recvtime.tv_usec / 1000;

    return r - s;
}

void usage()
{
    printf("Usage:\n  traceroute [] host\n");
    printf("Options:\n  --help\t\t\tRead this help and exit\n");
    exit(0);
}