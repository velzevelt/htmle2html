#include <string.h>

typedef enum {
    html_regular,
    html_comment,
    htmle_interp_begin,
    htmle_interp_end,
} token_type;

typedef struct {
    token_type type;
    char *string;
    size_t line_postion;
    size_t char_position;
} htmle_token;

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

    char *input_copy = calloc(input_size, sizeof(char));
    strncpy(input_copy, input, input_size);

    char *e_begin = NULL;
    char *e_end = NULL;
    char *html_comment = NULL;

    for (size_t i = 0; i < input_size; i++)
    {
        if (strncmp(&input_copy[i], "<!--", 4) == 0)
        {
            html_comment = &input_copy[i];
        }
        else if (strncmp(&input_copy[i], "<?e", 3) == 0)
        {
            e_begin = &input_copy[i];
        }
        else if (strncmp(&input_copy[i], "?>", 2) == 0)
        {
            e_end = &input_copy[i];
        }

        if (e_begin != NULL && e_end != NULL)
        {
            e_begin = NULL;
            e_end = NULL;
        }
    }

    return input;
}
