#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_CHAR_LIST_REQ, CliCharListReq)
class CliCharListReq : public CRosePacket {
	public:
		CliCharListReq();
		CliCharListReq(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliCharListReq() = default;


	protected:
		virtual void pack() override;
};

}