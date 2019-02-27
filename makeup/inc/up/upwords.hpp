#pragma once


#include "package.hpp"


namespace up::up_words
{

    template <typename tp_environment>
    void up_two_plus(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(0) + 2);
    }

    template <typename tp_environment>
    void up_two_minus(tp_environment& p_env)
    {
        p_env.stack.set(0, p_env.stack.get(0) - 2);
    }

    template <typename tp_environment>
    void up_clearstack(tp_environment& p_env)
    {
        p_env.stack.clear();
    }

    template <typename tp_environment>
    void up_clearfstack(tp_environment& p_env)
    {
        p_env.fstack.clear();
    }

    template <typename tp_environment>
    constexpr package::package_entry<tp_environment> UP_WORDS[] = {
        { "2+",          package::FUNC,  up_two_plus    },
        { "2-",          package::FUNC,  up_two_minus   },
        { "CLEARSTACK",  package::FUNC,  up_clearstack  },
        { "CLEARFSTACK", package::FUNC,  up_clearfstack },
    };

}
