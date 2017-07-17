//Taken from Michael Ehrmann source code of SunClock https://github.com/mehrmann/pebble-sunclock
#include "math.h"

/*
 * loosely based on
 * - http://stackoverflow.com/questions/11261170/c-and-maths-fast-approximation-of-a-trigonometric-function
 * - http://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
 */

float my_floor(float x)
{
  return ((int)x);
}

float my_fabs(float x)
{
  if (x<0) return -x;
  return x;
}

float my_atan(float x)
{
  if (x>0)
  {
    return (M_PI/2)*(0.596227*x + x*x)/(1 + 2*0.596227*x + x*x);
  }
  else
  {
    return -(my_atan(-x));
  }
}

/* not quite rint(), i.e. results not properly rounded to nearest-or-even */
float my_rint (float x)
{
  float t = my_floor (my_fabs(x) + 0.5);
  return (x < 0.0) ? -t : t;
}

/* minimax approximation to cos on [-pi/4, pi/4] with rel. err. ~= 7.5e-13 */
float cos_core (float x)
{
  float x8, x4, x2;
  x2 = x * x;
  x4 = x2 * x2;
  x8 = x4 * x4;
  /* evaluate polynomial using Estrin's scheme */
  return (-2.7236370439787708e-7 * x2 + 2.4799852696610628e-5) * x8 +
         (-1.3888885054799695e-3 * x2 + 4.1666666636943683e-2) * x4 +
         (-4.9999999999963024e-1 * x2 + 1.0000000000000000e+0);
}

/* minimax approximation to sin on [-pi/4, pi/4] with rel. err. ~= 5.5e-12 */
float sin_core (float x)
{
  float x4, x2;
  x2 = x * x;
  x4 = x2 * x2;
  /* evaluate polynomial using a mix of Estrin's and Horner's scheme */
  return ((2.7181216275479732e-6 * x2 - 1.9839312269456257e-4) * x4 +
          (8.3333293048425631e-3 * x2 - 1.6666666640797048e-1)) * x2 * x + x;
}

/* relative error < 7e-12 on [-50000, 50000] */
float my_sin (float x)
{
  float q, t;
  int quadrant;
  /* Cody-Waite style argument reduction */
  q = my_rint (x * 6.3661977236758138e-1);
  quadrant = (int)q;
  t = x - q * 1.5707963267923333e+00;
  t = t - q * 2.5633441515945189e-12;
  if (quadrant & 1) {
    t = cos_core(t);
  } else {
    t = sin_core(t);
  }
  return (quadrant & 2) ? -t : t;
}

float my_cos(float x)
{
  return my_sin(x + (M_PI/2));
}

float my_tan(float x)
{
  return my_sin(x) / my_cos(x);
}
