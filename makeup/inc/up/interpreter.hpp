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


#include <functional>
#include <deque>
#include <string>
#include <cctype>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <exception>
#include <limits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <thread>

#include "basetypes.hpp"
#include "error.hpp"
#include "stringutils.hpp"
#include "corewords.hpp"
#include "coreextwords.hpp"
#include "staticset.hpp"
#include "numberrunner.hpp"
#include "io.hpp"
#include "environment.hpp"


namespace up::interpreter
{

    struct out_of_range : public error::error_with_message
    {
        explicit out_of_range(std::string p_word);
    };

    struct unknown_runtime_type : public error::error_with_message
    {
        unknown_runtime_type();
    };

    template <typename tp_address_type, typename tp_environment>
    static void native_func_action(tp_address_type p_data_addr, tp_environment& p_env)
    {
        (*p_env.dict.template access_data<base_types::func_t<tp_environment>*>(p_data_addr))(p_env);
    }

    template <typename tp_address_type, typename tp_environment>
    static void constant_action(tp_address_type p_data_addr, tp_environment& p_env)
    {
        p_env.stack.push(p_env.dict.template access_data<base_types::num_t>(p_data_addr));
    }

    template <typename tp_address_type, typename tp_environment>
    static void two_constant_action(tp_address_type p_data_addr, tp_environment& p_env)
    {
        p_env.stack.template push<base_types::dnum_t>(
            p_env.dict.template access_data<base_types::dnum_t>(p_data_addr));
    }

    template <typename tp_address_type, typename tp_environment>
    static void variable_action(tp_address_type p_data_addr, tp_environment& p_env)
    {
        p_env.stack.push(p_env.dict.template access_data<base_types::num_t>(p_data_addr));
    }

    template <typename tp_address_type, typename tp_environment>
    static void native_variable_action(tp_address_type p_data_addr, tp_environment& p_env)
    {
        p_env.stack.push(*p_env.dict.template access_data<base_types::num_t*>(p_data_addr));
    }

    template <typename tp_address_type, typename tp_environment>
    static void address_action(tp_address_type p_data_addr, tp_environment& p_env)
    {
        p_env.stack.push(p_env.dict.template access_data<base_types::address_unit_t>(p_data_addr));
    }

    template <typename tp_address_type, typename tp_environment>
    static void value_action(tp_address_type p_data_addr, tp_environment& p_env)
    {
        p_env.stack.push(*p_env.dict.template access_data<base_types::num_t>(p_data_addr));
    }

    template <typename tp_address_type, typename tp_environment>
    static void native_value_action(tp_address_type p_data_addr, tp_environment& p_env)
    {
        p_env.stack.push(**p_env.dict.template access_data<base_types::num_t*>(p_data_addr));
    }

    template <typename environment_t>
    void read_literal(std::string_view p_word, environment_t& p_env)
    {
        number_runner::num_stream_t l_num_stream{ p_word };
        number_runner::number_runner<environment_t> l_number_runner{ p_env.base, p_env.dotmode };

        try
        {
            l_number_runner(l_num_stream, p_env);
        }
        catch(const number_runner::not_a_number&)
        {
            throw error::unknown_word{ static_cast<std::string>(p_word) };
        }
        catch(const number_runner::out_of_range&)
        {
            throw out_of_range{ static_cast<std::string>(p_word) };
        }
    }

    template <typename tp_environment_type>
    void sync_with_ui(tp_environment_type& p_env)
    {
        p_env.ui.update(p_env);
        p_env.ui.refill_input_buffer();
        p_env.ui.write_str(p_env.ui.input_buffer(), io::format::INPUT);
        p_env.ui.write_chr(' ');
    }

    constexpr struct async {} ASYNC;
    constexpr struct seq   {} SEQ;

    template <
        typename tp_ui_backend_type,
        typename tp_stack_backend_type,
        typename tp_fstack_backend_type,
        typename tp_rstack_backend_type,
        typename tp_dict_backend_type
    >
    void interpret(
        async,
        tp_ui_backend_type& p_ui_backend,
        tp_stack_backend_type& p_stack_backend,
        tp_fstack_backend_type& p_fstack_backend,
        tp_rstack_backend_type& p_rstack_backend,
        tp_dict_backend_type& p_dict_backend
    )
    {
        std::thread{ [&](){
                interpret(SEQ, p_ui_backend, p_stack_backend, p_fstack_backend, p_rstack_backend, p_dict_backend);
        }}.detach();
    }

    template <
        typename tp_ui_backend_type,
        typename tp_stack_backend_type,
        typename tp_fstack_backend_type,
        typename tp_rstack_backend_type,
        typename tp_dict_backend_type
    >
    void interpret(
        seq,
        tp_ui_backend_type& p_ui,
        tp_stack_backend_type& p_stack_backend,
        tp_fstack_backend_type& p_fstack_backend,
        tp_rstack_backend_type& p_rstack_backend,
        tp_dict_backend_type& p_dict_backend
    )
    {
        environment::environment l_env{
            p_ui,
            p_stack_backend,
            p_fstack_backend,
            p_rstack_backend,
            p_dict_backend
        };

        sync_with_ui(l_env);

        while(true)
        {
            try
            {
                auto l_word = l_env.ui.read_word();

                if(l_word.empty())
                {
                    l_env.ui.write_str(" ok\n", io::format::SUCCESS);
                    sync_with_ui(l_env);;
                }
                else
                {
                    auto l_entry_addr = l_env.dict.find_entry(static_cast<std::string>(l_word));

                    if(l_entry_addr)
                    {
                        auto l_entry_ptr =
                            l_env.dict.template access_data<typename decltype(l_env.dict)::entry_type>(*l_entry_addr);

                        switch(l_entry_ptr->type())
                        {
                            case dictionary::runtime_type::NATIVE_FUNCTION:
                                native_func_action(l_entry_ptr->data(), l_env);
                                break;

                            case dictionary::runtime_type::VARIABLE:
                                variable_action(l_entry_ptr->data(), l_env);
                                break;

                            case dictionary::runtime_type::CONSTANT:
                                constant_action(l_entry_ptr->data(), l_env);
                                break;

                            default:
                                throw unknown_runtime_type{};

                        };
                    }
                    else
                    {
                        read_literal(l_word, l_env);
                    }
                }
            }
            catch(const up::error::error& p_err)
            {
                l_env.ui.write_chr(' ');
                l_env.ui.write_str(p_err.what(), up::io::format::ERROR);
                l_env.ui.write_chr('\n');
                sync_with_ui(l_env);
            }
        }
    }

}

