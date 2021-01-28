
// [2021y-01m-24d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_CURSOR_USED_ 
#define dSQLITEDB_CURSOR_USED_ 1

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
           ~is_lambda() = delete;
            is_lambda() = delete;

            enum { value = checked::value };
        };

        template<class> struct get_lambda
        {
            enum { value = false };
        };

        template <class... Args> struct get_lambda< ::std::tuple<Args...> >
        {
            enum { value = true };
            using type = ::std::tuple<Args...>;
            static auto make(type& t) noexcept 
            {
                return [&t](const Args&... args)
                {
                    t = ::std::tie(args...);
                    return false;
                };
            }
        };

        template <class... Args> struct get_lambda< ::std::tuple<Args&...> >
        {
            enum { value = true };
            using type = ::std::tuple<Args&...>;
            static auto make(type& t) noexcept
            {
                return [&t](const Args&...args)
                {
                    t = ::std::tie(args...);
                    return false;
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

        #define dFOR_SQLITEDB(name) \
            ::std::enable_if_t<::db::detail::help<T>::name>

    } // namespace detail

    class request;

    class cursor
    {
        friend class request;

        template<class Request, class T>
        static dFOR_SQLITEDB(other) get(Request& owner, T& dst)
        { 
            using y = ::std::remove_reference_t<T>;
            using x = ::std::remove_cv_t<y>;
            const auto yes = owner.next();
            assert(yes); 
            (void)yes;
            dst = owner.template getValue<x>(0);
            assert(!owner.next());
        }

        template<class T>
        static dFOR_SQLITEDB(lambda) get(request& owner, T& dst)
        { 
            const auto count = cursor::template loop(owner, dst);
            (void) count;
        }

        template<class T>
        static dFOR_SQLITEDB(tuple) get(request& owner, T& dst)
        { 
            using agent = detail::get_lambda<T>;
            const auto& lambda = agent::make(dst);
            const auto count = cursor::template loop(owner, lambda);
            assert(count < 2);
            (void) count;
        }

        #undef dFOR_SQLITEDB
    private:
        template<class Request, class Lambda> 
        static size_t loop(Request& owner, const Lambda& lambda)
        {
            static_assert(
                detail::is_lambda<Lambda>::value,
                "excpect 'lambda'"
            );

            using x = decltype(
                cursor::template call(owner, lambda, &Lambda::operator())
            );

            static_assert(
                ::std::is_same<x, bool>::value,
                "the 'lambda' should return 'bool'"
            );

            size_t count = 0;
            while(owner.next())
            {
                const bool continue_
                    = cursor::template call(owner, lambda, &Lambda::operator());

                if(!continue_)
                    break;

                ++count;
            }
            return count;
        }

        template<class Request, class T> 
        static auto getArg(Request& owner, const size_t index) 
        { 
            using y = ::std::remove_reference_t<T>;
            using x = ::std::remove_cv_t<y>;
            return owner.template getValue<x>(index);
        }

        template<class R, class Obj, class ...Args, size_t ...N>
        static R callimpl(request& owner, 
            const Obj& obj, 
            R(Obj::*method)(Args...)const, 
            const ::std::index_sequence<N...> )
        {
            using tuple_t = ::std::tuple<Args...>;

            return (obj.*method)( 
                cursor::template getArg< 
                    request, ::std::tuple_element_t<N, tuple_t> 
                >(owner, N)... 
            );
        }

        template<class R, class Obj, class...Args> 
        static R call(request& owner, const Obj& obj, 
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
#endif // !dSQLITEDB_CURSOR_USED_
