
// [2021y-01m-24d] Idrisov Denis R.
//==============================================================================
//==============================================================================

#include <sqlitedb/management.hpp>
#include <sys/stat.h>
#include <cassert>
#include <string>

namespace db
{
    #ifdef _WIN32 
        #define dSTATE64  _stat64
        #define dWSTATE64 _wstat64
    #else
        #define dSTATE64 stat64
    #endif

} // namespace db

//==============================================================================
//==============================================================================

namespace db
{
    bool remove(const char* filename) noexcept
    {
        assert(filename);
        const bool success = ::std::remove(filename) == 0;
        return success;
    }
    bool remove(const str_t& filename) noexcept
    {
        return ::db::remove(filename.c_str());
    }

    bool exists(const char* filename) noexcept
    {
        assert(filename);
        struct dSTATE64 info;
        const auto re = ::dSTATE64(filename, &info);
        if (re != 0)
            return false;
        const bool success = (info.st_mode & S_IFREG) != 0;
        return  success;
    }
    bool exists(const str_t& filename) noexcept
    {
        return ::db::exists(filename.c_str());
    }

} // namespace db

//==============================================================================
//==============================================================================


