
#include <mygtest/modern.hpp>
#include <sqlitedb/sqlitedb.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONTENT

#define dTEST_COMPONENT db, connection
#define dTEST_METHOD content
#define dTEST_TAG tdd

namespace me = db;
//==============================================================================
//==============================================================================

namespace test_content
{
    using str_t = ::std::string;

    void fill(const db::connection& con, const str_t& name, const size_t count)
    {
        const str_t sql
            = "insert into " + name + "(login, commission) values (?,?)";
        for (size_t i = 0; i != count; ++i)
        {
            const size_t login = i;
            const size_t comm =  5 + i * 2;
            // dprint(std::cout << "login = " << login << ", commission = " << comm << '\n');
            con << sql << login << comm;
        }
    }

    void check(const db::connection& con, const str_t& name, const size_t count)
    {
        size_t index = 0;
        const str_t sql = "select * from " + name;
        const auto lambda = [&index, &count](const size_t login, const size_t comm)
        {
            // dprint(std::cout << "login = " << login << ", commission = " << comm << '\n');
            EXPECT_TRUE(login == index);
            EXPECT_TRUE(comm == 5 + index * 2);
            return (++index) != count - 1;
        };
        con << sql >> lambda;
        ASSERT_TRUE(index == count - 1);
    }

} // namespace test_content

using namespace test_content;
//==============================================================================
//==============================================================================

TEST_COMPONENT(000)
{
    db::remove("samples.db");
    ASSERT_TRUE(!db::exists("samples.db"));
    {
        db::connection con = db::connect("samples.db", db::eCREATE);

        ASSERT_TRUE(!con.existTable("users"));
        ASSERT_TRUE(!con.existTable("clients"));

        const char* sql_users =
            R"RAW(
            CREATE TABLE users (
                login        INTEGER   PRIMARY KEY,
                commission   INTEGER   NOT NULL
            )
        )RAW";

        const char* sql_clients =
            R"RAW(
            CREATE TABLE clients (
                login        INTEGER   PRIMARY KEY,
                commission   INTEGER   NOT NULL
            )
        )RAW";

        con << sql_users;
        ASSERT_TRUE(con.existTable("users"));

        con << sql_clients;
        ASSERT_TRUE(con.existTable("clients"));

        fill(con, "users"  , 10);
        fill(con, "clients", 10);

        check(con, "users"  , 10);
        check(con, "clients", 10);
    }
    ASSERT_TRUE(db::remove("samples.db"));
    ASSERT_TRUE(!db::exists("samples.db"));
}

//==============================================================================
//==============================================================================
#endif // ! TEST_CONTENT
