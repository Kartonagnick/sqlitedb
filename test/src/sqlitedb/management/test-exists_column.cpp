
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_EXISTS_COLUMN

#define dTEST_COMPONENT db
#define dTEST_METHOD existColumn
#define dTEST_TAG tdd

#include <sqlitedb/management.hpp>
#include <sqlitedb/connection.hpp>

#include "test-staff.hpp"
namespace staff = staff_sqlitedb;

using str_t = ::std::string;

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    const char* base = "test-000.txt";
    const char* table  = "users";
    const char* column = "age";
    
    staff::fileDelete(base);
    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_ANY_THROW(db::existColumn(base, table, column));
}

TEST_COMPONENT(001)
{
    const char* base = "test-000.txt";
    const char* table  = "users";
    const char* column = "age";

    staff::fileDelete(base);
    ASSERT_TRUE(!staff::fileExists(base));
    {
        auto out = staff::openWrite(base);
        out << "test-000\n";
    }
    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_ANY_THROW(db::existColumn(base, table, column));
    ASSERT_TRUE(staff::fileDelete(base));
}

TEST_COMPONENT(002)
{
    const char* base = "test-000.db";
    const char* table  = "users";
    const char* column = "age";

    staff::dbaseDelete(base);
    ASSERT_NO_THROW(db::connect(base, db::eCREATE));
    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_ANY_THROW(db::existColumn(base, table, column));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(003)
{
    const char* base = "test-000.db";
    const char* table  = "users";
    const char* column = "age";

    staff::dbaseDelete(base);
    ASSERT_NO_THROW(staff::makeTable(base, table));

    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_TRUE(db::existTable(base, table));
    ASSERT_TRUE(db::existColumn(base, table, column));
    ASSERT_TRUE(!db::existColumn(base, table, "no_exist"));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

// --- death

#ifndef NDEBUG

template<class s> static inline 
const char* ptr_(s&& p) noexcept { return p; }

template<class s1, class s2, class s3>
void deathColumn(s1&& base, s2&& table, s3&& column)
{
    const char* b = ptr_(base);
    const char* t = ptr_(table);
    const char* c = ptr_(column);
    ASSERT_DEATH_DEBUG(db::existColumn(b, t, c));
}
#define dDEATH(base, table, column) deathColumn(base, table, column);

TEST_COMPONENT(004)
{
    dDEATH(nullptr, nullptr, nullptr);
    dDEATH(nullptr, nullptr, ""     );
    dDEATH(nullptr, nullptr, "11"   );

    dDEATH(nullptr, ""     , nullptr);
    dDEATH(nullptr, ""     , ""     );
    dDEATH(nullptr, ""     , "11"   );

    dDEATH(nullptr, "11"   , nullptr);
    dDEATH(nullptr, "11"   , ""     );
    dDEATH(nullptr, "11"   , "11"   );
}

TEST_COMPONENT(005)
{
    dDEATH("" , nullptr, nullptr);
    dDEATH("" , nullptr, ""     );
    dDEATH("" , nullptr, "11"   );
           
    dDEATH("" , ""     , nullptr);
    dDEATH("" , ""     , ""     );
    dDEATH("" , ""     , "11"   );
           
    dDEATH("" , "11"   , nullptr);
    dDEATH("" , "11"   , ""     );
    dDEATH("" , "11"   , "11"   );
}

TEST_COMPONENT(006)
{
    dDEATH("11" , nullptr, nullptr);
    dDEATH("11" , nullptr, ""     );
    dDEATH("11" , nullptr, "11"   );

    dDEATH("11" , ""     , nullptr);
    dDEATH("11" , ""     , ""     );
    dDEATH("11" , ""     , "11"   );

    dDEATH("11" , "11"   , nullptr);
    dDEATH("11" , "11"   , ""     );
}

#endif // !!NDEBUG

//==============================================================================
//==============================================================================
#endif // ! TEST_EXISTS_COLUMN