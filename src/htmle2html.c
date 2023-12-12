#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <memory.c>
#include <cli.c>
#include <filesystem.c>

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

        int has_htmle = strncmp(file_extension, "htmle", 5) == 0;

        if (!has_htmle)
            continue;

        FILE *source_file = fopen(file_path, "rb");
        if (source_file == NULL)
        {
            fprintf(stderr, "File does not exist at path %s\n", file_path);
            continue;
        }

        char *new_file_path = change_file_extension(file_path, "html");
        if (new_file_path == NULL)
        {
            fprintf(stderr, "Invalid new path\n");
            fclose(source_file);
            continue;
        }

        printf("Changing file extension\nOld path: %s\nNew path: %s\n", file_path, new_file_path);

        FILE *out_file = fopen(new_file_path, "wb");
        if (out_file == NULL)
        {
            fprintf(stderr, "Cant create file at path %s\n", new_file_path);
            fclose(source_file);
            free(new_file_path);
            continue;
        }

        size_t line_position = 0;
        char string[2048];
        while (fgets(string, sizeof(string), source_file))
        {
            line_position++;
            char *out = string;
            char *contain_comment = strstr(string, "<!--");
            char *contain_e_begin = strstr(string, "<?e");
            char *contain_e_end = strstr(string, "?>");
            char *contain_include = strstr(string, "include");

            int syntax_error = !contain_comment && contain_e_begin && !contain_e_end;
            if (syntax_error)
            {
                fprintf(stderr, "Syntax error at line %i, expected ?>\n", line_position);
                strcpy(contain_e_begin, "Syntax error, expected ?>");

                fputs(out, out_file);
                continue;
            }

            int correct_include = !contain_comment && contain_e_begin && contain_e_end && contain_include;
            if (correct_include)
            {
                char *arg_begin = strstr(contain_include, "(\"");
                char *arg_end = strstr(contain_include, "\")");
                int correct_argument = arg_begin && arg_end;

                if (!correct_argument)
                {
                    fprintf(stderr, "Error at line %i, include expects one string argument e.g. include(\"file.txt\")\n", line_position);
                    strcpy(contain_e_begin, "Error, include expects one string argument e.g. include(\"file.txt\")");
                    fputs(out, out_file);
                    continue;
                }

                arg_begin += 2;
                arg_end -= 1;

                int argument_size = arg_end - arg_begin + 1;
                char argument[argument_size];
                strncpy(argument, arg_begin, argument_size);
                argument[argument_size] = '\0';

                char *include_file_path;
                for (size_t j = 0; j < exe_dir_info.length; ++j)
                {
                    if (strstr(exe_dir_info.files[j], argument))
                    {
                        include_file_path = exe_dir_info.files[j];
                        break;
                    }
                }

                if (include_file_path)
                {
                    FILE *include_file = fopen(include_file_path, "rb");
                    if (include_file)
                    {
                        char *include_content = get_file_contents(include_file);
                        fputs(include_content, out_file);
                        free(include_content);
                        fclose(include_file);
                        continue;
                    }
                }
                else
                {
                    fprintf(stderr, "Error at line %i, include file not found \n", line_position);
                    strcpy(contain_e_begin, "Error, include file not found");
                    fputs(out, out_file);
                    continue;
                }
            }

            // regular
            fputs(out, out_file);
        }

        fclose(source_file);
        fclose(out_file);
        free(new_file_path);
    }

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