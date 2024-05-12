#include "lcddialog.h"

using namespace lcdgui;

void GuiDialog::setLcdService(LcdGuiService *service) {
  lcd_gui_service = service;
}

void GuiDialog::render(lcd_native_type lcd, LcdGuiService &service) {
  //do nothing
}

void GuiDialog::input(const uint8_t &input, LcdGuiService &service) {
  //do nothing
}

GuiDialog::~GuiDialog() = default;

gui_dialog GuiDialog::clone() {
  return new GuiDialog();
}

void GuiDialog::close() {
  if(lcd_gui_service != nullptr) {
    lcd_gui_service->closeDialog();
  }
}

void GuiDialog::dispose() {
  if(lcd_gui_service != nullptr) {
    lcd_gui_service->disposeDialog();
  }
}

void GuiDialog::updateDisplay() {
  if(lcd_gui_service != nullptr) {
    lcd_gui_service->updateDisplay();
  }
}