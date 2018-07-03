//
// Created by xmh on 18-7-3.
//

#ifndef _XBINDER_HPP
#define _XBINDER_HPP
#include <tuple>
namespace xmh{
    template<typename Ret,typename...Args>
    struct function_type
    {
        using ret_type = Ret;
        using params_tuple_type = std::tuple<Args...>;
    };

    template<typename Ret,typename Class,typename...Args>
    struct function_type<Ret(Class::*)(Args...) const>
    {
        using ret_type = Ret;
        using params_tuple_type = std::tuple<Args...>;
    };
    template<typename...Args>
    struct function_trais;

    template<typename Ret,typename...Args>
    struct function_trais<Ret(*)(Args...)>:function_type<Ret,Args...>
    {
        static const bool is_member_function  = false;
    };

    template<typename Ret,typename Class,typename...Args>
    struct function_trais<Ret(Class::*)(Args...)>:function_type<Ret,Args...>
    {
        static const bool is_member_function  = true;
    };

    template<typename Functor>
    struct function_trais<Functor>:function_type<decltype(&Functor::operator())>
    {
        static const bool is_member_function  = false;
    };

    template<int N>
    struct Place_Holder
    {

    };

    Place_Holder<1> place_hold_1;
    Place_Holder<2> place_hold_2;
    Place_Holder<3> place_hold_3;
    Place_Holder<4> place_hold_4;
    Place_Holder<5> place_hold_5;
    Place_Holder<6> place_hold_6;
    Place_Holder<7> place_hold_7;
    Place_Holder<8> place_hold_8;
    Place_Holder<9> place_hold_9;
    Place_Holder<10> place_hold_10;
    Place_Holder<11> place_hold_11;
    Place_Holder<12> place_hold_12;
    Place_Holder<13> place_hold_13;
    Place_Holder<14> place_hold_14;
    Place_Holder<15> place_hold_15;
    Place_Holder<16> place_hold_16;
    Place_Holder<17> place_hold_17;
    Place_Holder<18> place_hold_18;
    Place_Holder<19> place_hold_19;
    Place_Holder<20> place_hold_20;

    template<typename T>
    struct filter
    {
        template<typename Give_Tuple>
        static auto do_filter(const T& t,Give_Tuple&& give_tuple)
        {
            return t;
        }
    };

    template<int N>
    struct filter<Place_Holder<N>>
    {
        template<typename Give_Tuple>
        static auto do_filter(const Place_Holder<N>&,Give_Tuple&& give_tuple)
        {
            return std::get<N-1>(give_tuple);
        }
    };

    template<typename Tuple,typename Give_Tuple,typename T,typename...Args>
    auto dispatch(std::tuple<T,Args...>&& tuple,Give_Tuple&& give_tuple,Tuple&& real_tuple)
    {
        auto real = std::tuple_cat(real_tuple,std::make_tuple(filter<T>::template do_filter(std::get<0>(tuple),give_tuple)));
        return dispatch(std::make_tuple(std::get<Args>(tuple)...),give_tuple,real);
    }

    template<typename Tuple,typename Give_Tuple,typename T>
    auto dispatch(std::tuple<T>&& tuple,Give_Tuple&& give_tuple,Tuple&& real_tuple)
    {
        auto real = std::tuple_cat(real_tuple,std::make_tuple(filter<T>::template do_filter(std::get<0>(tuple),give_tuple)));
        return real;
    };

    template<typename Tuple,typename Give_Tuple>
    auto dispatch(std::tuple<>&& tuple,Give_Tuple&& give_tuple,Tuple&& real_tuple)
    {
        return tuple;
    };

    template<typename F,typename...Args>
    struct x_binder
    {
        using place_parmas_tuple_type = std::tuple<Args...>;
        place_parmas_tuple_type place_tuple;
        using function_ret_type = typename function_trais<std::remove_reference_t<F>>::ret_type;

        x_binder(F&& function, const Args&...args):_function(std::forward<F>(function)),place_tuple(std::make_tuple(args...))
        {

        }
        template<typename...RealArgs>
        function_ret_type  operator()(RealArgs...rargs)
        {
            auto real = dispatch(std::move(place_tuple),std::make_tuple(rargs...),std::tuple<>{});
            return caller(real);
        }
        template<typename...real_Args>
        function_ret_type  caller(std::tuple<real_Args...>& tuple)
        {
            return _function(std::get<real_Args>(tuple)...);
        }
        F _function;
    };

    template<typename Class,typename Ret,typename...Args,typename...PArgs>
    struct x_binder<Ret(Class::*)(Args...),PArgs...>
    {
        using place_parmas_tuple_type = std::tuple<PArgs...>;
        using menmber_function_type = Ret(Class::*)(Args...);
        place_parmas_tuple_type place_tuple;
        using function_ret_type = Ret;
        Class _obj;
        x_binder(menmber_function_type function,const Class& obj, const PArgs&...args):_function(function),place_tuple(std::make_tuple(args...)),_obj(obj)
        {

        }
        template<typename...RealArgs>
        function_ret_type  operator()(RealArgs...rargs)
        {
            auto real = dispatch(std::move(place_tuple),std::make_tuple(rargs...),std::tuple<>{});
            return caller(real);
        }
        template<typename...real_Args>
        function_ret_type  caller(std::tuple<real_Args...>& tuple)
        {
            return (_obj.*_function)(std::get<real_Args>(tuple)...);
        }
        menmber_function_type _function;
    };


    template<typename F,typename...Args>
    std::enable_if_t<!function_trais<typename std::remove_reference<F>::type>::is_member_function,x_binder<F,std::remove_reference_t<Args>...>> make_xbinder(F&& function,Args&&...args)
{
    x_binder<F,std::remove_reference_t<Args>...> funtor(std::forward<F>(function),std::forward<Args>(args)...);
    return funtor;
};

template<typename Class,typename Ret,typename...Args,typename...PArgs>
std::enable_if_t<function_trais<Ret(Class::*)(Args...)>::is_member_function,x_binder<Ret(Class::*)(Args...),std::remove_reference_t<PArgs>...>> make_xbinder(Ret(Class::*pfunction)(Args...), const Class& obj,PArgs&&...args)
{
x_binder<Ret(Class::*)(Args...),std::remove_reference_t<PArgs>...> funtor(pfunction,obj,args...);
return funtor;
};

}
#endif //_XBINDER_HPP