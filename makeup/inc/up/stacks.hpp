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


#include <vector>
#include <cstring>
#include <cstddef>
#include <type_traits>
#include <algorithm>

#include "basetypes.hpp"
#include "typelist.hpp"
#include "math.hpp"
#include "memory.hpp"
#include "error.hpp"


namespace up::stacks
{

    struct stack_access_out_of_range final : public error::error
    {
        virtual const char* what() const noexcept
        {
            return "stack access out of range";
        }
    };

    struct stack_underflow : public error::error
    {
        virtual const char* what() const noexcept
        {
            return "stack underflow";
        }
    };

    template <typename tp_storage, typename tp_value_type, typename tp_default_value_type>
    struct stack_template_iterator
    {
        using container_type = tp_storage;
        using size_type = typename container_type::size_type;
        using difference_type = std::ptrdiff_t;
        using value_type = tp_value_type;
        using default_value_type = tp_default_value_type;
        using reference = tp_value_type&;
        using pointer = tp_value_type*;
        using const_reference = const tp_value_type&;
        using const_pointer = const tp_value_type*;

        stack_template_iterator(const container_type& p_container_ref, size_type p_index) noexcept
            : m_container_ref{ p_container_ref }
            , m_index{ p_index }
        { }

        stack_template_iterator& operator++() noexcept
        {
            m_index++;

            return *this;
        }

        stack_template_iterator operator++(int) noexcept
        {
            stack_template_iterator l_tmp{ *this };

            m_index++;

            return l_tmp;
        }

        stack_template_iterator& operator--() noexcept
        {
            m_index--;

            return *this;
        }

        stack_template_iterator operator--(int) noexcept
        {
            stack_template_iterator l_tmp{ *this };

            m_index--;

            return l_tmp;
        }

        stack_template_iterator operator+(difference_type p_diff) const noexcept
        {
            return { m_container_ref, m_index + p_diff };
        }

        stack_template_iterator operator-(difference_type p_diff) const noexcept
        {
            return { m_container_ref, m_index - p_diff };
        }

        bool operator==(const stack_template_iterator& p_other) const noexcept
        {
            return m_index == p_other.m_index;
        }

        bool operator!=(const stack_template_iterator& p_other) const noexcept
        {
            return m_index != p_other.m_index;
        }

        const_reference operator*() const noexcept
        {
            constexpr size_type l_aligned_size = math::ceil(sizeof(value_type), sizeof(default_value_type));
            const size_type l_recalculated_index = m_container_ref.size() - (m_index + 1) * l_aligned_size;

            return *reinterpret_cast<const_pointer>(m_container_ref.access(l_recalculated_index));
        }

      private:

        const container_type& m_container_ref;
        size_type m_index;

    };

    template <
        typename tp_value_types,
        typename tp_storage = memory::dynamic_storage
    >
    class stack_template
    {
        static_assert(type_list::size_v<tp_value_types> > 0,
                      "type list has to contain at least one type");

      public:

        using value_types = tp_value_types;
        using default_value_type = type_list::head_t<value_types>;
        using storage = tp_storage;
        using size_type = typename storage::size_type;

        template <typename tp_value_type>
        using const_reference = const tp_value_type&;

        template <typename tp_value_type>
        using reference = const_reference<tp_value_type>;

        template <typename tp_value_type>
        using const_iterator = stack_template_iterator<storage, tp_value_type, default_value_type>;

        template <typename tp_value_type>
        using iterator = const_iterator<tp_value_type>;

        template <typename tp_value_type>
        using const_reverse_iterator = std::reverse_iterator<const_iterator<tp_value_type>>;

        template <typename tp_value_type>
        using reverse_iterator = const_reverse_iterator<tp_value_type>;


        template <typename tp_value_type>
        static constexpr bool is_valid_value_v = type_list::contains_v<tp_value_types, tp_value_type>;

        template <typename tp_value_type>
        static constexpr size_type value_cell_count_v = math::ceil(sizeof(tp_value_type), sizeof(default_value_type));

        stack_template() = default;

        stack_template(const storage& p_container)
          : m_storage{ p_container }
        { }

        stack_template(storage&& p_container)
          : m_storage{ std::move(p_container) }
        { }

        storage& container() noexcept
        {
            return (m_storage);
        }

