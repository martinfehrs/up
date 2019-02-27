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


#include <exception>
#include <stdexcept>
#include <string>
#include <locale>

#include "staticset.hpp"


namespace up::string_utils
{

    namespace details
    {

        constexpr unsigned DECIMAL_BASE = 10;
        constexpr unsigned LETTER_COUNT = 'Z' - 'A' + 1;

        template <typename num_t>
        struct ToString
        {
            static_assert(std::is_unsigned_v<num_t>);

            std::string operator()(num_t p_num, num_t p_base) const noexcept
            {
                using namespace std::string_literals;

                const num_t l_quotient = p_num / p_base;
                const num_t l_reminder = p_num % p_base;
                char l_digit;

                if(l_reminder < DECIMAL_BASE)
                {
                    l_digit = '0' + l_reminder;
                }
                else
                if(l_reminder < DECIMAL_BASE + LETTER_COUNT)
                {
                    l_digit = 'A' + l_reminder - DECIMAL_BASE;
                }

                return ((l_quotient == 0) ? ""s : (*this)(l_quotient, p_base)) + l_digit;
            }
        };
    }


    template <typename... string_ts>
    std::string concat(string_ts&&... p_strings)
    {
        return (... + p_strings);
    }

    inline std::string to_string(std::string&& p_string) noexcept
    {
        return p_string;
    }

    inline std::string to_string(const std::string& p_string)
    {
        return p_string;
    }

    template <typename num_t, typename base_t>
    std::string to_string(num_t p_num, base_t p_base)
    {
        if constexpr(std::is_signed_v<base_t>)
        {
            if(p_base < 0)
            {
                throw std::domain_error{ "negative base" };
            }
        }

        if(p_base > details::DECIMAL_BASE + details::LETTER_COUNT)
        {
            throw std::domain_error{ "non-representable base" };
        }

        if constexpr(std::is_signed_v<num_t>)
        {
            using namespace std::string_literals;

            using unsigned_num_t = std::make_unsigned_t<num_t>;

            if(p_num < 0)
            {
                return "-"s + details::ToString<unsigned_num_t>{}(
                    static_cast<unsigned_num_t>(-p_num),
                    static_cast<unsigned_num_t>(p_base)
                );
            }
            else
            {
                return details::ToString<unsigned_num_t>{}(
                    static_cast<unsigned_num_t>(p_num),
                    static_cast<unsigned_num_t>(p_base)
                );
            }
        }
        else
        {
            return details::ToString<num_t>{}(p_num, static_cast<num_t>(p_base));
        }
    }   

    inline char to_uppercase(char p_chr)
    {
        return std::use_facet<std::ctype<char>>(std::locale()).toupper(p_chr);
    }

    inline const char* to_uppercase(char* p_begin, const char* p_end)
    {
        return std::use_facet<std::ctype<char>>(std::locale()).toupper(p_begin, p_end);
    }

    inline const char* to_uppercase(char* p_str, size_t p_len)
    {
        return to_uppercase(p_str, p_str + p_len);
    }

    inline bool case_insensitive_compare(const char* p_begin, const char* p_end, const char* p_begin2)
    {
        auto& facet = std::use_facet<std::ctype<char>>(std::locale());

        while(p_begin != p_end)
        {
            if(facet.toupper(*p_begin) != facet.toupper(*p_begin2))
                return false;
        }

        return true;
    }

    inline bool case_insensitive_compare(const char* p_str1, const char* p_str2, size_t p_len)
    {
        auto& facet = std::use_facet<std::ctype<char>>(std::locale());

        for(size_t i = 0; i < p_len; i++)
        {
            if(facet.toupper(p_str1[i]) != facet.toupper(p_str2[2]))
                return false;
        }

        return true;
    }

    constexpr auto WHITESPACE = static_set::make_static_set(' ', '\t', '\n', '\v');
    static constexpr auto EOS = -1;

    template <typename storage_t>
    struct is_string
    {
        static constexpr bool value = false;
    };

    template <>
    struct is_string<std::string>
    {
        static constexpr bool value = true;
    };

    template <>
    struct is_string<std::string_view>
    {
        static constexpr bool value = true;
    };

    template <typename storage_t, typename = typename std::enable_if<is_string<storage_t>::value>::type>
    struct string_stream
    {
        using traits_t = std::char_traits<char>;
        using value_t = traits_t::char_type;
        using int_t = traits_t::int_type;

        string_stream() = default;
        string_stream(const string_stream&) = delete;
        string_stream(string_stream&&) = default;
        string_stream& operator=(const string_stream&) = delete;
        string_stream& operator=(string_stream&&) = default;

        string_stream(const storage_t& storage)
            : _storage{ storage }
            , _pos{ std::begin(_storage) }
        { }

        string_stream(storage_t&& storage)
            : _storage{ storage }
            , _pos{ std::begin(_storage) }
        { }

        constexpr bool eos() const noexcept
        {
            return _pos == std::end(_storage);
        }

        constexpr int_t peek() const noexcept
        {
            return eos() ? EOS : *(_pos);
        }

        template <typename char_t, typename = decltype(std::declval<char_t>() == std::declval<int_t>())>
        constexpr bool is(char_t&& p_char) const
        {
            return peek() == std::forward<char_t>(p_char);
        }

        template <
            typename set_t, typename = std::enable_if_t<static_set::is_static_set_v<std::decay_t<set_t>>>,
            typename = void
        >
        constexpr bool is(set_t&& p_set) const
        {
            return p_set.contains(peek());
        }

        template <
            typename char_t,
            typename... tail_ts,
            typename = decltype(std::declval<char_t>() == std::declval<int_t>())
        >
        constexpr bool is(char_t&& p_char, tail_ts&&... p_tail) const
        {
            return this->is(std::forward<char_t>(p_char)) || is(std::forward<tail_ts>(p_tail)...);
        }

        template <
            typename set_t,
            typename... tail_ts,
            typename = std::enable_if_t<static_set::is_static_set_v<std::decay_t<set_t>>>,
            typename = void
        >
        constexpr bool is(set_t&& p_set, tail_ts&&... p_tail) const
        {
            return is(std::forward<set_t>(p_set)) || is(std::forward<tail_ts>(p_tail)...);
        }

        template <typename set_t, typename = std::enable_if_t<static_set::is_static_set_v<std::decay_t<set_t>>>>
        void ignore_all(set_t&& p_set) noexcept
        {
            while(is(std::forward<set_t>(p_set)))
            {
                ignore();
            }
        }

        int_t read() noexcept
        {
            return eos() ? EOS : *(_pos++);
        }

        void ignore() noexcept
        {
            read();
        }

        std::string_view read_word() noexcept
        {
            ignore_all(WHITESPACE);

            const auto offset = _pos - std::begin(_storage);
            std::string_view::size_type count = 0;

            while(!is(WHITESPACE, EOS))
            {
                count++;
                _pos++;
            }

            return { _storage.data() + offset, count };
        }

        string_stream& storage(const storage_t& storage)
        {
            _storage = storage;
            _pos = std::begin(_storage);

            return *this;
        }

        string_stream& storage(storage_t&& storage)
        {
            _storage = storage;
            _pos = std::begin(_storage);

            return *this;
        }

        const storage_t& storage() const noexcept
        {
            return _storage;
        }

        storage_t& storage() noexcept
        {
            return _storage;
        }

        size_t pos() const noexcept
        {
            return _pos - std::begin(_storage);
        }

    private:

        storage_t _storage;
        decltype(std::declval<storage_t>().begin()) _pos;
    };

}
