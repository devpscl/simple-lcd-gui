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

void GuiDialog::enable(LiquidCrystalGui& lcg) {
  //do nothing
}

void GuiDialog::disable(LiquidCrystalGui &lcg) {
  //do nothing
}

GuiDialog::GuiDialog(gui_dialog parent_dialog) {
  parent_dialog_ = parent_dialog;
}

GuiDialog::GuiDialog() : GuiDialog::GuiDialog(nullptr) {
  //do nothing
}

GuiDialog::~GuiDialog() = default;

gui_dialog GuiDialog::clone() {
  return new GuiDialog();
}

gui_dialog GuiDialog::parentDialog() {
  return parent_dialog_;
}

void GuiDialog::parentDialog(gui_dialog parent_dialog) {
  parent_dialog_ = parent_dialog;
}

void GuiDialog::close() {
  if(lcg_instance != nullptr && lcg_instance->currentDialog() == this) {
    lcg_instance->closeDialog();
    if(parent_dialog_ != nullptr) {
      lcg_instance->openDialog(parent_dialog_);
    }
  }
}

void GuiDialog::closeAll() {
  lcg_instance->closeDialog();
}

void GuiDialog::dispose() {
  if(lcg_instance != nullptr) {
    if(lcg_instance->currentDialog() == this) {
      lcg_instance->closeDialog();
      if(parent_dialog_ != nullptr) {
        lcg_instance->openDialog(parent_dialog_);
      }
    }
    delete this;
  }
}

void GuiDialog::updateDisplay() {
  if(lcg_instance != nullptr) {
    lcg_instance->updateDisplay();
  }
}

bool GuiDialog::isOpened() {
  if(lcg_instance == nullptr) {
    return false;
  }
  return lcg_instance->currentDialog() == this;
}

DialogType GuiDialog::type() const {
  return DialogType::Custom;
}
