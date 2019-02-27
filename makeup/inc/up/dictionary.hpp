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


#include <cstdint>
#include <cstring>
#include <string>
#include <optional>

#include "basetypes.hpp"
#include "package.hpp"
#include "stringutils.hpp"
#include "memory.hpp"


namespace up::dictionary
{

    template <typename tp_storage>
    class dictionary_template;


    enum class runtime_type
    {
        FUNCTION,
        NATIVE_FUNCTION,
        VARIABLE,
        NATIVE_VARIABLE,
        CONSTANT,
        VALUE,
        NATIVE_VALUE,
        ADDRESS,
        TWO_VARIABLE,
        NATIVE_TWO_VARIABLE,
        TWO_CONSTANT,
        TWO_VALUE,
        NATIVE_TWO_VALUE
    };

    template <typename tp_storage_type>
    class entry_template
    {

      public:

        using storage_type = tp_storage_type;
        using address_type = typename storage_type::address_type;

        constexpr entry_template(
            runtime_type p_type,
            base_types::unum_t p_word_size,
            std::optional<address_type> p_next_entry_addr,
            address_type p_data_addr
        ) noexcept
            : m_type{ p_type }
            , m_word_size{ p_word_size }
            , m_next_entry_addr{ p_next_entry_addr }
            , m_data_addr{ p_data_addr }
        {
        }

        entry_template(const entry_template&) = delete;
        entry_template& operator=(const entry_template&) = delete;

        runtime_type type() const noexcept
        {
            return m_type;
        }

        constexpr std::string_view word() const noexcept
        {
            return { reinterpret_cast<const base_types::char_t*>(this + 1), m_word_size };
        }

        constexpr address_type data() const noexcept
        {
            return m_data_addr;
        }

        constexpr std::optional<address_type> next() const noexcept
        {
            return m_next_entry_addr;
        }

      private:

        runtime_type m_type;
        base_types::unum_t m_word_size;
        std::optional<address_type> m_next_entry_addr;
        address_type m_data_addr;

    };

    template <typename tp_storage_type>
    class iterator_template
    {

      public:

        using storage_type = tp_storage_type;
        using address_type = typename storage_type::address_type;
        using dictionary_type = dictionary_template<storage_type>;
        using entry_type = entry_template<storage_type>;

        iterator_template(
            dictionary_type* p_dictionary_ptr, std::optional<address_type> p_entry_addr) noexcept
            : m_dictionary_ptr{ p_dictionary_ptr }
            , m_entry_addr{ p_entry_addr }
        {
        }

        explicit iterator_template(dictionary_type* p_dictionary_ptr) noexcept
            : m_dictionary_ptr{ p_dictionary_ptr }
        {
        }

        const entry_type& operator*() const noexcept
        {
            assert(m_dictionary_ptr);
            assert(m_entry_addr);

            return *m_dictionary_ptr->template access_data<entry_type>(*m_entry_addr);
        }

        const entry_type* operator->() const noexcept
        {
            assert(m_dictionary_ptr);
            assert(m_entry_addr);

            return m_dictionary_ptr->template access_data<entry_type>(*m_entry_addr);
        }

        bool operator==(iterator_template p_other) const noexcept
        {
            assert(m_dictionary_ptr == p_other.m_dictionary_ptr);

            return m_entry_addr = p_other.m_entry_addr;
        }

        bool operator!=(iterator_template p_other) const noexcept
        {
            assert(m_dictionary_ptr == p_other.m_dictionary_ptr);

            return m_entry_addr != p_other.m_entry_addr;
        }

        iterator_template& operator++() noexcept
        {
            assert(m_dictionary_ptr);
            assert(m_entry_addr);

            m_entry_addr = m_dictionary_ptr->template access_data<entry_type>(*m_entry_addr)->next();

            return *this;
        }

        iterator_template operator++(int) noexcept
        {
            assert(m_dictionary_ptr);
            assert(m_entry_addr);

            iterator_template l_tmp{ *this };

            m_entry_addr = this->next();

            return l_tmp;
        }

      private:

        dictionary_type* m_dictionary_ptr = nullptr;
        std::optional<address_type> m_entry_addr;

    };


    template <typename tp_storage_type>
    class dictionary_template
    {

      public:

        using storage_type = tp_storage_type;
        using pointer = typename storage_type::pointer;
        using const_pointer = typename storage_type::const_pointer;
        using address_type = typename storage_type::address_type;
        using size_type = typename storage_type::size_type;
        using entry_type = entry_template<storage_type>;
        using iterator = iterator_template<storage_type>;

        explicit dictionary_template(storage_type& p_storage)
            : m_storage{ p_storage }
        {
        }

