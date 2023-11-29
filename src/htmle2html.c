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

    for (int i = 0; i < 50; i++)
    {
        int count = count_dir_files(cwd);
        printf("Count files in %s\n", cwd);
        printf("Count: %i\n", count);
        sleep(15);
    }

}