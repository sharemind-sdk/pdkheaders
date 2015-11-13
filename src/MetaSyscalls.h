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

#ifndef SHAREMIND_PDKHEADERS_METASYSCALLS_H
#define SHAREMIND_PDKHEADERS_METASYSCALLS_H

#include <sharemind/libmodapi/api_0x1.h>

#include "ShareVector.h"
#include "SyscallsCommon.h"
#include "VmVector.h"

/**
 * Meta-syscalls for many common cases.
 */

namespace sharemind {

template <typename PdpiType, size_t pdkIndex>
struct __attribute__ ((visibility("internal"))) MetaSyscalls {

public:

    /**
     * SysCall: binary_vec<T1, T2, T3, Protocol>
     * Args:
     *      0) uint64[0]     pd index
     *      1) p[0]          LHS handle
     *      2) p[0]          RHS handle
     *      3) p[0]          output handle
     * Precondition:
     *      LHS handle is a vector of type T1.
     *      RHS handle is a vector of type T2.
     *      Output handle is a vector of type T3.
     */
    template <typename T1, typename T2, typename T3, typename Protocol>
    static SHAREMIND_MODULE_API_0x1_SYSCALL(binary_vec,
                                     args, num_args, refs, crefs,
                                     returnValue, c)
    {
        PdpiVmHandles<pdkIndex> handles;
        if (! SyscallArgs<4>::check (num_args, refs, crefs, returnValue) ||
            ! handles.get (c, args)) {
            return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
        }

        try {
            PdpiType* pdpi = static_cast<PdpiType *>(handles.pdpiHandle);
            if (! pdpi->isComputingNode ()) {
                return SHAREMIND_MODULE_API_0x1_OK;
            }

            void* lhsHandle = args[1].p[0];
            void* rhsHandle = args[2].p[0];
            void* resultHandle = args[3].p[0];

            if (! pdpi->template isValidHandle<T1>(lhsHandle) ||
                ! pdpi->template isValidHandle<T2>(rhsHandle) ||
                ! pdpi->template isValidHandle<T3>(resultHandle)) {
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
            }

            if (! pdpi->isComputingNode ()) {
                return SHAREMIND_MODULE_API_0x1_OK;
            }

            const ShareVec<T1>& param1 = *static_cast<ShareVec<T1>*>(lhsHandle);
            const ShareVec<T2>& param2 = *static_cast<ShareVec<T2>*>(rhsHandle);
            ShareVec<T3>& result = *static_cast<ShareVec<T3>*>(resultHandle);

            Protocol protocol(*pdpi);
            if (! protocol.invoke (param1, param2, result))
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

            return SHAREMIND_MODULE_API_0x1_OK;
        } catch (...) {
            return catchModuleApiErrors ();
        }
    }

    /**
     * SysCall: binary_arith_vec<T, Protocol>
     * Args:
     *      0) uint64[0]     pd index
     *      1) p[0]          vector handle (lhs)
     *      2) p[0]          vector handle (rhs)
     *      3) p[0]          output handle
     * Precondition:
     *      All handles are valid vectors of type T.
     */
    template <typename T, typename Protocol>
    static SHAREMIND_MODULE_API_0x1_SYSCALL(binary_arith_vec,
                                     args, num_args, refs, crefs,
                                     returnValue, c)
    {
        return binary_vec<T, T, T, Protocol>(args, num_args, refs, crefs, returnValue, c);
    }

    /**
     * SysCall: relational_vec<T, Protocol>
     * Args:
     *      0) uint64[0]     pd index
     *      1) p[0]          vector handle (lhs)
     *      2) p[0]          vector handle (rhs)
     *      3) p[0]          output handle
     * Precondition:
     *      Handles 1 and 2 are valid private vectors of type T.
     *      Handle nr. 3 is boolean private vector.
     *      All vectors are of equal length.
     */
    template <typename T, typename BoolT, typename Protocol>
    static SHAREMIND_MODULE_API_0x1_SYSCALL(relational_vec,
                                     args, num_args, refs, crefs,
                                     returnValue, c)
    {
        return binary_vec<T, T, BoolT, Protocol>(args, num_args, refs, crefs, returnValue, c);
    }

