void free_2deep_pointer(void **array, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        free(array[i]);
    }
    free(array);
}