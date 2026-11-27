#pragma once

#include <filesystem>
#include <string_view>

// Path of a file inside the Resources directory that sits next to the
// executable, so the game finds its assets whatever directory it is
// launched from.
std::filesystem::path ResourcePath(std::string_view relative);
