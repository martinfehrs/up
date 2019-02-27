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


#include <string>

#include "basetypes.hpp"


namespace up::io
{

    enum format
    {
        DEFAULT,
        INPUT,
        SUCCESS,
        ERROR
    };

    template <typename tp_backend_type>
    class user_interface
    {
      public:

        using backend_type = tp_backend_type;


        explicit user_interface(tp_backend_type& p_backend)
            : m_backend{ p_backend }
        {
        }

        void refill_input_buffer()
        {
            m_input_buffer = m_backend.refill_input_buffer();
            m_input_buffer_pos = 0;
        }

        base_types::dynamic_string_t read_word() noexcept
        {
            ignore_whitespace();

            base_types::dynamic_string_t l_word;

            while(m_input_buffer_pos < std::size(m_input_buffer) && !is_whitespace(m_input_buffer[m_input_buffer_pos]))
            {
                l_word += m_input_buffer[m_input_buffer_pos];
                m_input_buffer_pos++;
            }

            return l_word;
        }

        base_types::char_t* input_buffer() noexcept
        {
            return m_input_buffer.data();
        }

        const base_types::char_t* input_buffer() const noexcept
        {
            return m_input_buffer.data();
        }

        base_types::unum_t input_buffer_pos() const noexcept
        {
            return m_input_buffer_pos;
        }

        void write_chr(base_types::char_t p_chr)
        {
            m_backend.write_chr(p_chr, 1, format::DEFAULT);
        }

        void write_chr(base_types::char_t p_chr, base_types::unum_t p_count)
        {
            m_backend.write_chr(p_chr, p_count, format::DEFAULT);
        }

        void write_chr(base_types::char_t p_chr, format p_fmt)
        {
            m_backend.write_chr(p_chr, 1, p_fmt);
        }

        void write_str(base_types::string_view_t p_str)
        {
            m_backend.write_str(p_str, format::DEFAULT);
        }

        void write_str(base_types::string_view_t p_str, format p_fmt)
        {
            m_backend.write_str(p_str, p_fmt);
        }

        template <typename tp_environment_type>
        void update(tp_environment_type& p_env)
        {
            m_backend.update(p_env);
        }

      private:

        bool is_whitespace(base_types::char_t p_chr) const noexcept
        {
            return p_chr == 0x20 || p_chr == 0x09 || p_chr == 0x0B || p_chr == 0x0D || p_chr == 0x0A;
        }

        void ignore_whitespace() noexcept
        {
            while(m_input_buffer_pos < std::size(m_input_buffer) && is_whitespace(m_input_buffer[m_input_buffer_pos]))
            {
                m_input_buffer_pos++;
            }
        }

        base_types::unum_t m_input_buffer_pos = 0;
        base_types::dynamic_string_t m_input_buffer;
        backend_type& m_backend;

    };

}
