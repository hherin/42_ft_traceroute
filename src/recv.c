#include "../inc/traceroute.h"

static void recv_msg(t_trace *data);
static void print_stat(t_trace* data, struct timeval rtime, struct icmp icmp);

void select_recv_udp(t_trace *data)
{ 
    static int i = 1;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(data->recvsock, &readfds);
    
    struct timeval timeout = {3, 0};
    int ret = select(1024, &readfds, NULL, NULL, &timeout);
    if (data->next)
    {
        printf("\n%-2d ", i++);
    }
    if (ret > 0)
    {
        if (FD_ISSET(data->recvsock, &readfds))
        {
            recv_msg(data);
        }
    }
    else if (!ret)
    {
        printf("* ");
    }
    else
    {
        str_exit_error(strerror(errno), "Internal error: select failed", data, 1);
    }
}

static void recv_msg(t_trace *data)
{
    t_recvpacket pck;
    ft_bzero(&pck, sizeof(pck));

    struct sockaddr_in recvadd;
    socklen_t size_addr = sizeof(recvadd);
    ft_bzero(&recvadd, sizeof(recvadd));

    int ret = recvfrom(data->recvsock, &pck, sizeof(pck), MSG_DONTWAIT, (struct sockaddr*)&recvadd, &size_addr);
    if ( ret > 0)
    {
        struct timeval rtime;
        ft_bzero(&rtime, sizeof(rtime));
        gettimeofday(&rtime, NULL);
        if (data->next)
        {
            printf("%s ", get_ip_addr(&recvadd));
        }
        print_stat(data, rtime, pck.icmp);
        // printf("%.2fms ", diff_rtt(data->sendtime, rtime));
            // printf("%d ", htons(pck.udp.dest));
        if (pck.icmp.icmp_type == ICMP_DEST_UNREACH)
        {
            printf("ICMP UNREACH\n");
            freeaddrinfo(data->send_addinfo);
            exit(0);
        }
    }
    else
    {
        if (errno == EAGAIN)
            return;
        str_exit_error(strerror(errno), "Internal error: recvfrom failed :", data, 1);
    }


}

static void print_stat(t_trace* data, struct timeval rtime, struct icmp icmp)
{
    if (icmp.icmp_type == ICMP_TIME_EXCEEDED)
    {
        printf("%.2fms ", diff_rtt(data->sendtime, rtime));
    }
    else if (icmp.icmp_type == ICMP_DEST_UNREACH)
    {
        switch (icmp.icmp_code)
        {
            case ICMP_HOST_UNREACH:
                printf("!H ");
                break;
            case ICMP_NET_UNREACH:
                printf("!N ");
                break;
            case ICMP_PROT_UNREACH:
                printf("!P ");
                break;
            case ICMP_SR_FAILED:
                printf("!S ");
                break;
            case ICMP_FRAG_NEEDED:
                printf("!F ");
                break;
            case ICMP_PKT_FILTERED:
                printf("!X ");
                break;
            case ICMP_PREC_VIOLATION:
                printf("!V ");
                break;
            case ICMP_PREC_CUTOFF:
                printf("!C ");
                break;
            case ICMP_PORT_UNREACH:
                printf("%.2fms ", diff_rtt(data->sendtime, rtime));
                break;
            default:
                printf("!%d ", icmp.icmp_code);
                break;
        }
    }
}