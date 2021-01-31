
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONTAINER

#define dTEST_COMPONENT db, request
#define dTEST_METHOD container
#define dTEST_TAG vec_int

#include "test-staff.hpp"
#include <sqlitedb/sqlitedb.hpp>
#include <vector>

namespace staff = staff_sqlitedb;
using cont_t = std::vector<int>;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

static const char* base = "00-vec_container.db";

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

        cont_t dst;
        const char* sql = "select * from users where login = 300";
        con << sql >> dst;
        ASSERT_TRUE(dst.empty());
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(001)
{
    //--- От Базы пришла одна строке

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        cont_t dst;
        const char* sql = "select age from users where login = 3";
        con << sql >> dst;
        ASSERT_TRUE(dst.size() == 1);
        ASSERT_TRUE(dst.front() == 6);
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(002)
{
    //--- От Базы пришла одна строка
    //--- количество столбцов больше, чем полей

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        cont_t dst;
        const char* sql = "select * from users where login = 3";
        con << sql >> dst;
        ASSERT_TRUE(dst.size() == 1);
        ASSERT_TRUE(dst.front() == 3);
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(003)
{
    //--- От Базы пришло множество строк

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        cont_t dst;
        const char* sql = "select age from users";
        con << sql >> dst;
        ASSERT_TRUE(dst.size() == 10);

        int i = 0;
        for (const auto& el : dst)
        {
            ASSERT_TRUE(el == i * 2);
            ++i;
        }
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(004)
{
    //--- От Базы пришло множество строк
    //--- количество столбцов больше, чем полей

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        db::connection con = db::connect(base, db::eCREATE);
        staff::makeTable(con, "users");
        for (size_t i = 0; i != 10; ++i)
            staff::addToTable(con, i, i * 2);

        cont_t dst;
        const char* sql = "select * from users";
        con << sql >> dst;
        ASSERT_TRUE(dst.size() == 10);

        int i = 0;
        for (const auto& el : dst)
        {
            ASSERT_TRUE(el == i);
            ++i;
        }
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

//==============================================================================
//==============================================================================
#endif // ! TEST_CONTAINER
