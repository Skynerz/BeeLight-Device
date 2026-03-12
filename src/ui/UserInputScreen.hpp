#ifndef USERINPUTSCREEN_HPP
#define USERINPUTSCREEN_HPP

#include <functional>

#include "AbstractScreen.hpp"
class UserInputScreen : public AbstractScreen {
   public:
    using SubmitInputCallBack = std::function<void(const std::string&)>;
    
    struct Context : public AbstractScreen::Context {
        std::string title;
        std::string initialText;
        uint8_t maxInputLength = 20;
        SubmitInputCallBack onSubmit;
    };

    UserInputScreen(const std::string name = "UserInputScreen")
        : AbstractScreen(name), currentCharIndex_m(0), inputText_m(""), defaultInputText_m(""),
          maxInputLength_m(20), onSubmit_m(nullptr) {

          };

    void populate() override;
    void onTimerEvent() override;

    void setContext(const AbstractScreen::Context* context) override {
        const Context* userInputContext = static_cast<const Context*>(context);
        title_m                         = userInputContext->title;
        inputText_m                     = userInputContext->initialText;
        defaultInputText_m              = userInputContext->initialText;
        maxInputLength_m                = userInputContext->maxInputLength;
        onSubmit_m                      = userInputContext->onSubmit;
    }

   protected:
   static constexpr int kCharWidth = 20;
   static constexpr char inputs[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    static void onScroll(lv_event_t* event);
    static void onInsertCharClicked(lv_event_t* event);
    static void onRemoveCharClicked(lv_event_t* event);
    static void onResetButtonClicked(lv_event_t* event);
    static void onValidButtonClicked(lv_event_t* event);
    void selectCurrentChar(uint8_t charIndex);
    void updateInputArea();
    virtual void onSubmit() {
        if (onSubmit_m) {
            onSubmit_m(inputText_m);
        } else {
            getLogger().warn("No onSubmit callback defined");
        }
    }

    uint8_t currentCharIndex_m;
    std::string title_m;
    std::vector<lv_obj_t*> inputsLabel_m;
    lv_obj_t* inputArea_m;
    std::string inputText_m;
    bool cursorDisplayed_m = true;

    // context
    std::string defaultInputText_m;
    uint8_t maxInputLength_m;
    SubmitInputCallBack onSubmit_m;
};

#endif  // USERINPUTSCREEN_HPP
