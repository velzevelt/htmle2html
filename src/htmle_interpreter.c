#include <string.h>

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

    char *res = calloc(input_size, sizeof(char));
    strncpy(res, input, input_size);
    for (size_t i = 0; i <= input_size; i++)
    {
        if (strncmp(&res[i], "<?e", 3) == 0) //&& strstr(get_line_begin(&res[i]))) Respect comments
        {
            printf("Found thing\n");
        }
    }

    return res;
}
