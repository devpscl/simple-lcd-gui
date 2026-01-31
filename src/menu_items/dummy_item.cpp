#include "../../include/lcdmenu.h"

using namespace lcdgui;

void DummyMenuItem::build(LcdBuffer &buffer, const uint8_t &max_length) {
  buffer << text_;
}

bool DummyMenuItem::input(const uint8_t &input, menu_dialog dialog) {
  return EVENT_SEND;
}

DummyMenuItem::DummyMenuItem(const String& text) {
  text_ = text;
}

DummyMenuItem::DummyMenuItem(const char* text) {
  text_ = String(text);
}

String DummyMenuItem::text() const {
  return  text_ ;
}

void DummyMenuItem::text(const String& text) {
  text_ = text;
}

void DummyMenuItem::text(const char* text) {
  text_ = String(text);
}


menu_item DummyMenuItem::clone() const {
  return new DummyMenuItem(text_);
}

ItemType DummyMenuItem::type() const {
  return ItemType::Dummy;
}
