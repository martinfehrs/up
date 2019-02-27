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
#include <string>


namespace up::error
{

    struct error : public std::exception
    {
        virtual ~error();
    };

    class error_with_message : public error
    {

      public:

        error_with_message() = default;
        error_with_message(const error_with_message&) = delete;
        error_with_message& operator=(const error&) = delete;
        error_with_message(error_with_message&&) = default;
        error_with_message& operator=(error_with_message&&) = default;

        template <typename... tp_string_types>
        error_with_message(tp_string_types&&... p_strings)
          : m_msg{ (... + std::forward<tp_string_types>(p_strings)) }
        {
        }

        virtual const char* what() const noexcept override;

      private:

        std::string m_msg;

    };

    struct unknown_word : public error_with_message
    {
        explicit unknown_word(const std::string& p_word);
    };

    struct missing_name : public error_with_message
    {
        explicit missing_name(const std::string& p_word);
    };

}
