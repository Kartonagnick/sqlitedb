
#include <mygtest/modern.hpp>
#include <sqlitedb/sqlitedb.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_COLUMN

#define dTEST_COMPONENT db, connection
#define dTEST_METHOD column
#define dTEST_TAG tdd

//==============================================================================
//==============================================================================

#include <sqlitedb/connection.hpp>
#include "test-staff.hpp"
using str_t  = ::std::string;
namespace staff = staff_sqlitedb;
namespace me = db;

//==============================================================================
//==============================================================================

namespace
{
    const char* base = "000-sample.db";
}

//==============================================================================
//==============================================================================

// --- existColumn
TEST_COMPONENT(000)
{
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "users"));
    {
        const auto con = db::connect(base);
        ASSERT_TRUE( con.existTable("users"));
        ASSERT_TRUE( con.existColumn("users", "login"   ));
        ASSERT_TRUE( con.existColumn("users", "age"     ));
        ASSERT_TRUE(!con.existColumn("users", "no_exist"));
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(001)
{
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "users"));
    {
        const auto con = db::connect(base);
        ASSERT_TRUE( con.existTable("users"));

        ASSERT_TRUE( con.existColumn("users", "login"   ));
        ASSERT_TRUE( con.existColumn("users", "age"     ));
        ASSERT_TRUE(!con.existColumn("users", "no_exist"));

        ASSERT_TRUE(!con.existTable("no_exist"));
        ASSERT_ANY_THROW(con.existColumn("no_exist", "login"));
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}



#ifndef NDEBUG

template<class s> static inline 
const char* ptr_(s&& p) noexcept { return p; }

template<class s1, class s2>
void deathColumn(s1&& table, s2&& column)
{
    const char* t = ptr_(table);
    const char* c = ptr_(column);
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, "users"));
    const auto con = db::connect(base, db::eCREATE);
    ASSERT_DEATH_DEBUG(con.existColumn(t, c));
}
#define dDEATH(table, column) deathColumn(table, column);

TEST_COMPONENT(002)
{
    dDEATH(nullptr, nullptr);
    dDEATH(nullptr, ""     );
    dDEATH(nullptr, "11"   );

    dDEATH(""     , nullptr);
    dDEATH(""     , ""     );
    dDEATH(""     , "11"   );

    dDEATH("11"   , nullptr);
    dDEATH("11"   , ""     );
}

#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_COLUMN
