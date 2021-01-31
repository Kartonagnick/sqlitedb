
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONTAINER

#define dTEST_COMPONENT db, request
#define dTEST_METHOD container
#define dTEST_TAG vec_tuple

#include "test-staff.hpp"
#include <sqlitedb/sqlitedb.hpp>
#include <vector>

namespace staff = staff_sqlitedb;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

static const char* base = "00-vec_tuple.db";

using normal_t = std::vector<std::tuple<size_t, size_t>>;
using small_t  = std::vector<std::tuple<size_t>>;
using big_t    = std::vector<std::tuple<size_t, size_t, size_t>>;

//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    //--- От Базы пришел пустой ответ.

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        normal_t dst;
        const char* sql = "select * from users where login = 300";
        con << sql >> dst;
        ASSERT_TRUE(dst.empty());
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(001)
{
    //--- От Базы пришла одна строке
    //--- количество полей совпадает

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        normal_t dst;
        const char* sql = "select * from users where login = 3";
        con << sql >> dst;
        ASSERT_TRUE(dst.size() == 1);
        const auto& tuple = dst.front();
        ASSERT_TRUE(std::get<0>(tuple) == 3);
        ASSERT_TRUE(std::get<1>(tuple) == 6);
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}
TEST_COMPONENT(002)
{
    //--- От Базы пришла одна строке
    //--- количество меньше совпадает

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        small_t dst;
        const char* sql = "select * from users where login = 3";
        con << sql >> dst;
        ASSERT_TRUE(dst.size() == 1);

        const auto& tuple = dst.front();
        ASSERT_TRUE(std::get<0>(tuple) == 3);
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}
TEST_COMPONENT(003)
{
    //--- От Базы пришла одна строке
    //--- количество полей больше чем столбцов

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    const auto check = []
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        big_t dst;
        const char* sql = "select * from users where login = 3";
        con << sql >> dst;
    };

    #ifdef NDEBUG
        ASSERT_ANY_THROW(check());
    #else
        ASSERT_DEATH_DEBUG(check());
    #endif 

    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(004)
{
    //--- От Базы пришло множество строк
    //--- количество полей совпадает

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        normal_t dst;
        const char* sql = "select * from users";
        con << sql >> dst;
        ASSERT_TRUE(dst.size() == 10);

        size_t i = 0;
        for (const auto& tuple : dst)
        {
            ASSERT_TRUE(std::get<0>(tuple) == i);
            ASSERT_TRUE(std::get<1>(tuple) == i * 2);
            ++i;
        }
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}
TEST_COMPONENT(005)
{
    //--- От Базы пришло множество строк
    //--- количество полей меньше

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        small_t dst;
        const char* sql = "select * from users";
        con << sql >> dst;
        ASSERT_TRUE(dst.size() == 10);

        size_t i = 0;
        for (const auto& tuple : dst)
        {
            ASSERT_TRUE(std::get<0>(tuple) == i);
            ++i;
        }
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}
TEST_COMPONENT(006)
{
    //--- От Базы пришло множество строк
    //--- количество полей больше

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    const auto check = []
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        big_t dst;
        const char* sql = "select age from users";
        con << sql >> dst;
    };

    #ifdef NDEBUG
        ASSERT_ANY_THROW(check());
    #else
        ASSERT_DEATH_DEBUG(check());
    #endif 

    ASSERT_TRUE(staff::dbaseDelete(base));
}

//==============================================================================
//==============================================================================
#endif // ! TEST_CONTAINER
