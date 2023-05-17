#include "shader-program.hxx"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <efsw/efsw.hpp>
#include <glad/glad.h>

#include "file-last-modify-listener.hxx"
#include "glm/gtc/type_ptr.hpp"

namespace Kengine
{

GLuint create_program(const GLchar* vertex_path, const GLchar* fragment_path)
{
    GLuint program;

    std::stringstream vertex_code;
    std::ifstream     vertex_file;

    std::stringstream fragment_code;
    std::ifstream     fragment_file;

    vertex_file.exceptions(std::ifstream::failbit);
    fragment_file.exceptions(std::ifstream::failbit);

    try
    {
        vertex_file.open(vertex_path);
        vertex_code << vertex_file.rdbuf();
        vertex_file.close();

        fragment_file.open(fragment_path);
        fragment_code << fragment_file.rdbuf();
        fragment_file.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "Failed to load vertex code from file [" << vertex_path
                  << "]. Error: " << e.what() << std::endl;
    }

    std::string vertex_string_code   = vertex_code.str();
    std::string fragment_string_code = fragment_code.str();

    const GLchar* vertex_shader_code   = vertex_string_code.c_str();
    const GLchar* fragment_shader_code = fragment_string_code.c_str();

    GLuint vertex, fragment;
    GLint  succes;

    GLint             info_len;
    std::vector<char> info_log;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
    if (!succes)
    {
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &info_len);
        info_log.resize(info_len);
        glGetShaderInfoLog(vertex, info_len, nullptr, info_log.data());
        glDeleteShader(vertex);
        std::cerr << "Failed to compile vertex shader. Log: " << info_log.data()
                  << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_code, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
    if (!succes)
    {
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &info_len);
        info_log.resize(info_len);
        glGetShaderInfoLog(fragment, info_len, nullptr, info_log.data());
        glDeleteShader(fragment);
        std::cerr << "Failed to compile fragment shader. Log: "
                  << info_log.data() << std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &succes);
    if (!succes)
    {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
        info_log.resize(info_len);
        glGetProgramInfoLog(program, info_len, nullptr, info_log.data());
        glDeleteProgram(program);
        std::cerr << "Failed to link shader program. Log: " << info_log.data()
                  << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
};

shader_program::~shader_program() = default;

void reload_shader(void* data);

class shader_program_impl : public shader_program
{
public:
    shader_program_impl(const std::string& vertex_path,
                        const std::string& fragment_path)
        : vertex_path(vertex_path)
        , fragment_path(fragment_path)
    {
        auto f_listener = file_last_modify_listener::get_instance();
        vertex_listener_id =
            f_listener->add_file(vertex_path, &reload_shader, this);
        fragment_listener_id =
            f_listener->add_file(fragment_path, &reload_shader, this);
        this->program =
            create_program(vertex_path.c_str(), fragment_path.c_str());
    };

    ~shader_program_impl() override
    {
        glDeleteProgram(this->program);
        auto f_listener = file_last_modify_listener::get_instance();
        f_listener->remove_file(vertex_listener_id);
        f_listener->remove_file(fragment_listener_id);
    };

    void use() override { glUseProgram(this->program); };

    void set_uniform1f(std::string uniform_name, float value) override
    {
        auto uniform_location =
            glGetUniformLocation(program, uniform_name.data());
        glUniform1f(uniform_location, value);
    };

    void set_uniform_matrix3fv(std::string uniform_name,
                               glm::mat3&  matrix) override
    {
        auto uniform_location =
            glGetUniformLocation(program, uniform_name.data());
        glUniformMatrix3fv(
            uniform_location, 1, GL_FALSE, glm::value_ptr(matrix));
    };

    void reload_files()
    {
        glDeleteProgram(program);
        program = create_program(vertex_path.c_str(), fragment_path.c_str());
    };

private:
    GLuint      program;
    std::string vertex_path;
    std::string fragment_path;

    long vertex_listener_id;
    long fragment_listener_id;
};

void reload_shader(void* data)
{
    auto shader = reinterpret_cast<shader_program_impl*>(data);
    shader->reload_files();
};

shader_program* create_shader_program(const std::string& vertex_path,
                                      const std::string& fragment_path)
{
    return new shader_program_impl(vertex_path, fragment_path);
};
} // namespace Kengine
