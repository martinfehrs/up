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


#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <limits>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <cstring>
#include <iostream>

#include "math.hpp"
#include "basetypes.hpp"


namespace up::memory
{

    template <typename tp_storage_type>
    struct storage_iterator
    {
        using storage_type = tp_storage_type;
        using difference_type = base_types::num_t;
        using value_type = typename storage_type::value_type;
        using pointer = std::conditional<std::is_const_v<tp_storage_type>, const value_type*, value_type*>;
        using reference = std::conditional<std::is_const_v<tp_storage_type>, const value_type&, value_type&>;

        storage_type* m_storage_ptr = nullptr;
    };


    struct dynamic_storage
    {

    public:

        using value_type = base_types::address_unit_t;
        using size_type = base_types::unum_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using address_type = base_types::unum_t;


        dynamic_storage() = default;

        dynamic_storage(const dynamic_storage& p_other) noexcept
            : dynamic_storage{}
        {
            append(p_other.m_data_ptr, p_other.m_size);
        }

        dynamic_storage(dynamic_storage&& p_other) noexcept
            : dynamic_storage{}
        {
            swap(*this, p_other);
        }

        ~dynamic_storage() noexcept
        {
            empty();
        }

        dynamic_storage& operator=(dynamic_storage p_other) noexcept
        {
            swap(*this, p_other);

            return *this;
        }

        friend void swap(dynamic_storage& p_lhs, dynamic_storage& p_rhs) noexcept
        {
            using std::swap;

            swap(p_lhs.m_data_ptr, p_rhs.m_data_ptr);
            swap(p_lhs.m_size, p_rhs.m_size);
        }

        const void* access(address_type p_addr) const noexcept
        {
            return m_data_ptr + p_addr;
        }

        void set(address_type p_addr, const void* p_data_ptr, size_type p_count) noexcept
        {
            std::memcpy(m_data_ptr + p_addr, p_data_ptr, p_count);
        }

        template <typename tp_value_type, typename tp_transformator_type>
        void set(
            address_type p_addr,
            const tp_value_type* p_data_ptr,
            size_type p_byte_count,
            tp_transformator_type p_transformator
        ) noexcept
        {
            set(p_addr, p_data_ptr, p_byte_count);
            p_transformator(reinterpret_cast<tp_value_type*>(m_data_ptr + p_addr), p_byte_count);
        }

        template <typename tp_value_type, typename tp_transformator>
        void transform(address_type p_addr, size_type p_byte_count, tp_transformator p_transformator)
        {
            p_transformator(reinterpret_cast<tp_value_type*>(m_data_ptr + p_addr), p_byte_count);
        }

        void insert(address_type p_addr, const void* p_data_ptr, size_type p_count)
        {
            resize(size() + p_count);
            set(p_addr + p_count, m_data_ptr + p_addr, size() - p_addr - p_count);
            set(p_addr, p_data_ptr, p_count);
        }

        template <typename tp_value_type, typename tp_transformator_type>
        void insert(
            size_type p_index,
            const tp_value_type* p_data_ptr,
            size_type p_byte_count,
            tp_transformator_type p_transformator
        )
        {
            insert(p_index, p_data_ptr, p_byte_count);
            transform<tp_value_type>(p_index, p_byte_count, p_transformator);
        }

        void append(const void* p_data_ptr, size_type p_count)
        {
            insert(size(), p_data_ptr, p_count);
        }

        template <typename tp_value_type, typename tp_transformator_type>
        void append(
            const tp_value_type* p_data_ptr, size_type p_byte_count, tp_transformator_type p_transformator)
        {
            insert(size(), p_data_ptr, p_byte_count, p_transformator);
        }

        void erase(size_type p_index, size_type p_count)
        {           
            const size_type l_copy_count = size() - p_index - p_count;

            std::memcpy(m_data_ptr + p_index, m_data_ptr + p_index + p_count, l_copy_count);
            resize(size() - p_count);
        }

        void get(size_type p_index, void* p_result_ptr, size_type p_count) const noexcept
        {
            std::memcpy(p_result_ptr, m_data_ptr + p_index, p_count);
        }

        void clear(size_type p_index, size_type p_count) noexcept
        {
            std::memset(m_data_ptr + p_index, 0, p_count);
        }

        void empty() noexcept
        {
            if(m_data_ptr != nullptr)
                std::free(m_data_ptr);
        }

        size_type size() const noexcept
        {
            return m_size;
        }

