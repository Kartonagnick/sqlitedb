
// [2021y-01m-23d] Idrisov Denis R.

#include "exception.hpp"
#include "device.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <cassert>
#include <cstring>

//==============================================================================
//===[sqlite3] =================================================================
namespace db
{
    using stmtT = ::sqlite3_stmt;
    using sizeT = ::std::atomic_size_t;

    // cast
    namespace
    {
        inline int cast(const size_t value) noexcept
        {
            #ifndef NDEBUG
            using limit = ::std::numeric_limits<int>;
            assert(size_t(limit().max()) > value);
            #endif // !NDEBUG
            return static_cast<int>(value);
        }

        const str_t& cast(const str_t& v) noexcept
            { return v; }

        str_t cast(const char* v) 
        {
            assert(v);
            return v;
        }

        const char* c_str(const str_t& v) noexcept
            { return v.c_str(); }

        const char* c_str(const char* v) noexcept
        {
            assert(v);
            return v;
        }

    } // namespace

    // flags
    namespace
    {
        // true, если все указанные в my включены в flags
        static inline 
        bool hasFlags(const int my, const int flags) noexcept
            { return (my & flags) == my; }

        #if 0
        // true, если любой, из указаннных в my, включен в flags
        static inline 
        bool hasAnyFlags(const int my, const int flags) noexcept
            { return (my & flags) != 0; }
        #endif

        static inline 
        void addFlags(const int my, int& flags) noexcept
            { flags |= my; }

        static inline 
        void delFlags(const int my, int& flags) noexcept
            { flags &= (~my); }

        static inline 
        bool checkFlags(int flags) noexcept
        {
            const int my = eREADONLY | eREADWRITE | eCREATE | eOPEN_URI;
            ::db::delFlags(my, flags);
            return flags == 0;
        }

        static inline 
        int adaptive(const int mode) noexcept
        {
            assert(db::checkFlags(mode));
            int flags = 0;

            if (hasFlags(eCREATE, mode))
                ::db::addFlags(SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, flags);

            else if (hasFlags(eREADONLY, mode))
                ::db::addFlags(SQLITE_OPEN_READONLY, flags);

            else if (hasFlags(eREADWRITE, mode))
                ::db::addFlags(SQLITE_OPEN_READWRITE, flags);

            else if (hasFlags(eOPEN_URI, mode))
                ::db::addFlags(SQLITE_OPEN_URI, flags);

            return flags;
        }

    } // namespace

    // sqlite3
    namespace
    {
        static inline 
        void close(::sqlite3* device) noexcept
        {
            assert(device);
            const int ret = ::sqlite3_close(device);
            assert(ret == SQLITE_OK);
            (void) ret;
        }

        static inline 
        void setTimeout(::sqlite3* device, const size_t milliseconds)
        {
            assert(device);
            const int timeout = ::db::cast(milliseconds);
            const int ret = ::sqlite3_busy_timeout(device, timeout);
            if (SQLITE_OK == ret)
                return;

            const str_t msg = ::sqlite3_errmsg(device);
            throw std::runtime_error("[db::setTimeout] sqlite3_busy_timeout: " + msg);
        }

        static inline 
        void execSQL(::sqlite3* device, const char* sql) 
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
            throw db::exception(ret_code, reason);
        }

        static inline 
        void disconnect(::sqlite3*& device, const int flags) 
        {
            int walLog = 0;
            int walCheckpoints = 0;
            const int result = ::sqlite3_wal_checkpoint_v2(
                device, 
                nullptr, 
                SQLITE_CHECKPOINT_FULL, 
                &walLog, 
                &walCheckpoints
            );

            str_t reason;
            if(result != SQLITE_OK)
                reason += "[db::disconnect] failed to run a checkpoint operation\n";

            if(::sqlite3_get_autocommit(device) == 0)
                reason += "[db::disconnect] auto-commit mode must be enabled\n";

            if(db::hasFlags(db::eREADWRITE, flags))
            {
                try
                {
                    db::execSQL(device, "VACUUM;ANALYZE");
                }
                catch (const ::std::exception& e)
                {
                    reason += e.what();
                    reason += "\n[db::disconnect] "
                        "operation 'defragmented' or 'analyzed' failed\n";
                }
            }

            const int ret = ::sqlite3_close(device);
            device = nullptr;
            assert(ret == SQLITE_OK);
            if(ret != SQLITE_OK)
                reason += "[db::disconnect] operation 'close' failed\n";
            
            if (!reason.empty())
            {
                assert(reason.back() == '\n');
                reason.pop_back();
                throw ::std::runtime_error(reason);
            }
        }

