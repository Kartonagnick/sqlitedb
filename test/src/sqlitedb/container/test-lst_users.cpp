
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONTAINER

#define dTEST_COMPONENT db, request
#define dTEST_METHOD container
#define dTEST_TAG list_users

#include "test-staff.hpp"
#include <sqlitedb/sqlitedb.hpp>
#include <list>

namespace staff = staff_sqlitedb;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

static const char* base = "00-lst_users.db";

namespace test_list_users
{
    struct normal
    {
        size_t login;
        size_t age;
    };
    struct big
    {
        size_t login;
        size_t age;
        size_t dummy;
    };
    struct small
    {
        size_t login;
    };

    void operator >>(db::request&& out, normal& dst)
    {
        std::move(out) >> std::tie(dst.login, dst.age);
    }
    void operator >>(db::request&& out, small& dst)
    {
        std::move(out) >> std::tie(dst.login);
    }
    void operator >>(db::request&& out, big& dst)
    {
        std::move(out) >> std::tie(dst.login, dst.age, dst.dummy);
    }
} // namespace test_list_users
namespace test = test_list_users;
using normal_t = std::list<test::normal>;
using small_t  = std::list<test::small>;
using big_t    = std::list<test::big>;

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
        ASSERT_TRUE(dst.front().login == 3);
        ASSERT_TRUE(dst.front().age == 6);
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
        ASSERT_TRUE(dst.front().login == 3);
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
        for (const auto& el : dst)
        {
            ASSERT_TRUE(el.login == i);
            ASSERT_TRUE(el.age == i * 2);
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
        for (const auto& el : dst)
        {
            ASSERT_TRUE(el.login == i);
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
