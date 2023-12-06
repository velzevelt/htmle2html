#include <string.h>

const char *strstr_reverse(const char *haystack, const char *needle)
{
    size_t needle_size = strlen(needle);
    size_t haystack_size = strlen(haystack);

    for (size_t i = haystack_size; i >= 0; i--)
    {
        if (strncmp(&haystack[i], needle, needle_size) == 0)
        {
            return &haystack[i];
        }
        else if (i == 0)
        {
            break;
        }
    }

    return NULL;
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

    char input_copy[input_size];
    strncpy(input_copy, input, input_size);

    size_t token_count = 0;
    char *token = strtok(input_copy, " ");
    while (token)
    {
        token_count++;
        token = strtok(NULL, " ");
    }
    strncpy(input_copy, input, input_size);

    char **tokens = calloc(token_count, sizeof(char*));
    size_t i = 0;
    token = strtok(input_copy, " ");
    while (token)
    {
        tokens[i] = calloc(strlen(token) + 1, sizeof(char));
        strcpy(tokens[i], token);

        token = strtok(NULL, " ");
        i++;
    }

    size_t e_begin_position = NULL;
    size_t e_end_position = NULL;
    for(size_t j = 0; j < token_count; j++)
    {
        // printf("TOKEN: %s\n", tokens[j]);

        if (strncmp(tokens[j], "<?e", 3) == 0 && strncmp(tokens[j - 1], "<!--", 4) != 0)
        {
            e_begin_position = j;
        }
        else if (strncmp(tokens[j], "?>", 2) == 0 && strncmp(tokens[j - 1], "<!--", 4) != 0)
        {
            e_end_position = j;
        }

        if (e_begin_position && e_end_position)
        {
            printf("E begin position %i\n", e_begin_position);
            printf("E end position %i\n", e_end_position);

            int e_tokens_count = e_end_position - e_begin_position + 1;
            printf("E count %i\n", e_tokens_count);

            // char **e_tokens = &tokens[e_begin_position];
            char *e_tokens[e_tokens_count];// = &tokens[e_begin_position];
            memcpy(e_tokens, &tokens[e_begin_position], e_tokens_count * sizeof(char*));

            for (size_t k = 0; k < e_tokens_count; k++)
            {
                printf("E TOKEN %s\n", e_tokens[k]);
            }

            e_begin_position = NULL;
            e_end_position = NULL;
        }
    }


    return "123";
}
