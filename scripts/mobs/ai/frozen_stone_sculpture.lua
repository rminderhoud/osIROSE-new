registerNpc(652, {
  walk_speed        = 0,
  run_speed         = 100,
  scale             = 600,
  r_weapon          = 130,
  l_weapon          = 1093,
  level             = 0,
  hp                = 152,
  attack            = 32,
  hit               = 676,
  def               = 337,
  res               = 415,
  avoid             = 611,
  attack_spd        = 243,
  is_magic_damage   = 109,
  ai_type           = 0,
  give_exp          = 348,
  drop_type         = 276,
  drop_money        = 521,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 900,
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