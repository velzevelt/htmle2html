#include <string.h>

const char *interp_htmle(const char input[], const char file_path[], const dir_info *env)
{
    size_t input_size = strlen(input);

    if (input_size == 0)
    {
        return "";
    }

    char **out = calloc(input_size, sizeof(char *));

    char *line_begin = &input[0];
    size_t lines_size = 0;
    for (int i = 0; i <= input_size; i++)
    {
        // printf("Line begin: \t%p\n", line_begin);
        // printf("SYM %c\n", input[i]);

        if (input[i] == '\n' || i == input_size)
        {
            char *line_end = &input[i];
            // printf("Line end: \t%p\n", line_end);

            int distance = line_end - line_begin;
            // printf("DIST %i\n", distance);

            int min_dis = distance <= 0 ? 1 : distance;
            if (distance == 0)
            {
                out[lines_size] = calloc(1, sizeof(char));
                out[lines_size][0] = '\n';
            }
            else
            {
                out[lines_size] = (char *)calloc(distance, sizeof(char));
                strncpy(out[lines_size], line_begin, distance);
                out[lines_size][distance] = '\0';
                // printf("TT %s\n", out[lines_size]);
            }
            line_begin = line_end + 1;
            lines_size++;
        }
    }


    printf("SIZE %i\n", lines_size);
    for (int i = 0; i < lines_size; i++)
    {
        printf("AA: %s\n", out[i]);

        char *htmle_begin = strstr(out[i], "<?e");
        char *htmle_end = strstr(out[i], "?>");

        if (htmle_begin && !htmle_end)
        {
            int char_pos = strlen(out[i]) - strlen(htmle_begin) + 1;
            fprintf(stderr, "ERROR: Missing \'?>\' at %s %i:%i\n", get_file_name(file_path), i + 1, char_pos);
            continue;
        }

        if (!htmle_begin && htmle_end)
        {
            int char_pos = strlen(out[i]) - strlen(htmle_end) + 1;
            fprintf(stderr, "ERROR: Missing \'<?e\' at %s %i:%i\n", get_file_name(file_path), i + 1, char_pos);
            continue;
        }

        if (!htmle_begin && !htmle_end)
        {
            continue; // regular line nothing to interp
        }

        if (htmle_begin && htmle_end)
        {
            char *include = strstr(htmle_begin, "include");

            if (include)
            {
                char *arg_begin = strstr(include, "(\"");
                char *arg_end = strstr(include, "\")");

                if (!arg_begin || !arg_end)
                {
                    int char_pos = strlen(out[i]) - strlen(htmle_begin) + 1;
                    fprintf(stderr, "ERROR: include(\"path\") expects one string arg at %s %i:%i\n", get_file_name(file_path), i + 1, char_pos);
                    continue;
                }

                if (arg_begin && arg_end)
                {

                    arg_begin += 2;
                    arg_end--;
                    int arg_size = arg_end - arg_begin + 1;

                    if (arg_size <= 0)
                    {
                        int char_pos = strlen(out[i]) - strlen(htmle_begin) + 1;
                        fprintf(stderr, "ERROR: include(\"path\") expects one string arg at %s %i:%i\n", get_file_name(file_path), i + 1, char_pos);
                        continue;
                    }

                    char arg[arg_size];
                    strncpy(arg, arg_begin, arg_size);
                    arg[arg_size] = '\0';

                    printf("Arg is %s\n", arg);

                    for (int j = 0; j < env->length; j++)
                    {
                        char *file = env->files[j];
                        // printf("FILE IS %s\n", file);
                        if (strstr(file, arg))
                        {
                            printf("File is %s\n", file);
                            FILE *f = fopen(file, "rb");
                            if (file_exists_file(f))
                            {
                                char *content = get_file_contents(f);
                                size_t content_size = strlen(content);

                                if (content_size > 0)
                                {
                                    free(out[i]);
                                    out[i] = malloc(content_size * sizeof(char));
                                    strncpy(out[i], content, content_size);
                                    out[i][content_size] = '\0';

                                    printf("Out is %s\n", out[i]);
                                }

                                fclose(f);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    size_t total_out_size = 0;
    for (int i = 0; i < lines_size; i++)
    {
        total_out_size += strlen(out[i]);
    }
    printf("TOTAL SIZE: %i\n", total_out_size);

    char *res = malloc(sizeof(char) * total_out_size);

    for (int i = 0; i < lines_size; i++)
    {
        strncat(res, out[i], strlen(out[i]));
    }
    res[total_out_size] = '\0';

    printf("RES: \n%s\n", res);

    return "123";
}
