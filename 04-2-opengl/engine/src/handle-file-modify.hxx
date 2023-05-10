#pragma once

namespace Kengine
{

void handle_file_modify_listeners();
void file_modify_event(void* data);
void start_files_watch();

}; // namespace Kengine
