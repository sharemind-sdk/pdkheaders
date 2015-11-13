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

#ifndef SHAREMIND_PDKHEADERS_VALUETRAITS_H
#define SHAREMIND_PDKHEADERS_VALUETRAITS_H

#include <cstddef>
#include <cstdint>
#include <type_traits>


namespace sharemind {

/*
 * Hierarchy of types.
 */
struct __attribute__ ((visibility("internal"))) any_value_tag { };

/**
 * \brief Type traits to track static information about sharemind types.
 * Instantiating the template with some type succeeds only for legal sharemind types.
 */
template <typename ValueType>
struct __attribute__ ((visibility("internal"))) ValueTraits {

    /**
     * \brief Static hierarchy of types.
     * Splitting values into various categories allows for simpler overloading
     * for protocol implementations. For example one might have different
     * implementations for signed and unsigned values.
     * \code
     * template <typename T>
     * bool invoke (ShareVec<T>& arr, unsigned_value_tag) { ... }
     * template <typename T>
     * bool invoke (ShareVec<T>& arr, signed_value_tag) { ... }
     * \endcode
     * The correct one is automatically selected via a single call:
     * \code
     * invoke (arr, typename ValueTraits<T>::value_category ());
     * \endcode
     */
    using value_category = typename ValueType::value_category;

    /**
     * \brief Type of shares.
     */
    using share_type = typename ValueType::share_type;

    /**
     * \brief Type of public representation.
     * The public values are handled by the VM.
     */
    using public_type = typename ValueType::public_type;

    /**
     * \brief Type identifier.
     * This value is used by the heap to track types.
     */
    static constexpr uint8_t heap_type_id = ValueType::heap_type_id;

    /**
     * \brief Number of bits.
     * This value gives the number of bits used for the type
     */
    static constexpr size_t num_of_bits = ValueType::num_of_bits;

    /**
     * \brief Logarithm of the number of bits.
     * This value gives the logarithm of the number of bits
     */
    static constexpr size_t log_of_bits = ValueType::log_of_bits;

}; /* struct ValueTraits { */

template <typename T>
struct __attribute__ ((visibility("internal"))) is_any_value_tag :
    std::is_base_of<any_value_tag, typename ValueTraits<T>::value_category>
{ };

} /* namespace sharemind */

#endif /* SHAREMIND_PDKHEADERS_VALUETRAITS_H */
