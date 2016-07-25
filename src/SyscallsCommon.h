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

#ifndef SHAREMIND_PDKHEADERS_SYSCALLSCOMMON_H
#define SHAREMIND_PDKHEADERS_SYSCALLSCOMMON_H

#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <new>

#include <sharemind/libmodapi/api_0x1.h>

namespace sharemind {


/**
 * Basic syscall parameter verification.
 */
template < const std::size_t NumArgs
         , const bool        NeedReturnValue = false
         , const std::size_t NumRefs = 0u
         , const std::size_t NumCRefs = 0u
         >
struct __attribute__ ((visibility("internal"))) SyscallArgs {
    template <class T>
    static inline std::size_t countRefs(const T * refs) {
        if (!refs)
            return 0u;

        assert(refs->pData);
        std::size_t r = 1u;
        while ((++refs)->pData)
            r++;
        return r;
    }

    static inline bool check(const std::size_t num_args,
                             const SharemindModuleApi0x1Reference * const refs,
                             const SharemindModuleApi0x1CReference * const crefs,
                             const SharemindCodeBlock * const returnValue)
    {
        if (num_args != NumArgs)
            return false;

        if (NeedReturnValue && !returnValue)
            return false;

        if (countRefs(refs) != NumRefs)
            return false;

        if (countRefs(crefs) != NumCRefs)
            return false;

        return true;
    }
};

/**
 * Virtual machine handle representation. Validates that the handle is correct.
 */
template <std::size_t pdkIndex>
class __attribute__ ((visibility("internal"))) PdpiVmHandles: public SharemindModuleApi0x1PdpiInfo {
public: /* Methods: */
    PdpiVmHandles () {
        pdpiHandle = 0;
        pdHandle = 0;
        pdkIndex = 0u;
        moduleHandle = 0;
    }


    inline bool get (SharemindModuleApi0x1SyscallContext* c, SharemindCodeBlock* args, std::size_t index = 0) {
        assert (c != 0 && args != 0);

        const SharemindModuleApi0x1PdpiInfo * pdpiInfo = (*(c->get_pdpi_info))(c, args[index].uint64[0]);
        if (!pdpiInfo) {
            std::fprintf(stderr, "get_pd_process_instance_handle (%" PRIu64 ")\n", args[index].uint64[0]);
            return false;
        }

        if (pdpiInfo->pdkIndex != pdkIndex       // or wrong pdk is returned
            || pdpiInfo->moduleHandle != c->moduleHandle) // or module handle pointers mismatch
            return false;

        assert(pdpiInfo->pdpiHandle);

        SharemindModuleApi0x1PdpiInfo::operator = (*pdpiInfo);

        return true;
    }
};

/**
 * Centralized exception handling.
 */
inline SharemindModuleApi0x1Error catchModuleApiErrors() noexcept
        __attribute__ ((visibility("internal")));

inline SharemindModuleApi0x1Error catchModuleApiErrors () noexcept {
    try {
        if (const auto eptr = std::current_exception ()) {
            std::rethrow_exception (eptr);
        }
        else {
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
        }
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    }
}

} /* namespace sharemind */

#endif /* SHAREMIND_PDKHEADERS_SYSCALLSCOMMON_H */
