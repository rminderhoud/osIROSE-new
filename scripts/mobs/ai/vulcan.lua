registerNpc(365, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 750,
  r_weapon          = 155,
  l_weapon          = 0,
  level             = 0,
  hp                = 83,
  attack            = 32,
  hit               = 379,
  def               = 189,
  res               = 303,
  avoid             = 177,
  attack_spd        = 124,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 143,
  drop_type         = 71,
  drop_money        = 361,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 49,
  sell_tab0         = 49,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 17,
  height            = 0
});
function OnInit(entity)
  return true
end

function OnCreate(entity)
  return true
end

function OnDelete(entity)
  return true
end

function OnDead(entity)
end

function OnDamaged(entity)
end