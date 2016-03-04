#ifndef ACTOR_CONTROLLER_INCLUDED_8C5C8BE7_A2D8_4BE1_B46E_8FC50CB81479
#define ACTOR_CONTROLLER_INCLUDED_8C5C8BE7_A2D8_4BE1_B46E_8FC50CB81479


#include <core/interface/component.hpp>
#include <core/input/controller.hpp>


class Actor_controller : public Core::Component
{
private:

  uint32_t            get_rtti() const override { return 1; };

  void                on_start() override;
  void                on_update(const float dt) override;

private:

}; // class


#endif // inc guard