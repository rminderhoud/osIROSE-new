registerNpc(656, {
  walk_speed        = 0,
  run_speed         = 450,
  scale             = 670,
  r_weapon          = 550,
  l_weapon          = 1055,
  level             = 0,
  hp                = 167,
  attack            = 48,
  hit               = 754,
  def               = 397,
  res               = 731,
  avoid             = 484,
  attack_spd        = 165,
  is_magic_damage   = 108,
  ai_type           = 0,
  give_exp          = 352,
  drop_type         = 276,
  drop_money        = 525,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
  hit_material_type = 7,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 20,
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