
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONTENT

#define dTEST_COMPONENT db, request
#define dTEST_METHOD content
#define dTEST_TAG tdd

#include "test-staff.hpp"
#include <sqlitedb/sqlitedb.hpp>

namespace staff = staff_sqlitedb;
namespace test_content {}
namespace test = test_content;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

namespace test_content
{
    void addData(const db::connection& con, const str_t& name, const size_t count)
    {
        // dprint(std::cout << "src:\n");
        const str_t sql = "insert into " + name + "(login, age) values (?,?)";
        for (size_t i = 0; i != count; ++i)
        {
            const size_t login = i;
            const size_t age = 5 + i * 2;
            // dprint(std::cout << i << ") login = " << login << ", age = " << age << '\n');
            con << sql << login << age;
        }
    }

    void check(const db::connection& con, const str_t& name, const size_t count)
    {
        // dprint(std::cout << "dst:\n");
        size_t index = 0;
        const str_t sql = "select * from " + name;
        const auto lambda = [&index, &count](const size_t login, const size_t age)
        {
            // dprint(std::cout << index << ") login = " << login << ", age = " << age << '\n');
            EXPECT_TRUE(login == index);
            EXPECT_TRUE(age == 5 + index * 2);
            return (++index) != count - 1;
        };
        con << sql >> lambda;
        ASSERT_TRUE(index == count - 1);
    }

    void makeTable(const db::connection& con, const str_t& table)
    {
        const str_t sql = "CREATE TABLE " + table + 
        R"RAW((
            login  INTEGER PRIMARY KEY,
            age    INTEGER NOT NULL
        ))RAW";

        con << sql;
    }

    void checkTable(const db::connection& con, const str_t& table, const size_t count)
    {
        //--- create table
        test::makeTable(con, table);
        ASSERT_TRUE(con.existTable(table));

        //--- add data
        test::addData(con, table, count);

        //--- read data
        test::check(con, table, count);
    }

} // namespace test_content

static const char* base = "00-content.db";

//==============================================================================
//==============================================================================

//--- type_safe
TEST_COMPONENT(000)
{
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);
        ASSERT_TRUE(!con.existTable("users"));

        //--- create table and check content
        test::checkTable(con, "users" , 10);
        test::checkTable(con, "client", 10);
        test::checkTable(con, "mini"  , 2 );
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(001)
{
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);
        ASSERT_TRUE(!con.existTable("users"));

        //--- create table
        staff::makeTableAge(con, "users");

        //--- using incorrect arguments
        //--- столбец 'login' - PRIMARY KEY
        //--- в него можно вставить только числа из диапазона int64_t
        //
        //--- мы попытается вставить текст вместо числа.
        //--- должна сработать защита
        const char* sql = "insert into users (login, age) values (?,?)";
        #ifdef NDEBUG
            ASSERT_ANY_THROW(con << sql << "login" << 1);            
        #else
            ASSERT_DEATH_DEBUG(con << sql << "login" << 1);
        #endif
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(002)
{
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);
        ASSERT_TRUE(!con.existTable("users"));

        //--- create table
        staff::makeTableAge(con, "users");

        // std::cout << "begin\n";

        //--- если столбик не является PRIMARY KEY,
        //--- то sqlite3 позволяет пихать в него всё, что угодно.
        //--- столбец 'age' имеет тип INTEGER
        //--- а мы запихаем туда текст
        //--- и вставка должна пройти успешно
        const char* sql = "insert into users (login, age) values (?,?)";
        ASSERT_NO_THROW(con << sql << 1 << "text");
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(003)
{
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTableAge(con, "users");
        for(size_t i = 0; i != 10; ++i)
            staff::addToUsersTable(con, i, i);

        const char* sql = "select count() from users";

        //--- запрашиваем количество записей.
        //--- тип получателя указываем корректным
        size_t count = 0;
        ASSERT_NO_THROW(con << sql >> count);
        ASSERT_TRUE(count == 10);

        //--- запрашиваем количество записей.
        //--- тип получателя - std::string
        str_t dst;
        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE(dst == "10");
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}


//--- одно значение

TEST_COMPONENT(004)
{
    //--- запрашиваем одно знвчение, и получаем одно значение.
    //--- идеальный случай

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTableAge(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToUsersTable(con, i, i*2);

        str_t dst;
        const char* sql = "select count() from users";
        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE(dst == "7");
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(005)
{
    //--- запросили целую строку, хотя получатель - одно значение.
    //--- значение для получателя будет взято из первого столбца
    //--- остальные столбцы будут проигнорированны

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTableAge(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToUsersTable(con, i, i*2);

        const char* sql = "select * from users where login = 3";

        str_t dst;
        ASSERT_NO_THROW(con << sql >> dst);
        ASSERT_TRUE(dst == "3");
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}

TEST_COMPONENT(006)
{
    //--- запрашиваем несколько строк.
    //--- хотя в качестве получаетеля указываем одно идинственное значение
    //--- так вообще делать нелогично,  и неправильно.

    //--- RELEASE
    //--- в этом случае считываем значение первого столбика первой строки
    //--- остальные строки будут проигнорированы

    //--- DEBUG
    //--- должен сработать assert

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTableAge(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToUsersTable(con, i, i*2);

        str_t dst;
        const char* sql = "select * from users";
        
        #ifdef NDEBUG
            ASSERT_NO_THROW(con << sql >> dst);
            ASSERT_TRUE(dst == "3");
        #else
            ASSERT_DEATH_DEBUG(con << sql >> dst);
        #endif
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}




_TEST_COMPONENT(00422)
{
    //--- запрашиваем целую строку, 
    //--- хотя в качестве получаетеля указываем одно идинственное значение

    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTableAge(con, "users");
        for(size_t i = 3; i != 10; ++i)
            staff::addToUsersTable(con, i, i*2);

        const char* sql = "select * from users where login = 3";

        str_t dst;

        #ifdef NDEBUG
            ASSERT_NO_THROW(con << sql >> dst);
            //std::cout << dst << '\n';
            ASSERT_TRUE(dst == "3");
        #else
            con << sql >> dst;
            std::cout << "dst = " << dst << '\n';

            //ASSERT_DEATH_DEBUG(con << sql >> dst);
        #endif
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}


_TEST_COMPONENT(005)
{
    //--- Тест описывает ситуацию, когда по нашему запросу в базе ничего нет.
    ASSERT_NO_THROW(staff::dbaseDelete(base));
    {
        //--- create database
        db::connection con = db::connect(base, db::eCREATE);

        //--- create table
        staff::makeTableAge(con, "users");
        for(size_t i = 0; i != 10; ++i)
            staff::addToUsersTable(con, i, i*2);

        const char* sql = "select count(*) from users WHERE age = 300";

        str_t dst;
        con << sql >> dst;

        con << sql >> dst;
        std::cout << "age = " << dst << '\n';
        //ASSERT_NO_THROW(con << sql >> dst);
        //ASSERT_TRUE(dst == "10");
    }
    ASSERT_TRUE(staff::dbaseDelete(base));
}



//==============================================================================
//==============================================================================
#endif // ! TEST_CONTENT
