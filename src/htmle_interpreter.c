#include <string.h>

typedef struct {
    size_t resize_char_position;
    char *resize_content;
} resize_info;


const char *interp_htmle(const char input[], const char file_name[])
{
    size_t input_size = strlen(input);
    size_t line_position = 0;
    size_t relative_char_position = 0;
    size_t char_position = 0;
    
    char *output = malloc(input_size * sizeof(char));
    strncpy(output, input, input_size);


    for (; char_position < input_size; char_position++)
    {
        char *p_symbol = &output[char_position];
        relative_char_position++;

        if (*p_symbol == '\n')
        {
            line_position++;
            relative_char_position = 0;
        }
        else if (p_symbol[0] == '<' && p_symbol[1] == '?' && p_symbol[2] == 'e')
        {
            char *interp_end = strstr(p_symbol, "?>");
            if (interp_end)
            {
                char *inter_commands_begin = p_symbol + 3;
                char *inter_commands_end = interp_end - 1;
                int size = inter_commands_end - inter_commands_begin + 1;
                
                char *command;
                if (command = strstr(inter_commands_begin, "include"))
                {
                    char *arg_begin = strstr(command, "(\"");
                    char *arg_end = strstr(command, "\")");

                    if (arg_begin && arg_end)
                    {
                        printf("We have argument!\n");
                    }
                    else
                    {
                        char *error_message;
                        asprintf(&error_message, "ERROR: \'include\' command expects string argument \'path\' e.g. include(\"file_1.txt\") at line %i:%i in file %s\n", line_position + 1, relative_char_position + 1, file_name);
                        fprintf(stderr, error_message);
                        strncpy(p_symbol, error_message, strlen(error_message));
                    }

                }

                // printf("SIZE %i\n", size);
                // printf("S %c\n", inter_commands_end[0]);
            }
            else
            {
                char *error_message;
                asprintf(&error_message, "ERROR: Missing \'?>\' at line %i:%i in file %s\n", line_position + 1, relative_char_position + 1, file_name);
                fprintf(stderr, error_message);
                strncpy(p_symbol, error_message, strlen(error_message));
            }
        }
    }


    return output;
}