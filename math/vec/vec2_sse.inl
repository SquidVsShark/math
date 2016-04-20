#ifndef VECTOR2_SSE_INCLUDED_
#define VECTOR2_SSE_INCLUDED_
#ifdef MATH_USE_SSE


/*
  Vector2
  2D vector sse impl.
*/


#include "vec_types.hpp"
#include "../general/general.hpp"
#include <assert.h>


namespace math {


// Constants

vec2
vec2_zero()
{
  const float zero = 0.f;
  return(_mm_load_ps1(&zero));
}


vec2
vec2_one()
{
  const float one = 1.f;
  return(_mm_load_ps1(&one));
}


vec2
vec2_zero_one()
{
  return vec2_init(0.f, 1.f);
}

// Initialize

vec2
vec2_init(const float val)
{
  return vec2_init(val, val);
}


vec2
vec2_init(const float x, const float y)
{
  const float zero = 0;
  return(_mm_set_ps(zero, zero, y, x));
}


vec2
vec2_init_with_array(const float *arr)
{
  return(_mm_load_ps(arr));
}


// Get components.

float
vec2_get_x(const vec2 vec)
{
  return vec[0];
}


float
vec2_get_y(const vec2 vec)
{
  return vec[1];
}


void
vec2_to_array(const vec2 a, float *out_array)
{
  // TODO: Need to make sure out_array is 16 bytes aligned some how.
  //_mm_store_ps(out_array, a);
}


vec2
vec2_add(const vec2 a, const vec2 b)
{
  return(_mm_add_ps(a, b));
}


vec2
vec2_subtract(const vec2 a, const vec2 b)
{
  return(_mm_sub_ps(a, b));
}


vec2
vec2_multiply(const vec2 a, const vec2 b)
{
  return(_mm_mul_ps(a, b));
}


vec2
vec2_divide(const vec2 a, const vec2 b)
{
  return(_mm_div_ps(a, b));
}


vec2
vec2_lerp(const vec2 start, const vec2 end, const float dt)
{
  const vec2 difference = vec2_subtract(end, start);
  const vec2 scaled     = vec2_scale(difference, dt);
  const vec2 position   = vec2_add(start, scaled);

  return position;
}


vec2
vec2_scale(const vec2 a, const float scale)
{
  const vec2 scale_vec = vec2_init(scale);
  return vec2_multiply(a, scale_vec);
}


vec2
vec2_normalize(const vec2 a)
{
  const float length = vec2_length(a);

  assert(length != 0); // Don't pass zero vectors. (0,0);

  return vec2_scale(a, (1.f / length));
}


float
vec2_length(const vec2 a)
{
  // TODO: Better way with sse?

  const float squared = vec2_get_x(a) * vec2_get_x(a) +
                         vec2_get_y(a) * vec2_get_y(a);
  return sqrt(squared);
}


float
vec2_cross(const vec2 a, const vec2 b)
{
  return (vec2_get_x(a) * vec2_get_y(b)) -
         (vec2_get_y(a) * vec2_get_x(b));
}


float
vec2_dot(const vec2 a, const vec2 b)
{
  return (vec2_get_x(a) * vec2_get_x(b)) +
         (vec2_get_y(a) * vec2_get_y(b));
}


bool
vec2_is_equal(const vec2 a, const vec2 b)
{
	return
  (
    (vec2_get_x(a) == vec2_get_x(b))
    &&
    (vec2_get_y(a) == vec2_get_y(b))
  );
}


bool
vec2_is_not_equal(const vec2 a, const vec2 b)
{
	return !vec2_is_equal(a, b);
}


bool
vec2_is_near(const vec2 a, const vec2 b, const float error)
{
  return(
    is_near(vec2_get_x(a), vec2_get_x(b), error)
    &&
    is_near(vec2_get_y(a), vec2_get_y(b), error)
  );
}


bool
vec2_is_not_near(const vec2 a, const vec2 b, const float error)
{
	return !vec2_is_near(a, b, error);
}


} // ns


#endif // use sse
#endif // include guard