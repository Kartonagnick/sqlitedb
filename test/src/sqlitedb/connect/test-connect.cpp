
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONNECT

#define dTEST_COMPONENT db, connection
#define dTEST_METHOD connect
#define dTEST_TAG tdd

#include <sqlitedb/connection.hpp>
#include "test-staff.hpp"
namespace staff = staff_sqlitedb;
namespace me = db;
//==============================================================================
//==============================================================================

// --- db::eREADONLY
TEST_COMPONENT(000)
{
    const char* filename = "no_exist.db";
    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_ANY_THROW(db::connect(filename));
}
TEST_COMPONENT(001)
{
    const char* filename = "no_exist.db";
    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_NO_THROW(db::connect(filename, db::eCREATE));
    {
        const auto conn = db::connect(filename);
        #ifdef NDEBUG
            ASSERT_ANY_THROW(staff::makeTableAge(conn, "age"));
        #else
            ASSERT_DEATH_DEBUG(staff::makeTableAge(conn, "age"));
        #endif
    }
    ASSERT_TRUE(staff::fileDelete(filename));
}



#if 0

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


TEST_COMPONENT(004)
{
    // --- copy

    fileDelete("test.db");
    ASSERT_TRUE(!fileExists("test.db"));
    ASSERT_TRUE(!db::exists("test.db"));
    {
        db::connection con1 = db::connect("test.db", db::eCREATE);
        db::connection con2 = con1;
    }
    ASSERT_TRUE(db::exists("test.db"));
    ASSERT_TRUE(fileExists("test.db"));
    ASSERT_TRUE(db::remove("test.db"));
    ASSERT_TRUE(!fileExists("test.db"));
    ASSERT_TRUE(!db::exists("test.db"));
}

TEST_COMPONENT(005)
{
    // --- move

    fileDelete("test.db");
    ASSERT_TRUE(!fileExists("test.db"));
    ASSERT_TRUE(!db::exists("test.db"));
    {
        db::connection con1 = db::connect("test.db", db::eCREATE);
        db::connection con2 = std::move(con1);
    }
    ASSERT_TRUE(db::exists("test.db"));
    ASSERT_TRUE(fileExists("test.db"));
    ASSERT_TRUE(db::remove("test.db"));
    ASSERT_TRUE(!fileExists("test.db"));
    ASSERT_TRUE(!db::exists("test.db"));
}

TEST_COMPONENT(006)
{
    // --- operator=

    fileDelete("test-A.db");
    fileDelete("test-B.db");
    ASSERT_TRUE(!fileExists("test-A.db"));
    ASSERT_TRUE(!fileExists("test-B.db"));
    ASSERT_TRUE(!db::exists("test-A.db"));
    ASSERT_TRUE(!db::exists("test-B.db"));
    {
        db::connection con1 = db::connect("test-A.db", db::eCREATE);
        db::connection con2 = db::connect("test-B.db", db::eCREATE);
        con2 = con1;
    }
    ASSERT_TRUE(db::exists("test-A.db"));
    ASSERT_TRUE(db::exists("test-B.db"));
    ASSERT_TRUE(fileExists("test-A.db"));
    ASSERT_TRUE(fileExists("test-B.db"));
    ASSERT_TRUE(db::remove("test-A.db"));
    ASSERT_TRUE(db::remove("test-B.db"));
    ASSERT_TRUE(!fileExists("test-A.db"));
    ASSERT_TRUE(!fileExists("test-B.db"));
    ASSERT_TRUE(!db::exists("test-A.db"));
    ASSERT_TRUE(!db::exists("test-B.db"));
}

TEST_COMPONENT(007)
{
    // --- operator=

    fileDelete("test-A.db");
    fileDelete("test-B.db");
    ASSERT_TRUE(!fileExists("test-A.db"));
    ASSERT_TRUE(!fileExists("test-B.db"));
    ASSERT_TRUE(!db::exists("test-A.db"));
    ASSERT_TRUE(!db::exists("test-B.db"));
    {
        db::connection con1 = db::connect("test-A.db", db::eCREATE);
        db::connection con2 = db::connect("test-B.db", db::eCREATE);
        con2 = std::move(con1);
    }
    ASSERT_TRUE(db::exists("test-A.db"));
    ASSERT_TRUE(db::exists("test-B.db"));
    ASSERT_TRUE(fileExists("test-A.db"));
    ASSERT_TRUE(fileExists("test-B.db"));
    ASSERT_TRUE(db::remove("test-A.db"));
    ASSERT_TRUE(db::remove("test-B.db"));
    ASSERT_TRUE(!fileExists("test-A.db"));
    ASSERT_TRUE(!fileExists("test-B.db"));
    ASSERT_TRUE(!db::exists("test-A.db"));
    ASSERT_TRUE(!db::exists("test-B.db"));
}
#endif


//==============================================================================
//==============================================================================
#endif // ! TEST_CONNECT