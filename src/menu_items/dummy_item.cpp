#include "lcdmenu.h"

using namespace lcdgui;

void DummyMenuItem::build(LcdBuffer &buffer, const uint8_t &max_length) {
  buffer << text_;
}

bool DummyMenuItem::input(const uint8_t &input, gui_dialog dialog) {
  return EVENT_SEND;
}

DummyMenuItem::DummyMenuItem(const String &text) {
  text_ = text;
}

String DummyMenuItem::text() const {
  return { text_ };
}

void DummyMenuItem::text(const String &text) {
  text_ = text;
}

menu_item DummyMenuItem::clone() const {
  return new DummyMenuItem(text_);
}

ItemType DummyMenuItem::type() const {
  return ItemType::Dummy;
}
