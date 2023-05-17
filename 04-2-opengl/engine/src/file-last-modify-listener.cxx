#include "file-last-modify-listener.hxx"
#include "handle-file-modify.hxx"

#include <algorithm>
#include <chrono>
#include <efsw/efsw.hpp>
#include <filesystem>
#include <map>
#include <vector>

#include "handle-user-event.hxx"
#include "user-events.hxx"

namespace Kengine
{

struct file_modify_listener_info
{
    std::string          file_name;
    file_modify_callback func;
    void*                data;
    bool                 modify;
    long                 id;
};

using namespace std::chrono;

static std::vector<file_modify_listener_info> listeners;
static const high_resolution_clock            clock;
static const high_resolution_clock::duration  max_time_modify{ 2'000'000'000 };
static high_resolution_clock::time_point      last_modify;
static bool                                   any_file_modified = false;

void handle_file_modify_listeners()
{
    if (any_file_modified && (clock.now() - last_modify > max_time_modify))
    {
        any_file_modified = false;
        std::for_each(listeners.begin(),
                      listeners.end(),
                      [](file_modify_listener_info& i)
                      {
                          if (i.modify)
                          {
                              i.modify = false;
                              i.func(i.data);
                          }
                      });
    }
};

void file_modify_event(void* data)
{
    any_file_modified                                          = true;
    last_modify                                                = clock.now();
    reinterpret_cast<file_modify_listener_info*>(data)->modify = true;
};

static efsw::FileWatcher watcher;

void start_files_watch()
{
    watcher.watch();
};

class file_last_modify_listener_impl : public file_last_modify_listener,
                                       public efsw::FileWatchListener
{
    long add_file(std::string          file_path,
                  file_modify_callback f_modify_func,
                  void*                data) override
    {
        using namespace std::filesystem;
        path f_path(file_path);
        long id = watcher.addWatch(f_path.parent_path().string(), this);
        listeners.push_back(
            { f_path.filename().string(), f_modify_func, data, false, id });
        return id;
    };

    void remove_file(long id) override
    {
        watcher.removeWatch(id);
        listeners.erase(std::remove_if(listeners.begin(),
                                       listeners.end(),
                                       [&](file_modify_listener_info l)
                                       { return l.id == id; }));
    };

    void handleFileAction(efsw::WatchID      watchid,
                          const std::string& dir,
                          const std::string& filename,
                          efsw::Action       action,
                          std::string        oldFilename) override
    {
        auto f_listener = std::find_if(listeners.begin(),
                                       listeners.end(),
                                       [&](file_modify_listener_info& i)
                                       { return i.file_name == filename; });
        if (f_listener != listeners.end())
            switch (action)
            {
                case efsw::Actions::Modified:
                    push_user_event(
                        static_cast<int>(user_events::file_modified),
                        reinterpret_cast<user_event_func>(&file_modify_event),
                        reinterpret_cast<void*>(&(*f_listener)));
                    break;
                case efsw::Actions::Add:
                    break;
                case efsw::Actions::Delete:
                    break;
                case efsw::Actions::Moved:
                    break;
                default:
                    break;
            }
    };
};

static file_last_modify_listener* instance =
    new file_last_modify_listener_impl();

file_last_modify_listener* file_last_modify_listener::get_instance()
{
    return instance;
};

}; // namespace Kengine
