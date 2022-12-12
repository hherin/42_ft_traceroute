#include "../inc/traceroute.h"


void send_udp(t_trace *data)
{
    static int port = 33434, ttl = 1, inc = 1;

    setsockopt(data->sendsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    struct sockaddr_in *sendaddr = (struct sockaddr_in*)data->send_addinfo->ai_addr;
    sendaddr->sin_port = htons(port);
    
    ft_bzero(&data->sendtime, sizeof(data->sendtime));
    gettimeofday(&data->sendtime, NULL);
    if (sendto(data->sendsock, "", 0, MSG_DONTWAIT, (struct sockaddr*)sendaddr, sizeof(*sendaddr)) < 0)
    {
        str_exit_error(strerror(errno), "Internal : sendto fail :", data, 1);
    }

    if (data->block_recv % 3 == 0)
    {
        data->p_ip_addr = false;
    }
    if (!(inc++ % 3))
    {
        ttl++;
        port++;
    }
}



