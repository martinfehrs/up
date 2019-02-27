/**********************************************************************************************************************
 *                      __                                                                                            *
 *                     /\ \                                                                                           *
 *    ___ ___      __  \ \ \/'\      __   __  __  _____                                                               *
 *  /' __` __`\  /'__`\ \ \ , <    /'__`\/\ \/\ \/\ '__`\                                                             *
 *  /\ \/\ \/\ \/\ \L\.\_\ \ \\`\ /\  __/\ \ \_\ \ \ \L\ \                                                            *
 *  \ \_\ \_\ \_\ \__/.\_\\ \_\ \_\ \____\\ \____/\ \ ,__/                                                            *
 *   \/_/\/_/\/_/\/__/\/_/ \/_/\/_/\/____/ \/___/  \ \ \/                                                             *
 *                                                  \ \_\                                                             *
 *  (c) Martin Kalbfuß                               \/_/                                                             *
 **********************************************************************************************************************/
#pragma once


#include <type_traits>


namespace up::type_list
{

    template <template <typename, typename...> typename trait_tt, typename first_arg_t>
    struct curry
    {
        template <typename... arg_ts>
        using type = trait_tt<first_arg_t, arg_ts...>;
    };

    template <typename... entry_ts>
    struct type_list
    {
    };

    template <typename first_entry_t, typename second_entry_t>
    using type_pair_t = type_list<first_entry_t, second_entry_t>;

    using nil_t = type_list<>;


    template <typename type_list_t>
    struct head;

    template <template <typename...> typename type_list_tt, typename head_t, typename... tail_ts>
    struct head<type_list_tt<head_t, tail_ts...>>
    {
        using result_t = head_t;
    };

    template <>
    struct head<nil_t>
    {
        using result_t = nil_t;
    };

    template <typename type_list_t>
    using head_t = typename head<type_list_t>::result_t;


    template <typename type_list_t>
    struct tail;

    template <template <typename...> typename type_list_tt, typename head_t, typename... tail_ts>
    struct tail<type_list_tt<head_t, tail_ts...>>
    {
        using result_t = type_list<tail_ts...>;
    };

    template <>
    struct tail<nil_t>
    {
        using result_t = nil_t;
    };

    template <typename type_list_t>
    using tail_t = typename tail<type_list_t>::result_t;


    template <typename type_list_t>
    struct size;

    template <template <typename...> typename type_list_tt, typename... entry_ts>
    struct size<type_list_tt<entry_ts...>> : std::integral_constant<std::size_t, sizeof...(entry_ts)> { };

    template <typename type_list_t>
    constexpr auto size_v = size<type_list_t>::value;


    template <typename first_type_list_t, typename second_type_list_t>
    struct Concatenate;

    template <
        template<typename...> typename first_type_list_t,
        template<typename...> typename second_type_list_t,
        typename... first_entry_ts,
        typename... second_entry_ts
    >
    struct Concatenate<
        first_type_list_t<first_entry_ts...>,
        second_type_list_t<second_entry_ts...>
    >
    {
        using result_t = type_list<first_entry_ts..., second_entry_ts...>;
    };

    template <typename first_type_list, typename second_type_list>
    using concatenate_t = typename Concatenate<first_type_list, second_type_list>::result_t;


    template <typename type_list_t, template<typename...> typename condition_tt>
    struct FindFirst
    {
        using result_t = std::conditional_t<
            condition_tt<head_t<type_list_t>>{},
            head_t<type_list_t>,
            typename FindFirst<tail_t<type_list_t>, condition_tt>::result_t
        >;
    };

    template <template<typename...> typename condition_tt>
    struct FindFirst<nil_t, condition_tt>
    {
        using result_t = nil_t;
    };

    template <typename type_list_t, template<typename...> typename condition_tt>
    using find_first_t = typename FindFirst<type_list_t, condition_tt>::result_t;


    template <typename type_list_t, typename entry_t>
    struct contains;

    template <template <typename...> typename type_list_tt, typename entry_t, typename... entry_ts>
    struct contains<type_list_tt<entry_ts...>, entry_t>
    {
        static constexpr bool result = std::disjunction_v<std::is_same<entry_t, entry_ts>...>;
    };

    template <typename type_list_t, typename entry_t>
    constexpr auto contains_v = contains<type_list_t, entry_t>::result;


    template <typename type_list_t, template<typename> typename transformator_t>
    struct transform;

    template <typename type_list_t, template<typename> typename transformator_t>
    using transform_t = typename transform<type_list_t, transformator_t>::result_t;

    template <
        template <typename...> typename type_list_tt,
        template<typename> typename transformator_tt,
        typename... entry_ts
    >
    struct transform<type_list_tt<entry_ts...>, transformator_tt>
    {
        using result_t = type_list<typename transformator_tt<entry_ts>::result_t...>;
    };

    template <typename type_list_t, template<typename> typename predicate_tt>
    struct all_of;

    template <typename type_list_t, template<typename> typename predicate_tt>
    constexpr bool all_of_v = all_of<type_list_t, predicate_tt>::value;

    template <
        template<typename...> typename type_list_tt, template<typename> typename predicate_tt, typename... entry_ts>
    struct all_of<type_list_tt<entry_ts...>, predicate_tt>
        : std::bool_constant<std::conjunction_v<predicate_tt<entry_ts>...>> { };


    namespace details
    {
        template <typename t>
        struct IsDouble : std::bool_constant<std::is_same_v<t, double>> { };

        template <typename t>
        struct HasIntKey : std::bool_constant<std::is_same_v<head_t<t>, int>> { };

        //static_assert(std::is_same<Bind>)

        static_assert(std::is_same_v<head_t<type_list<int>>, int>);
        static_assert(std::is_same_v<head_t<type_list<>>, nil_t>);

        static_assert(std::is_same_v<tail_t<type_list<int, double>>, type_list<double>>);
        static_assert(std::is_same_v<tail_t<type_list<int>>, nil_t>);
        static_assert(std::is_same_v<tail_t<nil_t>, nil_t>);

        static_assert(size_v<nil_t> == 0);
        static_assert(size_v<type_list<int>> == 1);
        static_assert(size_v<type_list<int, double>> == 2);

        static_assert(std::is_same_v<concatenate_t<type_list<int>, type_list<double>>, type_list<int, double>>);
        static_assert(std::is_same_v<concatenate_t<nil_t, type_list<int>>, type_list<int>>);
        static_assert(std::is_same_v<concatenate_t<type_list<int>, nil_t>, type_list<int>>);
        static_assert(std::is_same_v<concatenate_t<nil_t, nil_t>, nil_t>);

        template <typename t>
        using is_double_t = std::is_same<double, t>;

        static_assert(std::is_same_v<find_first_t<type_list<int, double, char>, is_double_t>, double>);
        static_assert(std::is_same_v<find_first_t<type_list<int, float, char>, IsDouble>, nil_t>);
        static_assert(std::is_same_v<find_first_t<type_list<>, IsDouble>, nil_t>);

        static_assert(
            std::is_same_v<find_first_t<type_list<type_pair_t<float, double>,
            type_pair_t<int, long>>, HasIntKey>, type_pair_t<int, long>>
        );

        static_assert(contains_v<type_list<int, double, char>, double>);
        static_assert(!contains_v<type_list<int, float, char>, double>);
        static_assert(!contains_v<type_list<>, double>);

    }

}
