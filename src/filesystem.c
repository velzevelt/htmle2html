#define DIR_SEPARATOR '/'




int file_exists_at_path(const char *file_name)
{
    FILE *file;
    if ((file = fopen(file_name, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int file_exists_file(FILE *file)
{
    return file != NULL;
}

const char *get_file_extension(const char file_path[])
{
    int path_size = strlen(file_path);
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

    if (dot_pos != -1)
    {
        return &file_path[dot_pos + 1];
    }
    else
    {
        fprintf(stderr, "Failed to get file extension at path %s\n", file_path);
        return "";
    }
}

char *change_file_extension(const char file_path[], const char new_extension[])
{
    int path_size = strlen(file_path);

    const char *old_extension = get_file_extension(file_path);
    int old_extension_size = strlen(old_extension);
    int new_extension_size = strlen(new_extension);

    char *new_file_path = calloc(path_size - old_extension_size + new_extension_size + 1, sizeof(char));

    // path_file.old_extension
    strncpy(new_file_path, file_path, path_size - old_extension_size);                          // path_file.
    strncpy(&new_file_path[path_size - old_extension_size], new_extension, new_extension_size); // path_file.new_extension
    return new_file_path;
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
    strncpy(chunk, &file_path[dot_pos + 1], extension_size);
    int res = strncmp(chunk, extension, extension_size);
    return res;
}

const char *get_file_name(const char path[])
{
    int path_size = strlen(path);

    // if last char is dir separator we can't go above, so need to return same path
    if (path_size == 0 || path_size == 1)
    {
        return (char *)path;
    }

    int dir_separator_id = -1;
    for (int i = path_size - 1; i >= 1; i--)
    {
        if (path[i] == DIR_SEPARATOR)
        {
            dir_separator_id = i;
            break;
        }
    }

    if (dir_separator_id == -1)
    {
        printf("WARNING: no dir separator \"%c\" with steps found in provided path %s\n",
               DIR_SEPARATOR, path);
        return (char *)path;
    }

    return &path[dir_separator_id + 1];
}

const char *above(const char path[], int above_steps)
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

typedef struct
{
    char **files;
    size_t length;

} dir_info;

dir_info get_dir_files_of_type(const char *path, int (*condition)(struct dirent *))
{
    DIR *d = opendir(path);
    if (d == NULL)
    {
        perror("No such directory");
        dir_info r = {};
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

    char **_files;
    if (files_length > 0)
    {
        int i = 0;
        char **files = malloc(files_length * sizeof(char **));
        int path_size = strlen(path);
        while ((dir = readdir(d)) != NULL)
        {
            if (condition(dir))
            {
                int file_name_size = strlen(dir->d_name);
                files[i] = calloc(file_name_size + path_size + 1, sizeof(char));
                strncat(files[i], path, path_size);             // path
                files[i][path_size] = DIR_SEPARATOR;            // path/
                strncat(files[i], dir->d_name, file_name_size); // path/file_name
                i++;
            }
        }
        _files = files;
    }

    closedir(d);
    dir_info res = {_files, files_length};
    return res;
}

dir_info get_dir_files(const char *path)
{
    int con(struct dirent * dir)
    {
        return dir->d_type == DT_REG;
    }

    return get_dir_files_of_type(path, con);
}

dir_info get_dir_dir(const char *path)
{
    int con(struct dirent * dir)
    {
        return dir->d_type == DT_DIR && strncmp(dir->d_name, ".", 1) != 0 && strncmp(dir->d_name, "..", 2) != 0;
    }
    return get_dir_files_of_type(path, con);
}

void free_files(char **files, int size)
{
    for (int i = 0; i < size; i++)
    {
        // printf("FREED: %s\n", files[i]);
        free((char*)files[i]);
    }

    // printf("A %p\n", files);
    // free((char**)files);
}

void free_dir_info(dir_info *d)
{
    free_files(d->files, d->length);
}

void append_files(dir_info *self, const dir_info *other)
{
    int new_size = self->length + other->length;
    char **new_files = calloc(new_size, sizeof(char **));

    for (int i = 0; i < self->length; i++)
    {
        int t = strlen(self->files[i]);
        new_files[i] = malloc(sizeof(self->files[0]) * t);
        strncpy(new_files[i], self->files[i], t + 1);
    }

    for (int i = self->length, j = 0; i < new_size; i++, j++)
    {
        int t = strlen(other->files[j]);
        new_files[i] = malloc(sizeof(other->files[0]) * t);
        strncpy(new_files[i], other->files[j], t + 1);
    }

    free_dir_info(self);
    // free_2deep_pointer(self->files, self->length);
    // free_files(self->files, self->length);
    
    self->files = new_files;
    self->length = new_size;
}

/**
 * Return information about all files in directory and their subdirectories
 * include_subdir -> Include subdirectories to the final result?
 */
dir_info get_dir_files_rec(const char path[])
{
    dir_info files = get_dir_files(path);
    dir_info directories = get_dir_dir(path);

    for (int i = 0; i < directories.length; i++)
    {
        const char *p2 = directories.files[i];
        // printf("A %s\n", p2);

        dir_info files_2 = get_dir_files_rec(p2);

        // printf("files_2 len: %i\n", files_2.length);
        append_files(&files, &files_2);
    }

    return files;
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

char *exe_dir_path(char *arg_0)
{
    char *exe_path = realpath(arg_0, NULL);
    const char *exe_dir_path = above(exe_path, 1);
    return exe_dir_path;
}

char *get_file_contents(FILE *f)
{
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = malloc(length);

    if (buffer)
    {
        fread(buffer, 1, length, f);
    }

    return buffer;
}

int get_line_length(const char *line)
{
    int absolute_line_length = strlen(line);
    for (int i = 0; i < absolute_line_length; i++)
    {
        char ch = line[i];
        if (ch == '\n')
        {
            return i;
        }
    }

    return 0;
}