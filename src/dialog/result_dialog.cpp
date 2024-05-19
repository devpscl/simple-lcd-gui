#include "lcddialog.h"

using namespace lcdgui;

void ResultDialog::render(LiquidCrystalGui& lcg) {
  LcdBuffer buffer;
  uint8_t c_pos = 0;
  if(options_ & RESULT_OPTION_OK) {
    if(c_pos == option_cursor_) {
      buffer << "<OK>";
    } else {
      buffer << " OK ";
    }
    c_pos++;
  }
  if(options_ & RESULT_OPTION_YES) {
    if(c_pos > 0) {
      buffer.space();
    }
    if(c_pos == option_cursor_) {
      buffer << "<YES>";
    } else {
      buffer << " YES ";
    }
    c_pos++;
  }
  if(options_ & RESULT_OPTION_NO) {
    if(c_pos > 0) {
      buffer.space();
    }
    if(c_pos == option_cursor_) {
      buffer << "<NO>";
    } else {
      buffer << " NO ";
    }
  }
  buffer.center(lcg.columns());
  lcg.clear();
  lcg.cursor(0, 0);
  lcg.print(text_);
  lcg.cursor(0, 1);
  lcg.print(buffer.str());
}

void ResultDialog::input(LiquidCrystalGui& lcg, const uint8_t &input) {
  if(input == LCD_INPUT_LEFT && option_cursor_ > 0) {
    option_cursor_--;
    updateDisplay();
    return;
  }
  if(input == LCD_INPUT_RIGHT) {
    if(option_cursor_ < optionCount() - 1) {
      option_cursor_++;
      updateDisplay();
    }
    return;
  }
  if((input == LCD_INPUT_NEXT || input == LCD_INPUT_CANCEL) && choose_event_ != nullptr) {
    uint8_t action = choose_event_(input == LCD_INPUT_CANCEL ? RESULT_OPTION_CANCEL : focusedOption());
    switch (action) {
      case DIALOG_ONLY_CLOSE:
        close();
        break;
      case DIALOG_DISPOSE:
        dispose();
        break;
      default:break;
    }
  }
}

void ResultDialog::enable(LiquidCrystalGui &lcg) {
  lcg.flags(0);
}

ResultDialog::ResultDialog(gui_dialog parent, const String &text, uint8_t options,
                           result_choose_event_t choose_event) : GuiDialog(parent) {
  text_ = text;
  options_ = options;
  choose_event_ = choose_event;
}

ResultDialog::ResultDialog(const String& text, uint8_t options, result_choose_event_t choose_event)
        : ResultDialog::ResultDialog(nullptr, text, options, choose_event) {}

result_choose_event_t ResultDialog::chooseEvent() {
  return choose_event_;
}

uint8_t ResultDialog::options() const {
  return options_;
}

gui_dialog ResultDialog::clone() {
  return new ResultDialog(text_, options_, choose_event_);
}

uint8_t ResultDialog::optionCount() const {
  return BIT_INUM(options_, RESULT_OPTION_OK)  +
         BIT_INUM(options_, RESULT_OPTION_YES) +
         BIT_INUM(options_, RESULT_OPTION_NO);
}

uint8_t ResultDialog::focusedOption() const {
  uint8_t c_pos = 0;
  if(options_ & RESULT_OPTION_OK) {
    if(c_pos == option_cursor_) {
      return RESULT_OPTION_OK;
    }
    c_pos++;
  }
  if(options_ & RESULT_OPTION_YES) {
    if(c_pos == option_cursor_) {
      return RESULT_OPTION_YES;
    }
    c_pos++;
  }
  if(options_ & RESULT_OPTION_NO) {
    if(c_pos == option_cursor_) {
      return RESULT_OPTION_NO;
    }
  }
  return 0;
}

DialogType ResultDialog::type() const {
  return DialogType::ResultDialog;
}