    /**
     * SysCall: unary_vec<T, L, Protocol>
     * Args:
     *      0) uint64[0]     pd index
     *      1) p[0]          input handle
     *      2) p[0]          output handle
     * Precondition:
     *      Input handle is a vector of type T.
     *      Output handle is a vector of type L.
     */
    template <typename T, typename L, typename Protocol>
    static SHAREMIND_MODULE_API_0x1_SYSCALL(unary_vec,
                                     args, num_args, refs, crefs,
                                     returnValue, c)
    {
        PdpiVmHandles<pdkIndex> handles;
        if (! SyscallArgs<3>::check (num_args, refs, crefs, returnValue) ||
            ! handles.get (c, args)) {
            return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
        }

        try {
            PdpiType* pdpi = static_cast<PdpiType *>(handles.pdpiHandle);
            if (! pdpi->isComputingNode ()) {
                return SHAREMIND_MODULE_API_0x1_OK;
            }

            void* paramHandle = args[1].p[0];
            void* resultHandle = args[2].p[0];

            if (! pdpi->template isValidHandle<T>(paramHandle) ||
                ! pdpi->template isValidHandle<L>(resultHandle)) {
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
            }

            const ShareVec<T>& param = *static_cast<ShareVec<T>*>(paramHandle);
            ShareVec<L>& result = *static_cast<ShareVec<L>*>(resultHandle);

            Protocol protocol(*pdpi);
            if (! protocol.invoke (param, result))
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

            return SHAREMIND_MODULE_API_0x1_OK;
        } catch (...) {
            return catchModuleApiErrors ();
        }
    }

    /**
     * SysCall: unary_arith_vec<T, Protocol>
     * Args:
     *      0) uint64[0]     pd index
     *      1) p[0]          input handle
     *      2) p[0]          output handle
     * Precondition:
     *      Both handles are valid vectors of type T.
     */
    template <typename T, typename Protocol>
    static SHAREMIND_MODULE_API_0x1_SYSCALL(unary_arith_vec, args, num_args, refs, crefs, returnValue, c)
    {
        return unary_vec<T, T, Protocol>(args, num_args, refs, crefs, returnValue, c);
    }

    /**
     * SysCall: nullary_vec<T, Protocol>
     * Args:
     *      0) uint64[0]     pd index
     *      1) p[0]          output handle
     * Precondition:
     *      Output handle is valid vectors of type T.
     */
    template <typename T, typename Protocol>
    static SHAREMIND_MODULE_API_0x1_SYSCALL(nullary_vec,
                                     args, num_args, refs, crefs,
                                     returnValue, c)
    {
        PdpiVmHandles<pdkIndex> handles;
        if (! SyscallArgs<2>::check (num_args, refs, crefs, returnValue) ||
            ! handles.get (c, args)) {
            return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
        }

        try {
            PdpiType* pdpi = static_cast<PdpiType *>(handles.pdpiHandle);
            if (! pdpi->isComputingNode ()) {
                return SHAREMIND_MODULE_API_0x1_OK;
            }

            void* resultHandle = args[1].p[0];

            if (! pdpi->template isValidHandle<T>(resultHandle)) {
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
            }

            ShareVec<T>& result = *static_cast<ShareVec<T>*>(resultHandle);

            if (!Protocol(*pdpi).invoke(result))
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
            return SHAREMIND_MODULE_API_0x1_OK;
        } catch (...) {
            return catchModuleApiErrors ();
        }
    }