        static inline 
        ::sqlite3* getDevice(const str_t& filename, const int flags, const size_t timeout)
        {
            assert(!filename.empty());

            ::sqlite3* device = nullptr;
            const int ret = ::sqlite3_open_v2(
                filename.c_str(),
                &device,
                ::db::adaptive(flags),
                nullptr
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

        template<class S>
        static inline
        stmtT* prepare(::sqlite3* device, S&& sql)
        {
            assert(device);
            stmtT* cur = nullptr;
            const auto ret = ::sqlite3_prepare_v2(
                device, db::c_str(sql), -1, 
                &cur, nullptr
            );
            if(ret == SQLITE_OK)
            {
                assert(cur);
                return cur;
            }

            const str_t msg = ::sqlite3_errmsg(device);

            const str_t reason = "[db::begQuery] "
                "sqlite3_prepare_v2(" + db::cast(sql) + "): " + msg;

            const int result 
                = std::strncmp(msg.c_str(), "no such table: ", 15);

            if(result == 0)
                throw db::exception(db::exception::eTABLE_DOES_NOT_EXIST, reason);
            throw db::exception(ret, reason);
        }

    } //namespace

    enum eEXTENDED_ERROR
    {
        eDATABASE_NOT_AVAILABLE = 26,
        eTABLE_DOES_NOT_EXIST  = 200,  
    };

    const char* code2desc(const int code) noexcept
    {
        if (code == eTABLE_DOES_NOT_EXIST)
            return "table does not exist";
        const auto* msg =  ::sqlite3_errstr(code);
        assert(msg);
        return msg;
    }

    const char* code2name(const int code) noexcept
    {
        #define dSQLITE_CODE(code) case code: return #code
        switch (code)
        {
            dSQLITE_CODE(SQLITE_OK);
            dSQLITE_CODE(SQLITE_ERROR);
            dSQLITE_CODE(SQLITE_INTERNAL);
            dSQLITE_CODE(SQLITE_PERM);
            dSQLITE_CODE(SQLITE_ABORT);
            dSQLITE_CODE(SQLITE_BUSY);
            dSQLITE_CODE(SQLITE_LOCKED);
            dSQLITE_CODE(SQLITE_NOMEM);
            dSQLITE_CODE(SQLITE_READONLY);
            dSQLITE_CODE(SQLITE_INTERRUPT);
            dSQLITE_CODE(SQLITE_IOERR);
            dSQLITE_CODE(SQLITE_CORRUPT);
            dSQLITE_CODE(SQLITE_NOTFOUND);
            dSQLITE_CODE(SQLITE_FULL);
            dSQLITE_CODE(SQLITE_CANTOPEN);
            dSQLITE_CODE(SQLITE_PROTOCOL);
            dSQLITE_CODE(SQLITE_EMPTY);
            dSQLITE_CODE(SQLITE_SCHEMA);
            dSQLITE_CODE(SQLITE_TOOBIG);
            dSQLITE_CODE(SQLITE_CONSTRAINT);
            dSQLITE_CODE(SQLITE_MISMATCH);
            dSQLITE_CODE(SQLITE_MISUSE);
            dSQLITE_CODE(SQLITE_NOLFS);
            dSQLITE_CODE(SQLITE_AUTH);
            dSQLITE_CODE(SQLITE_FORMAT);
            dSQLITE_CODE(SQLITE_RANGE);
            dSQLITE_CODE(SQLITE_NOTADB);
            dSQLITE_CODE(SQLITE_NOTICE);
            dSQLITE_CODE(SQLITE_WARNING);
            dSQLITE_CODE(SQLITE_ROW);
            dSQLITE_CODE(SQLITE_DONE);
            dSQLITE_CODE(eTABLE_DOES_NOT_EXIST);
            #undef dSQLITE_CODE
        default:
            assert(false && "invalid-switch-case");
        }
        return "invalid-switch-case";
    }

} // namespace db


//==============================================================================
//===[connection::data] ========================================================
namespace db
{
    device::device(const str_t& filename, const int mode, const size_t timeout)
        : m_mutex()
        , m_device()
        , m_flags()
    {
        assert(!filename.empty());
        this->m_device 
            = ::db::getDevice(filename, mode, timeout);
    }

    device::~device()
    {
        this->disconnect();
        assert(!this->m_device);
    }
    void device::disconnect() 
    {
        ::std::lock_guard<std::mutex> 
            lock(this->m_mutex);

        if (!this->m_device)
            return;

        ::db::disconnect(this->m_device, this->m_flags);
    }

    stmtT* device::prepare(const str_t& sql)
    {
        ::std::lock_guard<std::mutex> 
            lock(this->m_mutex);

        assert(!sql.empty());
        assert(this->m_device);
        return ::db::prepare(this->m_device, sql);
    }

    stmtT* device::prepare(const char* sql)  
    {
        ::std::lock_guard<std::mutex> 
            lock(this->m_mutex);

        assert(sql);
        assert(*sql != '\0');
        assert(this->m_device);
        return ::db::prepare(this->m_device, sql);
    }

} // namespace db

//==============================================================================
//==============================================================================


