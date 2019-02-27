#pragma once


//#include <QObject>

//#include "up/contigiousstorage.hpp"

/*
class publishing_storage : public QObject
{

    Q_OBJECT

  public:

    using underlying_storage = up::contigious_storage::contigious_storage;
    using value_type = typename underlying_storage::value_type;
    using size_type = typename underlying_storage::size_type;
    using reference = typename underlying_storage::reference;
    using pointer = typename underlying_storage::pointer;
    using const_reference = typename underlying_storage::const_reference;
    using const_pointer = typename underlying_storage::const_pointer;

    void get(size_type p_index, pointer p_mem_ptr, size_type p_count) const noexcept
    {
        m_underlying_container.at(p_index, p_mem_ptr, p_count);
    }

    void set(size_type p_index, const_pointer p_mem_ptr, size_type p_count) noexcept
    {
        m_underlying_container.set(p_index, p_mem_ptr, p_count);
        content_changed();
    }

    const_pointer insert(size_type p_index, const_pointer p_mem_ptr, size_type p_count)
    {
        auto l_result_ptr = m_underlying_container.insert(p_index, p_mem_ptr, p_count);
        content_changed();

        return l_result_ptr;
    }

    void erase(size_type p_index, size_type p_count)
    {
        m_underlying_container.erase(p_index, p_count);
        content_changed();
    }

    pointer at(size_type p_index) noexcept
    {
        return m_underlying_container.at(p_index);
    }

    const_pointer at(size_type p_index) const noexcept
    {
        return m_underlying_container.at(p_index);
    }

    size_type size() const noexcept
    {
        return m_underlying_container.size();
    }

    void resize(size_type p_new_size)
    {
        m_underlying_container.resize(p_new_size);
    }

  signals:

    void content_changed();

  private:

    underlying_storage m_underlying_container;

};
*/
