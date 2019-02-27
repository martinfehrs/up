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
#include "error.hpp"


namespace up::error
{

    error::~error()  = default;


    const char* error_with_message::what() const noexcept
    {
        return m_msg.c_str();
    }


    unknown_word::unknown_word(const std::string& p_word)
        : error_with_message{ "unkown word ", p_word }
    { }


    missing_name::missing_name(const std::string& p_word)
        : error_with_message{ "missing trailing word name for defining word ", p_word }
    { }

}
