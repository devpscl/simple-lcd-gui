#include "lcdmenu.h"

using namespace lcdgui;

void ActionMenuItem::build(LcdBuffer &buffer, const uint8_t &max_length) {
  buffer << text_;
}

bool ActionMenuItem::input(const uint8_t &input, menu_dialog dialog) {
  if(input == LCD_INPUT_NEXT || input == LCD_INPUT_RIGHT) {
    if(event_ != nullptr) {
      event_();
    }
    return EVENT_CANCELLED;
  }
  return EVENT_SEND;
}

ActionMenuItem::ActionMenuItem(const String& text, action_event_t action_event) {
  text_ = text;
  event_ = action_event;
}

ActionMenuItem::ActionMenuItem(const char* text, action_event_t action_event) {
  text_ = String(text);
  event_ = action_event;
}

action_event_t ActionMenuItem::event() {
  return event_;
}

String ActionMenuItem::text() const {
  return  text_ ;
}

void ActionMenuItem::text(const String& text) {
  text_ = text;
}

void ActionMenuItem::text(const char* text) {
  text_ = text;
}

menu_item ActionMenuItem::clone() const {
  return new ActionMenuItem(text_, event_);
}

ItemType ActionMenuItem::type() const {
  return ItemType::Action;
}
