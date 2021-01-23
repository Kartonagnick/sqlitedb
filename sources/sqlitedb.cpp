
// [2021y-01m-23d] Idrisov Denis R.

#include <sqlitedb/sqlitedb.hpp>
#include <sqlite3.h>
#include <stdexcept>
#include <cassert>

//==============================================================================
//===[connection::data] ========================================================
namespace db
{
    // flags
    namespace
    {
        // true, если все указанные в my включены в flags
        inline bool has_flags(const int my, const int flags) noexcept
            { return (my & flags) == my; }

        // true, если любой, из указаннных в my, включен в flags
        inline bool has_any_flags(const int my, const int flags) noexcept
            { return (my & flags) != 0; }

        inline void add_flags(const int my, int& flags) noexcept
            { flags |= my; }

        inline void del_flags(const int my, int& flags) noexcept
            { flags &= (~my); }

        inline bool check_source_mode(int flags) noexcept
        {
            const int my = eREADONLY | eREADWRITE | eCREATE | eOPEN_URI;
            ::db::del_flags(my, flags);
            return flags == 0;
        }

        inline int adaptive(const int mode) noexcept
        {
            assert(db::check_source_mode(mode));
            int flags = 0;

            if (has_flags(eCREATE, mode))
                ::db::add_flags(SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, flags);

            else if (has_flags(eREADONLY, mode))
                ::db::add_flags(SQLITE_OPEN_READONLY, flags);

            else if (has_flags(eREADWRITE, mode))
                ::db::add_flags(SQLITE_OPEN_READWRITE, flags);

            else if (has_flags(eOPEN_URI, mode))
                ::db::add_flags(SQLITE_OPEN_URI, flags);

            return flags;
        }

    } // namespace

    // sqlite3
    namespace
    {
        inline void close(::sqlite3* device) noexcept
        {
            assert(device);
            const int ret = ::sqlite3_close(device);
            assert(ret == SQLITE_OK);
            (void) ret;
        }

        inline void setTimeout(::sqlite3* device, const size_t milliseconds)
        {
            #ifndef NDEBUG
                assert(device);
                using limit = ::std::numeric_limits<int>;
                assert(size_t(limit().max()) > milliseconds);
            #endif // !NDEBUG

            const int timeout
                = static_cast<int>(milliseconds);

            const int ret
                = ::sqlite3_busy_timeout(device, timeout);

            if (SQLITE_OK == ret)
                return;

            const str_t msg = ::sqlite3_errmsg(device);
            throw std::runtime_error("[db::setTimeout] sqlite3_busy_timeout: " + msg);
        }

        inline void execSQL(::sqlite3* device, const char* sql) 
        {
            assert(sql);
            assert(device);
            char* err_msg = nullptr;

            const int ret_code
                = ::sqlite3_exec(device, sql, nullptr, nullptr, &err_msg);

            if (ret_code == SQLITE_OK)
                return;

            assert(err_msg);
            const str_t msg_ = err_msg;
            const str_t sql_ = sql;

            const str_t reason
                = "[db::execSQL] '" + sql_ + "' (" + msg_ + ")";

            ::sqlite3_free(err_msg);
            throw std::runtime_error(reason);
        }

        inline ::sqlite3* getDevice(
            const str_t& filename,
            const eOPENMODE flags,
            const size_t timeout,
            const str_t& vfs)
        {
            ::sqlite3* device = nullptr;
            const int ret = ::sqlite3_open_v2(
                filename.c_str(),
                &device,
                ::db::adaptive(flags),
                vfs.c_str()
            );
            if (SQLITE_OK != ret)
            {
                ::db::close(device);
                throw std::runtime_error("[db::getDevice] "
                    "can`t open: '" + filename + "'"
                );
            }
            assert(device);
            try
            {
                if (timeout > 0)
                    ::db::setTimeout(device, timeout);

                const char pragma[] =
                    "PRAGMA synchronous = OFF; "
                    "PRAGMA foreign_keys = ON; "
                    "PRAGMA journal_mode = WAL; "
                    "PRAGMA wal_autocheckpoint = 250; "
                    "PRAGMA cache_size = 2000;"
                ;
                ::db::execSQL(device, pragma);
                return device;
            }
            catch (...)
            {
                ::sqlite3_close(device);
                throw;
            }
        }

        void disconnect(::sqlite3*& connector, const int flags) 
        {
            int walLog = 0;
            int walCheckpoints = 0;
            const int result = ::sqlite3_wal_checkpoint_v2(
                connector, 
                nullptr, 
                SQLITE_CHECKPOINT_FULL, 
                &walLog, 
                &walCheckpoints
            );

            str_t reason;
            if(result != SQLITE_OK)
                reason += "[db::disconnect] failed to run a checkpoint operation\n";

            if(::sqlite3_get_autocommit(connector) == 0)
                reason += "[db::disconnect] auto-commit mode must be enabled\n";

            if(db::has_flags(db::eREADWRITE, flags))
            {
                try
                {
                    db::execSQL(connector, "VACUUM;ANALYZE");
                }
                catch (const std::exception& e)
                {
                    reason += e.what();
                    reason += "\n[db::disconnect] "
                        "operation 'defragmented' or 'analyzed' failed\n";
                }
            }

            const int ret = ::sqlite3_close(connector);
            connector = nullptr;
            assert(ret == SQLITE_OK);
            if(ret != SQLITE_OK)
                reason += "[db::disconnect] operation 'close' failed\n";
            
            if (!reason.empty())
            {
                assert(reason.back() == '\n');
                reason.pop_back();
                throw std::runtime_error(reason);
            }
        }

    } //namespace

    class connection::device
    {
    public:
        device(const str_t& path, const eOPENMODE mode, 
            const size_t timeout, const str_t& vfs)
            : m_device()
            , m_cursor()
            , m_index()
            , m_flags()
        {
            this->m_device = ::db::getDevice(path, mode, timeout, vfs);
        }

        ~device()
        {
            assert(!this->m_device);
        }
        void disconnect() 
        {
            ::db::disconnect(this->m_device, this->m_flags);
        }

    private:
        ::sqlite3*      m_device;
        ::sqlite3_stmt* m_cursor;
        size_t          m_index;
        int             m_flags;
    };

} // namespace db

//==============================================================================
//=== [constructors] ===========================================================
namespace db
{
    connection::connection() noexcept
        : m_data()
    {}

    connection::~connection()
    {
        try
        {
            this->m_data->disconnect();
        }
        catch (const std::exception& e)
        {
            const char* reason = e.what();
            (void) reason;
            assert(false && "exception from destructor");
        }
    }

    connection::connection(const connection& rhs) noexcept
        : m_data(rhs.m_data)
    {}

    connection::connection(connection&& rhs) noexcept
        : m_data(std::move(rhs.m_data))
    {}

    connection& connection::operator=(const connection& rhs) noexcept
    {
        if (this != &rhs)
            this->m_data = rhs.m_data;
        return *this;
    }

    connection& connection::operator=(connection&& rhs) noexcept
    {
        if (this != &rhs)
            this->m_data = std::move(rhs.m_data);
        return *this;
    }

    connection connect(const str_t& path, const eOPENMODE mode, 
        const size_t timeout, const str_t& vfs)
    {
        connection result;
        result.m_data
            = ::std::make_shared<connection::device>(path, mode, timeout, vfs);
        return result;
    }

} // namespace db

//==============================================================================
//==============================================================================

