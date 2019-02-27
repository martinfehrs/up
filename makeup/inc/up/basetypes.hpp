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


#include <cstdint>
#include <string>
#include <string_view>


namespace up::base_types
{

    template <std::size_t BYTES>
    struct fixed_integer_traits;

    template <>
    struct fixed_integer_traits<1>
    {
        using signed_t = int8_t;
        using unsigned_t = uint8_t;
    };

    template <>
    struct fixed_integer_traits<2>
    {
        using signed_t = int16_t;
        using unsigned_t = uint16_t;
    };

    template <>
    struct fixed_integer_traits<4>
    {
        using signed_t = int32_t;
        using unsigned_t = uint32_t;
    };

    template <>
    struct fixed_integer_traits<8>
    {
        using signed_t = int64_t;
        using unsigned_t = uint64_t;
    };

    template <>
    struct fixed_integer_traits<16>
    {
        using signed_t = __int128_t;
        using unsigned_t = __uint128_t;
    };

    template <std::size_t BYTES>
    using int_t = typename fixed_integer_traits<BYTES>::signed_t;

    template <std::size_t BYTES>
    using uint_t = typename fixed_integer_traits<BYTES>::unsigned_t;


    constexpr std::size_t ADDRESS_UNIT_SIZE = sizeof(char);
    constexpr std::size_t CHAR_SIZE         = ADDRESS_UNIT_SIZE;
    constexpr std::size_t CELL_SIZE         = sizeof(intptr_t);
    constexpr std::size_t DCELL_SIZE        = 2 * CELL_SIZE;

    [[maybe_unused]] constexpr std::size_t FCELL_SIZE    = sizeof(double);
    [[maybe_unused]] constexpr std::size_t MAX_CELL_SIZE = sizeof(__int128_t);

    using address_unit_t   = uint_t<ADDRESS_UNIT_SIZE>;
    using char_t           = char;
    using num_t            = int_t<CELL_SIZE>;
    using unum_t           = uint_t<CELL_SIZE>;
    using dnum_t           = int_t<DCELL_SIZE>;
    using udnum_t          = uint_t<DCELL_SIZE>;
    using bool_t           = uint_t<CELL_SIZE>;
    using fnum_t           = double;
    using dynamic_string_t = std::basic_string<char_t>;
    using string_view_t    = std::basic_string_view<char_t>;

    template <typename tp_environment>
    using func_t = void(tp_environment&);

    namespace literals
    {

        constexpr num_t operator"" _num(unsigned long long p_val)
        {
            return static_cast<num_t>(p_val);
        }

        constexpr unum_t operator"" _unum(unsigned long long p_val)
        {
            return static_cast<unum_t>(p_val);
        }

        constexpr dnum_t operator"" _dnum(unsigned long long p_val)
        {
            return static_cast<num_t>(p_val);
        }

        constexpr udnum_t operator"" _udnum(unsigned long long p_val)
        {
            return static_cast<unum_t>(p_val);
        }

        constexpr bool_t TRUE = ~0U;
        constexpr bool_t FALSE = 0U;

    }

}
