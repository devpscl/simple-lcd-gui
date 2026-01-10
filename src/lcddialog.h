#ifndef SIMPLE_LCD_GUI_SRC_LCDDIALOG_H_
#define SIMPLE_LCD_GUI_SRC_LCDDIALOG_H_
#include "lcdcore.h"

namespace lcdgui {

#define RESULT_OPTION_YES     0x1
#define RESULT_OPTION_NO      0x2
#define RESULT_OPTION_OK      0x4

/**Only input*/
#define RESULT_OPTION_CANCEL  0x8

enum class DialogType {
  MenuDialog,
  ResultDialog,
  InputDialog,
  RenderDialog,
  Custom
};

class GuiDialog {
  gui_dialog parent_dialog_;
  dialog_event_t event_ = nullptr;

  void setInstance(LiquidCrystalGui* lcg);

  friend class LiquidCrystalGui;

 protected:
  LiquidCrystalGui* lcg_instance = nullptr;

  virtual void render(LiquidCrystalGui& lcg);

  virtual void input(LiquidCrystalGui& lcg, const uint8_t &input);

  virtual void enable(LiquidCrystalGui& lcg);

  virtual void disable(LiquidCrystalGui& lcg);

  friend class LiquidCrystalGui;

 public:

  explicit GuiDialog(gui_dialog parent_dialog);

  GuiDialog();

  virtual ~GuiDialog();

  void listener(dialog_event_t listener);

  virtual gui_dialog clone();

  gui_dialog parentDialog();

  void parentDialog(gui_dialog parent_dialog);

  void close();

  void closeAll();

  void dispose();

  void updateDisplay();

  bool isOpened();

  void openChild(gui_dialog dialog);

  void dispatchDialogEvent(uint8_t event);

  virtual DialogType type() const;

};

class ResultDialog : public GuiDialog {
  String text_;
  uint8_t options_;
  result_choose_event_t choose_event_;
  uint8_t option_cursor_ = 0;

  uint8_t optionCount() const;

  uint8_t focusedOption() const;

 protected:
  void render(LiquidCrystalGui& lcg) override;

  void input(LiquidCrystalGui& lcg, const uint8_t &input) override;

  void enable(LiquidCrystalGui& lcg) override;

 public:

  ResultDialog(gui_dialog parent, const String& text, uint8_t options, result_choose_event_t choose_event = nullptr);

  ResultDialog(const String& text, uint8_t options, result_choose_event_t choose_event = nullptr);

  result_choose_event_t chooseEvent();

  uint8_t options() const;

  gui_dialog clone() override;

  DialogType type() const override;

};

class InputDialog : public GuiDialog {
  String title_;
  String buffer_;
  input_event_t input_event_ = nullptr;

 protected:
  void render(LiquidCrystalGui& lcg) override;

  void input(LiquidCrystalGui& lcg, const uint8_t &input) override;

  void enable(LiquidCrystalGui& lcg) override;

  void disable(LiquidCrystalGui& lcg) override;

 public:

  InputDialog(gui_dialog parent, const String& title, input_event_t input_event = nullptr, const String& input = "");

  explicit InputDialog(const String& title, input_event_t input_event = nullptr, const String& input = "");

  String title();

  String& input();

  void put(const char* cstr);

  void put(char ch);

  void put(const String& str);

  void removeChars(uint8_t count = 1);

  gui_dialog clone() override;

  DialogType type() const override;

};

#ifdef LCD_U8G2_

class U8G2Dialog : public GuiDialog {
 public:
  DialogType type() const override;

 protected:
  void render(LiquidCrystalGui &lcg) final;

  virtual void render(U8G2 &u8g2, LiquidCrystalGui &lcg);

};

#endif

}

#endif //SIMPLE_LCD_GUI_SRC_LCDDIALOG_H_
