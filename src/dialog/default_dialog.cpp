#include "../../include/lcddialog.h"

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

void GuiDialog::inputRotaryEncoder(LiquidCrystalGui &lcg, const bool cw, const bool ccw, const bool pressed) {
  if (cw) {
    input(lcg, LCD_INPUT_RIGHT);
    return;
  }
  if (ccw) {
    input(lcg, LCD_INPUT_LEFT);
    return;
  }
  if (pressed) {
    input(lcg, LCD_INPUT_OK);
  }
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

void GuiDialog::listener(const dialog_event_t listener) {
  event_ = listener;
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

void GuiDialog::openChild(gui_dialog dialog) {
  if(lcg_instance != nullptr) {
    dialog->parentDialog(this);
    lcg_instance->openDialog(dialog);
  }
}

void GuiDialog::dispatchDialogEvent(uint8_t event) {
  if (event_ != nullptr) {
    event_(this, event);
  }
}


DialogType GuiDialog::type() const {
  return DialogType::Custom;
}
