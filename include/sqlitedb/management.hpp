
// [2021y-01m-24d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_MANAGMENT_USED_ 
#define dSQLITEDB_MANAGMENT_USED_ 100

#include <cassert>
#include <string>

//==============================================================================
//==============================================================================

namespace db
{
    using str_t = ::std::string;

    void cleanDatabase(const char* name);

    bool availableDatabase(const char* name);


    bool existDatabase(const char* name);


    bool existTable(
        const char* base,
        const char* table
    );


    bool existColumn(
        const char* base,
        const char* table,
        const char* column
    );


    bool dropDatabase(const str_t& name);
    bool dropDatabase(const char* name);


    bool dropTable(const char* base, const char* table);

    #if 0
    bool dropColumn(
        const char* base,
        const char* table,
        const char* column
    );
    #endif

} // namespace db

//==============================================================================
//==============================================================================

namespace db
{
    template<class s> void cleanDatabase(const s& name)
    {
        const auto* n = &name[0];
        assert(n);
        assert(*n != 0);
        db::cleanDatabase(n);
    }

    

    template<class s> bool availableDatabase(const s& name)
    {
        const auto* n = &name[0];
        assert(n);
        assert(*n != 0);
        return db::availableDatabase(n);
    }


    template<class s> bool existDatabase(const s& name)
    {
        const auto* n = &name[0];
        assert(n);
        assert(*n != 0);
        return db::existDatabase(n);
    }


    template <class s1, class s2> 
    bool existTable(const s1& base, const s2& table)
    {
        const auto* b = &base[0];
        const auto* t = &table[0];
        assert(b);
        assert(t);
        assert(*b != 0);
        assert(*t != 0);
        return db::existTable(b, t);
    }


    template <class s1, class s2, class s3> 
    bool existColumn(const s1& base, const s2& table, const s3& column)
    {
        const auto* b = &base[0];
        const auto* t = &table[0];
        const auto* c = &column[0];
        assert(b);
        assert(t);
        assert(c);
        assert(*b != 0);
        assert(*t != 0);
        assert(*c != 0);
        return db::existColumn(b, t, c);
    }


    template <class s1, class s2> 
    bool dropTable(const s1& base, const s2& table)
    {
        const auto* b = &base[0];
        const auto* t = &table[0];
        assert(b);
        assert(t);
        assert(*b != 0);
        assert(*t != 0);
        return db::dropTable(b, t);
    }


    template <class s1, class s2, class s3> 
    bool dropColumn(
        const s1& base,
        const s2& table,
        const s3& column)
    {
        const auto* b = &base[0];
        const auto* t = &table[0];
        const auto* c = &column[0];
        assert(b);
        assert(t);
        assert(c);
        assert(*b != 0);
        assert(*t != 0);
        assert(*c != 0);
        return db::dropColumn(b, t, c);
    }

} // namespace db

//==============================================================================
//==============================================================================

#endif // !dSQLITEDB_MANAGMENT_USED_

