#include "lcdmenu.h"

using namespace lcdgui;

void CounterMenuItem::build(LcdBuffer &buffer, const uint8_t &max_length) {
  String num_str = String(value_);
  buffer << text_;
  if(item_style_ == OptionItemStyle::Bracketed) {
    uint8_t len = num_str.length() + 3;
    if(len >= max_length) {
      return;
    }
    buffer.resize(max_length - len);
    buffer << ' ' << '<' << num_str << '>';
    return;
  }
  if(item_style_ == OptionItemStyle::ColonSplit) {
    uint8_t len = num_str.length() + 1;
    uint8_t space_between = max_length - text_.length() - len;
    if(len >= max_length || space_between & 0x80) {
      return;
    }
    buffer.resize(max_length - len - space_between);
    buffer << ':';
    buffer.space(space_between);
    buffer << num_str;
    return;
  }
}

bool CounterMenuItem::input(const uint8_t &input, gui_dialog dialog) {
  if(input == LCD_INPUT_LEFT) {
    if(value_ > min_value_) {
      value_--;
      if(change_event_ != nullptr) {
        change_event_(value_, -1);
      }
      dialog->updateDisplay();
    }
    return EVENT_CANCELLED;
  }
  if(input == LCD_INPUT_RIGHT) {
    if(value_ < max_value_) {
      value_++;
      if(change_event_ != nullptr) {
        change_event_(value_, +1);
      }
      dialog->updateDisplay();
    }
    return EVENT_CANCELLED;
  }
  return EVENT_SEND;
}

CounterMenuItem::CounterMenuItem(const String &text,
                                 int16_t value,
                                 int16_t min_value,
                                 int16_t max_value,
                                 const OptionItemStyle &item_style,
                                 counter_change_event_t change_event) {
  text_ = text;
  value_ = value;
  min_value_ = min_value;
  max_value_ = max_value;
  item_style_ = item_style;
  change_event_ = change_event;
}

int16_t CounterMenuItem::value() const {
  return value_;
}

int16_t CounterMenuItem::minValue() const {
  return min_value_;
}

int16_t CounterMenuItem::maxValue() const {
  return max_value_;
}

counter_change_event_t CounterMenuItem::changeEvent() {
  return change_event_;
}

String CounterMenuItem::text() const {
  return { text_ };
}

void CounterMenuItem::text(const String &text) {
  text_ = text;
}

menu_item CounterMenuItem::clone() const {
  return new CounterMenuItem(text_, value_, min_value_,
                             max_value_, item_style_, change_event_);
}

ItemType CounterMenuItem::type() const {
  return ItemType::Counter;
}