#include <core/interface/entity.hpp>
#include "../common/ids_component_types.hpp"
#include "npc_actor_controller.hpp"
#include "actor_base.hpp"
#include "../common/ids_game_events.hpp"
#include <iostream>


Npc_actor_controller::Npc_actor_controller()
{
  
}


void
Npc_actor_controller::on_update(const float dt)
{
  
}


void
Npc_actor_controller::on_event(const uint32_t id,
                                  const void *data,
                                  const size_t size_of_data)
{
  switch(id)
  {
    case(Game_event_id::got_shot):
      // Get model apply damage.
      Actor_base *base = get_entity().get_component<Actor_base>(Component_type::actor);
      base->take_damage();
  }
}