
// [2021y-01m-24d] Idrisov Denis R.

#pragma once
#ifndef ddSQLITEDB_CURSOR_USED_ 
#define ddSQLITEDB_CURSOR_USED_ 1

#include <sqlitedb/stmt.hpp>
#include <type_traits>
#include <cassert>
#include <tuple>

//==============================================================================
//==============================================================================
namespace db
{
    namespace detail
    {
        template<class lambda> class is_lambda
        {
            using x = ::std::remove_reference_t<lambda>;

            template<class u> static ::std::true_type
                check(decltype(&u::operator())*);

            template<class> static ::std::false_type
                check(...);

            using checked
                = decltype(check<x>(nullptr));
        public:
            is_lambda() = delete;
            ~is_lambda() = delete;

            enum { value = checked::value };
        };

        template<class> struct get_lambda
        {
            enum { value = false };
        };

        template <class... Args>
        struct get_lambda< ::std::tuple<Args...> >
        {
            enum { value = true };
            using type = ::std::tuple<Args...>;
            static auto gen(type& t) noexcept 
            {
                return [&t](const Args&... args)
                {
                    t = ::std::tie(args...);
                    return true;
                };
            }
        };

        template <class... Args>
        struct get_lambda< ::std::tuple<Args&...> >
        {
            enum { value = true };
            using type = ::std::tuple<Args&...>;
            static auto gen(type& t) noexcept
            {
                return [&t](const Args&...args)
                {
                    t = ::std::tie(args...);
                    return true;
                };
            }
        };

        template<class T> struct help
        {
            using y = std::remove_reference_t<T>;
            using x = std::remove_cv_t<y>;
            enum { lambda = detail::is_lambda<x>::value  };
            enum { tuple  = detail::get_lambda<x>::value };
            enum { other  = !lambda && !tuple };
        };

        #define dFOR_(name) \
            ::std::enable_if_t<::db::detail::help<T>::name>

    } // namespace detail

    class cursor
    {
    public:
        template<class T>
        static dFOR_(other) get(stmt& owner, T&& dst)
        { 
            using x = std::remove_reference_t<T>;
            const auto yes = owner.next();
            assert(yes);
            dst = owner.template getValue<x>(0);
            assert(!owner.next());
        }

        template<class T>
        static dFOR_(lambda) get(stmt& owner, T&& dst)
        { 
            const auto count = cursor::template loop(owner, dst);
            (void) count;
        }

        template<class T>
        static dFOR_(tuple) get(stmt& owner, T&& dst)
        { 
            using agent = detail::get_lambda<T>;
            const auto& lambda = agent::gen(dst);
            const auto count = cursor::template loop(owner, lambda);
            assert(count < 2);
            (void) count;
        }

        template<class Lambda> 
        static size_t loop(stmt& owner, const Lambda& lambda)
        {
            static_assert(
                detail::is_lambda<Lambda>::value,
                "excpect 'lambda'"
            );
            size_t count = 0;
            while(owner.next())
            {
                if(!cursor::template call(owner, lambda, &Lambda::operator()))
                    break;
                ++count;
            }
            return count;
        }

    private:
        template<class T> 
        static auto getArg(stmt& owner, const size_t index) 
        { 
            using x = ::std::remove_reference_t<T>;
            using arg_t = ::std::remove_cv_t<x>;
            return owner.getValue<arg_t>(index);
        }

        template<class R, class Obj, class ...Args, size_t ...N>
        static R callimpl(stmt& owner, 
            const Obj& obj, 
            R(Obj::*method)(Args...)const, 
            const ::std::index_sequence<N...> )
        {
            using tuple_t = ::std::tuple<Args...>;

            return (obj.*method)( 
                cursor::template getArg< 
                    ::std::tuple_element_t<N, tuple_t> 
                >(owner, N)... 
            );
        }

        template<class R, class Obj, class...Args> 
        static R call(stmt& owner, const Obj& obj, 
            R(Obj::*method)(Args...) const)
        {
            enum { count = sizeof...(Args) };
            using seq_t = ::std::make_index_sequence<count>;
            return cursor::template callimpl(
                owner, obj, method, seq_t{}
            );
        }
    };
    
} // namespace db

//==============================================================================
//==============================================================================
#endif // !ddSQLITEDB_STMP_USED_
