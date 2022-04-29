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


#ifndef SHAREMIND_PDKHEADERS_LIBPD_H
#define SHAREMIND_PDKHEADERS_LIBPD_H

#include <stdbool.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations: */
struct SharemindNodeConfiguration_;
typedef struct SharemindNodeConfiguration_ SharemindNodeConfiguration;
struct SharemindMessage_;
typedef struct SharemindMessage_ SharemindMessage;
struct SharemindNode_;
typedef struct SharemindNode_ SharemindNode;
struct SharemindNetworkConfiguration_;
typedef struct SharemindNetworkConfiguration_ SharemindNetworkConfiguration;
struct SharemindNetwork_;
typedef struct SharemindNetwork_ SharemindNetwork;
struct SharemindPdpiNetworkFacility_;
typedef struct SharemindPdpiNetworkFacility_ SharemindPdpiNetworkFacility;
struct SharemindPdNetworkFacility_;
typedef struct SharemindPdNetworkFacility_ SharemindPdNetworkFacility;


struct SharemindNodeConfiguration_ {
    size_t node_number;
    size_t miner_number;
    bool is_computing_node;
};

/** \brief Represents a simple network message for Sharemind nodes in Sharemind networks. */
struct SharemindMessage_ {

    /** Pointer to data or NULL on error. */
    const void * data;

    /** Size of data (may be zero). */
    size_t size;

}; /* struct SharemindMessage_ { */

enum SharemindNetworkError_ {

    /** No error. */
    SHAREMIND_NETWORK_OK = 0,

    /** A fatal out of memory condition occurred. */
    SHAREMIND_NETWORK_OUT_OF_MEMORY,

    /** An unexpected exception occurred. */
    SHAREMIND_NETWORK_UNKNOWN_ERROR,

    /** An invalid argument was given to the function. */
    SHAREMIND_NETWORK_INVALID_ARGUMENT,

    /** A fatal network timeout occurred. */
    SHAREMIND_NETWORK_TIMEOUT,

    /** No such node was found. */
    SHAREMIND_NETWORK_NODE_NOT_FOUND,

    /** No such miner was found. */
    SHAREMIND_NETWORK_MINER_NOT_FOUND,

    /** The network operation failed because the network is shutting down. */
    SHAREMIND_NETWORK_NETWORK_SHUTTING_DOWN,

    /** The network operation failed because networking has failed fatally. */
    SHAREMIND_NETWORK_NETWORK_FATAL_ERROR,

    /** The operation failed because user-set limits were reached. */
    SHAREMIND_NETWORK_CONFIGURATION_LIMITS_REACHED,

    /** The network configuration has already been initialized. */
    SHAREMIND_NETWORK_CONFIGURATION_ALREADY_INITIALIZED

};
typedef enum SharemindNetworkError_ SharemindNetworkError;

/** \brief Represents a Sharemind MPC node. */
struct SharemindNode_ {

    /**
      \param[in] node pointer to this node.
      \returns the code of the last error that occured while calling the methods
               of this object or SHAREMIND_NETWORK_OK if no error has
               occurred.
    */
    SharemindNetworkError (* const last_error)(const SharemindNode * node);

    /**
      \brief Clears any errors.
      \post last_error() will return SHAREMIND_NETWORK_OK.
      \param[in] node pointer to this node.
    */
    void (* const clear_error)(SharemindNode * node);

    /**
      \brief Indicates whether or not the node is computing node
      \param[in] node pointer to this node.
      \returns whether or not the node is computing node
    */
    bool (* const is_computing_node)(const SharemindNode * node);

    /**
      \brief Returns the mapped number of the node.
      \param[in] node pointer to this node.
      \returns the mapped number of the node. Numbering begins with 1.
    */
    size_t (* const get_node_number)(const SharemindNode * node);

    /**
      \brief Sends a message.
      \param[in] node pointer to this node.
      \param[in] message The message to send, must be valid.
      \returns an error code, if any.
    */
    SharemindNetworkError (* const send_message)(SharemindNode * node,
                                                 const SharemindMessage message);

    /**
      \brief Waits for a message.
      \param[in] node pointer to this node.
      \note The received message must be deallocated using free_message().
      \returns the message received or NULL on error.
    */
    SharemindMessage (* const receive_message)(SharemindNode * node);

    /**
      \brief Deallocates a message returned by receive_message().
      \param[in] node pointer to this node.
      \param[in] message The returned message to free.
    */
    void (* const free_message)(SharemindNode * node,
                                SharemindMessage * message);

}; /* struct SharemindNode_ { */

