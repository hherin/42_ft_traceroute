#include "../inc/traceroute.h"

static void get_options(char *av)
{
    if (!ft_strncmp(av, "--help", 6))
    {
        usage();
    }
    else
    {
        if (av[0] == '-')
        {
            fprintf(stderr, "ft_traceroute: invalid option -- '%c'\nTry './ft_traceroute --help' for more information.\n", av[1]);
            exit(2);
        }
        else
        {
            return;
        }
    }
}

void parse_options(char **av, t_trace *data)
{
    int i = 0;

    while (av[i] && av[i][0] == '-')
    {
        get_options(av[i]);
        i++;
    }
    while (av[i])
    {
        data->srvname = av[i];
        i++;
    }

    if (!data->srvname)
        usage();
}