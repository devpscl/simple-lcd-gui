#include "../../include/lcdmenu.h"

using namespace lcdgui;

void OptionMenuItem::build(LcdBuffer &buffer, const uint8_t &max_length) {
  String option_str = option_list_->option(option_cursor_);
  buffer << text_.c_str();
  if(item_style_ == OptionItemStyle::Bracketed) {
    uint8_t len = option_str.length() + 2;
    uint8_t space_between = max_length - text_.length() - len;
    if(len >= max_length || space_between & 0x80) {
      return;
    }
    buffer.resize(max_length - len - space_between);
    buffer.space(space_between);
    buffer << '<' << option_str << '>';
    return;
  }
  if(item_style_ == OptionItemStyle::ColonSplit) {
    uint8_t len = option_str.length() + 1;
    uint8_t space_between = max_length - text_.length() - len;
    if(len >= max_length || space_between & 0x80) {
      return;
    }
    buffer.resize(max_length - len - space_between);
    buffer << ':';
    buffer.space(space_between);
    buffer << option_str;
    return;
  }
}

bool OptionMenuItem::input(const uint8_t &input, menu_dialog dialog) {
  uint8_t count = option_list_->count();
  if(count == 0) {
    return EVENT_SEND;
  }
  if(input == LCD_INPUT_RIGHT || (input == LCD_INPUT_OK && !is_multi_editable())) {
    option_cursor_++;
    if(option_cursor_ >= count) {
      option_cursor_ = 0;
    }
    if(change_event_ != nullptr) {
      change_event_(option_cursor_);
    }
    dialog->updateDisplay();
    return EVENT_CANCELLED;
  }
  if(input == LCD_INPUT_LEFT) {
    if(option_cursor_ == 0) {
      option_cursor_ = count;
    }
    option_cursor_--;
    if(change_event_ != nullptr) {
      change_event_(option_cursor_);
    }
    dialog->updateDisplay();
    return EVENT_CANCELLED;
  }
  return EVENT_SEND;
}

OptionMenuItem::OptionMenuItem(const String& text, option_list option_list,
                               const OptionItemStyle& item_style,
                               option_change_event_t change_event) {
  text_ = text;
  option_list_ = option_list;
  item_style_ = item_style;
  change_event_ = change_event;
}

OptionMenuItem::OptionMenuItem(const char* text, option_list option_list,
                               const OptionItemStyle& item_style,
                               option_change_event_t change_event) {
  text_ = String(text);
  option_list_ = option_list;
  item_style_ = item_style;
  change_event_ = change_event;
}


option_change_event_t OptionMenuItem::changeEvent() {
  return change_event_;
}

uint8_t OptionMenuItem::currentOption() const {
  return option_cursor_;
}

void OptionMenuItem::currentOption(const uint8_t &index) {
  const uint8_t& count = option_list_->count();
  if(count == 0) {
    return;
  }
  if(index >= count) {
    option_cursor_ = count - 1;
    return;
  }
  option_cursor_ = index;
}

String OptionMenuItem::text() const {
  return  text_ ;
}

void OptionMenuItem::text(const String& text) {
  text_ = text;
}

void OptionMenuItem::text(const char* text) {
  text_ = String(text);
}

bool OptionMenuItem::is_multi_editable() const {
  return option_list_->count() > 2;
}

menu_item OptionMenuItem::clone() const {
  return new OptionMenuItem(text_, option_list_, item_style_,
                            change_event_);
}

ItemType OptionMenuItem::type() const {
  return ItemType::Option;
}
