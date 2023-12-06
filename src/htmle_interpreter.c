#include <string.h>

typedef enum {
    html_regular,
    html_comment,
    htmle_interp_begin,
    htmle_interp_end,
} token_type;

typedef struct {
    token_type t_type;
    char *origin;
    char *out;
    size_t line_postion;
    size_t char_position;
} htmle_token;


const *htmle_token parse_source(const char source[])
{
    size_t char_len = strlen(source) + 1;
    size_t line_len = 0;

    for (size_t i = 0; i <= char_len; i++)
    {
        if (source[i] == '\n' || source[i] == '\0')
        {
            line_len++;
        }
    }
}

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


    return "";
}
