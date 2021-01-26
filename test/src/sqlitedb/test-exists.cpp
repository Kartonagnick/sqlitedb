
#include <mygtest/modern.hpp>
//==============================================================================
//==============================================================================

#ifdef TEST_EXISTS

#define dTEST_COMPONENT db
#define dTEST_METHOD exists
#define dTEST_TAG tdd

#include "test-staff.hpp"
namespace staff = staff_sqlitedb;

namespace db
{
    using str_t = ::std::string;

    bool exists(const char*  name) noexcept;
    bool exists(const str_t& name) noexcept;

} // namespace db

//==============================================================================
//==============================================================================

//--- basic 
TEST_COMPONENT(000)
{
    const char* filename = "test-000.txt";

    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
    {
        auto out = staff::openWrite(filename);
        out << "test-000\n";
    }
    ASSERT_TRUE(staff::fileExists(filename));
    staff::fileDelete(filename);
    ASSERT_TRUE(!staff::fileExists(filename));
}

//==============================================================================
//==============================================================================
#endif // ! TEST_EXISTS