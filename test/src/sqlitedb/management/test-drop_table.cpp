
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_DROP_TABLE

#define dTEST_COMPONENT db
#define dTEST_METHOD dropTable
#define dTEST_TAG tdd

#include <sqlitedb/management.hpp>

#include "test-staff.hpp"
namespace staff = staff_sqlitedb;

using str_t = ::std::string;

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    const char* base = "test-000.db";
    const char* table = "age";

    staff::dbaseDelete(base);
    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_ANY_THROW(db::dropTable(base, table));
}
TEST_COMPONENT(001)
{
    const str_t base = "test-000.txt";
    const char* table = "age";

    staff::dbaseDelete(base);
    ASSERT_TRUE(!staff::fileExists(base));
    staff::openWrite(base) << "test-000\n";
    ASSERT_TRUE(staff::fileExists(base));
    ASSERT_ANY_THROW(db::dropTable(base, table));
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(002)
{
    const str_t base = "test-000.db";
    const char* table = "age";
    staff::fileDelete(base);
    ASSERT_TRUE(!staff::fileExists(base));
    ASSERT_NO_THROW(staff::makeTableAge(base, table));

    ASSERT_TRUE( db::dropTable(base, table));
    ASSERT_TRUE(!db::dropTable(base, "noexist"));
    ASSERT_TRUE( staff::fileDelete(base));
}

#ifndef NDEBUG

template<class s> static inline 
const char* ptr_(s&& p) noexcept { return p; }

template<class s1, class s2>
void deathTable(s1&& base, s2&& table)
{
    const char* b = ptr_(base);
    const char* t = ptr_(table);
    ASSERT_DEATH_DEBUG(db::dropTable(b, t));
}
#define dDEATH(base, table) deathTable(base, table);

TEST_COMPONENT(004)
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
#endif // ! TEST_DROP_TABLE