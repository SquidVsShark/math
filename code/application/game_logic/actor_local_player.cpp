#include "actor_local_player.hpp"
#include <renderer/renderer.hpp>
#include <btBulletCollisionCommon.h>
#include <systems/transform/transform.hpp>
#include <application/entity_factory.hpp>
#include <data/data.hpp>


Actor_local_player::Actor_local_player()
{

}


void
Actor_local_player::on_start()
{
 cube_id = Entity_factory::create_placement_cube(world_data).get_id();
}


void
Actor_local_player::on_update(const float dt)
{
  auto apply_gravity = [&](const Entity::Entity_id ent)
  {
    struct Kine_actor
    {
      bool is_grounded = false;
    } act;
  
    // Get transform of controller.

    const math::transform curr_trans = get_transform();
    
    // Cast ray downwards
    const btVector3 btFrom(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position), math::vec3_get_z(curr_trans.position));
    const btVector3 btTo(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position) - 2, math::vec3_get_z(curr_trans.position));
    btCollisionWorld::ClosestRayResultCallback feet_test(btFrom, btTo);
    
    m_world->dynamics_world.rayTest(btFrom, btTo, feet_test);

    Renderer::debug_line(btFrom, btTo, btVector3(1, 1, 0));
    
    // If not hit anything then go downwards.
    // Gravity
    if(!feet_test.hasHit())
    {
      const math::vec3 down = math::vec3_init(0, -0.01, 0);
      
      math::transform new_trans = curr_trans;
      new_trans.position = math::vec3_add(curr_trans.position, down);
      
      set_transform(new_trans);
    }
    else
    {
      const auto norm = feet_test.m_hitPointWorld;

      math::transform new_trans = curr_trans;
      math::vec3 pos = math::vec3_init(math::vec3_get_x(curr_trans.position), norm.y() + 2, math::vec3_get_z(curr_trans.position));
      new_trans.position = pos;
      
     set_transform(new_trans);
      
      act.is_grounded = true;
    }
    
    math::vec3 fwd;
    Transform::get_fwd_vec(&curr_trans, &fwd);
    fwd = math::vec3_scale(fwd, 3);
    
    const btVector3 fwdVec(math::vec3_get_x(fwd), math::vec3_get_y(fwd), math::vec3_get_z(fwd));
    const btVector3 btFaceStart(btFrom);
    const btVector3 btFwd = btFaceStart + fwdVec;
    
    btCollisionWorld::ClosestRayResultCallback face_ray(btFaceStart, btFwd);
    m_world->dynamics_world.rayTest(btFaceStart, btFwd, face_ray);
    
    Renderer::debug_line(btFaceStart, btFwd, btVector3(1, 0, 0));
    
    if(face_ray.hasHit())
    {
      // Draw cube at that point.
      Data::Entity cube_entity;
      Data::world_find_entity(world_data, &cube_entity, cube_id);
      
      if(cube_entity.is_valid())
      {
        auto trans = cube_entity.get_transform();
        const math::vec3 pos = math::vec3_init(face_ray.m_hitPointWorld.x(), face_ray.m_hitPointWorld.y(), face_ray.m_hitPointWorld.z());
        trans.position = pos;
        cube_entity.set_transform(trans);
        
        if(m_place_node)
        {
          auto ent = Entity_factory::create_random_cube(world_data);
          
          auto trans = ent.get_transform();
          trans.position = cube_entity.get_transform().position;
          ent.set_transform(trans);
          
          // Join rbs.
          void *user_ptr = face_ray.m_collisionObject->getUserPointer();
          const std::size_t ent_id = (std::size_t)user_ptr;
          const Entity::Entity_id collided_id = Entity::uint_as_entity(static_cast<uint32_t>(ent_id));
          
          ent.set_parent(collided_id);
        }
      }
    }
    
      m_place_node = false;
  };
  
  apply_gravity(get_entity());
  
  auto local_controls = [&](const Entity::Entity_id id)
  {
    const float delta_time = dt;
    
    Data::Entity ent;
    Data::world_find_entity(world_data, &ent, id);
    
    math::vec3 accum_movement = math::vec3_zero();
    const math::transform move_trans = ent.get_transform();
    
    if(math::vec3_get_z(pending_input) > 0)
    {
      const math::vec3 move_fwd = math::vec3_init(0,0,-1);
      accum_movement = math::vec3_add(accum_movement, move_fwd);
    }

    if(math::vec3_get_z(pending_input) < 0)
    {
      const math::vec3 move_fwd = math::vec3_init(0,0,+1);
      accum_movement = math::vec3_add(accum_movement, move_fwd);
    }
    
    if(math::vec3_get_x(pending_input) > 0)
    {
      const math::vec3 move_fwd = math::vec3_init(-1,0,0);
      accum_movement = math::vec3_add(accum_movement, move_fwd);
    }
    
    if(math::vec3_get_x(pending_input) < 0)
    {
      const math::vec3 move_fwd = math::vec3_init(+1,0,0);
      accum_movement = math::vec3_add(accum_movement, move_fwd);
    }
    
    pending_input = math::vec3_zero();
    
    if(math::vec3_length(accum_movement) != 0)
    {
      math::vec3 left;
      Transform::get_left_vec(&move_trans, &left);
      
      math::vec3 fwd = math::vec3_cross(Transform::get_world_up(), left);
    
//      const math::vec3 norm_movement            = math::vec3_normalize(accum_movement);
 //     const math::vec3 rotated_movement         = math::quat_rotate_point(move_trans.rotation, norm_movement);
//      const math::vec3 corrected_rotation       = math::vec3_init(math::vec3_get_x(rotated_movement), 0.f, math::vec3_get_z(rotated_movement)); // We're not interested in y movement.
      const math::vec3 norm_corrected_rotation  = math::vec3_normalize(fwd);
      const math::vec3 scaled_movement          = math::vec3_scale(norm_corrected_rotation, delta_time);
      const math::vec3 new_pos                  = math::vec3_add(move_trans.position, scaled_movement);
      const math::transform new_trans           = math::transform_init(new_pos, move_trans.scale, move_trans.rotation);
      
      ent.set_transform(new_trans);
    }
    
    // Rotation
    {
      const float rot_rad                 = static_cast<float>(math::vec2_get_y(head_rotations));
      const math::quat rot                = math::quat_init_with_axis_angle(0, 1, 0, rot_rad);
      const math::transform rot_trans     = ent.get_transform();
      const math::transform new_rot_trans = math::transform_init(rot_trans.position, rot_trans.scale, rot);
      
      ent.set_transform(new_rot_trans);
    }
    
    // Head
    {
      const math::transform rot_trans     = ent.get_transform();
      const float rot_rad                 = static_cast<float>(math::vec2_get_x(head_rotations));
      const math::vec3 axis               = math::vec3_init(1,0,0);
      const math::vec3 adjusted_axis      = math::quat_rotate_point(rot_trans.rotation, axis);
      const math::quat rot                = math::quat_init_with_axis_angle(adjusted_axis, rot_rad);
      const math::quat new_rot            = math::quat_multiply(rot_trans.rotation, rot);
      const math::transform new_rot_trans = math::transform_init(rot_trans.position, rot_trans.scale, new_rot);
      
      ent.set_transform(new_rot_trans);
    }
  };
  
  local_controls(get_entity());
}


void
Actor_local_player::move_fwd(const float fwd)
{
  const float accum_fwd = math::vec3_get_z(pending_input) + fwd;
  pending_input = math::vec3_init(math::vec3_get_x(pending_input), math::vec3_get_y(pending_input), accum_fwd);
}


void
Actor_local_player::move_right(const float right)
{
  const float accum_right = math::vec3_get_x(pending_input) + right;
  pending_input = math::vec3_init(math::vec3_get_x(pending_input), accum_right, math::vec3_get_z(pending_input));
}


void
Actor_local_player::action()
{
  //Entity_factory::create_random_cube(m_data_pool, pending_rbs, model_pool, texture_pool);
  m_place_node = true;
}


void
Actor_local_player::look_up(const float up)
{
  const float accum_up      = math::vec2_get_x(head_rotations) + up;
  const float clamped_accum = math::clamp(accum_up, +math::quart_tau(), -math::quart_tau());
  
  head_rotations = math::vec2_init(clamped_accum, math::vec2_get_y(head_rotations));
}


void
Actor_local_player::turn_right(const float right)
{
  const float accum_right = math::vec2_get_y(head_rotations) + right;
  head_rotations = math::vec2_init(math::vec2_get_x(head_rotations), accum_right);
}