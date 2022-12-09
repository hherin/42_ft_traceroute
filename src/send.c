#include "../inc/traceroute.h"


void send_udp(t_trace *data)
{
    static int port = 33434, ttl = 1, inc = 0;

    data->next = false;
    setsockopt(data->sendsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    struct sockaddr_in *sendaddr = (struct sockaddr_in*)data->send_addinfo->ai_addr;
    sendaddr->sin_port = htons(port);
    
    ft_bzero(&data->sendtime, sizeof(data->sendtime));
    gettimeofday(&data->sendtime, NULL);
    if (sendto(data->sendsock, "", 0, MSG_DONTWAIT, (struct sockaddr*)sendaddr, sizeof(*sendaddr)) < 0)
    {
        str_exit_error(strerror(errno), "Internal : sendto fail :", data, 1);
    }
    
    
    if (++inc == 3)
    {
        data->next = true;
        ttl++;
        port++;
        inc = 0;
    }
}



