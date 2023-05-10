#pragma once

#include <string>

#include "user-events.hxx"

namespace Kengine
{

using file_modify_callback = void (*)(void* data);

class file_last_modify_listener
{
public:
    virtual void add_file(std::string          file_path,
                          file_modify_callback f_modify_func,
                          void*                data) = 0;

    static file_last_modify_listener* get_instance();
};

}; // namespace Kengine
