// [2020y-11m-30d] Idrisov Denis R.
#pragma once
#ifndef dTOOLS_NUMERIC_CAST_USED_ 
#define dTOOLS_NUMERIC_CAST_USED_ 100

#include <type_traits>
#include <stdexcept>
#include <typeinfo>
#include <cassert>
#include <string>
#include <limits>

//================================================================================
//================================================================================
#ifndef dTOOLS_ENUM_TYPE_USED_ 
#define dTOOLS_ENUM_TYPE_USED_ 1
namespace tools
{
    namespace detail
    {
        template<class t, bool = ::std::is_enum<t>::value >
        struct type_of_enum
        {
            using type = ::std::underlying_type_t<t>;
        };

        template<class t>
        struct type_of_enum<t, false>
        {
            using type = t;
        };

    } // namespace detail

    template<class t> 
    using type_of_enum_t
        = typename ::tools::detail::type_of_enum<t>::type;

} // tools
#endif // !dTOOLS_ENUM_TYPE_USED_

//================================================================================
//================================================================================
namespace tools
{
    using str_t = ::std::string;

    namespace detail_numeric_cast
    {
        #define dDETAIL \
            ::tools::detail_numeric_cast

        template<class ret, class from>
        ret throw_error(const from val)
        {
            using from_ = ::tools::type_of_enum_t<from>;
            const from_ v = static_cast<from_>(val);
            const str_t t = typeid(ret).name();
            const str_t s = ::std::to_string(v);
            throw ::std::out_of_range(
                "tools::can_numeric_cast<" + t + ">(" + s + "): failed"
            );
        }

        template<class from, class to, class t>
        constexpr auto cast(const t value) noexcept
        {
            using from_ = ::tools::type_of_enum_t<from>;
            using to_   = ::tools::type_of_enum_t<to>;

            using x = ::std::common_type_t<from_, to_>;
            return static_cast<x>(value);
        }

        #define dMIN_LIMIT(to)                     \
            dDETAIL::cast<from, to>(               \
                (::std::numeric_limits<to>::min)() \
            )

        #define dMAX_LIMIT(to)                     \
            dDETAIL::cast<from, to>(               \
                (::std::numeric_limits<to>::max)() \
            )

        #define dTHE_VALUE(val) \
            dDETAIL::cast<from, ret>(val)

        template<class ret_type, class from_type>
        class help
        {
            using origin_from 
                = std::remove_cv_t<from_type>;

            using origin_ret  
                = std::remove_cv_t<ret_type>;

            using from 
                = ::tools::type_of_enum_t<origin_from>;

            using ret  
                = ::tools::type_of_enum_t<origin_ret>;

            static_assert(
                ::std::is_fundamental<from>::value, 
                "'from' must be fundamental type"
            );

            static_assert(
                ::std::is_fundamental<ret >::value, 
                "'ret' must be fundamental type" 
            );

            enum { a = ::std::is_floating_point<from>::value };
            enum { b = ::std::is_floating_point<ret>::value  };

            enum { c = ::std::is_integral<from>::value };
            enum { d = ::std::is_integral<ret>::value  };

            static_assert(
                (a && b) || (c && d),
                "support only 'floating VS floating' "
                "or 'integers VS integers'"
            );

            enum { same_types = ::std::is_same<from, ret>::value };
            enum { capacious = sizeof(ret) >= sizeof(from) };

            enum { floating = ::std::is_floating_point<from>::value };
            enum { ret_signed  = ::std::is_signed<ret >::value };
            enum { from_signed = ::std::is_signed<from>::value };

            enum { ret_unsigned  = !ret_signed  };
            enum { from_unsigned = !from_signed };
        
            enum { greater_uint =  capacious && ret_unsigned && from_unsigned };
            enum { less_uint    = !capacious && ret_unsigned && from_unsigned };

            enum { mode = 
                 floating     ?                  0:
                 same_types   ?                  1: 
                 greater_uint ?                  2:
                 less_uint    ?                  3: 
                 ret_signed   && from_unsigned ? 4: 
                 ret_unsigned && from_signed   ? 5: 
                 ret_signed   && from_signed   ? 6:
                                                 7 
            };
            static_assert(mode != 7, "internal error");

        public:
           ~help() = delete;
            help() = delete; 

            enum { value = mode };
        };

    } // namespace detail_numeric_cast

    #define dBOOL(usage)                              \
        constexpr ::std::enable_if_t<                 \
            dDETAIL::help<ret, from>::value == usage, \
            bool                                      \
        >

    // --- floating
    template<class ret, class from>
    dBOOL(0) can_numeric_cast(const from) noexcept
    {
        static_assert(
            ::std::is_floating_point<from>::value, 
            "'from' must be floating type"
        );

        static_assert(
            ::std::is_floating_point<ret >::value, 
            "'to' must be floating type"  
        );
        return sizeof(ret) >= sizeof(from);
    }

