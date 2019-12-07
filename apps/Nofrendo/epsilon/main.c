#include <api.h>
#include <nofrendo.h>

void main() {
  pushRectUniform(0, 0, LCD_WIDTH, LCD_HEIGHT, 0);
  nofrendo_main(0, NULL);
}
