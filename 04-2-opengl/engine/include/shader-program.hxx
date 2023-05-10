#pragma once

#include <string>

namespace Kengine
{

class shader_program
{
public:
    virtual void use() = 0;

    virtual void set_uniform1f(std::string uniform_name, float value) = 0;

    virtual ~shader_program();
};

shader_program* create_shader_program(const std::string& vertex_path,
                                      const std::string& fragment_path);
} // namespace Kengine
