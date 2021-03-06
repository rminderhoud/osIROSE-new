#include "map/change_map.h"

#include "logconsole.h"
#include "entity_system.h"
#include "cli_change_map_req.h"
#include "srv_change_map_reply.h"
#include "chat/whisper_chat.h"
#include "cmapclient.h"

#include "components/basic_info.h"
#include "components/client.h"
#include "components/level.h"
#include "components/life.h"
#include "components/magic.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Map::change_map_request(EntitySystem& entitySystem, Entity entity, const CliChangeMapReq&) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Map::change_map_request");
    logger->trace("entity {}", entity);
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto& life = entitySystem.get_component<Component::Life>(entity);
    const auto& magic = entitySystem.get_component<Component::Magic>(entity);
    const auto& level = entitySystem.get_component<Component::Level>(entity);
    // force send the packet as the client isn't technically on the map yet
    entitySystem.send_to(entity, SrvChangeMapReply::create(
        basicInfo.id, life.hp, magic.mp, level.xp, level.penaltyXp,
        entitySystem.get_world_time(), basicInfo.teamId), true); // FIXME: teamId is uint32_t but the packet expects a uint16_t
    if (const auto client_ptr = entitySystem.try_get_component<Component::Client>(entity)) {
        if (auto client = client_ptr->client.lock()) {
            client->set_on_map();
        }
    }
    Chat::send_whisper(entitySystem, entity, fmt::format("You are client {}", basicInfo.id));
    entitySystem.send_nearby_except_me(entity, CMapClient::create_srv_player_char(entitySystem, entity));
    const auto& nearby_entities = entitySystem.get_nearby(entity);
    for (auto other : nearby_entities) {
        if (other != entity) {
            entitySystem.send_to_entity(entity, other);
        }
    }
}