        address_type create(std::string_view p_word, runtime_type p_type, base_types::unum_t p_count = 0)
        {
            const address_type l_new_first_entry_addr = m_storage.size();
            const address_type l_data_addr = l_new_first_entry_addr + sizeof(entry_type) + p_word.size();
            const entry_type l_dict_entry{ p_type, p_word.size(), m_first_entry_addr, l_data_addr };

            m_storage.append(&l_dict_entry, sizeof(l_dict_entry));

            m_storage.append(p_word.data(), p_word.size(), [] (auto p_data_ptr, auto p_count) {
                return string_utils::to_uppercase(p_data_ptr, p_count);
            });

            m_storage.resize(m_storage.size() + p_count);

            m_first_entry_addr = l_new_first_entry_addr;
            m_size++;

            return l_new_first_entry_addr;
        }

        address_type create(
            std::string_view p_word, runtime_type p_type, const void* p_data_ptr, base_types::unum_t p_count)
        {
            const address_type l_entry_addr = create(p_word, p_type, p_count);

            set_data(access_data<entry_type>(l_entry_addr)->data(), p_data_ptr, p_count);

            return l_entry_addr;
        }

        template <typename tp_data>
        address_type create(std::string_view p_word, runtime_type p_type, const tp_data& p_data)
        {
            return create(p_word, p_type, &p_data, sizeof(tp_data));
        }

        template <typename tp_environment>
        void load_package(const package::package_entry<tp_environment>*& p_package_ptr)
        {
            while(!p_package_ptr->key().empty())
            {
                load_package_entry(*p_package_ptr);
                p_package_ptr++;
            }
        }

        void allot(base_types::unum_t p_bytes)
        {
            m_storage.resize(m_storage.size() + p_bytes);
        }

        address_type here()
        {
            return m_storage.size();
        }

        template <typename tp_environment, size_t N>
        void load_package(const package::package_entry<tp_environment>(&p_package)[N])
        {
            for(const auto& l_package_entry : p_package)
                load_package_entry(l_package_entry);
        }

        template <typename package_t>
        void load_packages(const package_t& p_package)
        {
            load_package(p_package);
        }

        template <typename package_t, typename... package_ts>
        void load_packages(const package_t& p_package, const package_ts&... p_packages)
        {
            load_packages(p_package);
            load_packages(p_packages...);
        }

        std::optional<address_type> find_entry(std::string p_word)
        {
            string_utils::to_uppercase(std::data(p_word), std::size(p_word));

            std::optional<address_type> l_entry_addr = m_first_entry_addr;

            while(l_entry_addr)
            {
                auto l_entry_ptr = access_data<entry_type>(*l_entry_addr);

                if(l_entry_ptr->word() == p_word)
                    return l_entry_addr;

                l_entry_addr = l_entry_ptr->next();
            }

            return {};
        }

        template <typename tp_value_type = void>
        const tp_value_type* access_data(address_type p_addr) const noexcept
        {
            return reinterpret_cast<const tp_value_type*>(m_storage.access(p_addr));
        }

        void set_data(address_type p_addr, const void* p_data_ptr, size_type p_byte_count)
        {
            m_storage.set(p_addr, p_data_ptr, p_byte_count);
        }

        size_type size() const noexcept
        {
            return m_size;
        }

        iterator begin() noexcept
        {
            return iterator{ this, m_first_entry_addr };
        }

        iterator end() noexcept
        {
            return iterator{ this };
        }

      private:

        template <typename tp_environment>
        void load_package_entry(const package::package_entry<tp_environment>& p_package_entry)
        {
            switch(p_package_entry.type())
            {
                case package::package_entry<tp_environment>::runtime_type::function:
                    create(p_package_entry.key(), runtime_type::NATIVE_FUNCTION, p_package_entry.get(package::FUNC));
                    break;
                case package::package_entry<tp_environment>::runtime_type::constant:
                    create(p_package_entry.key(), runtime_type::CONSTANT, p_package_entry.get(package::CONST));
                    break;
                case package::package_entry<tp_environment>::runtime_type::variable:
                    create(p_package_entry.key(), runtime_type::VARIABLE, p_package_entry.get(package::VAR));
                    break;
                case package::package_entry<tp_environment>::runtime_type::value:
                    create(p_package_entry.key(), runtime_type::VALUE, p_package_entry.get(package::VAL));
                    break;
                case package::package_entry<tp_environment>::runtime_type::variable_reference:
                    break;
                case package::package_entry<tp_environment>::runtime_type::unsigned_variable_reference:
                    break;
                case package::package_entry<tp_environment>::runtime_type::value_reference:
                    break;
                case package::package_entry<tp_environment>::runtime_type::unsigned_value_reference:
                    break;
            };
        }

        storage_type& m_storage;
        std::optional<address_type> m_first_entry_addr;
        size_type m_size = 0;
    };

    using dict_t = dictionary_template<memory::dynamic_storage>;
}
