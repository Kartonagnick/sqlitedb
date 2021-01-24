
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_DEVICE_USED_ 
#define dSQLITEDB_DEVICE_USED_ 1

#include <sqlitedb/connect.hpp>
#include <atomic>
struct sqlite3;
//==============================================================================
//==============================================================================
namespace db
{
    class connection;

    class device
    {
        using sizeT = ::std::atomic_size_t;
        using stmtT = ::sqlite3_stmt;
        friend class connection;
    public:
        device(const device&)            = delete;
        device(device&&)                 = delete;
        device& operator=(const device&) = delete;
        device& operator=(device&&)      = delete;

        device(
            const str_t& filename,
            const eOPENMODE mode,
            const size_t timeout
        );

        ~device();

        void disconnect();
    private:
        stmtT* begQuery(const char*  sql);
        stmtT* begQuery(const str_t& sql);
    private:
        sqlite3* m_device;
        int      m_flags;
    };

} // namespace db

//==============================================================================
//==============================================================================
#endif // !dSQLITEDB_DEVICE_USED_