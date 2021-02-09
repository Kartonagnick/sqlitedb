
// [2021y-01m-24d] Idrisov Denis R.

#pragma once
#ifndef dSQLITEDB_CURSOR_USED_ 
#define dSQLITEDB_CURSOR_USED_ 100

#include <sqlitedb/request.hpp>
#include <tuple>

//==============================================================================
//==============================================================================
namespace db
{
    namespace detail
    {
        template<class t> class is_container
        {
            using x = ::db::degradate<t>;

            template<class u> static ::std::true_type
                check(typename u::value_type*);

            template<class> static ::std::false_type
                check(...);

            using checked = decltype(check<x>(nullptr));
        public:
            ~is_container() = delete;
            is_container() = delete;
            enum { value = checked::value };
        };

        template<class t> class is_string
        {
            using x = ::db::degradate<t>;

            template<class u> static ::std::true_type
                check(decltype(&u::c_str));

            template<class> static ::std::false_type
                check(...);

            using checked = decltype(check<x>(nullptr));
        public:
            ~is_string() = delete;
            is_string() = delete;
            enum { value = checked::value };
        };

        template<class lambda> class is_lambda
        {
            using x = ::db::degradate<lambda>;

            template<class u> static ::std::true_type
                check(decltype(&u::operator())*);

            template<class> static ::std::false_type
                check(...);

            using checked = decltype(check<x>(nullptr));
        public:
            ~is_lambda() = delete;
            is_lambda() = delete;
            enum { value = checked::value };
        };

        template<class> struct is_tuple
        {
            enum { value = false };
        };

        template<class... Args>
        struct is_tuple< ::std::tuple<Args...> >
        {
            enum { value = true };
        };

        template<class T> struct method;

        template<class R, class C, class... Args>
        struct method<R(C::*)(Args...) const>
        {
            using tuple_t = ::std::tuple<Args...>;
            enum { count_arguments = sizeof...(Args) };
            using seq_t = ::std::make_index_sequence<count_arguments>;
        };

        template<class R, class C, class... Args>
        struct method<R(C::*)(Args...)>
        {
            using tuple_t = ::std::tuple<Args...>;
            enum { count_arguments = sizeof...(Args) };
            using seq_t = ::std::make_index_sequence<count_arguments>;
        };

        #if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
            // #pragma message("build for c++17...")
            #define dHAS_CPP17 1
        #endif

        #ifdef dHAS_CPP17
        template<class R, class C, class... Args>
        struct method<R(C::*)(Args...) const noexcept>
        {
            using tuple_t = ::std::tuple<Args...>;
            enum { count_arguments = sizeof...(Args) };
            using seq_t = ::std::make_index_sequence<count_arguments>;
        };

        template<class R, class C, class... Args>
        struct method<R(C::*)(Args...) noexcept>
        {
            using tuple_t = ::std::tuple<Args...>;
            enum { count_arguments = sizeof...(Args) };
            using seq_t = ::std::make_index_sequence<count_arguments>;
        };
        #endif // !dHAS_CPP17

        template<class T> class help
        {
            using x = ::db::degradate<T>;
            enum { is_cont = detail::is_container<x>::value };
            enum { is_str_ = detail::is_string<x>::value };
            enum { cont = is_cont && !is_str_ };
        public:
            enum { container = cont };
            enum { lambda = detail::is_lambda<x>::value };
            enum { tuple = detail::is_tuple<x>::value };
            enum { variable = !lambda && !tuple && !cont };
        };

        #define dFOR_SQLITEDB(name) \
            ::std::enable_if_t<::db::detail::help<T>::name>

    } // namespace detail

    class irerator
    {
        friend class cursor;
        friend class request;

        template<class T> dFOR_SQLITEDB(variable) get(T& dst) const
        {
            using x = ::db::degradate<T>;
            if (this->m_owner.m_start < 2)
            {
                this->check(1, "single variable");
                this->m_owner.m_start = 2;
            }
            dst = this->m_owner.get_value<x>(0);
        }

        template<class T> dFOR_SQLITEDB(tuple) get(T& dst) const
        {
            using noref_t = ::std::remove_reference_t<T>;
            using count_t = ::std::tuple_size<noref_t>;
            static_assert(count_t::value > 0,
                "not support: zero params of tuple"
                );
            if (this->m_owner.m_start < 2)
            {
                this->check(count_t::value, "tuple`s params");
                this->m_owner.m_start = 2;
            }
            this->tupleForeach(dst);
        }

        template<class T> dFOR_SQLITEDB(lambda) get(T&) const
        {
            assert(false && "shouldn't be running");
        }
        template<class T> dFOR_SQLITEDB(container) get(T&) const
        {
            assert(false && "shouldn't be running");
        }
    protected:
        irerator(request& owner)noexcept
            : m_owner(owner)
        {}

