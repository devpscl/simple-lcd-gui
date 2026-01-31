#include "../../include/lcdmenu.h"

using namespace lcdgui;

void MenuItem::build(LcdBuffer &buffer, const uint8_t &max_length) {
  //do nothing
}

bool MenuItem::input(const uint8_t &input, menu_dialog dialog) {
  return EVENT_SEND;
}

menu_item MenuItem::clone() const {
  return new MenuItem();
}

ItemType MenuItem::type() const {
  return ItemType::Custom;
}

bool MenuItem::is_multi_editable() const {
  return false;
}

MenuItem::~MenuItem() = default;