    template <typename T, typename L, typename Protocol>
    static SHAREMIND_MODULE_API_0x1_SYSCALL(opc_vec,
                                     args, num_args, refs, crefs,
                                     returnValue, c)
    {
        PdpiVmHandles<pdkIndex> handles;
        if (! SyscallArgs<3, false, 0, 1>::check (num_args, refs, crefs, returnValue) ||
            ! handles.get (c, args)) {
            return SHAREMIND_MODULE_API_0x1_INVALID_CALL; // Signal that the call was invalid.
        }

        try {
            PdpiType* pdpi = static_cast<PdpiType *>(handles.pdpiHandle);
            if (! pdpi->isComputingNode ()) {
                return SHAREMIND_MODULE_API_0x1_OK;
            }

            void* param1Handle = args[1].p[0];
            void* resultHandle = args[2].p[0];

            if (! pdpi->template isValidHandle<T>(param1Handle) ||
                ! pdpi->template isValidHandle<T>(resultHandle)) {
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
            }

            const ShareVec<T>& param1 = *static_cast<ShareVec<T>*>(param1Handle);
            const ImmutableVmVec<L> param2 (crefs[0]);
            ShareVec<T>& result = *static_cast<ShareVec<T>*>(resultHandle);

            if (!Protocol(*pdpi).invoke(param1, param2, result))
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
            return SHAREMIND_MODULE_API_0x1_OK;
        } catch (...) {
            return catchModuleApiErrors ();
        }
    }

    /**
     * \see opc_vec Same with the exception that arguments have the same type.
     */
    template <typename T, typename Protocol>
    static SHAREMIND_MODULE_API_0x1_SYSCALL(opc_arith_vec,
                                     args, num_args, refs, crefs,
                                     returnValue, c)
    {
        return opc_vec<T, T, Protocol>(args, num_args, refs, crefs, returnValue, c);
    }

    /**
     * SysCall: compare_vec<T, Protocol, flipParams>
     * Args:
     *      0) uint64[0]     pd index
     *      1) p[0]          input handle (lhs)
     *      2) p[0]          input handle (rhs)
     *      3) p[0]          output handle
     * Precondition:
     *      All input handles are valid vectors of type T.
     *      All vectors are of equal length.
     */
    template <typename T, typename BoolT, typename Protocol, bool flipParams>
    static SHAREMIND_MODULE_API_0x1_SYSCALL(compare_vec,
                                     args, num_args, refs, crefs,
                                     returnValue, c)
    {
        PdpiVmHandles<pdkIndex> handles;
        if (! SyscallArgs<4>::check (num_args, refs, crefs, returnValue) ||
            ! handles.get (c, args)) {
            return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
        }

        try {
            PdpiType* pdpi = static_cast<PdpiType *>(handles.pdpiHandle);
            if (! pdpi->isComputingNode ()) {
                return SHAREMIND_MODULE_API_0x1_OK;
            }

            void* param1Handle = args[flipParams ? 2 : 1].p[0];
            void* param2Handle = args[flipParams ? 1 : 2].p[0];
            void* resultHandle = args[3].p[0];

            if (! pdpi->template isValidHandle<T>(param1Handle) ||
                ! pdpi->template isValidHandle<T>(param2Handle) ||
                ! pdpi->template isValidHandle<BoolT>(resultHandle)) {
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
            }

            const ShareVec<T>& param1 = *static_cast<ShareVec<T>*>(param1Handle);
            const ShareVec<T>& param2 = *static_cast<ShareVec<T>*>(param2Handle);
            ShareVec<BoolT>& result = *static_cast<ShareVec<BoolT>*>(resultHandle);

            Protocol comparisonProtocol(*pdpi);
            if (!comparisonProtocol.invoke (param1, param2, result))
                return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;

            return SHAREMIND_MODULE_API_0x1_OK;
        } catch (...) {
            return catchModuleApiErrors ();
        }
    }

};

} /* namespace sharemind */

#endif /* SHAREMIND_PDKHEADERS_METASYSCALLS_H */
