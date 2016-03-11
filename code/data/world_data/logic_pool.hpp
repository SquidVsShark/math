#ifndef LOGIC_POOL_INCLUDED_2879D607_1430_4D5C_8806_974D5294E9F5
#define LOGIC_POOL_INCLUDED_2879D607_1430_4D5C_8806_974D5294E9F5


#include <core/interface/component.hpp>
#include <core/entity_id.hpp>
#include <stddef.h>


#define LOGIC_POOL_NUMBER_OF_SCRIPTS 512
#define LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE 128


namespace World_data {


/*
  Logic pool stores the application layers code.
  This memory pool allows us to store ptrs to the registered hooks.
  This allows us to only call hooks that have been registered saving on time.
  
  This is prob the most dangerous thing right now.
*/
struct Logic_pool
{
  Core::Entity_id           *entity_id;
  void*                     *object_locations;
  
  void*                     *objects_in_use; // This is a stack type thing.
  uint32_t                  objects_in_use_size = 0;
  
  void*                     *objects_on_start_pending_hooks;
  uint32_t                  objects_on_start_pending_hooks_size = 0;

  uint8_t                   *storage; // LOGIC_POOL_NUMBER_OF_SCRIPTS * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE
  
  const uint32_t            storage_size = LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE;
  const uint32_t            size         = LOGIC_POOL_NUMBER_OF_SCRIPTS;
};


/*!
  Initialises the pool.
*/
void
logic_pool_init(Logic_pool *pool);


/*!
  Returns a pointer to a valid location for a new object
*/
void*
logic_pool_get_slot(Logic_pool *pool, const Core::Entity_id id);


/*!
  Return how many components are attached to an entity.
*/
uint32_t
logic_pool_get_slot_count(Logic_pool *pool, const Core::Entity_id id);


/*!
  Removes all the slots associated with an entity.
*/
void
logic_pool_free_slots(Logic_pool *pool, const Core::Entity_id id);


// ** EVENT HOOKS ** //
/* These methods call logic event hooks. */


/*!
  Calls the on_start hooks of any pending logic scripts.
  After calling this the objects are removed from this data source and cannot be re-added.
*/
void
logic_pool_on_start_hook(Logic_pool *pool);


/*!
  Calls the on_update hooks of any pending logic scripts.
*/
void
logic_pool_on_early_update_hook(Logic_pool *pool, const float delta_time);


/*!
  Calls the on_update hooks of any pending logic scripts.
*/
void
logic_pool_on_update_hook(Logic_pool *pool, const float delta_time);


/*!
  Call the on_collision hooks.
*/
void
logic_pool_on_collision_hook(Logic_pool *pool, const Core::Entity_id id_a, const Core::Entity_id id_b);


} // ns


#endif // inc guard