
// [2021y-01m-24d] Idrisov Denis R.
//==============================================================================
//==============================================================================

#include "exception.hpp"
#include <sqlitedb/connection.hpp>
#include <sys/stat.h>
#include <cassert>
#include <string>

//==============================================================================
//==============================================================================

#define SQLITE_NOTADB      26   /* File opened that is not a database file */

//==============================================================================
//==============================================================================

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
//=== [remove/exists] ==========================================================

namespace db
{
    bool remove(const char* filename) noexcept
    {
        assert(filename);
        assert(filename[0] != 0);
        const bool success = ::std::remove(filename) == 0;
        return success;
    }
    bool remove(const str_t& filename) noexcept
    {
        assert(!filename.empty());
        return ::db::remove(filename.c_str());
    }

    bool exists(const char* filename) noexcept
    {
        assert(filename);
        assert(filename[0] != 0);
        struct dSTATE64 info;
        const auto re = ::dSTATE64(filename, &info);
        if (re != 0)
            return false;
        const bool success = (info.st_mode & S_IFREG) != 0;
        return  success;
    }
    bool exists(const str_t& filename) noexcept
    {
        assert(!filename.empty());
        return ::db::exists(filename.c_str());
    }

} // namespace db

//==============================================================================
//==============================================================================

namespace db
{
    void cleanDatabase(const char* name)
    {
        assert(name);
        assert(name[0] != 0);
        const auto con = db::connect(name, eREADWRITE);
        con.cleanDB();
    }


    bool existDatabase(const char* filename) noexcept
    {
        assert(filename);
        assert(filename[0] != 0);
        return db::exists(filename);
    }

    bool availableDatabase(const char* name)
    {
        try
        {
            if (!db::exists(name))
                return false;
            db::connect(name, eREADONLY);
            return true;
        }
        catch (const ::db::exception&)
        {
            return false;
        }
    }

    bool existTable(const char* base, const char* table)
    {
        assert(base);
        assert(table);
        assert(base[0]  != 0);
        assert(table[0] != 0);

        if (!db::availableDatabase(base))
            throw std::runtime_error(
                "[existTable] database '" + str_t(base) + "' not available"
            );

        const auto con = db::connect(base);
        return con.existTable(table);
    }

    bool existColumn(const char* base, const char* table, const char* column)
    {
        assert(base);
        assert(table);
        assert(column);
        assert(base[0]   != 0);
        assert(table[0]  != 0);
        assert(column[0] != 0);

        if (!db::availableDatabase(base))
            throw std::runtime_error(
                "[existColumn] database '" + str_t(base) + "' not available"
            );

        if (!db::existTable(base, table))
            throw std::runtime_error(
                "[existColumn] table '" + str_t(table) + "' not exist"
            );

        const auto con = db::connect(base);
        return con.existColumn(table, column);
    }



    bool dropDatabase(const str_t& name)
    {
        assert(!name.empty());
        const auto idx = name.rfind('.');
        if(idx != std::string::npos)
        {
            //std::string extension = filename.substr(idx+1);
            const str_t n = name.substr(0, idx);
            const str_t shm = n + ".db-shm";
            const str_t wal = n + ".db-wal";
            db::remove(shm);
            db::remove(wal);
        }
        else
        {
            // No extension found
        }
        return db::remove(name);
    }

    bool dropDatabase(const char* name)
    {
        assert(name);
        assert(name[0] != 0);
        const str_t n = name;;
        return db::dropDatabase(n);
    }


    bool dropTable(const char* base, const char* table)
    {
        assert(base);
        assert(table);
        assert(base[0]   != 0);
        assert(table[0]  != 0);

        if (!db::availableDatabase(base))
            throw std::runtime_error(
                "[dropTable] database '" + str_t(base) + "' not available"
            );

        const auto con = db::connect(base, db::eREADWRITE);
        return con.dropTable(table);
    }

    #if 0
    //--- the `sqlite3` does not support this feature
    void dropColumn(
        const char* base,
        const char* table,
        const char* column
    );
    #endif


} // namespace db
