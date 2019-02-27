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


#include "basetypes.hpp"
#include "stringutils.hpp"
#include "error.hpp"


namespace up::number_runner
{

    using num_stream_t = string_utils::string_stream<std::string_view>;


    struct not_a_number : public error::error
    {
        ~not_a_number() override;
    };

    struct out_of_range : public error::error
    {
        ~out_of_range() override;
    };

    template <typename environment_t>
    class number_runner
    {
      public:

        number_runner(base_types::num_t p_default_base, base_types::num_t p_dot_mode) noexcept;

        void operator()(num_stream_t& p_num_stream, environment_t& p_interpeter);

      private:

        void read_character(num_stream_t& p_num_stream, environment_t& p_env);

        void read_number(num_stream_t& p_num_stream, environment_t& p_env);

        void read_digit_separator(num_stream_t& p_num_stream);

        void read_numeric_symbol(num_stream_t& p_num_stream);

        base_types::num_t read_base(num_stream_t& p_num_stream) noexcept;

        bool is_single_cell_number(bool p_is_punctuated, bool p_has_exponent) noexcept;
        bool is_double_cell_number(bool p_is_punctuated, bool p_has_exponent) noexcept;
        bool is_floating_point_number(bool p_is_punctuated, bool p_has_exponent) noexcept;

        const base_types::num_t m_default_base;
        const base_types::num_t m_dot_mode;
    };

    constexpr base_types::num_t DECIMAL_BASE = 10;
    constexpr base_types::num_t HEXADECIMAL_BASE = 16;
    constexpr base_types::num_t BINARY_BASE = 2;
    constexpr base_types::num_t FIRST_DIGIT = '0';
    constexpr base_types::num_t FIRST_UPPERCASE_LETTER = 'A';
    constexpr base_types::num_t FIRST_LOWERCASE_LETTER = 'a';
    constexpr base_types::num_t DECIMAL_PREFIX = '#';
    constexpr base_types::num_t HEXADECIMAL_PREFIX ='$';
    constexpr base_types::num_t BINARY_PREFIX = '%';
    constexpr base_types::num_t MINUS_SIGN = '-';
    constexpr base_types::num_t CHARACTER_PREFIX = '\'';
    constexpr base_types::num_t CHARACTER_SUFFIX = '\'';

    constexpr auto DIGIT_SEPARATOR = static_set::make_static_set('.', '/', '-', '\'', ',');
    constexpr auto EXPONENT = static_set::make_static_set('E', 'e');
    constexpr auto PRINTABLE_CHARACTER = static_set::make_static_inclusive_range(0x20, 0x7E);

    constexpr base_types::udnum_t NUM_MAX  = std::numeric_limits<base_types::num_t>::max();
    constexpr base_types::udnum_t DNUM_MAX = std::numeric_limits<base_types::dnum_t>::max();
    constexpr base_types::udnum_t UNUM_MAX = std::numeric_limits<base_types::unum_t>::max();


    constexpr auto decimal_digit(base_types::num_t p_base)
    {
        return static_set::make_static_exclusive_range(FIRST_DIGIT, FIRST_DIGIT + p_base);
    }

    constexpr auto uppercase_letter_digit(base_types::num_t p_base)
    {
        return static_set::make_static_exclusive_range(FIRST_UPPERCASE_LETTER, FIRST_UPPERCASE_LETTER + p_base - DECIMAL_BASE);
    }

    constexpr auto lowercase_letter_digit(base_types::num_t p_base)
    {
        return static_set::make_static_exclusive_range(FIRST_LOWERCASE_LETTER, FIRST_LOWERCASE_LETTER + p_base - DECIMAL_BASE);
    }

    constexpr auto digit(base_types::num_t p_base)
    {
        return static_set::unify(decimal_digit(p_base), uppercase_letter_digit(p_base), lowercase_letter_digit(p_base));
    }


    class digit_sequence_reader
    {

      public:

        explicit digit_sequence_reader(base_types::num_t p_base)
            : m_base{ p_base }
            , m_digit{ digit(m_base) }
            , m_decimal_digit{ decimal_digit(m_base) }
            , m_lowercase_letter_digit{ lowercase_letter_digit(m_base) }
            , m_uppercase_letter_digit{ uppercase_letter_digit(m_base) }
        {
        }

