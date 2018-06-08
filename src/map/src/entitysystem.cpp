#include <set>
#include <vector>

#include "entitysystem.h"
#include "cmapclient.h"
#include "cmapserver.h"
#include "connection.h"
#include "systems/chatsystem.h"
#include "systems/inventorysystem.h"
#include "systems/luasystem.h"
#include "systems/mapsystem.h"
#include "systems/movementsystem.h"
#include "systems/partysystem.h"
#include "systems/updatesystem.h"

using namespace RoseCommon;
EntitySystem::EntitySystem(CMapServer *server) : systemManager_(*this), server_(server) {
  systemManager_.add<Systems::MovementSystem>();
  systemManager_.add<Systems::UpdateSystem>();
  systemManager_.add<Systems::ChatSystem>();
  systemManager_.add<Systems::InventorySystem>();
  systemManager_.add<Systems::PartySystem>();
  systemManager_.add<Systems::MapSystem>();
  systemManager_.add<Systems::LuaSystem>();
}
EntityManager& EntitySystem::getEntityManager() { return entityManager_; }

Entity EntitySystem::buildItemEntity(Entity creator, RoseCommon::Item&& item) {
  Entity e = create();
  e.assign<Item>(std::move(item));
  auto pos = creator.component<Position>();
  e.assign<Position>(pos->x_, pos->y_, pos->map_, 0);
  auto basic = e.assign<BasicInfo>();
  basic->ownerId_ = creator.component<BasicInfo>()->id_;
  basic->id_ = id_manager_.get_free_id();
  itemToEntity_[basic->id_] = e;
  return e;
}

void EntitySystem::registerEntity(Entity entity) {
  if (!entity) return;
  auto basic = entity.component<BasicInfo>();
  if (!basic || basic->name_ == "" || !basic->id_) return;
  nameToEntity_[basic->name_] = entity;
  idToEntity_[basic->id_] = entity;
}

Entity EntitySystem::getItemEntity(uint32_t id) { return itemToEntity_[id]; }

Entity EntitySystem::getEntity(const std::string& name) { return nameToEntity_[name]; }

Entity EntitySystem::getEntity(uint32_t charId) { return idToEntity_[charId]; }

void EntitySystem::update(double dt) {
  std::lock_guard<std::mutex> lock(access_);
  while (toDispatch_.size()) {
    auto tmp = std::move(toDispatch_.front());
    systemManager_.dispatch(tmp.first, *tmp.second);
    toDispatch_.pop();
  }
  systemManager_.update(dt);
  for (auto it : toDestroy_) {
    if (it) {
      saveCharacter(it.component<CharacterInfo>()->charId_, it);
      auto basic = it.component<BasicInfo>();
      nameToEntity_.erase(basic->name_);
      idToEntity_.erase(basic->id_);
      id_manager_.release_id(basic->id_);
      it.destroy();
    }
  }
  toDestroy_.clear();
}

void EntitySystem::destroy(Entity entity) {
  if (!entity) return;
  std::lock_guard<std::mutex> lock(access_);
  toDestroy_.push_back(entity);
}

Entity EntitySystem::create() { return entityManager_.create(); }

bool EntitySystem::isNearby(Entity a, Entity b) {
  return true;  // FIXME : actually implement the sight calculation instead of the distance
  if (!a || !b) return false;
  auto posa = a.component<Position>();
  auto posb = b.component<Position>();
  if (!posa || !posb) return false;  // FIXME : is it a bug if there is no position?
  if (posa->map_ != posb->map_) return false;
  double dist = (posa->x_ - posb->x_) * (posa->x_ - posb->x_) + (posa->y_ - posb->y_) * (posa->y_ - posb->y_);
  if (dist > NEARBY_DIST) return false;
  return true;
}

bool EntitySystem::dispatch(Entity entity, std::unique_ptr<RoseCommon::CRosePacket> packet) {
  if (!entity) return false;
  if (systemManager_.wouldDispatch(*packet)) {
    std::lock_guard<std::mutex> lock(access_);
    toDispatch_.emplace(std::make_pair(entity, std::move(packet)));
    return true;
  }
  return false;
}

Entity EntitySystem::loadCharacter(uint32_t charId, bool platinium) {
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  Core::CharacterTable characters{};
  Core::InventoryTable inventoryTable{};
  Core::SkillTable skillsTable{};

  auto charRes = conn(sqlpp::select(sqlpp::count(characters.id), sqlpp::all_of(characters))
                          .from(characters)
                          .where(characters.id == charId));

  std::lock_guard<std::mutex> lock(access_);
  auto entity = create();
  if (static_cast<long>(charRes.front().count) != 1L) {
    entity.destroy();
    return Entity();
  }
  const auto& charRow = charRes.front();

  entity.assign<Position>(charRow);
  entity.assign<BasicInfo>(charRow, id_manager_.get_free_id());
  entity.assign<Stats>(charRow);
  entity.assign<AdvancedInfo>(charRow);
  entity.assign<CharacterGraphics>(charRow);
  entity.assign<CharacterInfo>(charRow, platinium, charId);

  // TODO : write the pat initialization code
  auto skills = entity.assign<Skills>();
  auto skillRes =
      conn(sqlpp::select(skillsTable.id, skillsTable.level).from(skillsTable).where(skillsTable.charId == charId));
  skills->loadFromResult(skillRes);

  // TODO : write the hotbar table and loading code
  entity.assign<Hotbar>();
  entity.assign<StatusEffects>();
  entity.assign<RidingItems>();
  entity.assign<BulletItems>();

  // TODO : write the inventory code
  // auto luaSystem = systemManager_.get<Systems::LuaSystem>();
  auto inventory = entity.assign<Inventory>();

  auto invRes =
      conn(sqlpp::select(sqlpp::all_of(inventoryTable)).from(inventoryTable).where(inventoryTable.charId == charId));
  inventory->loadFromResult(invRes, get<Systems::InventorySystem>());

  Systems::UpdateSystem::calculateSpeed(entity);

  entity.assign<Quests>();

  Core::WishTable wish{};
  auto wishRes = conn(sqlpp::select(sqlpp::all_of(wish)).from(wish).where(wish.charId == charId));
  auto wishlist = entity.assign<Wishlist>();
  wishlist->loadFromResult(wishRes, get<Systems::InventorySystem>());

  // auto lua = entity.assign<EntityAPI>();

  // luaSystem->loadScript(entity, "function onInit()\ndisplay('test')\nend");
  // lua->onInit();

  registerEntity(entity);
  return entity;
}

