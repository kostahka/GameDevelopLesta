#include "user-events.hxx"
#include "handle-user-event.hxx"

#include <SDL_events.h>

namespace Kengine
{
void push_user_event(int             user_event_code,
                     user_event_func event_func,
                     void*           data)
{
    SDL_Event     event;
    SDL_UserEvent user_event;

    user_event.code  = user_event_code;
    user_event.type  = SDL_EVENT_USER;
    user_event.data1 = reinterpret_cast<void*>(event_func);
    user_event.data2 = data;

    event.type = SDL_EVENT_USER;
    event.user = user_event;

    SDL_PushEvent(&event);
};

void handle_user_event(SDL_UserEvent u_event)
{
    switch (u_event.code)
    {
#ifdef ENGINE_DEV
        case (int)user_events::file_modified:
            user_event_func func =
                reinterpret_cast<user_event_func>(u_event.data1);
            func(u_event.data2);
            break;
#endif
    }
};

}; // namespace Kengine
