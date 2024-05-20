#include "lcddialog.h"
#ifdef LCD_U8G2_

using namespace lcdgui;

DialogType U8G2Dialog::type() const {
  return DialogType::RenderDialog;
}

void U8G2Dialog::render(LiquidCrystalGui &lcg) {
  render(lcg.nativeLcd()->handle(), lcg);
}

void U8G2Dialog::render(U8G2 &u8g2, LiquidCrystalGui &lcg) {
  //do nothing
}

#endif