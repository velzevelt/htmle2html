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
    char *exe_dir = exe_dir_path(argv[0]);
    dir_info exe_dir_info = get_dir_files_rec(exe_dir);

    for (int i = 1; i < exe_dir_info.length; i++)
    {   
        char *file_path = exe_dir_info.files[i]; 
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

                char *contain;
                if (contain = strstr(file_content, "<?e"))
                {   
                    int substring_size = get_line_length(contain);
                    char substring[substring_size];

                    strncpy(substring, contain, substring_size);
                    substring[substring_size] = '\0';

                    char *command;
                    if (command = strstr(substring, "include"))
                    {
                        char *arg_begin;
                        char *arg_end;

                        arg_begin = strstr(command, "(\"");
                        arg_end = strstr(command, "\")");

                        arg_begin += 2;
                        arg_end--;

                        int size_between = arg_end - arg_begin + 1;

                        printf("size between arg_start and arg_end is %i\n", size_between);

                        char arg[size_between + 1];
                        strncpy(arg, arg_begin, size_between);
                        arg[size_between + 1] = '\0';

                        printf("parsed arg is %s\n", arg);
                        FILE *include_file = fopen(arg, "rb");

                        if (file_exists_file(include_file))
                        {
                            char *include_content = get_file_contents(include_file);
                            // printf("Include content is %s\n", include_content);

                            char *new_file_content = calloc(strlen(file_content) + strlen(include_content) + 10, sizeof(char));
                            strncpy(new_file_content, file_content, strlen(file_content));
                            strncpy(&new_file_content[strlen(file_content)], include_content, strlen(include_content));

                            printf("New file content is %s\n", new_file_content);

                            fclose(include_file);
                        }
                        else
                        {
                            fprintf(stderr, "Can't find file: %s, exe_dir: %s\n", arg, exe_dir);
                        }
                    }
                }

                FILE *new_file = fopen(new_file_path, "wb");
                fprintf(new_file, file_content);
                fclose(new_file);
            }

            fclose(f);
        }
        else
        {
            fprintf(stderr, "File does not exist at path %s\n", file_path);
        }

    }

}