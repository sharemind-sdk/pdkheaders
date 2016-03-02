/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */


#ifndef SHAREMIND_MINER_PDINCOMINGMESSAGE_H
#define SHAREMIND_MINER_PDINCOMINGMESSAGE_H

#include <sharemind/libpd.h>
#include <Hairball/NetworkMessage.h>


namespace sharemind {

class PdIncomingMessage : public IncomingNetworkMessageNoCopy {

public: /* Methods: */

    PdIncomingMessage(const SharemindMessage & message, SharemindNode * sender)
        : IncomingNetworkMessageNoCopy(message.data, message.size)
        , m_senderNode(sender)
    {
        assert(m_senderNode);
        // Intentionally empty
    }

    ~PdIncomingMessage() noexcept override {
        SharemindMessage m = {data, size};
        m_senderNode->free_message(m_senderNode, &m);
    }

private: /* Methods: */
    SharemindNode * m_senderNode;

}; /* class PdIncomingMessage {*/

} /* namespace sharemind { */

#endif /* SHAREMIND_MINER_PDINCOMINGMESSAGE_H */
