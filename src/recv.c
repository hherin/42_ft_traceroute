#include "../inc/traceroute.h"

static void recv_msg(t_trace *data);
static void print_stat(t_trace* data, struct timeval rtime, struct icmp icmp, struct sockaddr_in* sin);

void select_recv_udp(t_trace *data)
{ 
    static int i = 1;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(data->recvsock, &readfds);
    
    struct timeval timeout = {3, 0};
    int ret = select(1024, &readfds, NULL, NULL, &timeout);
    if (!(data->block_recv % 3))
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
        printf("*  ");
        data->block_recv++;
    }
    else
    {
        str_exit_error(strerror(errno), "Internal error: select failed", data, 1);
    }

    if ((data->error) && !(data->block_recv % 3))
    {
        printf("\n");
        end_traceroute(data);
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
    if (ret > 0)
    {
        struct timeval rtime;
        ft_bzero(&rtime, sizeof(rtime));
        gettimeofday(&rtime, NULL);
        print_stat(data, rtime, pck.icmp, &recvadd);
        if ((pck.icmp.icmp_code == ICMP_PORT_UNREACH || data->error) && !(data->block_recv % 3))
        {
            printf("\n");
            end_traceroute(data);
        }
    }
    else
    {
        if (errno == EAGAIN)
            return;
        str_exit_error(strerror(errno), "Internal error: recvfrom failed :", data, 1);
    }
}

static void print_stat(t_trace* data, struct timeval rtime, struct icmp icmp, struct sockaddr_in *rsin)
{
    if (!data->p_ip_addr)
    {
        printf("%s  ", get_ip_addr(rsin));
        data->p_ip_addr = true;
    }
    if (icmp.icmp_type == ICMP_TIME_EXCEEDED)
    {
        printf("%.3fms ", diff_rtt(data->sendtime, rtime));
    }
    else if (icmp.icmp_type == ICMP_DEST_UNREACH)
    {
        switch (icmp.icmp_code)
        {
            case ICMP_HOST_UNREACH:
                printf("!H ");
                data->error = true;
                break;
            case ICMP_NET_UNREACH:
                printf("!N ");
                data->error = true;
                break;
            case ICMP_PROT_UNREACH:
                printf("!P ");
                data->error = true;
                break;
            case ICMP_SR_FAILED:
                printf("!S ");
                data->error = true;
                break;
            case ICMP_FRAG_NEEDED:
                printf("!F ");
                data->error = true;
                break;
            case ICMP_PKT_FILTERED:
                printf("!X ");
                data->error = true;
                break;
            case ICMP_PREC_VIOLATION:
                printf("!V ");
                data->error = true;
                break;
            case ICMP_PREC_CUTOFF:
                printf("!C ");
                data->error = true;
                break;
            case ICMP_PORT_UNREACH:
                printf("%.3fms ", diff_rtt(data->sendtime, rtime));
                break;
            default:
                printf("!%d ", icmp.icmp_code);
                data->error = true;
                break;
        }
    }
    data->block_recv++;
}