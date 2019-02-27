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
#include "stacks.hpp"
#include "memory.hpp"
#include "stringutils.hpp"
#include "dictionary.hpp"
#include "error.hpp"


namespace up::core_words
{

    using namespace base_types::literals;


    template <typename tp_environment>
    void up_store(tp_environment& p_env)
    {
        *reinterpret_cast<base_types::num_t*>(p_env.stack.get(0)) = p_env.stack.get(1);
        p_env.stack.drop(2);
    }

    template <typename tp_environment>
    void up_star(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) * p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_star_slash(tp_environment& p_env)
    {
        const auto intermediate_result =
            p_env.stack.template get<base_types::dnum_t>(2) * p_env.stack.template get<base_types::dnum_t>(1);

        p_env.stack.replace(3, intermediate_result / p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_star_slash_mod(tp_environment& p_env)
    {
        const auto intermediate_result =
            p_env.stack.template get<base_types::dnum_t>(2) * p_env.stack.template get<base_types::dnum_t>(1);

        p_env.stack.replace(3, intermediate_result % p_env.stack.get(0), intermediate_result / p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_plus(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) + p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_minus(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) - p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_dot(tp_environment& p_env)
    {
        p_env.ui.write_str(string_utils::to_string(p_env.stack.get(0), p_env.base) + ' ');
        p_env.stack.drop();
    }

    template <typename tp_environment>
    void up_slash(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) / p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_slash_mod(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) % p_env.stack.get(0), p_env.stack.get(1) / p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_zero_less(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(0) < 0 ? TRUE : FALSE);
    }

    template <typename tp_environment>
    void up_zero_equals(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(0) == 0 ? TRUE : FALSE);
    }

    template <typename tp_environment>
    void up_one_plus(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(0) + 1);
    }

    template <typename tp_environment>
    void up_one_minus(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(0) - 1);
    }

