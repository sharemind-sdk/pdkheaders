/*
 * Copyright (C) 2015 Cybernetica
 *
 * Research/Commercial License Usage
 * Licensees holding a valid Research License or Commercial License
 * for the Software may use this file according to the written
 * agreement between you and Cybernetica.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl-3.0.html.
 *
 * For further information, please contact us at sharemind@cyber.ee.
 */

#ifndef SHAREMIND_PDKHEADERS_VMVECTOR_H
#define SHAREMIND_PDKHEADERS_VMVECTOR_H

#include <cstddef>
#include <sharemind/AssertReturn.h>
#include <sharemind/module-apis/api_0x1.h>
#include <vector>
#include "ValueTraits.h"

namespace sharemind {

/**
 * Base class to represent VM references and memory.
 * As the data is owned by the VM we don't need to manage it.
 */
template <typename T>
class __attribute__ ((visibility("internal"))) VmVecBase {

public: /* Types: */

    using value_type = T;
    using const_iterator = const value_type*;

public: /* Methods: */

    VmVecBase() noexcept
        : m_begin(nullptr)
        , m_size(0u)
    { }

    VmVecBase(T * const begin, std::size_t const size) noexcept
        : m_begin(assertReturn(begin))
        , m_size(size)
    { }

    std::size_t size() const noexcept { return m_size; }

    value_type const & operator [](std::size_t const i) const noexcept
    { return *(m_begin + i); }

    bool empty() const noexcept { return m_size == 0u; }
    const_iterator begin() const noexcept { return m_begin; }
    const_iterator end() const noexcept { return m_begin + m_size; }

    void const * data() const noexcept
    { return static_cast<void const *>(m_begin); }

protected: /* Fields: */

    T * m_begin;
    std::size_t m_size;

}; /* class VmVecBase { */

/**
 * Mutable vector of VM data, can be constructed from VM references.
 */
template <typename T>
class __attribute__ ((visibility("internal"))) MutableVmVec : VmVecBase<typename T::public_type> {

private: /* Types: */

    using Base = VmVecBase<typename T::public_type>;

public: /* Types: */

    using value_type = typename Base::value_type;
    using const_iterator = typename Base::const_iterator;
    using iterator = value_type*;

public: /* Methods: */

    explicit
    MutableVmVec (const SharemindModuleApi0x1Reference& ref)
        : Base (static_cast<value_type*>(ref.pData), ref.size / sizeof (value_type))
    { }

    /* For debugging only! */
    explicit
    MutableVmVec (std::vector<value_type>& vec)
        : Base ((assert (!vec.empty ()), &vec[0]), vec.size ())
    { }

    void* data () { return static_cast<void*>(this->m_begin); }
    iterator begin () { return this->m_begin; }
    iterator end () { return this->m_begin + this->m_size; }
    value_type& operator [] (size_t i) { return *(this->m_begin + i); }

    static constexpr typename T::value_category value_category () {
        return typename T::value_category ();
    }

    using Base::empty;
    using Base::size;
    using Base::operator [];
    using Base::begin;
    using Base::end;

}; /* class MutableVmVec { */

/**
 * Immutable vector of VM data, can be constructed from VM constant references.
 */
template <typename T>
class __attribute__ ((visibility("internal"))) ImmutableVmVec : VmVecBase<const typename T::public_type> {

private: /* Types: */

    using Base = VmVecBase<const typename T::public_type>;

public: /* Types: */

    using value_type = typename Base::value_type;
    using const_iterator = typename Base::const_iterator;

public: /* Methods: */

    explicit
    ImmutableVmVec (const SharemindModuleApi0x1CReference& cref)
        : Base (static_cast<value_type*>(cref.pData), cref.size / sizeof (value_type))
    { }

    explicit
    ImmutableVmVec (const MutableVmVec<T>& mvec)
        : Base (mvec)
    { }

    static constexpr typename T::value_category value_category () {
        return typename T::value_category ();
    }

    using Base::data;
    using Base::empty;
    using Base::size;
    using Base::operator [];
    using Base::begin;
    using Base::end;

}; /* class ImmutableVmVec { */


} /* namespace sharemind */

#endif /* SHAREMIND_PDKHEADERS_VMVECTOR_H */
