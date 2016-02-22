#ifndef GUN_CONTROLLER_INCLUDED_5E443709_5CCF_4BF4_97B3_DDA49C682523
#define GUN_CONTROLLER_INCLUDED_5E443709_5CCF_4BF4_97B3_DDA49C682523


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>


/*!
  This controller will keep the gun_model state updated.
  It will also calculate the bullets and sped etc.
*/
class Gun_controller : public Core::Component
{
public:

  uint32_t              get_rtti() const override { return Component_type::gun_controller; }
  
  void                  on_update(const float dt) override;
  void                  on_event(const uint32_t id, const void *data, const size_t size_of_data) override;
  
  void                  fire_gun();
  void                  reload();

}; // class


#endif // inc guard