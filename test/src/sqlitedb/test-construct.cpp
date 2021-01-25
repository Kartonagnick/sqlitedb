
#include <mygtest/modern.hpp>
#include <sqlitedb/sqlitedb.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONSTRUCTORS

#define dTEST_COMPONENT db, connection
#define dTEST_METHOD constructor
#define dTEST_TAG tdd

namespace me = db;

//==============================================================================
//==============================================================================

#include <string>
#include <cassert>
#include <sys/stat.h>

namespace test_construct
{
    #ifdef WIN32 
        #define dSTATE64  _stat64
        #define dWSTATE64 _wstat64
    #else
        #define dSTATE64 stat64
    #endif //!WIN32

    using str_t = std::string;

    inline bool fileDelete(const str_t filename) noexcept
    {
        const bool success
            = ::remove(filename.c_str()) == 0;
        return success;
    }

    inline bool fileExists(const str_t& filename) noexcept
    {
        struct dSTATE64 info;
        const auto re = ::dSTATE64(filename.c_str(), &info);
        if (re != 0)
            return false;
        const bool success = (info.st_mode & S_IFREG) != 0;
        return  success;
    }

} // namespace test_construct
using namespace test_construct;

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    try
    {
        // --- db::eREADONLY
        ASSERT_TRUE(!fileExists("no_exist.db"));
        ASSERT_TRUE(!db::exists("no_exist.db"));
        db::connection con = db::connect("no_exist.db");
        FAIL() << "expected std::exception";
    }
    catch (const std::exception& e)
    {
        dprint(std::cout << e.what() << '\n');
        SUCCEED();
    }
}

TEST_COMPONENT(001)
{
    try
    {
        // --- db::eREADWRITE
        ASSERT_TRUE(!fileExists("no_exist.db"));
        ASSERT_TRUE(!db::exists("no_exist.db"));
        db::connection con 
            = db::connect("no_exist.db", db::eREADWRITE);
        FAIL() << "expected std::exception";
    }
    catch (const std::exception& e)
    {
        dprint(std::cout << e.what() << '\n');
        SUCCEED();
    }
}

TEST_COMPONENT(002)
{
    // --- db::eCREATE
    fileDelete("test.db");
    ASSERT_TRUE(!fileExists("test.db"));
    ASSERT_TRUE(!db::exists("test.db"));
    {
        db::connection con = db::connect("test.db", db::eCREATE);
    }
    ASSERT_TRUE(db::exists("test.db"));
    ASSERT_TRUE(fileExists("test.db"));
    ASSERT_TRUE(fileDelete("test.db"));
    ASSERT_TRUE(!fileExists("test.db"));
    ASSERT_TRUE(!db::exists("test.db"));
}

TEST_COMPONENT(003)
{
    // --- db::eCREATE
    fileDelete("test.db");
    ASSERT_TRUE(!fileExists("test.db"));
    ASSERT_TRUE(!db::exists("test.db"));
    {
        db::connection con = db::connect("test.db", db::eCREATE);
    }
    ASSERT_TRUE(db::exists("test.db"));
    ASSERT_TRUE(fileExists("test.db"));
    ASSERT_TRUE(db::remove("test.db"));
    ASSERT_TRUE(!fileExists("test.db"));
    ASSERT_TRUE(!db::exists("test.db"));
}

//==============================================================================
//==============================================================================
#endif // ! TEST_CONSTRUCTORS