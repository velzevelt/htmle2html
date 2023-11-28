#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIR_SEPARATOR '/'

int file_exists(const char *file_name)
{
    FILE *file;
    if ((file = fopen(file_name, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

char *above(const char path[]) {
    // iterate from end
    for (int i = strlen(path) - 1; i >= 0; i--) {
        char current_char = path[i];
        int if_eql = strcmp(current_char, DIR_SEPARATOR);

        printf("\n");
    }

    return (char*)path;
}


int main(int argc, char **argv) {

    const char test_string[] = "I wanna die";
    above(test_string);
    
    return 0;


    FILE *p_file;
    char ch;

    // const char *file_path = "/home/velz/Документы/projects/htmle2html/data/test_1.txt";
    char *exe_path = realpath(argv[0], NULL);
    char *file_path;
    char *root_path;
    // asprintf(&root_path, "%s/..", exe_path);
    asprintf(&root_path, "%s/..", exe_path);
    root_path = realpath(root_path, NULL);

    printf(root_path);
    printf("\n");
    return 0;

    asprintf(&file_path, "%s/data/test_1.txt", root_path);

    printf(file_path);
    printf("\n");
    printf("/home/velz/Документы/projects/htmle2html/data/test_1.txt");
    printf("\n");


    // printf("%i\n", file_exists(file_path));
    return 0;

    // p_file = fopen(file_path, "r");
    // p_file = fopen(file_path, "r");

    if (p_file == NULL) 
    {
        char *error_message;
        asprintf(&error_message, "Can't open file at %s", file_path);
        perror(error_message);
        return 1;
    }

    printf("content of this file are \n");
 
    do {
        ch = fgetc(p_file);
        printf("%c", ch);
    } while (ch != EOF);
 
    printf("\n");
    fclose(p_file);

    return 0;
}