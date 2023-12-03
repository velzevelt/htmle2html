typedef struct {
    char **optv;
    size_t optc;
} opt_info;

opt_info get_short_opts(int argc, char *argv[])
{
    char **opts = calloc(argc, sizeof(char*));
    int total_size = 0;
    for (int i = 0; i < argc; i++)
    {
        char *arg = argv[i];
        if (arg[0] == '-' && arg[1] != '-')
        {
            int arg_size = strlen(arg);
            opts[total_size] = calloc(arg_size, sizeof(char));
            strncpy(opts[total_size], arg, arg_size);
            total_size++;
        }
    }

    opt_info res = {opts, total_size};
    return res;
}

opt_info get_long_opts(int argc, char *argv[])
{
    char **opts = calloc(argc, sizeof(char*));
    int total_size = 0;
    for (int i = 0; i < argc; i++)
    {
        char *arg = argv[i];
        if (arg[0] == '-' && arg[1] == '-')
        {
            int arg_size = strlen(arg);
            opts[total_size] = calloc(arg_size, sizeof(char));
            strncpy(opts[total_size], arg, arg_size);
            total_size++;
        }
    }

    opt_info res = {opts, total_size};
    return res;
}

opt_info get_just_args(int argc, char *argv[])
{
    char **opts = calloc(argc, sizeof(char*));
    int total_size = 0;
    for (int i = 0; i < argc; i++)
    {
        char *arg = argv[i];
        if (arg[0] != '-' && arg[1] != '-')
        {
            int arg_size = strlen(arg);
            opts[total_size] = calloc(arg_size, sizeof(char));
            strncpy(opts[total_size], arg, arg_size);
            total_size++;
        }
    }

    opt_info res = {opts, total_size};
    return res;
}


