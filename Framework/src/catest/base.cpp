#include <catest/base.h>
#include <sstream>

namespace catest
{
    std::string get_filename(const char* filename)
    {
        const char* lastSlash = strrchr(filename, '/');
        const char* lastBackslash = strrchr(filename, '\\');
        const char* lastSeparator = std::max(lastSlash, lastBackslash);

        if (lastSeparator != nullptr)
        {
            return std::string(lastSeparator + 1);
        }

        return filename;
    }
    std::string get_fail_reason(const char* filename, int32_t line, const std::string& reason_c, const  char* test_func)
    {
        std::stringstream reason;
        reason << reason_c << "(" << get_filename(filename) << " L" << line << " - " << test_func << ")";
        return reason.str();
    }
}