#include "lcddialog.h"

using namespace lcdgui;

void GuiDialog::setLcdService(LcdGuiService *service) {
  service_ = service;
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
  if(service_ != nullptr) {
    service_->closeDialog();
  }
}

void GuiDialog::dispose() {
  if(service_ != nullptr) {
    service_->disposeDialog();
  }
}

void GuiDialog::updateDisplay() {
  if(service_ != nullptr) {
    service_->updateDisplay();
  }
}