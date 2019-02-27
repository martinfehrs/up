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


#include "stacks.hpp"
#include "dictionary.hpp"
#include "io.hpp"
#include "corewords.hpp"
#include "coreextwords.hpp"
#include "upwords.hpp"


namespace up::environment
{

    template <
        typename tp_ui_backend_type,
        typename tp_stack_backend_type,
        typename tp_fstack_backend_type,
        typename tp_rstack_backend_type,
        typename tp_dict_backend_type
    >
    struct environment
    {


        environment(
            tp_ui_backend_type& p_ui_backend,
            tp_stack_backend_type& p_stack_backend,
            tp_fstack_backend_type& p_fstack_backend,
            tp_rstack_backend_type& p_rstack_backend,
            tp_dict_backend_type& p_dict_backend
        )
            : ui{ p_ui_backend }
            , stack{ p_stack_backend }
            , fstack{ p_fstack_backend }
            , rstack{ p_rstack_backend }
            , dict{ p_dict_backend }
        {
            dict.load_packages(
                up::core_words::CORE_WORDS<environment>,
                up::core_ext_words::CORE_EXT_WORDS<environment>,
                up::up_words::UP_WORDS<environment>
            );
        }

        io::user_interface<tp_ui_backend_type> ui;
        stacks::stack_template_t<tp_stack_backend_type> stack;
        stacks::fstack_template_t<tp_fstack_backend_type> fstack;
        stacks::rstack_template_t<tp_rstack_backend_type> rstack;
        dictionary::dictionary_template<tp_dict_backend_type> dict;
        up::base_types::num_t base = 10;
        up::base_types::num_t dotmode = 0;

    };

}
