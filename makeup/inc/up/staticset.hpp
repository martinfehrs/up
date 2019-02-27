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
#include <utility>


namespace up::static_set
{

    template <typename value_t>
    struct is_static_set : std::false_type
    {
    };

    template <typename value_t>
    constexpr auto is_static_set_v = is_static_set<value_t>::value;

    template <typename ...entry_ts>
    class static_set;

    template <typename... entry_ts>
    struct is_static_set<static_set<entry_ts...>> : std::true_type
    {
    };

    template <typename entry_t>
    class static_set<entry_t>
    {

      public:

        template <typename value_t>
        explicit constexpr static_set(value_t&& p_value)
            : m_head{ std::forward<value_t>(p_value) }
        {
        }

        template <typename value_t>
        constexpr bool contains(value_t&& p_value) const
        {
            return m_head == p_value;
        }

      private:

        entry_t m_head;
    };

    template <typename entry_t, typename... entry_ts>
    class static_set<entry_t, entry_ts...>
    {

      public:

        template <typename value_t, typename... value_ts>
        explicit constexpr static_set(value_t&& p_value, value_ts&&... p_values)
          : m_head{ std::forward<value_t>(p_value) }
          , m_tail{ std::forward<value_ts>(p_values)... }
        {
        }

        template <typename value_t>
        constexpr bool contains(value_t&& p_value) const
        {
            return m_head == p_value || m_tail.contains(std::forward<value_t>(p_value));
        }

      private:

        entry_t m_head;
        static_set<entry_ts...> m_tail;
    };

    template <typename... value_ts>
    constexpr auto make_static_set(value_ts&&... p_values)
    {
        return static_set<value_ts...>{ std::forward<value_ts>(p_values)... };
    }

    template <typename first_t, typename last_t>
    class static_inclusive_range
    {

      public:

        template <typename value1_t, typename value2_t>
        explicit constexpr static_inclusive_range(value1_t&& p_first, value2_t&& p_last)
            : m_first{ std::forward<value1_t>(p_first) }
            , m_last{ std::forward<value2_t>(p_last) }
        {
        }

        template <typename value_t>
        constexpr bool contains(value_t&& p_value) const
        {
            return p_value >= m_first && p_value <= m_last;
        }

      private:

        first_t m_first;
        last_t m_last;
    };

    template <typename first_t, typename last_t>
    struct is_static_set<static_inclusive_range<first_t, last_t>> : std::true_type
    {
    };

    template <typename first_t, typename last_t>
    constexpr auto make_static_inclusive_range(first_t&& p_first, last_t&& p_last)
    {
        return static_inclusive_range<first_t, last_t>{ std::forward<first_t>(p_first), std::forward<last_t>(p_last) };
    }

    template <typename first_t, typename last_t>
    class StaticExclusiveRange
    {

      public:

        template <typename value1_t, typename value2_t>
        explicit constexpr StaticExclusiveRange(value1_t&& p_first, value2_t&& p_last)
            : m_first{ std::forward<value1_t>(p_first) }
            , m_last{ std::forward<value2_t>(p_last) }
        {
        }

        template <typename value_t>
        constexpr bool contains(value_t&& p_value) const
        {
            return p_value >= m_first && p_value < m_last;
        }

      private:

        first_t m_first;
        last_t m_last;
    };

    template <typename first_t, typename last_t>
    struct is_static_set<StaticExclusiveRange<first_t, last_t>> : std::true_type
    {
    };

    template <typename first_t, typename last_t>
    constexpr auto make_static_exclusive_range(first_t&& p_first, last_t&& p_last)
    {
        using namespace std;

        return StaticExclusiveRange<decay_t<first_t>, decay_t<last_t>>{
            forward<first_t>(p_first),
            forward<last_t>(p_last)
        };
    }

    template <typename... subset_ts>
    class Unification;

    template <typename... subset_ts>
    struct is_static_set<Unification<subset_ts...>> : std::true_type
    {
    };

    template <typename subset_t>
    class Unification<subset_t>
    {

      public:

        template <typename set_t>
        explicit constexpr Unification(set_t&& p_value)
            : m_head{ std::forward<set_t>(p_value) }
        {
        }

        template <typename value_t>
        constexpr bool contains(value_t&& p_value) const
        {
            return m_head.contains(std::forward<value_t>(p_value));
        }

      private:

        subset_t m_head;
    };

    template <typename subset_t, typename... subset_ts>
    class Unification<subset_t, subset_ts...>
    {

      public:

        template <typename set_t, typename... set_ts>
        explicit constexpr Unification(set_t&& p_set, set_ts&&... p_sets)
          : m_head{ std::forward<set_t>(p_set) }
          , m_tail{ std::forward<set_ts>(p_sets)... }
        {
        }

        template <typename value_t>
        constexpr bool contains(value_t&& p_value) const
        {
            return m_head.contains(p_value) || m_tail.contains(std::forward<value_t>(p_value));
        }

      private:

        subset_t m_head;
        Unification<subset_ts...> m_tail;
    };

    template <typename... set_ts>
    constexpr auto unify(set_ts&&... p_sets)
    {
        return Unification<set_ts...>{ std::forward<set_ts>(p_sets)... };
    }

}

