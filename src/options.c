#include "../inc/traceroute.h"

static void get_options(char *av, t_trace *data, int ac)
{
    (void)data;
    if (!ft_strncmp(av, "-help", 6))
    {
        usage();
    }
    else
    {
        char opt;
        for (int i = 1; av[i] && (opt = av[i]); i++)
        {
            switch (opt)
            {
                default:
                {
                    fprintf(stderr, "Bad option '-%c' (argc %d)\n", opt, ac);
                    exit(2);
                }
            }
        }
    }

}


void parse_options(char **av, t_trace *data)
{
    for (int i = 0; av[i]; i++)
    {
        if (*av[i] == '-')
        {
            get_options(av[i], data, i + 1);
        }
        else if (ft_isalpha(*av[i]))
        {
            if (data->srvname)
            {
                fprintf(stderr, "ERROR %s\n", data->srvname);
                exit(2);
            }
            data->srvname = *av;
        }
    }
    if (!data->srvname)
        usage();
}