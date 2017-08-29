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

#ifndef SHAREMIND_PDKHEADERS_SHAREDVALUEHEAP_H
#define SHAREMIND_PDKHEADERS_SHAREDVALUEHEAP_H

#include <cstdint>
#include <unordered_map>
#include "ShareVector.h"
#include "ValueTraits.h"


namespace sharemind {

/**
 * \brief Heap of share vectors.
 * This class tracks in a type safe manner all of the allocated share vectors.
 */
class __attribute__ ((visibility("internal"))) SharedValueHeap {

private: /* Types: */

    using impl_t = std::unordered_map<ShareVecBase *, uint8_t>;

public: /* Methods: */

    SharedValueHeap () { }
    SharedValueHeap(const SharedValueHeap &) = delete;
    SharedValueHeap & operator=(const SharedValueHeap &) = delete;

    /**
     * Frees all stored share vectors.
     */
    ~SharedValueHeap () {
        for (impl_t::iterator i = m_pointers.begin (), e = m_pointers.end (); i != e; ++ i) {
            delete i->first;
        }
    }

    /**
     * Insert vector share vector of type T into the heap.
     * \param[in] vec Vector to be inserted into the heap.
     * \retval true If vector was inserted into the heap successfully, and it wasn't stored in the heap before.
     * \retval false If vec was null pointer, or if the vector was already stored in the heap.
     */
    template <typename T>
    bool insert (ShareVec<T>* vec) {
        uint8_t heap_type_id = ValueTraits<T>::heap_type_id;
        if (vec)
            return m_pointers.insert (std::make_pair (static_cast<ShareVecBase *>(vec), heap_type_id)).second;
        return false;
    }

    /**
     * Erases a share vector of given type from the heap.
     * \param[in] vec The vector to be erased.
     * \retval true If vector was successfully freed from the heap.
     * \retval false If the vector was not stored, or was stored with incorrect type.
     */
    template <typename T>
    bool erase (ShareVec<T>* vec) {
        impl_t::iterator i = m_pointers.find (vec);
        if (i != m_pointers.end ()) {
            if (i->second == ValueTraits<T>::heap_type_id) {
                delete i->first;
                m_pointers.erase (i);
                return true;
            }
        }

        return false;
    }

    /**
     * Checks if given handle is stored in the heap with given type.
     * \param[in] hndl A handle to a share vector.
     * \retval true If the \a hndl was stored in the heap with type \a T.
     * \retval false If the handle is not stored in the heap, or is stored with incorrect type.
     */
    template <typename T>
    bool check (void* hndl) const {
        impl_t::const_iterator i = m_pointers.find (static_cast<ShareVecBase*>(hndl));
        if (i != m_pointers.end ()) {
            return (i->second == ValueTraits<T>::heap_type_id);
        }

        return false;
    }

private: /* Fields: */

    impl_t m_pointers;

}; /* class SharedValueHeap { */

} /* namespace sharemind */

#endif /* SHAREMIND_PDKHEADERS_SHAREDVALUEHEAP_H */
