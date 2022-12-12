#include "../inc/traceroute.h"

static void client_setup(t_trace *data);
static void route_tracing(t_trace* data);

int main(int ac, char **av)
{
    if (ac < 2)
        usage();

    t_trace data = {
        .sendsock = -1,
        .recvsock = -1,
        .nb_probe = 64,
        .p_ip_addr = false,
    };

    parse_options(&av[1], &data);

    client_setup(&data);

    route_tracing(&data);
    
    freeaddrinfo(data.send_addinfo);
    return 0;
}

static void client_setup(t_trace *data)
{
    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_RAW,
        .ai_protocol = IPPROTO_UDP,
    };

    int retaddinfo = getaddrinfo(data->srvname, NULL, &hints, &data->send_addinfo);
    if (retaddinfo)
    {
        str_exit_error(gai_strerror(retaddinfo), "ft_traceroute:", data, 1);
    }
    if ((data->recvsock = socket(AF_INET, SOCK_RAW, ICMP_PROTO)) < 0)
    {
        str_exit_error(strerror(errno), "Internal error:", data, 1);
    }
    if ((data->sendsock = socket(AF_INET, SOCK_DGRAM, UDP_PROTO)) < 0)
    {
        str_exit_error(strerror(errno), "Internal error:", data, 1);
    }
}

static void route_tracing(t_trace* data)
{
    char *dst= get_ip_addr((struct sockaddr_in*)data->send_addinfo->ai_addr);
    printf("ft_traceroute to %s (%s), %d hops max", data->srvname, dst, data->nb_probe);

    while (data->nb_probe--)
    {
        send_udp(data);

        select_recv_udp(data);
    }
}


