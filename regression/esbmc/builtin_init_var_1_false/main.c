#include <assert.h>

int main() {
  int v[10];
  v[9] = 5;

  assert(v[9] == 5);
  __ESBMC_init_object(v);
  assert(v[9] == 5);
}
