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


#include <array>
#include <string_view>
#include <variant>

#include "basetypes.hpp"


namespace up::package
{

    constexpr struct func_tag     {} FUNC;
    constexpr struct const_tag    {} CONST;
    constexpr struct uconst_tag   {} UCONST;
    constexpr struct var_tag      {} VAR;
    constexpr struct uvar_tag     {} UVAR;
    constexpr struct val_tag      {} VAL;
    constexpr struct uval_tag     {} UVAL;
    constexpr struct var_ref_tag  {} VAR_REF;
    constexpr struct uvar_ref_tag {} UVAR_REF;
    constexpr struct val_ref_tag  {} VAL_REF;
    constexpr struct uval_ref_tag {} UVAL_REF;


    template <typename tp_environment_type>
    class package_entry
    {

      public:

        enum class runtime_type
        {
            function,
            constant,
            variable,
            value,
            variable_reference,
            unsigned_variable_reference,
            value_reference,
            unsigned_value_reference
        };

        constexpr package_entry(
            std::string_view p_key, func_tag, base_types::func_t<tp_environment_type>* p_func_ptr) noexcept
            : package_entry{ p_key, runtime_type::function, p_func_ptr }
        {
        }

        constexpr package_entry(std::string_view p_key, const_tag, base_types::num_t p_num) noexcept
            : package_entry{ p_key, runtime_type::constant, p_num }
        {
        }

        constexpr package_entry(std::string_view p_key, uconst_tag, base_types::unum_t p_unum) noexcept
            : package_entry{ p_key, runtime_type::constant, p_unum }
        {
        }

        constexpr package_entry(std::string_view p_key, var_tag, base_types::num_t p_num) noexcept
            : package_entry{ p_key, runtime_type::variable, p_num }
        {
        }

        constexpr package_entry(std::string_view p_key, uvar_tag, base_types::unum_t p_unum) noexcept
            : package_entry{ p_key, runtime_type::variable, p_unum }
        {
        }

        constexpr package_entry(std::string_view p_key, val_tag, base_types::num_t p_num) noexcept
            : package_entry{ p_key, runtime_type::value, p_num }
        {
        }

        constexpr package_entry(std::string_view p_key, uval_tag, base_types::unum_t p_unum) noexcept
            : package_entry{ p_key, runtime_type::value, p_unum }
        {
        }

        constexpr package_entry(std::string_view p_key, var_ref_tag, base_types::num_t& p_num) noexcept
            : package_entry{ p_key, runtime_type::variable_reference, p_num }
        {
        }

        constexpr package_entry(std::string_view p_key, uvar_ref_tag, base_types::unum_t& p_unum) noexcept
            : package_entry{ p_key, runtime_type::unsigned_variable_reference, p_unum }
        {
        }

        constexpr package_entry(std::string_view p_key, uval_ref_tag, base_types::num_t& p_num_ref) noexcept
            : package_entry{ p_key, runtime_type::value_reference, p_num_ref }
        {
        }

        constexpr package_entry(std::string_view p_key, uval_ref_tag, base_types::unum_t& p_unum_ref) noexcept
            : package_entry{ p_key, runtime_type::unsigned_value_reference, &p_unum_ref }
        {
        }

        constexpr std::string_view key() const noexcept
        {
            return m_key;
        }

        constexpr runtime_type type() const noexcept
        {
            return m_type;
        }

        constexpr auto get(func_tag) const noexcept
        {
            return std::get<base_types::func_t<tp_environment_type>*>(m_value);
        }

        constexpr auto get(const_tag) const noexcept
        {
            return std::get<base_types::num_t>(m_value);
        }

        constexpr auto get(uconst_tag) const noexcept
        {
            return std::get<base_types::num_t>(m_value);
        }

        constexpr auto get(var_tag) const noexcept
        {
            return std::get<base_types::num_t>(m_value);
        }

        constexpr auto get(uvar_tag) const noexcept
        {
            return std::get<base_types::num_t>(m_value);
        }

        constexpr auto get(val_tag) const noexcept
        {
            return std::get<base_types::num_t>(m_value);
        }

        constexpr auto get(uval_tag) const noexcept
        {
            return std::get<base_types::num_t>(m_value);
        }

      private:

        template <typename tp_value_type>
        constexpr package_entry(std::string_view p_key, runtime_type p_type, tp_value_type p_value) noexcept
            : m_key{ p_key }
            , m_type{ p_type }
            , m_value{ p_value }
        {
        }

        base_types::string_view_t m_key;
        runtime_type m_type;

        std::variant<
            base_types::func_t<tp_environment_type>*,
            base_types::num_t,
            base_types::unum_t,
            base_types::num_t*,
            base_types::unum_t*
        > m_value;

    };

}
