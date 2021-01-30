
// [2021y-01m-24d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_CURSOR_USED_ 
#define dSQLITEDB_CURSOR_USED_ 1

#include <type_traits>
#include <cassert>
#include <string>
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

        template<class T> struct get_args;
		
		template<class R, class C, class... Args>
		struct get_args<R(C::*)(Args...) const>
		{
			enum { value = sizeof...(Args) };
		};

		template<class R, class C, class... Args>
		struct get_args<R(C::*)(Args...)>
		{
			enum { value = sizeof...(Args) };
		};

        #if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
            // #pragma message("build for c++17...")
            #define dCPP17
        #endif

        #ifdef dCPP17
		    template<class R, class C, class... Args>
		    struct get_args<R(C::*)(Args...) const noexcept>
		    {
			    enum { value = sizeof...(Args) };
		    };

		    template<class R, class C, class... Args>
		    struct get_args<R(C::*)(Args...) noexcept>
		    {
			    enum { value = sizeof...(Args) };
		    };
        #endif // !dCPP17

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
            using y = ::std::remove_reference_t<T>;
            using x = ::std::remove_cv_t<y>;
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

            cursor::template check(owner, 1, "single variable");

            const auto yes = owner.next();
            if (!yes)
                throw ::std::runtime_error("[cursor::get] unexpectedly no data");
            dst = owner.template get_value<x>(0);
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
            static_assert(
                ::std::tuple_size<T>::value > 0,
                "not support: zero params of tuple"
            );

            using agent = detail::get_lambda<T>;
            const auto& lambda = agent::make(dst);
            const auto count 
                = cursor::template loop(owner, lambda);
            (void) count;
            assert(!owner.next());
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

            using foo = decltype(&Lambda::operator());
			enum { args = detail::get_args<foo>::value };
            static_assert(args > 0, "not support: zero arguments");
            cursor::template check(owner, args, "lambda`s arguments");
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

        template<class Request> 
        static void check(Request& owner, const size_t args, const char* dsc) 
        {
            assert(dsc);
            const size_t columns = owner.columns();
            assert(args <= columns);
            if (args <= columns)
                return;

            const str_t sc = ::std::to_string(columns);
            const str_t sp = ::std::to_string(args);
            const str_t msg = dsc;

            const str_t reason
                = "[cursor::check] out range: "
                "columns = " + sc + " VS " +
                msg + " = " + sp;

            throw ::std::runtime_error(reason);
        }

        template<class Request, class T> 
        static auto getArg(Request& owner, const size_t index) 
        { 
            using y = ::std::remove_reference_t<T>;
            using x = ::std::remove_cv_t<y>;
            return owner.template get_value<x>(index);
        }

        template<class R, class Obj, class ...Args, size_t ...N>
        static R callimpl(request& owner, 
            const Obj& obj, 
            R(Obj::*method)(Args...) const, 
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
