#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <cli.c>
#include <filesystem.c>
#include <htmle_interpreter.c>


#if defined(WIN32)
#include <asprintf.h>
#endif


int main(int argc, char **argv)
{   
    char *exe_dir = exe_dir_path(argv[0]);
    dir_info exe_dir_info = get_dir_files_rec(exe_dir);

    for (int i = 1; i < exe_dir_info.length; i++)
    {   
        const char *file_path = exe_dir_info.files[i]; 
        FILE *f = fopen(file_path, "rb");

        if (file_exists_file(f))
        {
            char *file_extension = get_file_extension(file_path);

            if (strncmp(file_extension, "htmle", 5) == 0)
            {
                char *new_file_path = change_file_extension(file_path, "html");
                printf("Changing file extension\nOld path: %s\nNew path: %s\n", file_path, new_file_path);

                
                char *file_content = get_file_contents(f);
                // printf("file content:\n%s\n", file_content);

                char *contain_htmle;
                if (contain_htmle = strstr(file_content, "<?e"))
                {   
                    char *interp_out = interp_htmle(file_content);
                    printf("Interp result: %s\n", interp_out);
                }

                // FILE *new_file = fopen(new_file_path, "wb");
                // fprintf(new_file, file_content);
                // fclose(new_file);
            }

            fclose(f);
        }
        else
        {
            fprintf(stderr, "File does not exist at path %s\n", file_path);
        }

    }

}