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

    char *res = calloc(input_size, sizeof(char));
    strncpy(res, input, input_size);
    for (size_t i = 0; i <= input_size; i++)
    {
        if (strncmp(&res[i], "<?e", 3) == 0) //&& strstr(get_line_begin(&res[i]))) Respect comments
        {
            printf("Found thing\n");
        }
    }

    printf("TEST STRSTR REVERSE\n");
    char *t = "DD \n ABOBA BIBA A";
    char *t2 = "BO";
    char *t3 = strstr_reverse(t, t2);
    printf("OUT %s\n", t3);

    return res;
}
