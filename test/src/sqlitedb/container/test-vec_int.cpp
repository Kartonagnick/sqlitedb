
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONTAINER

#define dTEST_COMPONENT db, request
#define dTEST_METHOD container
#define dTEST_TAG vec_int

#include "test-staff.hpp"
#include <sqlitedb/sqlitedb.hpp>

namespace staff = staff_sqlitedb;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

static const char* base = "00-container.db";

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

        std::vector<int> dst;
        const char* sql = "select * from users where login = 300";
        con << sql >> dst;
        ASSERT_TRUE(dst.empty());
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}


#if 0

TEST_COMPONENT(001)
{
    //--- используем объект пользовательского класса в качестве источника данных
    //--- количество полей в структуре больше, чем нужно

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        const auto check = [] 
        {
            //--- create database
            db::connection con = db::connect(base, db::eCREATE);

            //--- create table
            staff::makeTable(con, "users");

            const str_t sql_write = "insert into users (login, age) values (?,?)";
        
            for (size_t i = 0; i != 10; ++i)
            {
                const test::big req { i, i * 2, i * 3 };
                con << sql_write << req;
            }
        };

        #ifdef NDEBUG
            ASSERT_ANY_THROW(check());
        #else
            ASSERT_DEATH_DEBUG(check());
        #endif 
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(002)
{
    //--- используем объект пользовательского класса в качестве источника данных
    //--- количество полей в структуре меньше, чем нужно

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        const auto check = [] 
        {
            //--- create database
            db::connection con = db::connect(base, db::eCREATE);

            //--- create table
            staff::makeTable(con, "users");

            const str_t sql_write = "insert into users (login, age) values (?,?)";
        
            for (size_t i = 0; i != 10; ++i)
            {
                const test::small req { i };
                con << sql_write << req;
            }
        };

        #ifdef NDEBUG
            ASSERT_ANY_THROW(check());
        #else
            ASSERT_DEATH_DEBUG(check());
        #endif 
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

//--- destination

TEST_COMPONENT(003)
{
    //--- используем объект пользовательского класса в качестве получателя данных
    //--- количество полей в структуре правильное
    //--- ответ базы состоит из одной строки

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");

        const str_t sql_write = "insert into users (login, age) values (?,?)";
        
        for (size_t i = 0; i != 10; ++i)
            con << sql_write << test::normal{ i, i * 2 };

        const str_t sql_read = "select * from users where login = 3";

        test::normal dst { 0, 0 };
        con << sql_read >> dst;

        ASSERT_TRUE(dst.login == 3);
        ASSERT_TRUE(dst.age   == 6);
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(004)
{
    //--- используем объект пользовательского класса в качестве получателя данных
    //--- количество полей в структуре больше, чем нужно

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        const auto check = [] 
        {
            //--- create database
            db::connection con = db::connect(base, db::eCREATE);

            //--- create table
            staff::makeTable(con, "users");

            const str_t sql_write = "insert into users (login, age) values (?,?)";
        
            for (size_t i = 0; i != 10; ++i)
                con << sql_write << test::normal{ i, i * 2 };

            const str_t sql_read = "select age from users where login = ?";

            test::normal dst { 0, 0 };
            con << sql_read << 3 >> dst;
            ASSERT_TRUE(dst.age   == 6);
        };

        #ifdef NDEBUG
            ASSERT_ANY_THROW(check());
        #else
            ASSERT_DEATH_DEBUG(check());
        #endif 
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(005)
{
    //--- используем объект пользовательского класса в качестве получателя данных
    //--- количество полей в структуре меньше, чем нужно

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");

        const str_t sql_write = "insert into users (login, age) values (?,?)";
        
        for (size_t i = 0; i != 10; ++i)
            con << sql_write << test::normal{ i, i * 2 };

        const str_t sql_read = "select login, age from users where login = ?";

        test::small dst { 0 };
        con << sql_read << 3 >> dst;
        ASSERT_TRUE(dst.login == 3);
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(006)
{
    //--- используем объект пользовательского класса в качестве получателя данных
    //--- База прислала в ответ несколько строк

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        const auto check = []
        {
            //--- create database
            db::connection con = db::connect(base, db::eCREATE);

            //--- create table
            staff::makeTable(con, "users");

            const str_t sql_write = "insert into users (login, age) values (?,?)";

            for (size_t i = 1; i != 10; ++i)
                con << sql_write << test::normal{ i, i * 2 };

            const str_t sql_read = "select * from users";

            test::normal dst{ 20, 20 };
            con << sql_read >> dst;
            ASSERT_TRUE(dst.login == 1);
            ASSERT_TRUE(dst.age == 2);
        };
        #ifdef NDEBUG
            ASSERT_NO_THROW(check());
        #else
            ASSERT_DEATH_DEBUG(check());
        #endif 
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(007)
{
    //--- используем объект пользовательского класса в качестве получателя данных
    //--- База прислала пустой ответ

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTable(con, "users");

        const str_t sql_write = "insert into users (login, age) values (?,?)";
        
        for (size_t i = 0; i != 10; ++i)
            con << sql_write << test::normal{ i, i * 2 };

        const str_t sql_read = "select * from users where login = ?";

        test::normal dst { size_t(-1), size_t(-1) };
        con << sql_read << 1000 >> dst;
        ASSERT_TRUE(dst.login == size_t(-1));
        ASSERT_TRUE(dst.age   == size_t(-1));
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

#endif

//==============================================================================
//==============================================================================
#endif // ! TEST_CONTAINER