        void read_digit(num_stream_t& p_num_stream)
        {
            base_types::udnum_t l_digit_value;

            if(p_num_stream.is(m_decimal_digit))
            {
                l_digit_value = p_num_stream.read() - FIRST_DIGIT;
            }
            else
            if(p_num_stream.is(m_lowercase_letter_digit))
            {
                l_digit_value = p_num_stream.read() - FIRST_LOWERCASE_LETTER + DECIMAL_BASE;
            }
            else
            if(p_num_stream.is(m_uppercase_letter_digit))
            {
                l_digit_value = p_num_stream.read() - FIRST_UPPERCASE_LETTER + DECIMAL_BASE;
            }
            else
            {
                throw not_a_number{};
            }

            if((std::numeric_limits<base_types::udnum_t>::max() - l_digit_value) / m_base < m_integer_value)
            {
                throw out_of_range{};
            }

            m_integer_value = m_integer_value * m_base + l_digit_value;
            m_digit_count++;
        }

        void read_digit_separator(num_stream_t& p_num_stream)
        {
            p_num_stream.ignore();
            m_is_punctuated = true;
            m_double_point_location = m_digit_count;
        }

        void read_symbol(num_stream_t& p_num_stream)
        {
            if(p_num_stream.is(m_digit))
            {
                read_digit(p_num_stream);
            }
            else
            if(p_num_stream.is(DIGIT_SEPARATOR))
            {
                read_digit_separator(p_num_stream);
            }
            else
            {
                throw not_a_number{};
            }
        }

        void operator()(num_stream_t& p_num_stream)
        {
            init();

            read_digit(p_num_stream);

            while(!p_num_stream.is(string_utils::EOS, EXPONENT))
            {
                read_symbol(p_num_stream);
            }

            if(m_double_point_location == 0)
            {
                m_double_point_location = m_digit_count;
            }
        }

        bool is_punctuated() const noexcept
        {
            return m_is_punctuated;
        }

        base_types::udnum_t integer_value() const noexcept
        {
            return m_integer_value;
        }

        base_types::num_t double_point_location() const noexcept
        {
            return m_double_point_location;
        }

        base_types::num_t digit_count() const noexcept
        {
            return m_digit_count;
        }

      private:

        void init() noexcept
        {
            m_integer_value = 0;
            m_double_point_location = 0;
            m_is_punctuated = false;
            m_digit_count = 0;
        }

        const base_types::num_t m_base;
        const decltype(digit(std::declval<base_types::num_t>())) m_digit;
        const decltype(decimal_digit(std::declval<base_types::num_t>())) m_decimal_digit;
        const decltype(lowercase_letter_digit(std::declval<base_types::num_t>())) m_lowercase_letter_digit;
        const decltype(uppercase_letter_digit(std::declval<base_types::num_t>())) m_uppercase_letter_digit;

        bool m_is_punctuated;
        base_types::num_t m_double_point_location;
        base_types::num_t m_digit_count;
        base_types::udnum_t m_integer_value;

    };

    template <typename environment_t>
    number_runner<environment_t>::number_runner(base_types::num_t p_default_base, base_types::num_t p_dot_mode) noexcept
      : m_default_base{ p_default_base }
      , m_dot_mode{ p_dot_mode }
    {
    }

    template <typename environment_t>
    base_types::num_t number_runner<environment_t>::read_base(num_stream_t& p_num_stream) noexcept
    {
        if(p_num_stream.is(DECIMAL_PREFIX))
        {
            p_num_stream.ignore();
            return DECIMAL_BASE;
        }
        else
        if(p_num_stream.is(HEXADECIMAL_PREFIX))
        {
            p_num_stream.ignore();
            return HEXADECIMAL_BASE;
        }
        else
        if(p_num_stream.is(BINARY_PREFIX))
        {
            p_num_stream.ignore();
            return BINARY_BASE;
        }
        else
        {
            return m_default_base;
        }
    }

    inline base_types::num_t read_sign(num_stream_t& p_num_stream) noexcept
    {
        if(p_num_stream.is('-'))
        {
            p_num_stream.ignore();
            return -1;
        }
        else
        {
            return 1;
        }
    }

    inline auto read_exponent(num_stream_t& p_num_stream, digit_sequence_reader& p_digit_sequence_reader)
    {
        using namespace up::base_types::literals;

        if(!p_num_stream.is(EXPONENT))
        {
            return std::pair{ false, 0_udnum };
        }

        p_num_stream.ignore();

        if(p_num_stream.is(string_utils::EOS))
        {
            return std::pair{ true, 0_udnum };
        }

        const base_types::udnum_t l_sign = read_sign(p_num_stream);

        p_digit_sequence_reader(p_num_stream);

        const auto l_exponent_value = static_cast<base_types::num_t>(p_digit_sequence_reader.integer_value());

        return std::pair{ true, l_sign * l_exponent_value };
    }

