/*
 * Copyright (C) Cybernetica AS
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

#ifndef SHAREMIND_PDKHEADERS_IOCALLBACKS_H
#define SHAREMIND_PDKHEADERS_IOCALLBACKS_H

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback function to read data from a source into a preallocated buffer.
 *
 * @param[in] ctx           Pointer to callback context containing state (e.g. source).
 * @param[out] data_out     Pointer to a preallocated output buffer, where the.
 *                          callback must write up to `len` bytes.
 * @param[in] len           Buffer length in bytes.
 * @param[out] bytes_read   Number of bytes actually read (can be <len for partial reads).
 *
 * @return                  `true` on success, `false` on error.
 */
typedef bool (*read_callback_t)(void * ctx, void * data_out, size_t len, size_t * bytes_read);

/**
 * @brief Callback function to write all data from a buffer to a destination.
 *
 * @param[in] ctx           Pointer to callback context containing state (e.g. destination).
 * @param[in] data_in       Pointer to a buffer containing data to be written.
 * @param[in] len           Buffer length in bytes.
 *
 * @return                  `true` on success, `false` on error.
 */
typedef bool (*write_callback_t)(void * ctx, void const * data_in, size_t len);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_PDKHEADERS_IOCALLBACKS_H */