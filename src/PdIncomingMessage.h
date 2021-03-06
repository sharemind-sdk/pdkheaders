/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */


#ifndef SHAREMIND_PDKHEADERS_PDINCOMINGMESSAGE_H
#define SHAREMIND_PDKHEADERS_PDINCOMINGMESSAGE_H

#include <sharemind/NetworkMessage.h>

#include "libpd.h"


namespace sharemind {

class PdIncomingMessage : public IncomingNetworkMessage {

public: /* Methods: */

    PdIncomingMessage(SharemindMessage const & message, SharemindNode & sender)
        : IncomingNetworkMessage(message.data, message.size)
        , m_senderNode(sender)
    {}

    ~PdIncomingMessage() noexcept override {
        SharemindMessage m{data, size};
        m_senderNode.free_message(&m_senderNode, &m);
    }

private: /* Fields: */

    SharemindNode & m_senderNode;

}; /* class PdIncomingMessage {*/

} /* namespace sharemind { */

#endif /* SHAREMIND_PDKHEADERS_PDINCOMINGMESSAGE_H */
