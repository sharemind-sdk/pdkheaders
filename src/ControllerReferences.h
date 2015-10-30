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

#ifndef SHAREMIND_PDKHEADERS_CONTROLLERREFERENCES_H
#define SHAREMIND_PDKHEADERS_CONTROLLERREFERENCES_H

#include <cstddef>
#include <sharemind/libcmodapi/api_0x1.h>
#include <sharemind/Random/IRandom.h>

#include "BitVector.h"
#include "ValueTraits.h"


namespace sharemind {

/**
 * Base class to represent controller references.
 * As the data is owned by the controller we don't need to manage it.
 */
template <typename T>
class __attribute__ ((visibility("internal"))) CtrlVecBase {

public: /* Types: */

    using value_type = T;
    using const_iterator = const value_type*;

public: /* Methods: */

    CtrlVecBase ()
        : m_begin (0)
        , m_end (0)
    { }

    CtrlVecBase (T* begin, size_t size)
        : m_begin((assert(begin), begin))
        , m_end (begin + size)
    { }

    inline size_t size () const { return (m_end - m_begin); }
    inline const value_type& operator [] (size_t i) const { return *(m_begin + i); }
    inline bool empty () const { return m_begin >= m_end; }
    inline const_iterator begin () const { return m_begin; }
    inline const_iterator end () const { return m_end; }

protected: /* Fields: */

    T*  m_begin;
    T*  m_end;

}; /* class CtrlVecBase { */

/**
 * Mutable vector of controller data, can be constructed from controller references.
 */
template <typename T>
class __attribute__ ((visibility("internal"))) MutableCtrlVec : CtrlVecBase<T> {

private: /* Types: */

    using Base = CtrlVecBase<T>;

public: /* Types: */

    using value_type = typename Base::value_type;
    using const_iterator = typename Base::const_iterator;
    using iterator = value_type*;

public: /* Methods: */

    explicit
    MutableCtrlVec (const SharemindControllerModuleApi0x1Reference& ref)
        : Base (static_cast<value_type*>(ref.pData), ref.size / sizeof (value_type))
    { }

    /* For debugging only! */
    explicit
    MutableCtrlVec (std::vector<value_type>& vec)
        : Base ((assert (!vec.empty ()), &vec[0]), vec.size ())
    { }

    iterator begin () { return this->m_begin; }
    iterator end () { return this->m_end; }
    value_type& operator [] (size_t i) { return *(this->m_begin + i); }

    void randomize(IRandom & rng) {
        if (! empty ())
            rng.fillBlock (begin(), end());
    }

    using Base::empty;
    using Base::size;
    using Base::operator [];
    using Base::begin;
    using Base::end;

}; /* class MutableCtrlVec { */

template <>
void MutableCtrlVec<bool>::randomize (IRandom & rng) {
    if (! empty ()) {
        BitVec<> bits (end() - begin());
        bits.randomize(rng);
        size_t i = 0;
        for (iterator it = begin (); it != end (); ++it, ++i)
            *it = bits[i];
    }
}

/**
 * Immutable vector of controller data, can be constructed from controller constant references.
 */
template <typename T>
class __attribute__ ((visibility("internal"))) ImmutableCtrlVec : CtrlVecBase<const T> {

private: /* Types: */

    using Base = CtrlVecBase<const T>;

public: /* Types: */

    using value_type = typename Base::value_type;
    using const_iterator = typename Base::const_iterator;

public: /* Methods: */

    explicit
    ImmutableCtrlVec (const SharemindControllerModuleApi0x1CReference& cref)
        : Base (static_cast<value_type*>(cref.pData), cref.size / sizeof (value_type))
    { }

    explicit
    ImmutableCtrlVec (const MutableCtrlVec<T>& immvec)
        : Base (immvec)
    { }

    using Base::empty;
    using Base::size;
    using Base::operator [];
    using Base::begin;
    using Base::end;

}; /* class ImmutableCtrlVec { */


} /* namespace sharemind */

#endif /* SHAREMIND_PDKHEADERS_CONTROLLERREFERENCES_H */
