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

    char *token;
    char *saveptr;

    token = strtok_r(input_copy, " \n", &saveptr);
    while (token)
    {
        char delimiter = input[token - input_copy + strlen(token)];
        if (delimiter == '\n')
        {
            printf("NEW LINE! %s\n", token);
        }

        // printf("TOKEN %s\n", token);
        token = strtok_r(NULL, " \n", &saveptr);
    }

    // token = strtok_r(input_copy, " \n", saveptr);
    // while (token)
    // {
    //     int new_line = strstr(token, "\n");
    //     printf("TOKEN %s\n", token);
    //     token = strtok_r(NULL, " \n", saveptr);
    // }


    strncpy(input_copy, input, input_size);

    return "123";
}
