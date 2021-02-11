
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONNECT

#define dTEST_COMPONENT db, connection
#define dTEST_METHOD connect
#define dTEST_TAG tdd

#include <sqlitedb/connection.hpp>
#include "test-staff.hpp"
using str_t  = ::std::string;
namespace staff = staff_sqlitedb;

//==============================================================================
//==============================================================================

// --- db::eCREATE
TEST_COMPONENT(000)
{
    // must be not exist
    // read : allowed
    // write: allowed

    const char* filename = "000-sample.db";
    staff::dbaseDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    {
        const auto connect = db::connect(filename, db::eCREATE);
        ASSERT_NO_THROW(staff::makeTable(connect, "age"));
        ASSERT_NO_THROW(staff::addToTable(connect, "age", 1, 2));
    }
    ASSERT_TRUE(staff::dbaseDelete(filename));
}

// --- db::eREADONLY
TEST_COMPONENT(001)
{
    // must be exist

    const char* filename = "no_exist.db";
    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_ANY_THROW(db::connect(filename));
}

TEST_COMPONENT(002)
{
    // read : allowed
    // write: not allowed

    const char* filename = "000-sample.db";
    staff::dbaseDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_NO_THROW(db::connect(filename, db::eCREATE));
    {
        const auto conn = db::connect(filename);
        #ifdef NDEBUG
            ASSERT_ANY_THROW(staff::makeTable(conn, "age"));
        #else
            ASSERT_DEATH_DEBUG(staff::makeTable(conn, "age"));
        #endif
    }
    ASSERT_TRUE(staff::dbaseDelete(filename));
}

// --- db::eREADWRITE
TEST_COMPONENT(003)
{
    // must be exist

    const char* filename = "no_exist.db";
    staff::dbaseDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_ANY_THROW(db::connect(filename, db::eREADWRITE));
}

TEST_COMPONENT(004)
{
    // read : allowed
    // write: allowed

    const char* filename = "000-sample.db";
    staff::dbaseDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    ASSERT_NO_THROW(db::connect(filename, db::eCREATE));
    {
        const auto conn = db::connect(filename, db::eREADWRITE);
        ASSERT_NO_THROW(staff::makeTable(conn, "users"));
        ASSERT_NO_THROW(staff::addToTable(conn, 1,1));
        ASSERT_NO_THROW(staff::addToTable(conn, 2,2));
        ASSERT_NO_THROW(staff::addToTable(conn, 3,3));
    }
    ASSERT_TRUE(staff::dbaseDelete(filename));
}


TEST_COMPONENT(005)
{
    // --- copy

    const char* filename = "000-sample.db";
    staff::dbaseDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    {
        db::connection con1 = db::connect(filename, db::eCREATE);
        db::connection con2 = con1;
    }
    ASSERT_TRUE(staff::fileExists(filename));
    ASSERT_TRUE(staff::dbaseDelete(filename));
}

TEST_COMPONENT(006)
{
    // --- move

    const char* filename = "000-sample.db";
    staff::dbaseDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    {
        db::connection con1 = db::connect(filename, db::eCREATE);
        db::connection con2 = std::move(con1);
    }
    ASSERT_TRUE(staff::fileExists(filename));
    ASSERT_TRUE(staff::dbaseDelete(filename));
}

TEST_COMPONENT(007)
{
    // --- operator=

    const char* base1 = "000-sample.db";
    const char* base2 = "001-sample.db";

    staff::dbaseDelete(base1);
    staff::dbaseDelete(base2);
    ASSERT_TRUE(!staff::fileExists(base1));
    ASSERT_TRUE(!staff::fileExists(base2));
    {
        db::connection con1 = db::connect(base1, db::eCREATE);
        db::connection con2 = db::connect(base2, db::eCREATE);
        con2 = con1;
    }
    ASSERT_TRUE(staff::fileExists(base1));
    ASSERT_TRUE(staff::fileExists(base2));
    ASSERT_TRUE(staff::dbaseDelete(base1));
    ASSERT_TRUE(staff::dbaseDelete(base2));
}

TEST_COMPONENT(008)
{
    // --- operator=(move)

    const char* base1 = "000-sample.db";
    const char* base2 = "001-sample.db";

    staff::dbaseDelete(base1);
    staff::dbaseDelete(base2);
    ASSERT_TRUE(!staff::fileExists(base1));
    ASSERT_TRUE(!staff::fileExists(base2));
    {
        db::connection con1 = db::connect(base1, db::eCREATE);
        db::connection con2 = db::connect(base2, db::eCREATE);
        con2 = std::move(con1);
    }
    ASSERT_TRUE(staff::fileExists(base1));
    ASSERT_TRUE(staff::fileExists(base2));
    ASSERT_TRUE(staff::dbaseDelete(base1));
    ASSERT_TRUE(staff::dbaseDelete(base2));
}

//==============================================================================
//==============================================================================

#ifndef NDEBUG

TEST_COMPONENT(009)
{
    const char* filename = nullptr;
    ASSERT_DEATH_DEBUG(db::connect(filename));
}

TEST_COMPONENT(010)
{
    const char* filename = "";
    ASSERT_DEATH_DEBUG(db::connect(filename));
}

TEST_COMPONENT(011)
{
    const str_t filename = "";
    ASSERT_DEATH_DEBUG(db::connect(filename));
}

TEST_COMPONENT(012)
{
    const char* base = "012-sample.db";
    ASSERT_DEATH_DEBUG(db::connect(base, 1000500));
}

#endif // !!NDEBUG

//==============================================================================
//==============================================================================

TEST_COMPONENT(013)
{
    const char* base = "002-sample.db";

    staff::dbaseDelete(base);
    ASSERT_TRUE(!staff::fileExists(base));
    {
        db::connection con = db::connect(base, db::eCREATE | db::eREADWRITE);
        staff::makeTable(con, "users");
        ASSERT_TRUE(con.existTable("users"));
    }
    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(014)
{
    const char* base = "014-sample.db";

    staff::dbaseDelete(base);
    ASSERT_TRUE(!staff::fileExists(base));
    {
        db::connection con1 = db::connect(base, db::eCREATE | db::eREADWRITE);
        staff::makeTable(con1, "users");
        ASSERT_TRUE(con1.existTable("users"));
        staff::addToTable(con1, 12, 12);
        staff::addToTable(con1, 22, 22);
        staff::addToTable(con1, 32, 32);

        {
            db::connection con2 = db::connect(base, db::eCREATE | db::eREADWRITE);
            staff::makeTable(con2, "cats");
            ASSERT_TRUE(con2.existTable("cats"));
        }
    }
    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(015)
{
    const char* base = "015-saturn.db";

    staff::dbaseDelete(base);
    ASSERT_TRUE(!staff::fileExists(base));
    {
        db::connect(base, db::eCREATE);

        db::connection con2 = db::connect(base, db::eREADWRITE);        
        staff::makeTable(con2, "users");
        staff::addToTable(con2, 42, 32);
        {
            db::connection con1 = db::connect(base);
        }
        staff::addToTable(con2, 32, 32);
    }
    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

//==============================================================================
//==============================================================================
#endif // ! TEST_CONNECT