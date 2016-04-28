/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */


#ifndef SHAREMIND_MINER_PDOUTGOINGMESSAGE_H
#define SHAREMIND_MINER_PDOUTGOINGMESSAGE_H

#include <sharemind/libpd.h>
#include <sharemind/NetworkMessage.h>

namespace sharemind {

class PdOutgoingMessage: public OutgoingNetworkMessage {

public: /* Methods: */

    PdOutgoingMessage(SharemindNode & destination)
        : m_destination(destination) {}

    bool send() const {
        /** \bug Currently we send pure data without the header. As a result the
                 miner will construct a new message with required header and
                 COPY (!!!) the data over to that message before sending.
                 Refactor this to remove the extra copying of sent data. */
        return m_destination.send_message(&m_destination, { data, size })
               == SHAREMIND_NETWORK_OK;
    }

private: /* Fields: */

    SharemindNode & m_destination; /**< Destination node: */
};

} /* namespace sharemind { */

#endif /* SHAREMIND_MINER_PDOUTGOINGMESSAGE_H */
