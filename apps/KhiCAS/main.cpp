#include <api.h>

extern "C" {
int ext_main();
}

namespace Ion {
namespace Timing {
uint64_t millis() {
  return millis();
}
}
}

int main() {
  return ext_main();
}

