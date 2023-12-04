#include <assert.h>

void free_2deep_pointer(void **array, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        free(array[i]);
    }
    free(array);
}


int has_null_terminator(const char *str, size_t max_line_length)
{
    for (int i = 0; i <= max_line_length; i++)
    {
        if (i == max_line_length)
            return 0;

        if (str[i] == '\0')
            return 1;
    }

    assert(1); // Unreachable
}
