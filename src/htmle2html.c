#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>

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
            } else {
                above_steps_counter++;
            }
        }
    }

    if (dir_separator_id == -1)
    {
        printf("WARNING: no dir separator \"%c\" with steps %i founded in provided path %s\n",
               DIR_SEPARATOR, above_steps, path);
        return (char *)path;
    }

    int res_size = dir_separator_id + 1;
    char *res = malloc(sizeof(char) * res_size);
    for (int i = 0; i < res_size - 1; i++)
    {
        res[i] = path[i];
    }
    res[res_size] = '\0';
    return res;
}

int main(int argc, char **argv)
{

    FILE *file;
    char ch;

    char *exe_path = realpath(argv[0], NULL);
    char *exe_dir_path = above(exe_path, 1);
    char *file_path;

    printf(exe_dir_path);

    return 0;

    file = fopen(file_path, "r");
    file = fopen(file_path, "r");

    if (file == NULL)
    {
        char *error_message;
        asprintf(&error_message, "Can't open file at %s", file_path);
        perror(error_message);
        return 1;
    }

    printf("content of this file are \n");

    do
    {
        ch = fgetc(file);
        printf("%c", ch);
    } while (ch != EOF);

    printf("\n");
    fclose(file);

    return 0;
}