        void resize(size_type p_new_size)
        {
            if(p_new_size == 0 && m_size > 0)
            {
                free(m_data_ptr);
                m_data_ptr = nullptr;
            }
            else if(m_size == 0)
            {
                pointer l_new_data_ptr = reinterpret_cast<pointer>(std::malloc(p_new_size * sizeof(value_type)));

                if(l_new_data_ptr == nullptr)
                    throw std::bad_alloc{};

                m_data_ptr = l_new_data_ptr;
            }
            else
            {
                pointer l_new_data_ptr = reinterpret_cast<pointer>(std::realloc(m_data_ptr, p_new_size));

                if(l_new_data_ptr == nullptr)
                    throw std::bad_alloc{};

                m_data_ptr = l_new_data_ptr;
            }

            m_size = p_new_size;
        }

        bool operator==(const dynamic_storage& p_other) const noexcept
        {
            return m_size == p_other.m_size && std::memcmp(m_data_ptr, p_other.m_data_ptr, m_size) == 0;
        }

        bool operator!=(const dynamic_storage& p_other) const noexcept
        {
            return m_size != p_other.m_size || std::memcmp(m_data_ptr, p_other.m_data_ptr, m_size) != 0;
        }

      private:

        pointer m_data_ptr = nullptr;
        size_type m_size = 0;

    };

    /*
    struct dynamic_cow_storage
    {

    public:

        using value_type = base_types::address_unit_t;
        using size_type = base_types::unum_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;

        dynamic_cow_storage()
            : m_impl_ptr{ new dynamic_storage }
        {
        }

        dynamic_cow_storage(const dynamic_cow_storage& p_other) noexcept
            : m_impl_ptr{ p_other.m_impl_ptr }
        {
        }

        dynamic_cow_storage& operator=(const dynamic_cow_storage& p_other) noexcept
        {
            m_impl_ptr = p_other.m_impl_ptr;

            return *this;
        }

        void* at(size_type p_index) noexcept
        {
            return m_impl_ptr->at(p_index);
        }

        const void* at(size_type p_index) const noexcept
        {
            return m_impl_ptr->at(p_index);
        }

        void* insert(size_type p_index, size_type p_count)
        {
           return m_impl_ptr->insert(p_index, p_count);
        }

        void* insert(size_type p_index, const void* p_data_ptr, size_type p_count)
        {
            return m_impl_ptr->insert(p_index, p_data_ptr, p_count);
        }

        void* append(size_type p_count)
        {
            return m_impl_ptr->append(p_count);
        }

        void* append(const void* p_data_ptr, size_type p_count)
        {
            return m_impl_ptr->append(p_data_ptr, p_count);
        }

        // Is only guaranteed to work for POD-types copyable types.
        void erase(size_type p_index, size_type p_count)
        {
            m_impl_ptr->erase(p_index, p_count);
        }

        void* get(size_type p_index, void* p_result_ptr, size_type p_count) const noexcept
        {
            return m_impl_ptr->get(p_index, p_result_ptr, p_count);
        }

        void* set(size_type p_index, const void* p_data_ptr, size_type p_count) noexcept
        {
            return m_impl_ptr->set(p_index, p_data_ptr, p_count);
        }

        void clear(size_type p_index, size_type p_count) noexcept
        {
            m_impl_ptr->clear(p_index, p_count);
        }

        void empty() noexcept
        {
            m_impl_ptr->empty();
        }

        size_type size() const noexcept
        {
            return m_impl_ptr->size();
        }

        void* data() const noexcept
        {
            return m_impl_ptr->data();
        }

        // Is only guaranteed to work for POD-types.
        void resize(size_type p_new_size)
        {
            m_impl_ptr->resize(p_new_size);
        }

        bool operator==(const dynamic_cow_storage& p_other) const noexcept
        {
            return m_impl_ptr == p_other.m_impl_ptr || *m_impl_ptr == *p_other.m_impl_ptr;
        }

        bool operator!=(const dynamic_cow_storage& p_other) const noexcept
        {
            return m_impl_ptr != p_other.m_impl_ptr && *m_impl_ptr != *p_other.m_impl_ptr;
        }

      private:

        std::shared_ptr<dynamic_storage> m_impl_ptr = nullptr;

    };
    */

    template <typename tp_value_type>
    constexpr auto align(tp_value_type* p_address, base_types::unum_t p_alignment)
    {
        return reinterpret_cast<tp_value_type*>(math::ceil(reinterpret_cast<base_types::unum_t>(p_address), p_alignment));
    }

}
