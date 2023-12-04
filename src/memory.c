void free_2deep_pointer(void **array, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        free(array[i]);
    }
    free(array);
}


// printf("CHECK NULL terminator\n");
    // #define MAX_LINE_LEN 1024
    // for (int i = 0; i < lines_size; i++)
    // {
    //     char *line = out[i];

    //     for (int j = 0; j <= MAX_LINE_LEN; j++)
    //     {  
    //         if (j == MAX_LINE_LEN)
    //         {
    //             // perror("line is not null terminated");
    //             printf("line is not null terminated\n");
    //             exit(1);
    //         }

    //         char ch = line[j];
    //         if (ch == '\0')
    //         {
    //             printf("null term position %i\n", j);
    //             break;
    //         }
    //         // printf("LINE %i\n", j);
    //     }
    // }