#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>

#if defined(WIN32)
#include <asprintf.h>
#endif

#define DIR_SEPARATOR '/'

int file_exists(const char *file_name)
{
    FILE *file;
    if ((file = fopen(file_name, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int file_has_extension(const char file_path[], const char extension[])
{
    int path_size = strlen(file_path);
    int extension_size = strlen(extension);

    int dot_pos = -1;
    for (int i = 0; i < path_size; i++)
    {
        char item = file_path[i];
        if (item == '.')
        {
            dot_pos = i;
            break;
        }
    }

    char chunk[extension_size];
    for (int i = dot_pos + 1, j = 0; i < path_size; i++, j++)
    {
        chunk[j] = file_path[i];
    }
    int res = strncmp(chunk, extension, extension_size);
    return res;
}

char *above(const char path[], int above_steps)
{
    int path_size = strlen(path);

    // if last char is dir separator we can't go above, so need to return same path
    if (path_size == 0 || path_size == 1)
    {
        return (char *)path;
    }
    assert(above_steps > 0);

    int dir_separator_id = -1;
    int above_steps_counter = 1;
    for (int i = path_size - 1; i >= 1; i--)
    {
        if (path[i] == DIR_SEPARATOR)
        {
            if (above_steps_counter == above_steps)
            {
                dir_separator_id = i;
                break;
            }
            else
            {
                above_steps_counter++;
            }
        }
    }

    if (dir_separator_id == -1)
    {
        printf("WARNING: no dir separator \"%c\" with steps %i found in provided path %s\n",
               DIR_SEPARATOR, above_steps, path);
        return (char *)path;
    }

    int res_size = dir_separator_id + 1;
    char *res = malloc(sizeof(char) * res_size);
    strncpy(res, path, res_size - 1);
    res[res_size] = '\0';
    return res;
}

struct dir_info
{
    char **files;
    size_t length;
};

// https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
struct dir_info get_dir_files(char *path)
{
    DIR *d = opendir(path);
    if (d == NULL)
    {
        perror("No such directory");
        struct dir_info r = {};
        return r;
    }

    struct dirent *dir;
    int files_length = 0;
    while ((dir = readdir(d)) != NULL)
    {
        if (dir->d_type == DT_REG)
        {
            files_length++;
        }
    }
    rewinddir(d);

    int i = 0;
    char *files[files_length];
    while ((dir = readdir(d)) != NULL)
    {
        if (dir->d_type == DT_REG)
        {
            files[i] = (char *)malloc(strlen(dir->d_name) + 1);
            strncpy(files[i], dir->d_name, strlen(dir->d_name));
            i++;
        }
    }
    closedir(d);

    struct dir_info res = {files, files_length};
    return res;
}

char *reverse_string(const char str[])
{
    int size = strlen(str);
    char *res = malloc(sizeof(char) * (size + 1));
    for (int i = size - 1, j = 0; i >= 0; i--, j++)
    {
        res[j] = str[i];
    }
    res[size] = '\0';

    return res;
}


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


int main(int argc, char **argv)
{
    opt_info short_opts = get_short_opts(argc, argv);

    // for (int i = 0; i < short_opts.optc; i++) 
    // {
    //     printf("%s\n", short_opts.optv[i]);
    // }

    opt_info long_opts = get_long_opts(argc, argv);

    for (int i = 0; i < long_opts.optc; i++) 
    {
        printf("%s\n", long_opts.optv[i]);
    }


    // printf("arg are %s\n", test.optv[1]);

    // for (int d = 0; d < test.optc; d++)
    // {
    //     printf("arg are %s\n", test.optv[0]);
    // }
    // char *exe_path = realpath(argv[0], NULL);
    // char *exe_dir_path = above(exe_path, 1);
    // struct dir_info bin_dir = get_dir_files(exe_dir_path);

    // char *file_path;

    // printf("\n%s %i\n", exe_dir_path, file_exists(exe_dir_path));

    // FILE *file;
    // char ch;

    // file = fopen(file_path, "r");

    // if (file == NULL)
    // {
    //     char *error_message;
    //     asprintf(&error_message, "Can't open file at %s", file_path);
    //     perror(error_message);
    //     return 1;
    // }

    // printf("content of this file are \n");

    // do
    // {
    //     ch = fgetc(file);
    //     printf("%c", ch);
    // } while (ch != EOF);

    // printf("\n");
    // fclose(file);

    // return 0;
}