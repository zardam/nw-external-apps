#include "os.h"
#include "giac.h"
#include "kdisplay.h"

using namespace giac;
using namespace std;

__attribute__((noinline)) int main_(int argc,char ** argv){
  lcd_init(lcd_type()); // clrscr();
#if 0 // def MICROPY_LIB 
  // mp_stack_ctrl_init();
  char * heap=micropy_init(64e3,512e3);
  if (!heap)
    return 1;
#endif
  vx_var=identificateur("x");
  giac::context c;
  xcas::console_main(&c);
#if 0 // def MICROPY_LIB
  mp_deinit(); free(heap);
#endif
  return 0;
}

int main(int argc,char ** argv){
#ifdef MICROPY_LIB
  mp_stack_ctrl_init();
#endif
  main_(argc,argv);
}

// old code
#if 0
  // identificateur x("x");
  // COUT << _factor(pow(x,4,&c)-1,&c) << endl;
  COUT << "Enter expression to eval, 0 to quit, ?command for help " << endl;
  for (unsigned i=0;;++i){
    //console_cin.foreground_color(nio::COLOR_RED);
    COUT << i << ">> ";
    //console_cin.foreground_color(nio::COLOR_BLACK);
    string s;
    CIN >> s;
#ifdef MICROPY_LIB
    micropy_eval(s.c_str()); 
#else
    //console_cin.foreground_color(nio::COLOR_GREEN);
    ctrl_c=interrupted=false;
    giac::gen g(s,&c);
    if (g==0)
      return 0;
    // COUT << "type " << g.type << endl;
    // if (g.type==_SYMB) COUT << g._SYMBptr->sommet << endl;
    // COUT << "before eval " << g << endl; wait_key_pressed();
    g=eval(g,&c);
    //console_cin.foreground_color(nio::COLOR_BLUE);
    COUT << g << endl;
#endif
  }
#if 0 // def MICROPY_LIB
  mp_deinit();
  if (heap) free(heap);
#endif
  return 0;
#endif

