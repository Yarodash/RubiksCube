#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
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


unsigned int compile_shader(unsigned int shader_type, const std::string& source)
{
    unsigned int shader_id = glCreateShader(shader_type);
    const char* source_c = source.c_str();
    glShaderSource(shader_id, 1, &source_c, nullptr);
    glCompileShader(shader_id);

    int result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(shader_id, length, &length, message);

        printf("Failed to compile %s shader:\n%s",
            (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment"),
            message);

        glDeleteShader(shader_id);
        exit(-1);
    }

    return shader_id;
}


unsigned int create_program(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program_id = glCreateProgram();

    unsigned int vertex_shader_id = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fragment_shader_id = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);
    glValidateProgram(program_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}


unsigned int create_program_from_files(const std::string& shaders_name)
{
    return create_program(
        get_file_contents("shaders/" + shaders_name + ".vert"),
        get_file_contents("shaders/" + shaders_name + ".frag")
    );
}