struct SharemindNetworkConfiguration_ {
    /**
      \param[in] config pointer to this object.
      \returns the number of the local node mapped to the local miner. Numbering begins with 1.
    */
    size_t (* const get_local_node_number)(const SharemindNetworkConfiguration * config);

    /**
      \param[in] config pointer to this object.
      \returns whether or not the local node is computing node.
    */
    bool (* const get_local_is_computing_node)(const SharemindNetworkConfiguration * config);

    /**
      \param[in] config pointer to this object.
      \returns whether or not the local node is master node.
    */
    bool (* const get_local_is_master_node)(const SharemindNetworkConfiguration * config);

}; /* struct SharemindNetworkConfiguration_ { */

/** \brief A new network object. */
struct SharemindNetwork_ {

    /**
      \brief Frees this SharemindNetwork instance.
      \param[in] network pointer to this network.
    */
    void (* const free)(SharemindNetwork * network);

    /**
      \brief Returns a reference to the node.
      \param[in] nodeId The module-side node ID whose node pointer to return.
      \returns the pointer to the node or NULL on error.
    */
    SharemindNode * (* const get_node)(SharemindNetwork * network,
                                       size_t nodeId);

    const SharemindNetworkConfiguration * (* const get_configuration)(SharemindNetwork * network);

}; /* struct SharemindNetwork_ { */

/** \brief A PDPI network facility. */
struct SharemindPdpiNetworkFacility_ {

    /**
      \param[in] facility pointer to this facility.
      \returns the code of the last error that occured while calling the methods
               of this object or SHAREMIND_NETWORK_OK if no error has
               occurred.
    */
    SharemindNetworkError (* const last_error)(const SharemindPdpiNetworkFacility * facility);

    /**
      \brief Clears any errors.
      \post last_error() will return SHAREMIND_NETWORK_OK.
      \param[in] facility pointer to this facility.
    */
    void (* const clear_error)(SharemindPdpiNetworkFacility * facility);

    /**
      \brief Creates and returns a new PDPI network object.
      \param[in] facility pointer to this facility.
      \param[in] config pointer to the respective network configuration object.
      \warning The ownership of the returned object is not transferred to the caller.
      \returns a new PDPI network object or NULL on error.
    */
    SharemindNetwork * (* const new_network)(
            SharemindPdpiNetworkFacility * facility,
            const SharemindNetworkConfiguration * config);

}; /* struct SharemindPdpiNetworkFacility_ { */

/** \brief A PD network facility. */
struct SharemindPdNetworkFacility_ {

    /**
      \param[in] facility pointer to this facility.
      \returns the code of the last error that occured while calling the methods
               of this object or SHAREMIND_NETWORK_OK if no error has
               occurred.
    */
    SharemindNetworkError (* const last_error)(const SharemindPdNetworkFacility * facility);

    /**
      \brief Clears any errors.
      \post last_error() will return SHAREMIND_NETWORK_OK.
      \param[in] facility pointer to this facility.
    */
    void (* const clear_error)(SharemindPdNetworkFacility * facility);

    /**
      \brief Checks whether a server with the given name is known.
      \param[in] facility pointer to this facility.
      \param[in] serverName the name of the server to search for.
      \param[out] serverNumberOut if non-null, the number of the server.
      \returns whether a server with the given name is known.
    */
    bool (* const haveServer)(
            const SharemindPdNetworkFacility * facility,
            char const * serverName,
            size_t * serverNumberOut);

    /**
      \brief Initializes the network configuration and returns a pointer to the
             configuration object.
      \param[in] facility pointer to this facility.
      \param[in] confs the configurations for Sharemind nodes.
      \param[in] numberOfNodeConfigurations the number of node configurations in confs.
      \returns the network configuration object or NULL on error.
     */
    const SharemindNetworkConfiguration * (* const init_configuration)(
            SharemindPdNetworkFacility * facility,
            const SharemindNodeConfiguration * confs,
            size_t numberOfNodeConfigurations);

    /**
      \brief Creates and returns a PD network object.
      \param[in] facility pointer to this facility.
      \param[in] config pointer to the respective network configuration object.
      \warning The ownership of the returned object is not transferred to the caller.
      \returns the PD network object or NULL on error.
    */
    SharemindNetwork * (* const new_network)(
            SharemindPdNetworkFacility * facility,
            const SharemindNetworkConfiguration * config);
}; /* struct SharemindPdNetworkFacility_ { */

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SHAREMIND_PDKHEADERS_LIBPD_H */