    template <typename environment_t>
    bool number_runner<environment_t>::is_single_cell_number(bool p_is_punctuated, bool p_has_exponent) noexcept
    {
        return (!p_is_punctuated && !p_has_exponent) || (p_is_punctuated && m_dot_mode == 1);
    }

    template <typename environment_t>
    bool number_runner<environment_t>::is_double_cell_number(bool p_is_punctuated, bool p_has_exponent) noexcept
    {
        return p_is_punctuated && m_dot_mode == 1 && !p_has_exponent;
    }

    template <typename environment_t>
    bool number_runner<environment_t>::is_floating_point_number(bool p_is_punctuated, bool p_has_exponent) noexcept
    {
        return (p_is_punctuated && m_dot_mode == 2) || p_has_exponent;
    }

    template <typename environment_t>
    void number_runner<environment_t>::operator()(num_stream_t& p_num_stream, environment_t& p_env)
    {
        p_num_stream.is(CHARACTER_PREFIX)
            ? read_character(p_num_stream, p_env)
            : read_number(p_num_stream, p_env);

        if(!p_num_stream.is(string_utils::EOS))
        {
            throw not_a_number{};
        }
    }

    template <typename environment_t>
    void number_runner<environment_t>::read_character(num_stream_t& p_num_stream, environment_t& p_env)
    {
        p_num_stream.ignore();

        if(!p_num_stream.is(PRINTABLE_CHARACTER))
        {
            throw not_a_number{};
        }

        p_env.stack.push(static_cast<base_types::num_t>(p_num_stream.read()));

        if(!p_num_stream.is(CHARACTER_SUFFIX))
        {
            throw not_a_number{};
        }

        p_num_stream.ignore();
    }

    template <typename environment_t>
    void number_runner<environment_t>::read_number(num_stream_t& p_num_stream, environment_t& p_env)
    {
        using namespace std;

        const auto l_base = read_base(p_num_stream);
        const auto l_sign = read_sign(p_num_stream);

        digit_sequence_reader l_digit_sequence_reader{ l_base };

        l_digit_sequence_reader(p_num_stream);

        const auto l_is_punctuated = l_digit_sequence_reader.is_punctuated();
        const auto l_integer_value = l_digit_sequence_reader.integer_value();
        const auto l_digit_count = l_digit_sequence_reader.digit_count();
        const auto l_double_point_location = l_digit_sequence_reader.double_point_location();

        bool l_has_exponent;
        base_types::num_t l_exponent;

        std::tie(l_has_exponent, l_exponent) = read_exponent(p_num_stream, l_digit_sequence_reader);

        // Checking for integer overflow. Overflow checking for unsigned double-cell numbers isn't necessary, because
        // there's allready a check inside DigitSequenceReader::read_digit.

        if(is_single_cell_number(l_is_punctuated, l_has_exponent) && l_sign == -1 && l_integer_value > NUM_MAX)
        {
            // integer value exceeds allowed maximum value for signed single-cell number
            throw out_of_range{};
        }

        if(is_single_cell_number(l_is_punctuated, l_has_exponent) && l_sign == 1 && l_integer_value > UNUM_MAX)
        {
            throw out_of_range{};
        }

        if(is_double_cell_number(l_is_punctuated, l_has_exponent) && l_sign == -1 && l_integer_value > DNUM_MAX)
        {
            // integer value exceeds allowed maximum value for signed double-cell number
            throw out_of_range{};
        }

        // floating point nummber
        if(is_floating_point_number(l_is_punctuated, l_has_exponent))
        {
            const auto l_fractional_digit_count = l_digit_count - l_double_point_location;
            const auto l_mantissa = l_sign * static_cast<double>(l_integer_value) / pow(l_base, l_fractional_digit_count);
            p_env.fstack.push(l_mantissa * pow(l_base, l_exponent));
        }
        else
        if(l_is_punctuated && m_dot_mode == 0)
        {
            p_env.stack.push(l_sign * static_cast<base_types::dnum_t>(l_integer_value));
        }
        else
        if(l_integer_value > static_cast<base_types::udnum_t>(numeric_limits<base_types::num_t>::max()) && l_sign == 1)
        {
            // It's an unsigned number!
            p_env.stack.push(static_cast<base_types::unum_t>(l_integer_value));
        }
        else
        {
            // It's a signed number!
            p_env.stack.push(l_sign * static_cast<base_types::num_t>(l_integer_value));
        }
    }

}
