#ifndef SIMPLE_LCD_GUI_SRC_LCDDIALOG_H_
#define SIMPLE_LCD_GUI_SRC_LCDDIALOG_H_
#include "lcdcore.h"

namespace lcdgui {

#define RESULT_OPTION_YES     0x1
#define RESULT_OPTION_NO      0x2
#define RESULT_OPTION_OK      0x4

/**Only input*/
#define RESULT_OPTION_CANCEL  0x8

class GuiDialog {

 private:

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

  virtual ~GuiDialog();

  virtual gui_dialog clone();

  void close();

  void dispose();

  void updateDisplay();

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

  ResultDialog(const String& text, uint8_t options, result_choose_event_t choose_event = nullptr);

  result_choose_event_t chooseEvent();

  uint8_t options() const;

  gui_dialog clone() override;

};

}

#endif //SIMPLE_LCD_GUI_SRC_LCDDIALOG_H_
