#include <stdio.h>
#include <stdlib.h>
#include "read_file.h"


bool read_binary_file(struct memory_range* out, const char* const file_path)
{
    FILE* script_file;

    if (!out || !file_path)
    {
        return false;
    }   

    fopen_s(&script_file, file_path, "rb");
    if (script_file == NULL)
    {
        printf("\tFailed to open file %s\n", file_path);
        return false;
    }

    size_t file_size = 0;

    fseek(script_file, 0, SEEK_END);
    file_size = (size_t)ftell(script_file);
    fseek(script_file, 0, SEEK_SET);

    size_t buffer_size = (file_size);

    void* file_buffer = malloc(buffer_size);
    if (file_buffer == false)
    {
        puts("Failed to allocate memory!");
        fclose(script_file);
        return false;
    }

    if (fread_s(file_buffer, file_size, 1, file_size, script_file) == 0)
    {
        puts("Error reading file!");
        fclose(script_file);
        free(file_buffer);
        return false;
    }

    fclose(script_file);

    out->begin = file_buffer;
    out->end = out->begin + buffer_size;

    return true;
}