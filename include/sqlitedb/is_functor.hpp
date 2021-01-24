// [2021y-01m-24d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_IS_FUNCTOR_USED_ 
#define dSQLITEDB_IS_FUNCTOR_USED_

#include <type_traits>

#define dDETAIL_CONSTANT(...) \
    ::std::integral_constant<bool, detail::__VA_ARGS__::value>

namespace db
{
    namespace detail
    {
        template<class lambda> class is_functor
        {
            using x 
                = ::std::remove_reference_t<lambda>;

            template<class u> static ::std::true_type
                check(decltype(&u::operator())*);

            template<class> static ::std::false_type
                check(...);

            using checked
                = decltype(check<x>(nullptr));
        public:
            is_functor() = delete;
            enum { value = checked::value };
        };

    } // namespace detail

    template<class F> class is_functor
        : public dDETAIL_CONSTANT(is_functor<F>)
    {};

    #define dFOR_FUNCTOR_DB(T) \
        ::std::enable_if_t<::db::is_functor<T>::value>

    #define dFOR_NOT_FUNCTOR_DB(T) \
        ::std::enable_if_t<!::db::is_functor<T>::value>


} // namespace db 

//==============================================================================
//==============================================================================
#endif // !dSQLITEDB_IS_FUNCTOR_USED_
