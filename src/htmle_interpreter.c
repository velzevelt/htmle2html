#include <string.h>

typedef enum {
    unknown,
    comment,
    new_line,
    e_begin,
    e_end,

} token_type;

typedef struct {
    token_type t_type;
    size_t line_position;
    char *source;
    char *out;
} token_info;

const char *interp_htmle(const char input[], const char file_path[], const dir_info *env)
{
    if (input == NULL)
    {
        return NULL;
    }

    size_t input_size = strlen(input) + 1;

    if (input_size == 0)
    {
        return NULL;
    }

    char input_copy[input_size];
    strncpy(input_copy, input, input_size);

    size_t line_count = 1;
    for (size_t i = 0; i <= input_size; i++)
    {
        if (input_copy[i] == '\n')
            line_count++;
    }

    printf("LINES %i\n", line_count);


    char **lines = calloc(line_count, sizeof(char*));
    char *line;
    line = strtok(input_copy, "\n");
    size_t i = 0;
    while (line)
    {
        size_t line_size = strlen(line) + 1;
        lines[i] = calloc(line_size, sizeof(char));
        strncpy(lines[i], line, line_size);
        lines[i][line_size] = '\0';

        // printf("TOKEN %s\n", lines[i]);

        line = strtok(NULL, "\n");
        i++;
    }



    for (size_t i = 0; i < line_count - 1; i++)
    {
        // printf("LINE %s\n", lines[i]);
        // size_t line_size = strlen(lines[i]) + 1;
        
        char *comment = strstr(lines[i], "<!--");
        char *e_begin = strstr(lines[i], "<?e");
        char *e_end = strstr(lines[i], "?>");

        if (!comment && e_begin && e_end)
        {
            char *include = strstr(e_begin, "include");
            if (include)
            {
                char *arg_begin = strstr(include, "(\"");
                char *arg_end = strstr(include, "\")");
                if (arg_begin && arg_end)
                {
                    arg_begin += 2;
                    arg_end -= 1;

                    size_t arg_size = arg_end - arg_begin + 1; 
                    char arg[arg_size];
                    strncpy(arg, arg_begin, arg_size);
                    arg[arg_size] = '\0';

                    printf("Include is %s\n", arg);

                    char *match = arg;
                    for (size_t j = 0; j < env->length; j++)
                    {
                        if (strstr(env->files[j], arg))
                        {
                            match = env->files[j];
                            printf("Closest %s\n", env->files[j]);
                            break;
                        }
                    }

                    FILE *include_file = fopen(match, "rb");
                    if (include_file)
                    {
                        char *file_content = get_file_contents(include_file);
                        free(lines[i]);

                        lines[i] = file_content;
                        // printf("INCLUDE File content %s\n", lines[i]);

                        fclose(include_file);
                    }
                    else
                    {
                        fprintf(stderr, "Cant find file %s\n", match);
                    }
                }
            }
        }
    }

    size_t total_size = 0;
    for (size_t i = 0; i < line_count - 1; i++)
    {
        total_size += strlen(lines[i]);
    }
    total_size++;

    char *res = calloc(total_size, sizeof(char));
    for (size_t i = 0; i < line_count - 1; i++)
    {
        size_t s = strlen(lines[i]) + 2;
        char copy_line[s];
        strncpy(copy_line, lines[i], s - 1);
        copy_line[s - 1] = '\n';
        copy_line[s] = '\0';

        printf("COPY %s\n", lines[i]);

        strncat(res, copy_line, s + 1);
        free(lines[i]);
    }
    free(lines);

    res[total_size] = '\0';

    return res;
}
