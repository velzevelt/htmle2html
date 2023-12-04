#include <string.h>

const char *interp_htmle(const char input[], const char file_path[], const dir_info *env)
{
    if (input == NULL)
    {
        return NULL;
    }

    size_t input_size = strlen(input);

    if (input_size == 0)
    {
        return NULL;
    }

    char **out = calloc(input_size, sizeof(char **));

    char *line_begin = &input[0];
    size_t lines_size = 0;
    for (int i = 0; i <= input_size; i++)
    {
        // printf("Line begin: \t%p\n", line_begin);
        // printf("SYM %c\n", input[i]);

        if (input[i] == '\n' || input[i] == '\0')
        {
            char *line_end = &input[i];
            // printf("Line end: \t%p\n", line_end);

            int distance = line_end - line_begin;
            
            // printf("DIST %i\n", distance);

            int min_dis = distance <= 0 ? 1 : distance;
            if (distance == 0)
            {
                out[lines_size] = calloc(1, sizeof(char));
                out[lines_size][0] = '\0';
            }
            else
            {
                out[lines_size] = (char *)calloc(distance + 2, sizeof(char));
                strncpy(out[lines_size], line_begin, distance);
                out[lines_size][distance] = '\0';
                printf("Copy result %s\n", out[lines_size]);
            }

            line_begin = line_end + 1;
            lines_size++;
        }
    }


    size_t res_size = 0;
    for (int i = 0; i < lines_size; i++)
        res_size += strlen(out[i]);

    // printf("RES SIZE %i\n", res_size);
    char *res = calloc(res_size, sizeof(char));
    for (int i = 0; i < lines_size; i++)
    {
        // strncat(res, out[i], strlen(out[i]));
        // strcat(res, out[i]);
        // printf("CUR RES IS %s\n", res);
    }


    for (int i = 0; i < lines_size; i++)
    {
        free(out[i]);
    }
    free(out);

    // printf("RES IS %s\n", res);
    return res;
}
