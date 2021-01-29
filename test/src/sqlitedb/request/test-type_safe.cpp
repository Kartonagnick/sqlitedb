
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_TYPE_SAFE

#define dTEST_COMPONENT db, request
#define dTEST_METHOD type_safe
#define dTEST_TAG tdd

#include "test-staff.hpp"
#include <sqlitedb/sqlitedb.hpp>

namespace staff = staff_sqlitedb;
namespace test_type_safe {}
namespace test = test_type_safe;
using str_t = ::std::string;

//==============================================================================
//==============================================================================

namespace test_type_safe
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

} // namespace test_type_safe

static const char* base = "00-content.db";

//==============================================================================
//==============================================================================

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

//==============================================================================
//==============================================================================
#endif // ! TEST_TYPE_SAFE