void EntitySystem::saveCharacter(uint32_t charId, Entity entity) {
  if (!entity) return;
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  Core::CharacterTable characters{};

  using sqlpp::parameter;

  auto update = sqlpp::dynamic_update(conn.get(), characters).dynamic_set().where(characters.id == charId);
  entity.component<Position>()->commitToUpdate<decltype(characters)>(update);
  entity.component<BasicInfo>()->commitToUpdate<decltype(characters)>(update);
  entity.component<Stats>()->commitToUpdate<decltype(characters)>(update);
  entity.component<AdvancedInfo>()->commitToUpdate<decltype(characters)>(update);
  // entity.component<CharacterGraphics>()->commitToUpdate(update);
  entity.component<CharacterInfo>()->commitToUpdate<decltype(characters)>(update);
  // entity.component<Hotbar>()->commitToUpdate(update);
  // entity.component<StatusEffects>()->commitToUpdate(update);
  // entity.component<RidingItems>()->commitToUpdate(update);
  // entity.component<BulletItems>()->commitToUpdate(update);

  conn->run(update);

  // entity.component<Skills>()->commitToUpdate(updateSkills);

  Core::InventoryTable inv{};
  auto invRes = conn(sqlpp::select(sqlpp::all_of(inv)).from(inv).where(inv.charId == charId));

  const auto& items = entity.component<Inventory>()->items_;

  std::vector<size_t> toDelete;
  std::vector<size_t> toUpdate;
  std::set<size_t> modified;
  std::vector<size_t> toInsert;

  for (const auto& row : invRes) {
    if (row.slot >= Inventory::maxItems)
      toDelete.emplace_back(row.slot);  // FIXME: that should never happen
    else if (!items[row.slot])
      toDelete.emplace_back(row.slot);
    else if (items[row.slot] != Item(row))
      toUpdate.emplace_back(row.slot);
    modified.insert(row.slot);
  }
  size_t i = 0;
  for (const auto& item : items) {
    if (item && modified.find(i) == modified.end()) toInsert.emplace_back(i);
    ++i;
  }

  for (auto it : toDelete) conn(sqlpp::remove_from(inv).where(inv.charId == charId and inv.slot == it));
  for (auto it : toUpdate) {
    auto update = sqlpp::dynamic_update(conn.get(), inv).dynamic_set().where(inv.charId == charId and inv.slot == it);
    items[it].commitToUpdate<decltype(inv)>(update);
    conn->run(update);
  }
  for (auto it : toInsert) {
    auto insert = sqlpp::dynamic_insert_into(conn.get(), inv).dynamic_set();
    items[it].commitToInsert<decltype(inv)>(insert);
    insert.insert_list.add(inv.slot = it);
    insert.insert_list.add(inv.charId = charId);
    conn->run(insert);
  }
}

Entity EntitySystem::create_warpgate(std::string alias, int dest_map_id, float dest_x, float dest_y, float dest_z,
                    int map_id, float x, float y, float z, float angle,
                    float x_scale, float y_scale, float z_scale) {
  return {};
}

Entity EntitySystem::create_npc(std::string npc_lua, int npc_id, int map_id, float x, float y, float z, float angle) {
    Entity e = create();
    e.assign<BasicInfo>(id_manager_.get_free_id());
    e.assign<AdvancedInfo>();
    e.assign<CharacterInfo>();
    e.assign<Npc>(npc_id);
    auto pos = e.assign<Position>(x * 100, y * 100, map_id, 0);

    pos->z_ = static_cast<uint16_t>(z); //FIXME: that's weird
    pos->angle_ = angle;
    //e.assign<EntityApi>();
  return e;
}

Entity EntitySystem::create_spawner(std::string alias, int mob_id, int mob_count,
                   int spawner_limit, int spawner_interval, int spawner_range,
                   int map_id, float x, float y, float z) {
  return {};
}

void EntitySystem::bulk_destroy(const std::vector<Entity>& s) {
  std::lock_guard<std::mutex> lock(access_);
  toDestroy_.insert(toDestroy_.end(), s.begin(), s.end());
}

LuaScript::ScriptLoader& EntitySystem::get_script_loader() noexcept {
  return server_->get_script_loader();
}
