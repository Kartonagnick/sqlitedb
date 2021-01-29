
#pragma once
#ifndef dTEST_STAFF_USED_ 
#define dTEST_STAFF_USED_ 1
//==============================================================================
//==============================================================================

#include <fstream>
#include <string>

namespace db
{
    class connection;
}

namespace staff_sqlitedb
{
    using str_t = ::std::string;

    bool dbaseDelete(const str_t& path) noexcept;
    bool dbaseDelete(const char*  path) noexcept;

    bool fileDelete(const str_t& path) noexcept;
    bool fileDelete(const char*  path) noexcept;

    bool fileExists(const str_t& path) noexcept;
    bool fileExists(const char*  path) noexcept;

    ::std::ofstream openWrite(const str_t& path, const bool append = true);
    ::std::ofstream openWrite(const char*  path, const bool append = true);

    ::std::ifstream openRead(const str_t& path);
    ::std::ifstream openRead(const char*  path);

    void makeTableAge(const str_t& base, const str_t& table);

    void makeTableAge(const db::connection& base, const str_t& table);

    void addToUsersTable(const str_t& path, const size_t login, const size_t age);
    void addToUsersTable(const db::connection& base, const size_t login, const size_t age);

} // namespace staff_sqlitedb

//==============================================================================
//==============================================================================
#endif // !dTEST_STAFF_USED_