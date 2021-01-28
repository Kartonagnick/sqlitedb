
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
        ASSERT_NO_THROW(staff::makeTableAge(connect, "age"));
        ASSERT_NO_THROW(staff::addToAgeTable(connect, 1, 2));
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
            ASSERT_ANY_THROW(staff::makeTableAge(conn, "age"));
        #else
            ASSERT_DEATH_DEBUG(staff::makeTableAge(conn, "age"));
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
        ASSERT_NO_THROW(staff::makeTableAge(conn, "age"));
        ASSERT_NO_THROW(staff::addToAgeTable(conn, 1,1));
        ASSERT_NO_THROW(staff::addToAgeTable(conn, 2,2));
        ASSERT_NO_THROW(staff::addToAgeTable(conn, 3,3));
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

#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_CONNECT