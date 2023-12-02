#include <string.h>

const char *interp_htmle(const char input[], const char file_path[], const dir_info *env)
{
    size_t input_size = strlen(input);

    if (input_size == 0)
    {
        return input;
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
                // out[lines_size] = calloc(2, sizeof(char));
                // out[lines_size][0] = '\n';
                // out[lines_size][1] = '\0';
                
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
    }

    return "123";
    // return output;
}