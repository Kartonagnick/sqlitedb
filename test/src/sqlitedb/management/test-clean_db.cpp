
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CLEAN_DATABASE

#define dTEST_COMPONENT db
#define dTEST_METHOD cleanDatabase
#define dTEST_TAG tdd

#include <sqlitedb/connection.hpp>
#include <sqlitedb/management.hpp>

#include "test-staff.hpp"
namespace staff = staff_sqlitedb;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

#ifndef NDEBUG
TEST_COMPONENT(000)
{
    ASSERT_DEATH_DEBUG(db::cleanDatabase(""));
}
TEST_COMPONENT(001)
{
    const char* p = nullptr;
    ASSERT_DEATH_DEBUG(db::cleanDatabase(p));
    (void)p;
}
TEST_COMPONENT(002)
{
    const str_t p;
    ASSERT_DEATH_DEBUG(db::cleanDatabase(p));
    (void)p;
}
#endif // !NDEBUG

TEST_COMPONENT(003)
{
    const char* base = "test-optima.txt";
    staff::dbaseDelete(base);
    auto con = db::connect(base, db::eCREATE);
    con.cleanDB();
}

TEST_COMPONENT(004)
{
    const char* base = "test-optima.txt";
    staff::dbaseDelete(base);
    auto con = db::connect(base, db::eCREATE | db::eREADWRITE);
    con.cleanDB();
}
TEST_COMPONENT(005)
{
    const char* base = "test-optima.txt";
    staff::dbaseDelete(base);
    db::connect(base, db::eCREATE);
    auto con = db::connect(base, db::eREADWRITE);
    con.cleanDB();
}
TEST_COMPONENT(006)
{
    const char* base = "test-optima.txt";
    staff::dbaseDelete(base);
    db::connect(base, db::eCREATE);
    auto con = db::connect(base, db::eREADONLY);
    ASSERT_ANY_THROW(con.cleanDB());
}
TEST_COMPONENT(007)
{
    const char* base = "test-optima.txt";
    staff::dbaseDelete(base);
    ASSERT_ANY_THROW(db::cleanDatabase(base));
}
TEST_COMPONENT(008)
{
    const char* base = "test-optima.txt";
    staff::dbaseDelete(base);
    db::connect(base, db::eCREATE);
    db::cleanDatabase(base);
}

//==============================================================================
//==============================================================================
#endif // ! TEST_CLEAN_DATABASE