#pragma once

#include <string>
#include <fstream>

inline std::string get_file_contents(std::string filename)
{
    const char* filename_c = filename.c_str();

    std::FILE* fp = fopen(filename_c, "rb");
    if (fp)
    {
        std::string contents;
        std::fseek(fp, 0, SEEK_END);
        contents.resize(std::ftell(fp));
        std::rewind(fp);
        std::fread(&contents[0], 1, contents.size(), fp);
        std::fclose(fp);
        return contents;
    }

    printf("Error with file %s", filename_c);
    exit(-1);
}
