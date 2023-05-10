#pragma once

namespace Kengine
{
enum class user_events
{
#ifdef ENGINE_DEV
    file_modified = 0,
#endif
};

using user_event_func = void (*)(void* data);

void push_user_event(int             user_event_code,
                     user_event_func event_func,
                     void*           data);

}; // namespace Kengine
