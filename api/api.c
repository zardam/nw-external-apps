#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

extern void (* const *_api_base)(void);

uint64_t millis(void) {
  return ((uint64_t (*)(void))_api_base[0])();
}

void msleep(uint32_t ms) {
  ((void (*)(uint32_t))_api_base[1])(ms);
}

uint64_t scanKeyboard(void) {
  return ((uint64_t (*)(void))_api_base[2])();
}

void pushRect(int16_t x, int16_t y, uint16_t w, uint16_t h, const uint16_t * pixels) {
  ((void (*)(int16_t, int16_t, uint16_t, uint16_t, const uint16_t *))_api_base[3])(x, y, w, h, pixels);
}

void pushRectUniform(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color) {
  ((void (*)(int16_t, int16_t, uint16_t, uint16_t, uint16_t))_api_base[4])(x, y, w, h, color);
}

int16_t drawTextLarge(const char *text, int16_t x, int16_t y, uint16_t fg, uint16_t bg) {
  return ((int16_t (*)(const char *, int16_t, int16_t, uint16_t, uint16_t))_api_base[5])(text, x, y, fg, bg);
}

int16_t drawTextSmall(const char *text, int16_t x, int16_t y, uint16_t fg, uint16_t bg) {
  return ((int16_t (*)(const char *, int16_t, int16_t, uint16_t, uint16_t))_api_base[6])(text, x, y, fg, bg);
}

void waitForVBlank(void) {
  ((void (*)(void))_api_base[7])();
}

void clipboardStore(const char *text) {
  ((void (*)(const char *))_api_base[8])(text);
}

const char * clipboardText(void) {
  return ((const char * (*)(void))_api_base[9])();
}

// khicas from here, subject to change
int numworks_draw_string(int x, int y, int c, int bg, const char * s, bool fake) {
  return ((int (*)(int, int, int, int, const char *, bool))_api_base[64])(x, y, c, bg, s, fake);
}

int numworks_draw_string_small(int x, int y, int c, int bg, const char * s, bool fake) {
  return ((int (*)(int, int, int, int, const char *, bool))_api_base[65])(x, y, c, bg, s, fake);
}

void numworks_set_pixel(int x, int y, int c) {
  ((void (*)(int, int, int))_api_base[66])(x, y, c);
}

void numworks_fill_rect(int x, int y, int w, int h, int c) {
  ((void (*)(int, int, int, int, int))_api_base[67])(x, y, w, h, c);
}

int numworks_get_pixel(int x, int y) {
  return ((int (*)(int, int))_api_base[68])(x, y);
}

void numworks_hide_graph() {
  ((void (*)(void))_api_base[69])();
}

void numworks_wait_1ms(int ms) {
  ((void (*)(int))_api_base[70])(ms);
}

bool waitforvblank() {
  return ((bool (*)(void))_api_base[71])();
}

void statuslinemsg(const char * msg) {
  ((void (*)(const char *))_api_base[72])(msg);
}

extern void *_heap_base;
extern void *_heap_ptr;
void statusline(int mode) {
  ((void (*)(int, size_t))_api_base[73])(mode, _heap_ptr - _heap_base);
}

int getkey(bool allow_suspend) {
  return ((int (*)(bool))_api_base[74])(allow_suspend);
}

void GetKey(int * key) {
  ((void (*)(int *))_api_base[75])(key);
}

bool alphawasactive() {
  return ((bool (*)(void))_api_base[76])();
}

void lock_alpha() {
  ((void (*)(void))_api_base[77])();
}

void reset_kbd() {
  ((void (*)(void))_api_base[78])();
}

bool back_key_pressed() {
  return ((bool (*)(void))_api_base[79])();
}

void enable_back_interrupt() {
  ((void (*)(void))_api_base[80])();
}

void disable_back_interrupt() {
  ((void (*)(void))_api_base[81])();
}

bool os_set_angle_unit(int mode) {
  return ((bool (*)(int))_api_base[82])(mode);
}

int os_get_angle_unit() {
  return ((int (*)(void))_api_base[83])();
}

int os_file_browser(const char ** filenames, int maxrecords, const char * extension) {
  return ((int (*)(const char **, int, const char *))_api_base[84])(filenames, maxrecords, extension);
}

bool file_exists(const char * filename) {
  return ((bool (*)(const char *))_api_base[85])(filename);
}

bool erase_file(const char * filename) {
  return ((bool (*)(const char *))_api_base[86])(filename);
}

const char * read_file(const char * filename) {
  return ((const char * (*)(const char *))_api_base[87])(filename);
}

bool write_file(const char * filename, const char * s, size_t len) {
  return ((bool (*)(const char *, const char *, size_t))_api_base[88])(filename,s,len);
}

const char * mp_hal_input(const char * prompt) {
  return ((const char * (*)(const char *))_api_base[89])(prompt);
}