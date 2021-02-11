
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_DEVICE_USED_ 
#define dSQLITEDB_DEVICE_USED_ 100

#include <sqlitedb/connection.hpp>
#include <mutex>

struct sqlite3;
struct sqlite3_stmt;
//==============================================================================
//==============================================================================
namespace db
{
    class connection;

    class device
    {
        using mutexT = ::std::mutex;
        using stmtT = ::sqlite3_stmt;
        friend class connection;
    public:
        device(const device&)            = delete;
        device(device&&)                 = delete;
        device& operator=(const device&) = delete;
        device& operator=(device&&)      = delete;

        device(
            const str_t& filename,
            const int mode,
            const size_t timeout
        );

        ~device();

        void cleanDB() ;
        void disconnect();
    private:
        stmtT* prepare(const char*  sql);
        stmtT* prepare(const str_t& sql);
    private:
        mutexT   m_mutex;
        sqlite3* m_device;
        int      m_flags;
    };

} // namespace db

//==============================================================================
//==============================================================================
#endif // !dSQLITEDB_DEVICE_USED_