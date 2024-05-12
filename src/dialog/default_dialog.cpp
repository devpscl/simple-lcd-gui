#include "lcddialog.h"

using namespace lcdgui;

void GuiDialog::setInstance(LiquidCrystalGui* instance) {
  lcg_instance = instance;
}

void GuiDialog::render(LiquidCrystalGui& lcg) {
  //do nothing
}

void GuiDialog::input(LiquidCrystalGui& lcg, const uint8_t &input) {
  //do nothing
}

GuiDialog::~GuiDialog() = default;

gui_dialog GuiDialog::clone() {
  return new GuiDialog();
}

void GuiDialog::close() {
  if(lcg_instance != nullptr) {
    lcg_instance->closeDialog();
  }
}

void GuiDialog::dispose() {
  if(lcg_instance != nullptr) {
    lcg_instance->disposeDialog();
  }
}

void GuiDialog::updateDisplay() {
  if(lcg_instance != nullptr) {
    lcg_instance->updateDisplay();
  }
}