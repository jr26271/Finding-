#include <assert.h>

int main() {
  int v;
  v = 0;

  assert(v == 0);
  __ESBMC_init_object(&v);
  __ESBMC_assume(v > 0);
  assert(v > 0);
}
