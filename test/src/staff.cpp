
#include <test-staff.hpp>

#include <sqlitedb/connection.hpp>
#include <sys/stat.h>
#include <stdexcept>
#include <cassert>

//==============================================================================
//==============================================================================

namespace staff_sqlitedb
{
    #ifdef _WIN32 
        #define dSTATE64  _stat64
        #define dWSTATE64 _wstat64
    #else
        #define dSTATE64 stat64
    #endif 

    bool fileDelete(const str_t& path) noexcept
    {
        assert(!path.empty());
        const bool success
            = ::remove(path.c_str()) == 0;
        return success;
    }

    bool fileDelete(const char* path) noexcept
    {
        assert(path);
        return fileDelete(str_t(path));
    }



    bool dbaseDeleteImpl(const str_t& name, const str_t& ext) noexcept
    {
        assert(!name.empty());
        namespace me = staff_sqlitedb;
        const str_t shm = name + ext + "-shm";
        const str_t wal = name + ext + "-wal";
        me::fileDelete(shm);
        me::fileDelete(wal);
        return me::fileDelete(name + ext);
    }

    bool dbaseDelete(const str_t& path) noexcept
    {
        assert(!path.empty());
        namespace me = staff_sqlitedb;
        const auto pos = path.rfind('.');
        if(pos != std::string::npos)
        {
            const str_t ext  = '.' + path.substr(pos + 1);
            const str_t name = path.substr(0, pos);
            return me::dbaseDeleteImpl(name, ext);
        }
        const bool d2 = me::dbaseDeleteImpl(path, ".db");
        const bool d1 = me::dbaseDeleteImpl(path, "");
        return d1 || d2;
    }


    bool dbaseDelete(const char* path) noexcept
    {
        assert(path);
        namespace me = staff_sqlitedb;
        return me::dbaseDelete(str_t(path));

    }

    bool fileExists(const str_t& path) noexcept
    {
        assert(!path.empty());
        struct dSTATE64 info;
        const auto re = ::dSTATE64(path.c_str(), &info);
        if (re != 0)
            return false;
        const bool success = (info.st_mode & S_IFREG) != 0;
        return  success;
    }

    bool fileExists(const char* path) noexcept
    {
        assert(path);
        return fileExists(str_t(path));
    }

    ::std::ofstream openWrite(const str_t& path, const bool append)
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
    
    ::std::ofstream openWrite(const char* path, const bool append)
    {
        assert(path);
        return openWrite(str_t(path), append);
    }

    ::std::ifstream openRead(const str_t& path)
    {
        assert(!path.empty());
        const auto flags = ::std::ios::in;
        ::std::ifstream in(path, flags);
        if(!in)
            throw ::std::runtime_error("[openRead] can`t open: '" + path + "'");
        return in;
    }

    ::std::ifstream openRead(const char* path)
    {
        assert(path);
        return openRead(str_t(path));
    }

} // namespace staff_sqlitedb

//==============================================================================
//==============================================================================
namespace staff_sqlitedb
{
    void makeTableAge(const db::connection& conn, const str_t& table)
    {
        const str_t sql = "CREATE TABLE " + table + 
R"RAW((
    login  INTEGER PRIMARY KEY,
    age    INTEGER NOT NULL
))RAW";

         conn << sql;
    }

    void makeTableAge(const str_t& base, const str_t& table)
    {
         db::connection conn = db::connect(base, db::eCREATE);
         makeTableAge(conn, table);
    }

    void addToAgeTable(const str_t& path, const int login, const size_t age)
    {
        assert(!path.empty());
        const str_t sql = "insert into age (login, age) values (?,?)";
        db::connection con = db::connect(path, db::eREADWRITE);
        con << sql << login << age;
    }

} // namespace staff_sqlitedb