registerNpc(298, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 300,
  r_weapon          = 50,
  l_weapon          = 1128,
  level             = 0,
  hp                = 1,
  attack            = 10,
  hit               = 18,
  def               = 65,
  res               = 31,
  avoid             = 16,
  attack_spd        = 10,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 89,
  drop_type         = 1,
  drop_money        = 0,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 90,
  sell_tab0         = 90,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 19,
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