#ifndef SIMPLE_LCD_MENU_SRC_LCDMENU_H_
#define SIMPLE_LCD_MENU_SRC_LCDMENU_H_
#include "lcddialog.h"

namespace lcdgui {

class MenuItem;
class DummyMenuItem;
class ActionMenuItem;
class CounterMenuItem;
class OptionMenuItem;
class OptionList;

typedef MenuItem *menu_item;
typedef DummyMenuItem *dummy_menu_item;
typedef CounterMenuItem *counter_menu_item;
typedef ActionMenuItem *action_menu_item;
typedef OptionMenuItem *option_menu_item;

typedef OptionList *option_list;

class OptionList {
  String *arr_;
  uint8_t count_;

 public:

  OptionList(String *arr, const uint8_t &count);

  String option(const uint8_t &index);

  String option(const uint8_t &index) const;

  uint8_t count() const;

  String operator[](const uint8_t &index);

};

enum class OptionItemStyle {
  Bracketed,
  ColonSplit
};

enum class ItemType {
  Dummy,
  Action,
  Counter,
  Option,
  Custom
};

class MenuItem {
 protected:
  virtual void build(LcdBuffer &buffer, const uint8_t &max_length);

  virtual bool input(const uint8_t &input, gui_dialog dialog);

  friend class MenuDialog;

 public:
  virtual ~MenuItem();

  virtual menu_item clone() const;

  virtual ItemType type() const;

};

class DummyMenuItem : public MenuItem {
  String text_;

 protected:
  void build(LcdBuffer &buffer, const uint8_t &max_length) override;

  bool input(const uint8_t &input, gui_dialog dialog) override;

 public:
  explicit DummyMenuItem(const String &text);

  String text() const;

  void text(const String& text);

  menu_item clone() const override;

  ItemType type() const override;

};

class ActionMenuItem : public MenuItem {
  String text_;
  action_event_t event_;

 protected:
  void build(LcdBuffer &buffer, const uint8_t &max_length) override;

  bool input(const uint8_t &input, gui_dialog dialog) override;

 public:
  explicit ActionMenuItem(const String &text, action_event_t action_event = nullptr);

  action_event_t event();

  String text() const;

  void text(const String& text);

  menu_item clone() const override;

  ItemType type() const override;

};

class CounterMenuItem : public MenuItem {
  String text_;
  int16_t value_ = 0;
  int16_t min_value_ = 0;
  int16_t max_value_ = 0;
  OptionItemStyle item_style_;
  counter_change_event_t change_event_;

 protected:
  void build(LcdBuffer &buffer, const uint8_t &max_length) override;

  bool input(const uint8_t &input, gui_dialog dialog) override;

 public:

  explicit CounterMenuItem(const String &text, int16_t value = 0, int16_t min_value = 0,
                  int16_t max_value = 10, const OptionItemStyle& item_style = OptionItemStyle::Bracketed,
                  counter_change_event_t change_event = nullptr);

  int16_t value() const;

  void value(const int16_t& value);

  int16_t minValue() const;

  void minValue(const int16_t& min);

  int16_t maxValue() const;

  void maxValue(const int16_t& max);

  counter_change_event_t changeEvent();

  String text() const;

  void text(const String& text);

  menu_item clone() const override;

  ItemType type() const override;

};

class OptionMenuItem : public MenuItem {
  String text_;
  option_list option_list_;
  option_change_event_t change_event_;
  uint8_t option_cursor_ = 0;
  OptionItemStyle item_style_;

 protected:
  void build(LcdBuffer &buffer, const uint8_t &max_length) override;

  bool input(const uint8_t &input, gui_dialog dialog) override;

 public:

  OptionMenuItem(const String& text, option_list option_list,
                 const OptionItemStyle& item_style = OptionItemStyle::Bracketed,
                 option_change_event_t change_event = nullptr);

  option_change_event_t changeEvent();

  uint8_t currentOption() const;

  void currentOption(const uint8_t& index);

  String text() const;

  void text(const String& text);

  menu_item clone() const override;

  ItemType type() const override;

};

class MenuDialog : public GuiDialog {
  MenuItem** menu_items_ = nullptr;
  size_t item_count_ = 0;
  size_t item_cursor_ = 0;
  size_t cursor_offset = 0;
  input_event_t input_event_ = nullptr;

 protected:
  void render(lcd_native_type lcd, LcdGuiService& service) override;

  void input(const uint8_t &input, LcdGuiService& service) override;

 public:

  ~MenuDialog() override;

  MenuDialog(MenuItem** menu_items, const size_t &count);

  void inputEvent(input_event_t event);

  size_t cursor() const;

  void cursor(const size_t &cursor);

  MenuItem* itemAt(const size_t &index);

  size_t itemCount() const;

  gui_dialog clone() override;

  template<typename T>
  T itemAt(const size_t &index) {
    return reinterpret_cast<T>(itemAt(index));
  }

};

}

#endif //SIMPLE_LCD_MENU_SRC_LCDMENU_H_
