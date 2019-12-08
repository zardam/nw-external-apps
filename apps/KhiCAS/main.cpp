#include <extapp_api.h>
#include <cstring>
#ifdef SIMULATOR
#include <FL/Fl.H>
#endif

#ifndef SIMULATOR
namespace Ion {
namespace Timing {
uint64_t millis() {
  return extapp_millis();
}
}
}

extern "C" const char * mp_hal_input(const char * prompt) {
  return 0;
}

#endif

extern "C" {

int ext_main();
void extapp_main() {
  ext_main();
}

uint64_t millis() {
  return extapp_millis();
}

int os_file_browser(const char ** filenames, int maxrecords, const char * extension) {
  int n = extapp_fileListWithExtension(filenames, maxrecords, extension, EXTAPP_RAM_FILE_SYSTEM);
  filenames[n] = 0;
  return n;
}

bool file_exists(const char * filename) {
  return extapp_fileExists(filename, EXTAPP_RAM_FILE_SYSTEM);
}

bool erase_file(const char * filename){
  return extapp_fileErase(filename, EXTAPP_RAM_FILE_SYSTEM);
}

const char * read_file(const char * filename){
  const char * ptr = extapp_fileRead(filename, NULL, EXTAPP_RAM_FILE_SYSTEM);
  if(ptr) {
    return ptr + 1;
  } else {
    return ptr;
  }
}

bool write_file(const char * filename, const char * content, size_t len) {
  if(len) {
    return extapp_fileWrite(filename, content, len, EXTAPP_RAM_FILE_SYSTEM);
  } else {
    return extapp_fileWrite(filename, content, strlen(content) + 1, EXTAPP_RAM_FILE_SYSTEM);
  }
}

int numworks_draw_string(int x, int y, int c, int bg, const char * text, bool fake) {
  return extapp_drawTextLarge(text, x, y + 18, c, bg, fake);
}

int numworks_draw_string_small(int x, int y, int c, int bg, const char * text, bool fake) {
  return extapp_drawTextSmall(text, x, y + 18, c, bg, fake);
}

bool waitforvblank() {
  return extapp_waitForVBlank();
}

void numworks_set_pixel(int x, int y, int color) {
  if (x<0 || x>=LCD_WIDTH || y<0 || y>=LCD_HEIGHT-18)
    return;
  extapp_pushRectUniform(x, y + 18, 1, 1, color);
}

void numworks_fill_rect(int x, int y, int w, int h, int c) {
  if (x<0) {
    w += x;
    x=0;
  }
  if (y<0) {
    h += y;
    y=0;
  }
  if (h+y>=LCD_HEIGHT-18)
    h=LCD_HEIGHT-18-y;
  if (x+w>=LCD_WIDTH)
    w=LCD_WIDTH-x;
  if (h<=0 || w<=0)
    return;
  extapp_pushRectUniform(x, y + 18, w, h, c);
}

int numworks_get_pixel(int x, int y) {
  uint16_t color;
  extapp_pullRect(x, y, 1, 1, &color);
  return color;
}

void numworks_wait_1ms(int ms) {
  for (int i=0; i<ms/128; ++i) {
#ifdef SIMULATOR
    extapp_msleep(0);
#endif
    uint64_t scan = extapp_scanKeyboard();
    if (scan & SCANCODE_Back) {
      return;
    }
    extapp_msleep(128);
  }
  extapp_msleep(ms % 128);
}

static bool interruptible = true;

bool back_key_pressed() {
  static int c = 0;

  ++c;
  if (!interruptible || c%1024 != 0) {
    return false;
  } else {
    #ifdef SIMULATOR
    extapp_msleep(0);
    #endif
    return extapp_scanKeyboard() & SCANCODE_Back;
  }
}

void enable_back_interrupt() {
  interruptible = true;
}

void disable_back_interrupt() {
  interruptible = false;
}

bool os_set_angle_unit(int mode) {
  return false;
}

int os_get_angle_unit() {
  return 0;
}

void numworks_hide_graph() {
}

void numworks_show_graph() {
}

void statuslinemsg(const char * msg) {
  uint16_t c;
  extapp_pullRect(0, 0, 1, 1, &c);
  extapp_pushRectUniform(0, 0, 280, 18, c);
  if(strlen(msg) > 25) {
    extapp_drawTextSmall(msg, 0, 0, 65535, c, false);
  } else {
    extapp_drawTextLarge(msg, 0, 0, 65535, c, false);
  }
}

void statusline(int mode, size_t heap) {
}

void lock_alpha() {
  extapp_lockAlpha();
}

void reset_kbd() {
  extapp_resetKeyboard();
}

static bool alphawasactive_ = false;
bool alphawasactive () {
  return alphawasactive_;
}

int getkey(bool allow_suspend) {
  return extapp_getKey(allow_suspend, &alphawasactive_);
}

void GetKey(int * key) {
  *key = getkey(true);
}

}