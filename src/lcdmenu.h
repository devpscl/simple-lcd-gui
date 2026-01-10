#ifndef SIMPLE_LCD_MENU_SRC_LCDMENU_H_
#define SIMPLE_LCD_MENU_SRC_LCDMENU_H_
#include "lcddialog.h"

namespace lcdgui {

#define MENU_FLAG_CYLINDER_SCROLL     0x1
#define MENU_FLAG_DISALLOW_MUTLIMENU  0x2


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
  const char** arr_;
  uint8_t count_;

 public:

  OptionList(const char** arr, const uint8_t &count);

  const char* option(const uint8_t &index);

  const char* option(const uint8_t &index) const;

  uint8_t count() const;

  const char* operator[](const uint8_t &index);

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

  virtual bool input(const uint8_t &input, menu_dialog dialog);

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

  bool input(const uint8_t &input, menu_dialog dialog) override;

 public:
  explicit DummyMenuItem(const String& text);

  explicit DummyMenuItem(const char* text);

  String text() const;

  void text(const String& text);

  void text(const char* text);

  menu_item clone() const override;

  ItemType type() const override;

};

class ActionMenuItem : public MenuItem {
  String text_;
  action_event_t event_;

 protected:
  void build(LcdBuffer &buffer, const uint8_t &max_length) override;

  bool input(const uint8_t &input, menu_dialog dialog) override;

 public:
  explicit ActionMenuItem(const String& text, action_event_t action_event = nullptr);

  explicit ActionMenuItem(const char* text, action_event_t action_event = nullptr);

  action_event_t event();

  String text() const;

  void text(const String& text);

  void text(const char* text);

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

  bool input(const uint8_t &input, menu_dialog dialog) override;

 public:

  explicit CounterMenuItem(const String& text, int16_t value = 0, int16_t min_value = 0,
                  int16_t max_value = 10, OptionItemStyle item_style = OptionItemStyle::Bracketed,
                  counter_change_event_t change_event = nullptr);

  explicit CounterMenuItem(const char* text, int16_t value = 0, int16_t min_value = 0,
                int16_t max_value = 10, OptionItemStyle item_style = OptionItemStyle::Bracketed,
                counter_change_event_t change_event = nullptr);

  int16_t value() const;

  void value(const int16_t& value);

  int16_t minValue() const;

  void minValue(const int16_t& min);

  int16_t maxValue() const;

  void maxValue(const int16_t& max);

  counter_change_event_t changeEvent();

  String text() const;

  void text(String& text);

  void text(const char* text);

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

  bool input(const uint8_t &input, menu_dialog dialog) override;

 public:

  OptionMenuItem(const String& text, option_list option_list,
                 const OptionItemStyle& item_style = OptionItemStyle::Bracketed,
                 option_change_event_t change_event = nullptr);

  OptionMenuItem(const char* text, option_list option_list,
                 const OptionItemStyle& item_style = OptionItemStyle::Bracketed,
                 option_change_event_t change_event = nullptr);

  option_change_event_t changeEvent();

  uint8_t currentOption() const;

  void currentOption(const uint8_t& index);

  String text() const;

  void text(const String& text);

  void text(const char* text);

  menu_item clone() const override;

  ItemType type() const override;

};

class MenuDialog : public GuiDialog {
  MenuItem** menu_items_ = nullptr;
  size_t item_count_ = 0;
  size_t item_cursor_ = 0;
  size_t cursor_offset_ = 0;
  uint8_t flags_ = 0;
  input_event_t input_event_ = nullptr;

 protected:
  void render(LiquidCrystalGui& lcg) override;

  void input(LiquidCrystalGui& lcg, const uint8_t &input) override;

  void enable(LiquidCrystalGui &lcg) override;

 public:
  MenuDialog(gui_dialog parent, MenuItem** menu_items, const size_t &count);

  MenuDialog(MenuItem** menu_items, const size_t &count);

  ~MenuDialog() override;

  uint8_t flags() const;

  void flags(uint8_t flags);

  uint8_t rowOf(const size_t& index);

  size_t indexOf(menu_item item);

  void renderRow(uint8_t row);

  void renderItem(const size_t& index);

  void renderItem(menu_item item);

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

  void openChild(gui_dialog dialog);

  DialogType type() const override;

};

}

#endif //SIMPLE_LCD_MENU_SRC_LCDMENU_H_
