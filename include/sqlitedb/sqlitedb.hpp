
// [2021y-01m-23d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_USED_ 
#define dSQLITEDB_USED_ 1

#include <string>
#include <memory>

//==============================================================================
//=== [connection] =============================================================
namespace db
{
    using str_t = ::std::string;

    enum eOPENMODE 
    { 
        eREADONLY  = 1<<1, /// < Open with 'read only' mode
        eREADWRITE = 1<<2, /// < Open with 'read and write' mode
        eCREATE    = 1<<3, /// < Create the database if it does not exist.
        eOPEN_URI  = 1<<4  /// < open for creating database
    };

    class connection
    {
        class device;

        friend connection connect(
            const str_t&, 
            const eOPENMODE, 
            const size_t, 
            const str_t&
        );

        connection() noexcept;
    public:
        ~connection();

        connection(const connection&)            noexcept;
        connection(connection&&)                 noexcept;
        connection& operator=(const connection&) noexcept;
        connection& operator=(connection&&)      noexcept;
    private:
        using shared_t = std::shared_ptr<device>;
        shared_t m_data;
    };

} // namespace db

//==============================================================================
//=== [connect] ================================================================
namespace db
{
    class settings;

    connection connect(
        const str_t& path, 
        const eOPENMODE mode = eREADONLY,
        const size_t timeout = 150,
        const str_t& vfs     = nullptr
    );

} // namespace db

//==============================================================================
//==============================================================================

#endif // !dSQLITEDB_USED_

