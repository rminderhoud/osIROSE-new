#include "cli_partychat.h"
#include "throwassert.h"

namespace RoseCommon {

CliPartyChat::CliPartyChat() : CRosePacket(ePacketType::PAKCS_PARTY_CHAT) {}

CliPartyChat::CliPartyChat(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_PARTY_CHAT, "Not the right packet: " << to_underlying(get_type()));
	reader.get_string(message_);
}

CliPartyChat::CliPartyChat(const std::string& message) : CRosePacket(ePacketType::PAKCS_PARTY_CHAT), message_(message) {
}

const std::string& CliPartyChat::message() const { return message_; }

CliPartyChat& CliPartyChat::set_message(const std::string& data) { message_ = data; return *this; }


void CliPartyChat::pack(CRoseBasePolicy& writer) const {
	writer.set_string(message_);
}

CliPartyChat CliPartyChat::create(const std::string& message) {


	return CliPartyChat(message);
}

CliPartyChat CliPartyChat::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliPartyChat(reader);
}
std::unique_ptr<CliPartyChat> CliPartyChat::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliPartyChat>(reader);
}

}