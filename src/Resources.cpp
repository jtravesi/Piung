#include "Resources.h"

#include <string>
#include <system_error>

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#elif defined(__APPLE__)
    #include <cstdint>
    #include <mach-o/dyld.h>
#endif

namespace
{
    std::filesystem::path ExecutableDir()
    {
#if defined(_WIN32)
        std::wstring buffer(MAX_PATH, L'\0');
        const DWORD length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
        if (length > 0 && length < buffer.size())
        {
            buffer.resize(length);
            return std::filesystem::path(buffer).parent_path();
        }
#elif defined(__APPLE__)
        std::uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);
        std::string buffer(size, '\0');
        if (_NSGetExecutablePath(buffer.data(), &size) == 0)
        {
            return std::filesystem::path(buffer.c_str()).parent_path();
        }
#else
        std::error_code error;
        const std::filesystem::path exe = std::filesystem::read_symlink("/proc/self/exe", error);
        if (!error)
        {
            return exe.parent_path();
        }
#endif
        // Fall back to the working directory, the old behaviour.
        return std::filesystem::current_path();
    }
}

std::filesystem::path ResourcePath(std::string_view relative)
{
    static const std::filesystem::path resources = ExecutableDir() / "Resources";
    return resources / relative;
}
