#ifndef ft_traceroute_h
# define ft_traceroute_h

// fprintf
# include <stdio.h>
// socket
# include <sys/socket.h>
// gai_strerror
# include <netdb.h>
// struct udp
# include <netinet/udp.h>
// struct icmphdr
# include <netinet/ip_icmp.h>
// struct ip
# include <netinet/ip.h>
// errno
# include <errno.h>
// strerror
# include <string.h>
// select
# include <sys/select.h>
// bool
# include <stdbool.h>
// gettimeofday
# include <sys/time.h>
// inet_ntop
# include <arpa/inet.h>

# include "../lib/inc/libft.h"

# define UDP_PROTO 17
# define ICMP_PROTO 1


typedef struct recvpacket
{
    struct iphdr ip;
    struct icmp icmp;
    struct udphdr udp;
}               t_recvpacket;

typedef struct trace
{
    char *srvname;
    int sendsock;
    int recvsock;
    unsigned int nb_probe;
    struct addrinfo* send_addinfo;
    unsigned int option;
    struct timeval sendtime;
    bool next;

}              t_trace;


void usage();
void parse_options(char **av, t_trace *data);
void select_recv_udp(t_trace *data);
void send_udp(t_trace *data);

void str_exit_error(const char *strerr, char *buf, t_trace *data, int exitcode);
double diff_rtt(struct timeval stime, struct timeval rtime);
char *get_ip_addr(struct sockaddr_in *sin);
void usage();

#endif