    template <typename tp_environment>
    void up_two_star(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(0) << 1);
    }

    template <typename tp_environment>
    void up_two_slash(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(0) >> 1);
    }

    template <typename tp_environment>
    void up_less_than(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) < p_env.stack.get(0) ? TRUE : FALSE);
    }

    template <typename tp_environment>
    void up_equals(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) == p_env.stack.get(0) ? TRUE : FALSE);
    }

    template <typename tp_environment>
    void up_greater_than(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) > p_env.stack.get(0) ? TRUE : FALSE);
    }

    template <typename tp_environment>
    void up_abs(tp_environment& p_env)
    {
        p_env.stack.set(0, std::abs(p_env.stack.get(0)));
    }

    template <typename tp_environment>
    void up_drop(tp_environment& p_env)
    {
        p_env.stack.drop();
    }

    template <typename tp_environment>
    void up_dupe(tp_environment& p_env)
    {
        p_env.stack.push(p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_mod(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) % p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_c_r(tp_environment& p_env)
    {
        p_env.ui.write_chr('\n');
    }

    template <typename tp_environment>
    void up_space(tp_environment& p_env)
    {
        p_env.ui.write_chr(' ');
    }

    template <typename tp_environment>
    void up_spaces(tp_environment& p_env)
    {
        p_env.ui.write_chr(' ', p_env.stack.get(0));
        p_env.stack.drop();
    }

    template <typename tp_environment>
    void up_depth(tp_environment& p_env)
    {
        p_env.stack.push(static_cast<base_types::num_t>(p_env.stack.size()));
    }

    template <typename tp_environment>
    void up_and(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) & p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_or(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) | p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_xor(tp_environment& p_env)
    {
        p_env.stack.replace(2, p_env.stack.get(1) ^ p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_invert(tp_environment& p_env)
    {
        p_env.stack.set(0, ~p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_min(tp_environment& p_env)
    {
        p_env.stack.replace(2, std::min(p_env.stack.get(1), p_env.stack.get(0)));
    }

    template <typename tp_environment>
    void up_max(tp_environment& p_env)
    {
        p_env.stack.replace(2, std::max(p_env.stack.get(1), p_env.stack.get(0)));
    }

    template <typename tp_environment>
    void up_swap(tp_environment& p_env)
    {
        base_types::num_t l_tmp = p_env.stack.get(0);
        p_env.stack.set(0, p_env.stack.get(1));
        p_env.stack.set(1, l_tmp);
    }

    template <typename tp_environment>
    void up_over(tp_environment& p_env)
    {
        p_env.stack.push(p_env.stack.get(1));
    }

    template <typename tp_environment>
    void up_rot(tp_environment& p_env)
    {
        const base_types::num_t l_tmp = p_env.stack.get(2);

        p_env.stack.set(2, p_env.stack.get(1));
        p_env.stack.set(1, p_env.stack.get(0));
        p_env.stack.set(0, l_tmp);
    }

    template <typename tp_environment>
    void up_negate(tp_environment& p_env)
    {
        p_env.stack.set(0, -p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_variable(tp_environment& p_env)
    {
        std::string_view l_word = p_env.ui.read_word();

        if(l_word.empty())
            throw error::missing_name{ "VARIABLE" };

        p_env.dict.create(l_word, dictionary::runtime_type::VARIABLE, base_types::CELL_SIZE);
    }

    template <typename tp_environment>
    void up_constant(tp_environment& p_env)
    {
        base_types::num_t l_value = p_env.stack.get(0);
        std::string_view l_word = p_env.ui.read_word();

        if(l_word.empty())
            throw error::missing_name{ "CONSTANT" };

        p_env.dict.create(
            p_env.ui.read_word(),
            dictionary::runtime_type::CONSTANT,
            &l_value,
            sizeof(base_types::num_t)
        );

        p_env.stack.drop();
    }

    template <typename tp_environment>
    void up_fetch(tp_environment& p_env)
    {
        p_env.stack.replace(1, *reinterpret_cast<base_types::num_t*>(p_env.stack.get(0)));
    }

    template <typename tp_environment>
    void up_question(tp_environment& p_env)
    {
        p_env.terminal.write_str(
            to_string(*reinterpret_cast<base_types::num_t*>(p_env.stack.get(0)), p_env.base) + ' ');

        p_env.stack.drop();
    }

    template <typename tp_environment>
    void up_s_to_d(tp_environment& p_env)
    {
        p_env.stack.template replace<base_types::num_t, base_types::dnum_t>(1, p_env.stack.get(0));
    }

    template <typename tp_environment>
    void up_to_r(tp_environment& p_env)
    {
        p_env.rstack.push(p_env.stack.get(0));
        p_env.stack.drop();
    }

    template <typename tp_environment>
    void up_r_from(tp_environment& p_env)
    {
        p_env.stack.push(p_env.rstack.get(0));
        p_env.rstack.drop();
    }

    template <typename tp_environment>
    void up_r_fetch(tp_environment& p_env)
    {
        p_env.stack.push(p_env.rstack.get(0));
    }

    template <typename tp_environment>
    void up_two_drop(tp_environment& p_env)
    {
        p_env.stack.template drop<base_types::dnum_t>();
    }

    template <typename tp_environment>
    void up_two_dup(tp_environment& p_env)
    {
        p_env.stack.template push<base_types::dnum_t>(p_env.stack.template get<base_types::dnum_t>(0));
    }

    template <typename tp_environment>
    void up_two_over(tp_environment& p_env)
    {
        p_env.stack.template push<base_types::dnum_t>(p_env.stack.template get<base_types::dnum_t>(1));
    }

    template <typename tp_environment>
    void up_two_swap(tp_environment& p_env)
    {
        base_types::dnum_t l_tmp = p_env.stack.template get<base_types::dnum_t>(0);

        p_env.stack.template set<base_types::dnum_t>(0, p_env.stack.template get<base_types::dnum_t>(1));
        p_env.stack.template set<base_types::dnum_t>(1, l_tmp);
    }

    template <typename tp_environment>
    void up_b_l(tp_environment& p_env)
    {
        p_env.stack.template push<base_types::char_t>(' ');
    }

    template <typename tp_environment>
    void up_emit(tp_environment& p_env)
    {
        p_env.ui.write_chr(p_env.stack.get(0));
        p_env.stack.drop();
    }

    template <typename tp_environment>
    void up_create(tp_environment& p_env)
    {
        p_env.dict.create(p_env.ui.read_word(), dictionary::runtime_type::ADDRESS);
    }

    template <typename tp_environment>
    void up_allot(tp_environment& p_env)
    {
        p_env.dict.allot(p_env.stack.get(0));
        p_env.stack.drop();
    }

    template <typename tp_environment>
    void up_here(tp_environment& p_env)
    {
        p_env.stack.push(p_env.dict.here());
    }

    template <typename tp_environment>
    void up_cells(tp_environment& p_env)
    {
        p_env.stack.template replace<base_types::unum_t>(
            1, p_env.stack.template get<base_types::unum_t>(0) * base_types::CELL_SIZE);
    }

    template <typename tp_environment>
    void up_chars(tp_environment& p_env)
    {
        p_env.stack.template replace<base_types::unum_t>(
            1, p_env.stack.template get<base_types::unum_t>(0) * base_types::CHAR_SIZE);
    }

    template <typename tp_environment>
    void up_cell_plus(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.template get<base_types::char_t*>(0) + base_types::CELL_SIZE);
    }

    template <typename tp_environment>
    void up_char_plus(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.template get<base_types::char_t*>(0) + base_types::CHAR_SIZE);
    }

    template <typename tp_environment>
    void up_aligned(tp_environment& p_env)
    {
        p_env.stack.replace(1, memory::align(p_env.stack.template get<base_types::char_t*>(0), base_types::CELL_SIZE));
    }

    template <typename tp_environment>
    void up_align(tp_environment& p_env)
    {
        p_env.dict.allot(math::ceil(p_env.dict.here(), base_types::CELL_SIZE) - p_env.dict.here());
    }


    template <typename environment_t>
    constexpr package::package_entry<environment_t> CORE_WORDS[] = {
        {           "!",  package::FUNC,                up_store },
    //  {           "#",  package::FUNC,          up_number_sign },
    //  {          "#>",  package::FUNC,  up_number_sign_greater },
    //  {          "#>",  package::FUNC,        up_number_sign_s },
    //  {           "'",  package::FUNC,                 up_tick },
    //  {           "(",  package::FUNC,                up_paren },
        {           "*",  package::FUNC,                 up_star },
        {          "*/",  package::FUNC,           up_star_slash },
        {       "*/MOD",  package::FUNC,       up_star_slash_mod },
        {           "+",  package::FUNC,                 up_plus },
    //  {          "+!",  package::FUNC,           up_plus_store },
    //  {       "+LOOP",  package::FUNC,            up_plus_loop },
    //  {           ",",  package::FUNC,                up_comma },
        {           "-",  package::FUNC,                up_minus },
        {           ".",  package::FUNC,                  up_dot },
    //  {         ".\"",  package::FUNC,               dot_quote },
        {           "/",  package::FUNC,                up_slash },
        {        "/MOD",  package::FUNC,            up_slash_mod },
        {          "0<",  package::FUNC,            up_zero_less },
        {          "0=",  package::FUNC,          up_zero_equals },
        {          "1+",  package::FUNC,             up_one_plus },
        {          "1-",  package::FUNC,            up_one_minus },
    //  {          "2!",  package::FUNC,            up_two_store },
        {          "2*",  package::FUNC,             up_two_star },
        {          "2/",  package::FUNC,            up_two_slash },
    //  {          "2@",  package::FUNC,            up_two_fetch },
        {       "2DROP",  package::FUNC,             up_two_drop },
        {        "2DUP",  package::FUNC,              up_two_dup },
        {       "2OVER",  package::FUNC,             up_two_over },
        {       "2SWAP",  package::FUNC,             up_two_swap },
    //  {           ":",  package::FUNC,                up_colon },
    //  {           ";",  package::FUNC,            up_semicolon },
        {           "<",  package::FUNC,            up_less_than },
    //  {          "<#",  package::FUNC,     up_less_number_sign },
        {           "=",  package::FUNC,               up_equals },
        {           ">",  package::FUNC,         up_greater_than },
    //  {       ">BODY",  package::FUNC,              up_to_body },
    //  {         ">IN",  package::FUNC,                up_to_in },
    //  {      ">NUMBER", package::FUNC,            up_to_number },
        {           ">R", package::FUNC,                 up_to_r },
    //  {         "?DUP", package::FUNC,        up_question_dupe },
        {            "@", package::FUNC,                up_fetch },
    //  {        "ABORT", package::FUNC,                up_abort },
    //  {      "ABORT\"", package::FUNC,          up_abort_quote },
        {          "ABS", package::FUNC,                  up_abs },
    //  {       "ACCEPT", package::FUNC,               up_accept },
        {        "ALIGN", package::FUNC,                up_align },
        {      "ALIGNED", package::FUNC,              up_aligned },
        {        "ALLOT", package::FUNC,                up_allot },
        {          "AND", package::FUNC,                  up_and },
    //  {         "BASE", package::FUNC,                 up_base },
    //  {        "BEGIN", package::FUNC,                up_begin },
        {           "BL", package::FUNC,                  up_b_l },
    //  {           "C!", package::FUNC,              up_c_store },
    //  {          "C\"", package::FUNC,              up_c_quote },
    //  {           "C,", package::FUNC,              up_c_comma },
    //  {           "C@", package::FUNC,              up_c_fetch },
        {        "CELL+", package::FUNC,            up_cell_plus },
        {        "CELLS", package::FUNC,                up_cells },
    //  {         "CHAR", package::FUNC,                 up_char },
        {        "CHAR+", package::FUNC,            up_char_plus },
        {        "CHARS", package::FUNC,                up_chars },
        {     "CONSTANT", package::FUNC,             up_constant },
    //  {        "COUNT", package::FUNC,                up_count },
        {           "CR", package::FUNC,                  up_c_r },
        {       "CREATE", package::FUNC,               up_create },
    //  {      "DECIMAL", package::FUNC,              up_decimal },
        {        "DEPTH", package::FUNC,                up_depth },
    //  {           "DO", package::FUNC,                   up_do },
    //  {        "DOES>", package::FUNC,                 up_does },
        {         "DROP", package::FUNC,                 up_drop },
        {          "DUP", package::FUNC,                 up_dupe },
    //  {         "ELSE", package::FUNC,                 up_else },
        {         "EMIT", package::FUNC,                 up_emit },
    //  { "ENVIRONMENT?", package::FUNC,    up_environment_query },
    //  {     "EVALUATE", package::FUNC,             up_evaluate },
    //  {      "EXECUTE", package::FUNC,              up_execute },
    //  {         "EXIT", package::FUNC,                 up_exit },
    //  {         "FILL", package::FUNC,                 up_fill },
    //  {         "FIND", package::FUNC,                 up_find },
    //  {       "FM/MOD", package::FUNC,        up_f_m_slash_mod },
        {         "HERE", package::FUNC,                 up_here },
    //  {         "HOLD", package::FUNC,                 up_hold },
    //  {            "I", package::FUNC,                    up_i },
    //  {           "IF", package::FUNC,                   up_if },
    //  {    "IMMEDIATE", package::FUNC,            up_immediate },
        {       "INVERT", package::FUNC,               up_invert },
    //  {            "J", package::FUNC,                    up_j },
    //  {          "KEY", package::FUNC,                  up_key },
    //  {        "LEAVE", package::FUNC,                up_leave },
    //  {      "LITERAL", package::FUNC,              up_literal },
    //  {         "LOOP", package::FUNC,                 up_loop },
    //  {       "LSHIFT", package::FUNC,              up_l_shift },
    //  {           "M*", package::FUNC,               up_m_star },
        {          "MAX", package::FUNC,                  up_max },
        {          "MIN", package::FUNC,                  up_min },
        {          "MOD", package::FUNC,                 up_mod  },
    //  {         "MOVE", package::FUNC,                 up_move },
        {       "NEGATE", package::FUNC,               up_negate },
        {           "OR", package::FUNC,                   up_or },
        {         "OVER", package::FUNC,                 up_over },
    //  {     "POSTPONE", package::FUNC,             up_postpone },
    //  {         "QUIT", package::FUNC,                 up_quit },
        {           "R>", package::FUNC,               up_r_from },
        {           "R@", package::FUNC,              up_r_fetch },
    //  {      "RECURSE", package::FUNC,              up_recurse },
    //  {       "REPEAT", package::FUNC,               up_repeat },
        {          "ROT", package::FUNC,                  up_rot },
    //  {       "RSHIFT", package::FUNC,              up_r_shift },
    //  {          "S\"", package::FUNC,              up_s_quote },
        {          "S>D", package::FUNC,               up_s_to_d },
    //  {         "SIGN", package::FUNC,                 up_sign },
    //  {       "SM/REM", package::FUNC,        up_s_m_slash_rem },
        {        "SPACE", package::FUNC,                up_space },
        {       "SPACES", package::FUNC,               up_spaces },
    //  {        "STATE", package::FUNC,                up_state },
        {         "SWAP", package::FUNC,                 up_swap },
    //  {         "THEN", package::FUNC,                 up_then },
    //  {         "TYPE", package::FUNC,                 up_type },
    //  {           "U.", package::FUNC,                up_u_dot },
    //  {           "U<", package::FUNC,          up_u_less_than },
    //  {          "UM*", package::FUNC,             up_u_m_star },
    //  {       "UM/MOD", package::FUNC,        up_u_m_slash_mod },
    //  {       "UNLOOP", package::FUNC,               up_unloop },
    //  {        "UNTIL", package::FUNC,                up_until },
        {     "VARIABLE", package::FUNC,             up_variable },
    //  {        "WHILE", package::FUNC,                up_while },
    //  {         "WORD", package::FUNC,                 up_word },
        {          "XOR", package::FUNC,                  up_xor },
    //  {            "[", package::FUNC,         up_left_bracket },
    //  {          "[']", package::FUNC,         up_bracket_tick },
    //  {       "[CHAR]", package::FUNC,         up_bracket_char },
    //  {            "]", package::FUNC,        up_right_bracket },
    };

}
