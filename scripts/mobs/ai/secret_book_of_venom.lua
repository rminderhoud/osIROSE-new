registerNpc(637, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 780,
  r_weapon          = 228,
  l_weapon          = 0,
  level             = 0,
  hp                = 128,
  attack            = 44,
  hit               = 556,
  def               = 302,
  res               = 524,
  avoid             = 340,
  attack_spd        = 118,
  is_magic_damage   = 108,
  ai_type           = 0,
  give_exp          = 342,
  drop_type         = 276,
  drop_money        = 506,
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