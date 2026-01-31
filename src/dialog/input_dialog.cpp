#include "../../include/lcddialog.h"

using namespace lcdgui;

void InputDialog::render(LiquidCrystalGui &lcg) {
  const uint8_t& rows = lcg.rows();
  const uint8_t& cols = lcg.columns();
  lcg.showCursor(false);
  size_t str_len = buffer_.length();
  uint8_t cursor_row = 0;
  for(uint8_t row = 0; row < rows; row++) {
    if(row == 0) {
      LcdBuffer buffer;
      buffer << title_;
      buffer.resize(cols);
      lcg.cursor(0, row);
      lcg.print(buffer);
      continue;
    }
    if((rows == 2 && row == 1) || (rows > 2 && row == 2)) {
      LcdBuffer buf;
      buf << buffer_;
      buf.resize(cols);
      lcg.cursor(0, row);
      lcg.print(buf);
      cursor_row = row;
      continue;
    }
    lcg.clearRow(row);
  }
  lcg.cursor(str_len >= cols ? cols - 1 : str_len, cursor_row);
  lcg.showCursor(true);
}

void InputDialog::input(LiquidCrystalGui &lcg, const uint8_t &input) {
  if(input_event_ != nullptr) {
    input_event_(input);
  }
}

void InputDialog::enable(LiquidCrystalGui &lcg) {
  lcg.flags(FLAG_CURSOR_BLINKING);
}

void InputDialog::disable(LiquidCrystalGui &lcg) {
  lcg.flags(0);
}

InputDialog::InputDialog(gui_dialog parent, const String &title, input_event_t input_event,
                         const String &input) : GuiDialog(parent) {
  title_ = title;
  input_event_ = input_event;
  buffer_ = input;
}

InputDialog::InputDialog(const String &title, input_event_t input_event,
                         const String &input) : InputDialog::InputDialog(nullptr, title, input_event, input) {}

String InputDialog::title() {
  return { title_ };
}

String &InputDialog::input() {
  return buffer_;
}

void InputDialog::put(const char *cstr) {
  put(String(cstr));
}

void InputDialog::put(char ch) {
  put(String(ch));
}

void InputDialog::put(const String &str) {
  buffer_ += str;
  if(lcg_instance == nullptr) {
    return;
  }
  const uint8_t& cols = lcg_instance->columns();
  if(buffer_.length() > cols) {
    buffer_.remove(cols, 0xFFFF);
  }
}

void InputDialog::removeChars(uint8_t count) {
  size_t len = buffer_.length();
  buffer_ = buffer_.substring(0, count > len ? 0 : len - count);
}

gui_dialog InputDialog::clone() {
  return new InputDialog(title_, input_event_, buffer_);
}

DialogType InputDialog::type() const {
  return DialogType::InputDialog;
}
