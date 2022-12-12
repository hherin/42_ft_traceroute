#include "../inc/traceroute.h"

static void get_options(char *av, int ac)
{
    if (!ft_strncmp(av, "-help", 6))
    {
        usage();
    }
    else
    {
        if (av[0] == '-')
        {
            fprintf(stderr, "Bad option '-%c' (argc %d)\n", av[1], ac + 1);
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
        get_options(av[i], i + 1);
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