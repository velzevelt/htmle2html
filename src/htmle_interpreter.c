#include <string.h>

const char *interp_htmle(const char input[], const char file_path[], const dir_info *env)
{
    size_t input_size = strlen(input);
    size_t line_position = 0;

    char *input_copy = malloc(sizeof(char) * input_size);
    strncpy(input_copy, input, input_size);

    char **output = malloc(sizeof(char *) * input_size);

    char *token = strtok(input_copy, "\n");

    while (token != NULL)
    {
        printf("%s\n", token);

        char *start = strstr(token, "<?e");
        char *end = strstr(token, "?>");
        if (start)
        {
            if (end)
            {
            }
            else
            {
                int char_position = start - token + 1;
                fprintf(stderr, "ERROR: Missing ?> at %i:%i in file %s\n", line_position, char_position, get_file_name(file_path));
            }
        }
        else
        {
            int token_size = strlen(token);
            output[line_position] = malloc(sizeof(char) * token_size);
            strncpy(output[line_position], token, token_size);
            output[line_position][token_size] = '\0';
            printf("Just copy of size %i: %s\n", token_size, output[line_position]);
        }

        line_position++;
        token = strtok(NULL, "\n");
    }

    free(input_copy);
    // return output;

    return "12312312";
}