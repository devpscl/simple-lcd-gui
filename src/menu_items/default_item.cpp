#include "lcdmenu.h"

using namespace lcdgui;

void lcdgui::MenuItem::build(LcdBuffer &buffer, const uint8_t &max_length) {
  //do nothing
}

bool lcdgui::MenuItem::input(const uint8_t &input, gui_dialog dialog) {
  return EVENT_SEND;
}

menu_item lcdgui::MenuItem::clone() const {
  return new MenuItem();
}

ItemType lcdgui::MenuItem::type() const {
  return ItemType::Custom;
}

MenuItem::~MenuItem() = default;