    // --- same types 
    template<class ret, class from>
    dBOOL(1) can_numeric_cast(const from) noexcept
    {
        using x = ::std::remove_cv_t<ret>;
        using z = ::std::remove_cv_t<from>;

        using ret_  = ::tools::type_of_enum_t<x>;
        using from_ = ::tools::type_of_enum_t<z>;

        static_assert(
            ::std::is_integral<from_>::value, 
            "'from' must be integral type"
        );

        static_assert(
            ::std::is_same<from_, ret_>::value, 
            "'ret' and 'from' must be same types"
        );

        return true;
    }

    // --- sizeof(from-unsigned) <= sizeof(to-unsigned)
    template<class ret, class from>
    dBOOL(2) can_numeric_cast(const from) noexcept
    { 
        using ret_  = ::tools::type_of_enum_t<ret>;
        using from_ = ::tools::type_of_enum_t<from>;
        static_assert(::std::is_integral<from_>::value, "'from' must be integral type");
        static_assert(::std::is_integral<ret_ >::value, "'ret' must be integral type" );
        static_assert(::std::is_unsigned<from_>::value, "'from' must be unsigned type");
        static_assert(::std::is_unsigned<ret_ >::value, "'ret' must be unsigned type" );
        static_assert(sizeof(from_) <= sizeof(ret_)    , "sizeof(from) <= sizeof(to)"  );
        return true;
    }

    // --- sizeof(from-unsigned) > sizeof(to-unsigned)
    template<class ret, class from>
    dBOOL(3) can_numeric_cast(const from val) noexcept
    {
        using ret_  = ::tools::type_of_enum_t<ret>;
        using from_ = ::tools::type_of_enum_t<from>;

        static_assert(::std::is_integral<from_>::value, "'from' must be integral type");
        static_assert(::std::is_integral<ret_ >::value, "'ret' must be integral type" );
        static_assert(::std::is_unsigned<from_>::value, "'from' must be unsigned type");
        static_assert(::std::is_unsigned<ret_ >::value, "'ret' must be unsigned type" );
        static_assert(sizeof(from_) > sizeof(ret_)     , "sizeof(from) > sizeof(ret)"  );

        return dMAX_LIMIT(ret_) >= dTHE_VALUE(val);
    }

    // --- from(unsigned) to signed
    template<class ret, class from>
    dBOOL(4) can_numeric_cast(const from val) noexcept
    { 
        using ret_  = ::tools::type_of_enum_t<ret>;
        using from_ = ::tools::type_of_enum_t<from>;

        static_assert(::std::is_integral<from_>::value, "'from' must be integral type");
        static_assert(::std::is_integral<ret_ >::value, "'ret' must be integral type" );
        static_assert(::std::is_unsigned<from_>::value, "'from' must be unsigned type");
        static_assert(::std::is_signed<ret_   >::value, "'ret' must be signed type"   );

        return dMAX_LIMIT(ret_) >= dTHE_VALUE(val);
    }

    // --- from(unsigned) to signed
    template<class ret, class from>
    dBOOL(5) can_numeric_cast(const from val) noexcept
    {
        using ret_  = ::tools::type_of_enum_t<ret>;
        using from_ = ::tools::type_of_enum_t<from>;

        static_assert(::std::is_integral<from_>::value, "'from' must be integral type");
        static_assert(::std::is_integral<ret_ >::value, "'ret' must be integral type" );
        static_assert(::std::is_signed<from_  >::value, "'from' must be signed type"  );
        static_assert(::std::is_unsigned<ret_ >::value, "'ret' must be unsigned type" );

        return static_cast<from_>(val) < 0 ? false: 
            dMAX_LIMIT(ret_) >= dTHE_VALUE(val);
    }

    // from(signed) to signed
    template<class ret, class from>
    dBOOL(6) can_numeric_cast(const from val) noexcept
    {
        using ret_  = ::tools::type_of_enum_t<ret>;
        using from_ = ::tools::type_of_enum_t<from>;

        static_assert(::std::is_integral<from_>::value, "'from' must be integral type");
        static_assert(::std::is_integral<ret_>::value , "'ret' must be integral type" );
        static_assert(::std::is_signed<from_>::value  , "'from' must be signed type"  );
        static_assert(::std::is_signed<ret_>::value   , "'ret' must be signed type"   );

        return 
            dTHE_VALUE(val)  <  dMIN_LIMIT(ret_) ? false: 
            dMAX_LIMIT(ret_) >= dTHE_VALUE(val);
    }

    #undef dTHE_VALUE
    #undef dMIN_LIMIT
    #undef dMAX_LIMIT
    #undef dDETAIL
    #undef dBOOL

} // namespace tools

//================================================================================
//================================================================================

namespace tools
{
    template<class ret, class from>
    constexpr ret assert_numeric_cast(const from val) noexcept
    {
        assert((tools::can_numeric_cast<ret, from>(val)));
        return static_cast<ret>(val);
    }

    template<class ret, class from>
    constexpr ret numeric_cast(const from val) 
    {
        return tools::can_numeric_cast<ret, from>(val) ?
            static_cast<ret>(val):
            detail_numeric_cast::throw_error<ret>(val);
    }

} // namespace tools

//================================================================================

#endif // !dTOOLS_NUMERIC_CAST_USED_

