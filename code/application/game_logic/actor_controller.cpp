#include "actor_controller.hpp"
#include "bullet_controller.hpp"
#include <core/interface/entity.hpp>
#include <core/input/input.hpp>
#include <math/general/general.hpp>
#include <iostream>
#include "../level_functions.hpp"
#include "entity_factory.hpp"


void
Actor_controller::on_start()
{
  std::cout << "I output" << std::endl;
}


void
Actor_controller::on_update(const float dt)
{
  const Core::Input::Controller controller = Core::Input::get_controller(Core::Input::Player::one);
  m_position += controller.get_axis(0).x * dt;
  
  const math::vec3 position = Level::get_point_on_cirlce(m_position);
 
  Core::Transform curr_trans = get_entity().get_transform();
  curr_trans.set_position(position);
  
  get_entity().set_transform(curr_trans);
  
  
  if(controller.is_button_down(Core::Input::Button::button_0))
  {
    Core::Entity bullet = Entity_factory::create_bullet(m_world_data);

    Bullet_controller *bullet_controller = bullet.get_component<Bullet_controller>(0);
    assert(bullet_controller);
    
    if(bullet_controller)
    {
      bullet_controller->set_position(get_entity().get_transform().get_position());
      bullet_controller->set_direction(-1);
    }
  }
  
//  // Movement
//  math::vec3 new_pos = curr_trans.get_position();
//  {
//    const Core::Input::Axis movement = controller.get_axis(0);
//    const float fwd  = movement.y * dt;
//    const float left = movement.x * dt;
//
//    const math::vec3 curr_pos = curr_trans.get_position();
//    
//    const math::vec3 scaled_fwd = math::vec3_scale(curr_trans.get_forward(), fwd);
//    const math::vec3 scaled_left = math::vec3_scale(curr_trans.get_left(), left);
//    
//    const math::vec3 move_fwd = math::vec3_add(curr_pos, scaled_fwd);
//    const math::vec3 move_left = math::vec3_add(move_fwd, scaled_left);
//    
//    new_pos = move_left;
//  }
  
//  Core::Transform new_trans = curr_trans;
//  new_trans.set_position(new_pos);
  
//  get_entity().set_transform(new_trans);
}
