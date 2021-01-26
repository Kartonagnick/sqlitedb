
#pragma once
//==============================================================================
//==============================================================================

#include <sys/stat.h>
#include <stdexcept>
#include <cassert>
#include <fstream>
#include <string>

namespace staff_sqlitedb
{
    #ifdef _WIN32 
        #define dSTATE64  _stat64
        #define dWSTATE64 _wstat64
    #else
        #define dSTATE64 stat64
    #endif 

    using str_t = ::std::string;

    inline bool fileDelete(const str_t path) noexcept
    {
        assert(!path.empty());
        const bool success
            = ::remove(path.c_str()) == 0;
        return success;
    }

    inline bool fileDelete(const char* path) noexcept
    {
        assert(path);
        return fileDelete(str_t(path));
    }

    inline bool fileExists(const str_t& path) noexcept
    {
        assert(!path.empty());
        struct dSTATE64 info;
        const auto re = ::dSTATE64(path.c_str(), &info);
        if (re != 0)
            return false;
        const bool success = (info.st_mode & S_IFREG) != 0;
        return  success;
    }
    inline bool fileExists(const char* path) noexcept
    {
        assert(path);
        return fileExists(str_t(path));
    }

    inline ::std::ofstream openWrite(const str_t& path, const bool append = true)
    {
        assert(!path.empty());
        const auto flag = append?
            ::std::ios::app : ::std::ios::trunc;

        const auto flags = ::std::ios::out | flag;

        ::std::ofstream out(path, flags);
        if (!out)
            throw ::std::runtime_error("[openWrite] can`t open: '" + path + "'");
        return out;
    }
    inline ::std::ofstream openWrite(const char* path, const bool append = true)
    {
        assert(path);
        return openWrite(str_t(path), append);
    }

    inline ::std::ifstream openRead(const str_t& path)
    {
        assert(!path.empty());
        const auto flags = ::std::ios::in;
        ::std::ifstream in(path, flags);
        if(!in)
            throw ::std::runtime_error("[openRead] can`t open: '" + path + "'");
        return in;
    }
    inline ::std::ifstream openRead(const char* path)
    {
        assert(path);
        return openRead(str_t(path));
    }

} // namespace staff_sqlitedb

//==============================================================================
//==============================================================================

