#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <cli.c>
#include <filesystem.c>


#if defined(WIN32)
#include <asprintf.h>
#endif


int main(int argc, char **argv)
{
    char *cwd = current_dir_path(argv[0]);
    struct dir_info d = get_dir_files(cwd);

    for (int i = 0; i < d.length; i++)
    {
        printf("%s\n", d.files[i]);
    }

    struct dir_info d2 = get_dir_dir(cwd);

    for (int i = 0; i < d2.length; i++)
    {
        printf("%s\n", d2.files[i]);
    }

    append_files(&d2, &d);

    printf("AFTER APPEND size %i\n", d2.length);
    for (int i = 0; i < d2.length; i++)
    {
        printf("%s\n", d2.files[i]);
    }


    // for (int i = 0; i < 50; i++)
    // {
    //     int count = count_dir_files(cwd);
    //     printf("Count files in %s\n", cwd);
    //     printf("Count: %i\n", count);
    //     sleep(15);
    // }

}