#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <memory.c>
#include <cli.c>
#include <filesystem.c>
#include <htmle_interpreter.c>

#if defined(WIN32)
#include <asprintf.h>
#endif

void compile_rec(int argc, char **argv)
{
    const char *exe_dir = exe_dir_path(argv[0]);
    dir_info exe_dir_info = get_dir_files_rec(exe_dir);

    for (int i = 0; i < exe_dir_info.length; i++)
    {
        const char *file_path = exe_dir_info.files[i];
        const char *file_extension = get_file_extension(file_path);

        if (strncmp(file_extension, "htmle", 5) == 0)
        {
            const char *new_file_path = change_file_extension(file_path, "html");
            printf("Changing file extension\nOld path: %s\nNew path: %s\n", file_path, new_file_path);

            FILE *f = fopen(file_path, "rb");
            if (file_exists_file(f))
            {
                const char *file_content = get_file_contents(f);
                // printf("file content:\n%s\n", file_content);

                const char *interp_out = interp_htmle(file_content, file_path, &exe_dir_info);
                printf("Interp result: %s\n", interp_out);

                if (interp_out != NULL)
                {
                    FILE *new_file = fopen(new_file_path, "wb");
                    fprintf(new_file, interp_out);
                    fclose(new_file);
                    free(interp_out);
                }
                
            }
            else
            {
                fprintf(stderr, "File does not exist at path %s\n", file_path);
            }
            free(new_file_path);
        }
    }
    // free_dir_info(&exe_dir_info);
    free_2deep_pointer(exe_dir_info.files, exe_dir_info.length);
    free(exe_dir);
}

void print_help()
{   
    const char *help_message = 
        "htmle2html [DIR]\n"
        "  --help  Show this message\n";
    printf("%s\n", help_message);
}

int is_help_option(int argc, char **argv)
{
    opt_info long_opts = get_long_opts(argc, argv);
    int res = 0;
    for (int i = 0; i < long_opts.optc; i++)
    {
        // printf("%s\n", long_opts.optv[i]);

        if (strncmp(long_opts.optv[i], "--help", 6) == 0)
        {
            res = 1;
            break;
        }
    }

    free_2deep_pointer(long_opts.optv, long_opts.optc);
    return res;
}

int main(int argc, char **argv)
{
    if (is_help_option(argc, argv))
    {

        print_help();
        return 0;
    }
    else
    {
        compile_rec(argc, argv);
        return 0;
    }
}