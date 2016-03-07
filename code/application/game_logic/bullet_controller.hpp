#ifndef BULLET_CONTROLLER_INCLUDED_1FD4957E_331B_47C9_A0FD_99A1A087B927
#define BULLET_CONTROLLER_INCLUDED_1FD4957E_331B_47C9_A0FD_99A1A087B927


#include <core/interface/component.hpp>
#include <math/vec/vec3.hpp>


class Bullet_controller : public Core::Component
{
public:

  void          set_position(const math::vec3 position);
  void          set_direction(const uint32_t i);

private:

  void          on_update(const float dt) override;
  
private:

  float         m_direction = -1.f;
  const float   m_death_zone_top = 10.f;
  const float   m_death_zone_bottom = -10.f;

}; // class



#endif // inc guard