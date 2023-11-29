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

int file_has_extension(const char file_path[], const char extension[])
{
    int path_size = strlen(file_path);
    int extension_size = strlen(extension);

    int dot_pos = -1;
    for (int i = 0; i < path_size; i++)
    {
        char item = file_path[i];
        if (item == '.')
        {
            dot_pos = i;
            break;
        }
    }

    char chunk[extension_size];
    for (int i = dot_pos + 1, j = 0; i < path_size; i++, j++)
    {
        chunk[j] = file_path[i];
    }
    int res = strncmp(chunk, extension, extension_size);
    return res;
}

char *above(const char path[], int above_steps)
{
    int path_size = strlen(path);

    // if last char is dir separator we can't go above, so need to return same path
    if (path_size == 0 || path_size == 1)
    {
        return (char *)path;
    }
    assert(above_steps > 0);

    int dir_separator_id = -1;
    int above_steps_counter = 1;
    for (int i = path_size - 1; i >= 1; i--)
    {
        if (path[i] == DIR_SEPARATOR)
        {
            if (above_steps_counter == above_steps)
            {
                dir_separator_id = i;
                break;
            }
            else
            {
                above_steps_counter++;
            }
        }
    }

    if (dir_separator_id == -1)
    {
        printf("WARNING: no dir separator \"%c\" with steps %i found in provided path %s\n",
               DIR_SEPARATOR, above_steps, path);
        return (char *)path;
    }

    int res_size = dir_separator_id + 1;
    char *res = malloc(sizeof(char) * res_size);
    strncpy(res, path, res_size - 1);
    res[res_size] = '\0';
    return res;
}

struct dir_info
{
    char **files;
    size_t length;
};


struct dir_info get_dir_files_of_type(char *path, int (*condition)(struct dirent*))
{
    DIR *d = opendir(path);
    if (d == NULL)
    {
        perror("No such directory");
        struct dir_info r = {};
        return r;
    }

    struct dirent *dir;
    int files_length = 0;
    while ((dir = readdir(d)) != NULL)
    {
        if (condition(dir))
        {
            files_length++;
        }
    }
    rewinddir(d);

    int i = 0;
    char **files = calloc(files_length, sizeof(char**));
    int path_size = strlen(path);
    while ((dir = readdir(d)) != NULL)
    {
        if (condition(dir))
        {
            int file_name_size = strlen(dir->d_name);
            files[i] = calloc(file_name_size + path_size + 2, sizeof(char*));
            strncat(files[i], path, path_size);
            files[i][path_size] = DIR_SEPARATOR;
            strncat(files[i], dir->d_name, file_name_size);
            i++;
        }
    }
    closedir(d);

    struct dir_info res = {files, files_length};
    return res;
}


// https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
struct dir_info get_dir_files(char *path)
{   
    int con(struct dirent *dir)
    {
        return dir->d_type == DT_REG;
    }

    return get_dir_files_of_type(path, con);
}

struct dir_info get_dir_dir(char *path)
{
    int con(struct dirent *dir)
    {
        return dir->d_type == DT_DIR && strncmp(dir->d_name, ".", 1) != 0 && strncmp(dir->d_name, "..", 2) != 0;
    }


    return get_dir_files_of_type(path, con);
    // Get list of dir
    // Get full list of dir
    // Get all files
}

void free_files(char *files[], int size)
{
    for (int i = 0; i < size; i++)
        free(files[i]);
    free(files);
}

void append_files(struct dir_info *self, struct dir_info *other)
{
    int new_size = self->length + other->length;
    char **new_files = calloc(new_size, sizeof(char**));

    for (int i = 0; i < self->length; i++)
    {
        new_files[i] = calloc(strlen(self->files[i]), sizeof(self->files[i]));
        strncpy(new_files[i], self->files[i], strlen(self->files[i]));
    }

    for (int i = self->length, j = 0; i < new_size; i++, j++)
    {
        new_files[i] = calloc(strlen(other->files[j]), sizeof(other->files[j]));
        strncpy(new_files[i], other->files[j], strlen(other->files[j]));
    }

    free_files(self->files, self->length);
    self->files = new_files;
    self->length = new_size;
}

struct dir_info get_dir_dir_rec(const char path[], struct dir_info accum)
{
    struct dir_info total_dir;
    total_dir.length = 0;
    struct dir_info current_dir = get_dir_dir(path);
}



int count_dir_files(const char path[])
{
    DIR *d = opendir(path);
    if (d == NULL)
    {
        perror("No such directory");
        return 0;
    }

    struct dirent *dir;
    int files_length = 0;
    while ((dir = readdir(d)) != NULL)
    {
        if (dir->d_type == DT_REG)
        {
            files_length++;
        }
    }
    closedir(d);

    return files_length;
}


char *current_dir_path(char *arg_0)
{
    char *exe_path = realpath(arg_0, NULL);
    char *exe_dir_path = above(exe_path, 1);
    return exe_dir_path;
}