        const storage& container() const noexcept
        {
            return (m_storage);
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        void push(tp_value_type p_value)
        {
            insert<tp_value_type>(0, p_value);
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        void drop(size_type p_count = 1)
        {
            if(p_count > size<tp_value_type>())
                throw stack_underflow{};

            erase<tp_value_type>(0, p_count);
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        void erase(size_type p_index, size_type p_count = 1)
        {
            if((p_index + p_count) > size<tp_value_type>())
                throw stack_access_out_of_range{};

            constexpr size_type l_aligned_size = math::ceil(sizeof(tp_value_type), sizeof(default_value_type));
            size_type l_recalculated_index = m_storage.size() - (p_index + p_count) * l_aligned_size;

            m_storage.erase(l_recalculated_index, p_count * l_aligned_size);

        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        void insert(size_type p_index, tp_value_type p_value)
        {
            constexpr size_type l_aligned_size = math::ceil(sizeof(tp_value_type), sizeof(default_value_type));

            const size_type l_recalculated_index = m_storage.size() - p_index * l_aligned_size;

            m_storage.insert(
                l_recalculated_index, reinterpret_cast<typename storage::pointer>(&p_value), l_aligned_size);
        }

        template <
            typename tp_drop_value_type = default_value_type,
            typename tp_set_value_type = tp_drop_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_drop_value_type>>,
            typename = std::enable_if_t<is_valid_value_v<tp_set_value_type>>
        >
        void replace(size_type p_count, tp_set_value_type p_value)
        {
            if(p_count > size<tp_drop_value_type>())
                throw stack_underflow{};

            constexpr size_type l_aligned_set_type_size =
                math::ceil(sizeof(tp_set_value_type), sizeof(default_value_type));

            constexpr size_type l_aligned_drop_type_size =
                math::ceil(sizeof(tp_drop_value_type), sizeof(default_value_type));

            if constexpr(l_aligned_set_type_size > l_aligned_drop_type_size)
                m_storage.resize(m_storage.size() + (l_aligned_set_type_size - l_aligned_drop_type_size));

            set<tp_set_value_type>(p_count - 1, p_value);
            drop<tp_drop_value_type>(p_count - 1);
        }

        template <
            typename tp_drop_value_type = default_value_type,
            typename tp_set_value_type = tp_drop_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_drop_value_type>>,
            typename = std::enable_if_t<is_valid_value_v<tp_set_value_type>>
        >
        void replace(size_type p_count, tp_set_value_type p_value1, tp_set_value_type p_value2)
        {     
            if(p_count > size<tp_drop_value_type>())
                throw stack_underflow{};

            constexpr size_type l_aligned_set_type_size =
                math::ceil(sizeof(tp_set_value_type), sizeof(default_value_type));

            constexpr size_type l_aligned_drop_type_size =
                math::ceil(sizeof(tp_drop_value_type), sizeof(default_value_type));

            if constexpr(2 * l_aligned_set_type_size > l_aligned_drop_type_size)
                m_storage.resize(m_storage.size() + 2 * (l_aligned_set_type_size - l_aligned_drop_type_size));

            set<tp_set_value_type>(p_count - 1, p_value1);
            set<tp_set_value_type>(p_count - 2, p_value2);
            drop<tp_drop_value_type>(p_count - 2);
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        tp_value_type get(size_type p_index) const
        {
            if(p_index >= size<tp_value_type>())
                throw stack_access_out_of_range{};

            constexpr size_type l_unaligned_size = sizeof(tp_value_type);
            constexpr size_type l_aligned_size = math::ceil(sizeof(tp_value_type), sizeof(default_value_type));
            const size_type l_recalculated_index = m_storage.size() - (p_index  + 1) * l_aligned_size;

            tp_value_type l_result;

            m_storage.get(l_recalculated_index, &l_result, l_unaligned_size);

            return l_result;
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        void set(size_type p_index, tp_value_type p_value)
        {
            if(p_index >= size<tp_value_type>())
                throw stack_access_out_of_range{};

            constexpr size_type l_unaligned_size = sizeof(tp_value_type);
            constexpr size_type l_aligned_size = math::ceil(sizeof(tp_value_type), sizeof(default_value_type));
            const size_type l_recalculated_index = m_storage.size() - (p_index  + 1) * l_aligned_size;

            if constexpr (l_unaligned_size != l_aligned_size)
            {
                std::byte l_transformed_value[l_aligned_size]{};

                std::memcpy(l_transformed_value, p_value, l_unaligned_size);
                m_storage.set(l_recalculated_index, l_transformed_value, l_aligned_size);
            }
            else
            {
                m_storage.set(
                    l_recalculated_index, reinterpret_cast<typename storage::pointer>(&p_value), l_aligned_size);
            }
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        size_type size() const
        {
            constexpr size_type l_aligned_size = math::ceil(sizeof(tp_value_type), sizeof(default_value_type));


            return m_storage.size() / l_aligned_size;
        }

        void clear()
        {
            m_storage.erase(0, m_storage.size());
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        const_iterator<tp_value_type> begin() const noexcept
        {
            return { m_storage,  0 };
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        const_iterator<tp_value_type> end() const noexcept
        {
            return { m_storage, size<tp_value_type>() };
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        const_iterator<tp_value_type> cbegin() const noexcept
        {
            return { m_storage, 0 };
        }

        template <
            typename tp_value_type = default_value_type,
            typename = std::enable_if_t<is_valid_value_v<tp_value_type>>
        >
        const_iterator<tp_value_type> cend() const noexcept
        {
            return { m_storage, size<tp_value_type>() };
        }

      private:

        storage m_storage;
    };

    using stack_value_type_list_t = type_list::type_list<
        base_types::num_t,
        base_types::unum_t,
        base_types::dnum_t,
        base_types::udnum_t,
        base_types::char_t,
        base_types::bool_t,
        base_types::num_t*,
        base_types::unum_t*,
        base_types::dnum_t*,
        base_types::udnum_t*,
        base_types::char_t*,
        base_types::address_unit_t*,
        const base_types::num_t*,
        const base_types::unum_t*,
        const base_types::dnum_t*,
        const base_types::udnum_t*,
        const base_types::char_t*,
        const base_types::address_unit_t*
    >;

    using rstack_value_type_list_t = type_list::type_list<base_types::num_t, base_types::unum_t, base_types::char_t>;
    using fstack_value_type_list_t = type_list::type_list<double>;

    template <typename tp_backend_type>
    using stack_template_t = stack_template<stack_value_type_list_t, tp_backend_type>;

    template <typename tp_backend_type>
    using rstack_template_t = stack_template<rstack_value_type_list_t, tp_backend_type>;

    template <typename tp_backend_type>
    using fstack_template_t = stack_template<fstack_value_type_list_t, tp_backend_type>;

}
