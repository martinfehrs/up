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


#include "package.hpp"
#include "error.hpp"
#include "dictionary.hpp"


namespace up::core_ext_words
{

    struct not_a_value : public error::error_with_message
    {
        explicit not_a_value(const std::string& p_word);
    };

    using namespace base_types::literals;

    template <typename tp_environment>
    void up_zero_greater(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(0) > 0 ? TRUE : FALSE);
    }

    template <typename tp_environment>
    void up_false(tp_environment& p_env)
    {
        p_env.stack.push(FALSE);
    }

    template <typename tp_environment>
    void up_pick(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(p_env.stack.get(0) + 1));
    }

    template <typename tp_environment>
    void up_roll(tp_environment& p_env)
    {
        const auto pos = p_env.stack.get(0) + 1;
        const auto tmp = p_env.stack.get(pos);

        for(auto i = pos; i > 0; i--)
            p_env.stack.set(i, p_env.stack.get(i - 1));

        p_env.stack.replace(2, tmp);
    }

    template <typename tp_environment>
    void up_true(tp_environment& p_env)
    {
        p_env.stack.push(TRUE);
    }

    template <typename tp_environment>
    void up_value(tp_environment& p_env)
    {
        base_types::num_t l_value = p_env.stack.get(0);
        std::string_view l_word = p_env.ui.read_word();

        if(l_word.empty())
            throw error::missing_name{ "VALUE" };

        p_env.dict.create(l_word, dictionary::runtime_type::VALUE, &l_value, sizeof(base_types::num_t));
        p_env.stack.drop();
    }

    template <typename tp_environment>
    void up_to(tp_environment& p_env)
    {
        auto l_word = p_env.ui.read_word();
        auto l_entry_addr = p_env.dict.find_entry(static_cast<std::string>(l_word));

        if (!l_entry_addr)
        {
            throw error::unknown_word{ static_cast<std::string>(l_word) };
        }

        auto l_entry_ptr = p_env.dict.template access_data<typename decltype(p_env.dict)::entry_type>(*l_entry_addr);

        if(l_entry_ptr->type() == dictionary::runtime_type::VALUE)
        {
            const base_types::num_t l_value = p_env.stack.get(0);

            p_env.dict.set_data(l_entry_ptr->data(), &l_value, sizeof(base_types::num_t));
        }
        else
        if(l_entry_ptr->type() == dictionary::runtime_type::NATIVE_VALUE)
        {
            **reinterpret_cast<base_types::num_t* const *>(l_entry_ptr->data()) = p_env.stack.get(0);
        }
        else
        {
            throw not_a_value{ static_cast<std::string>(l_word) };
        }

        p_env.stack.drop();
    }


    template <typename tp_environment>
    constexpr package::package_entry<tp_environment> CORE_EXT_WORDS[] = {
        {    "0>", package::FUNC, up_zero_greater  },
        { "FALSE", package::FUNC,        up_false  },
        {  "PICK", package::FUNC,         up_pick  },
        {  "ROLL", package::FUNC,         up_roll  },
        {  "TRUE", package::FUNC,         up_true  },
        { "VALUE", package::FUNC,         up_value },
        {    "TO", package::FUNC,         up_to    },
    };

}
