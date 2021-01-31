
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_CONSEPT

#define dTEST_COMPONENT db
#define dTEST_METHOD consept
#define dTEST_TAG tdd

//==============================================================================
//==============================================================================

#include <stdexcept>
#include <iostream>
#include <cassert>
#include <random>
#include <chrono>
#include <string>
#include <vector>

int random(const int a, const int b) noexcept
{
    assert(a <= b);
    using clockT  = ::std::chrono::steady_clock;
    using engineT = ::std::default_random_engine;
    using randomT = ::std::uniform_int_distribution<int>;

    static engineT engine(
        static_cast<unsigned>(clockT::now().time_since_epoch().count())
    );
    return randomT(a, b)(engine);
}

class request
{
    friend struct connection;

    request(): vec(), error(true) 
    {
        this->vec.emplace_back(1);
    }
public:
    request(request&& rhs) noexcept
	    :vec(std::move(rhs.vec))
        ,error(std::move(rhs.error))
	{ rhs.error = false; }

    request(const request&)             = delete;
    request& operator =(const request&) = delete;
    request& operator =(request&&)      = delete;	

    ~request() noexcept(false)  // can throw exception
    {
        dprint(std::cout << "~request(): begin...\n");
        this->commit();
        dprint(std::cout << "~request(): done\n");
    }

    request&& operator << (const std::string& v) && noexcept
    {
        this->error = v == "ok" ? false : true;
        return std::move(*this);
    }
    
    request&& operator << (const int v) && 
    { 
        this->checkArgument(v);
        this->vec.emplace_back(v);
        return std::move(*this);
    }

    void operator >> (int& dst) &&
    { 
        this->commit();
        dst = 0;
    }
private:
    void checkArgument(const int v); 
    void commit();                   
    std::vector<int> vec;    
    bool error;
};
                                   
void request::checkArgument(const int v)
{
    if(v % 2 != 0)
        return;

    this->vec.clear();
    const auto s = std::to_string(v);
    dprint(std::cerr << "checkArgument(): throw logic_error\n");
    throw std::logic_error("must be odd: '" + s + "'");
}
                                   
void request::commit()
{
    if(this->vec.empty())
        return;

    this->vec.clear();
    dprint(std::cout << "commit(): begin...\n");
    if (!this->error)
    {
        dprint(std::cout << "commit(): done\n");
        return;
    }
    dprint(std::cerr << "commit(): throw logic_error\n");
    throw std::logic_error("database is BUSY");
}                                   
                                   
struct connection
{
    request operator << (const char*) const { return request(); }
};

TEST_COMPONENT(000)
{
    ASSERT_NO_THROW(
        connection() << "SQL" << 111 << "ok";
    );
}

TEST_COMPONENT(001)
{
    try // exception from operator<<
    {
        connection() << "SQL" << 111 << 222;
        FAIL();
    }
    catch (const std::exception& e)
    {
        (void)e;
        dprint(std::cerr << e.what() << '\n');
        SUCCEED();
    }
}

TEST_COMPONENT(002)
{
    try // exception from operator>>
    {
        int result = 0;
        connection() << "SQL" << 111 >> result;
        FAIL();
    }
    catch (const std::exception& e)
    {
        (void)e;
        dprint(std::cerr << e.what() << '\n');
        SUCCEED();
    }
}

TEST_COMPONENT(003)
{
    try // exception from destructor
    {
        connection() << "SQL" << 111;
        FAIL();
    }
    catch (const std::exception& e)
    {
        (void)e;
        dprint(std::cerr << e.what() << '\n');
        SUCCEED();
    }
}

TEST_COMPONENT(004)
{
    // --- getting an 'lvalue' from a 'prvalue' is possible, but it's a bad idea
    request&& req = connection() << "SQL";
    std::move(req) << "ok";
}

//==============================================================================
//==============================================================================
#endif // ! TEST_CONSEPT
