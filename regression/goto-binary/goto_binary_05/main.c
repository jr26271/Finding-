// esbmc  --output-goto main.goto

#ifdef __GNUC__
#include <assert.h>
#include <math.h>
#include <fenv.h>

float setRoundingModeAndCast(int mode, double d) {
  fesetround(mode);
  return (float)d;
}

void test (int mode, double d, float result) {
  float f = setRoundingModeAndCast(mode,d);
  assert(f == result);
  return;
}
#endif

int main (void)
{
  #if defined(__GNUC__) && defined(FE_DOWNWARD)
  test(FE_DOWNWARD, 0x1.0p+128, 0x1.fffffep+127f);
  #endif

  return 1;
}