        void check(const size_t args, const char* dsc) const
        {
            assert(dsc);
            const size_t columns = this->m_owner.columns();
            assert(args <= columns);
            if (args <= columns)
                return;

            const str_t sc = ::std::to_string(columns);
            const str_t sp = ::std::to_string(args);
            const str_t msg = dsc;

            const str_t reason
                = "[irerator::check] out range: "
                "columns = " + sc + " VS " +
                msg + " = " + sp;

            throw ::std::runtime_error(reason);
        }
    protected:

        template <class Tuple, size_t... i>
        void tupleForeachImpl(Tuple&& tuple, const ::std::index_sequence<i...>) const
        {
            const auto unused =
            {
                true, (
                    this->m_owner.get_value(i, ::std::get<i>(::std::forward<Tuple>(tuple))),
                    void(),
                    true
                ) ...
            };
            (void)unused;
        }

        template <class Tuple> void tupleForeach(Tuple&& tuple) const
        {
            using noref_t = ::std::remove_reference_t<Tuple>;
            using count_t = ::std::tuple_size<noref_t>;
            using index_t = ::std::make_index_sequence<count_t::value>;
            irerator::tupleForeachImpl(::std::move(tuple), index_t());
        }

    protected:
        ::db::request& m_owner;
    };


    class cursor : irerator
    {
        friend class request;

        cursor(request& owner) noexcept
            : irerator(owner)
        {}

        template<class T> dFOR_SQLITEDB(variable) get(T& dst) const
        {
            this->get_as_variable(dst);
        }

        template<class T> dFOR_SQLITEDB(lambda) get(T& dst) const
        {
            this->loop(dst);
        }

        template<class T> dFOR_SQLITEDB(tuple) get(T& dst) const
        {
            this->get_as_tuple(dst);
        }

        template<class T> dFOR_SQLITEDB(container) get(T& dst) const
        {
            while (this->m_owner.next())
            {
                dst.emplace_back();
                auto& value = dst.back();
                std::move(this->m_owner) >> value;
            }
        }

        #undef dFOR_SQLITEDB

    private:
        template<class T> void get_as_variable(T& dst) const
        {
            this->check(1, "single variable");
            const auto yes = this->m_owner.next();
            if (!yes)
                throw ::std::runtime_error("[cursor::get_as_variable] unexpectedly no data");

            using x = ::db::degradate<T>;
            dst = this->m_owner.get_value<x>(0);
            assert(!this->m_owner.next());
        }

        template<class Tuple> void get_as_tuple(Tuple& dst) const
        {
            using noref_t = ::std::remove_reference_t<Tuple>;
            using count_t = ::std::tuple_size<noref_t>;

            static_assert(count_t::value > 0, "not support: zero params of tuple");
            this->check(count_t::value, "tuple`s params");

            if (this->m_owner.next())
                this->tupleForeach(dst);

            assert(!this->m_owner.next());
        }
    private:
        template<class Lambda> void loop(const Lambda& lambda) const
        {
            static_assert(detail::is_lambda<Lambda>::value,
                "expected 'lambda'"
                );
            using x = decltype(this->call(lambda, &Lambda::operator()));

            static_assert(::std::is_same<x, bool>::value,
                "the 'lambda' should return 'bool'"
                );

            using foo_t = decltype(&Lambda::operator());
            using method_t = ::db::detail::method<foo_t>;
            static_assert(
                method_t::count_arguments > 0,
                "not support: zero arguments of callback"
                );
            this->check(method_t::count_arguments, "lambda`s arguments");

            while (this->m_owner.next())
                if (!this->call(lambda, &Lambda::operator()))
                    break;
        }

        template<class T> auto get_arg(const size_t index) const
        {
            using x = ::db::degradate<T>;
            return this->m_owner.get_value<x>(index);
        }

        template<class Obj, class Method, size_t ...i>
        auto callimpl(const Obj& obj, Method method, const ::std::index_sequence<i...>) const
        {
            using z = ::db::detail::method<Method>;
            using x = typename z::tuple_t;
            return (obj.*method)(
                this->get_arg<::std::tuple_element_t<i, x> >(i) ...
                );
        }

        template<class Obj, class Method> auto call(const Obj& obj, Method method) const
        {
            using x = ::db::detail::method<Method>;
            using seq_t = typename x::seq_t;
            return this->callimpl(obj, method, seq_t{});
        }
    };

} // namespace db

//==============================================================================
//==============================================================================
#endif // !dSQLITEDB_CURSOR_USED_
