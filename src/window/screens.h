#pragma once

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>

#include "app_context.h"

// Экран 0: стартовое меню
class StartMenuScreen {
    public:
        explicit StartMenuScreen(AppContext& ctx);
        ftxui::Component renderer() const {return renderer_;}

        int start_selected_opt = 0; // нужен MainMenuScreen (хоткей)
    private:
        AppContext& ctx_;

        std::vector<std::string> start_menu_items_;
        ftxui::Component start_menu_;
        ftxui::Component start_menu_hotkey_;
        ftxui::Component start_menu_container_;
        ftxui::Component renderer_;
};

// Экран 1: вход в систему
class LoginScreen {
    public:
        explicit LoginScreen(AppContext& ctx);
        ftxui::Component renderer() const {return renderer_;}
    private:
        AppContext& ctx_;

        int login_focus_ind_ = 0; // 0 - login, 1 - password, 2 - btns
        ftxui::Component input_login_;
        ftxui::Component input_password_;
        ftxui::Component log_btns_;
        ftxui::Component login_container_;
        ftxui::Component renderer_;
};

// Экран 2: регистрация
class RegisterScreen {
    public:
        explicit RegisterScreen(AppContext& ctx);
        ftxui::Component renderer() const {return renderer_;}
    private:
        AppContext& ctx_;

        std::string repeat_pass_;
        int reg_focus_ind_ = 0; // 0 - login, 1 - password, 2 - btns
        ftxui::Component input_login_reg_;
        ftxui::Component input_password_reg_;
        ftxui::Component repeat_password_;
        ftxui::Component reg_btns_;
        ftxui::Component reg_container_;
        ftxui::Component renderer_;
};

// Экран 3: информация
class InformationScreen {
    public:
        explicit InformationScreen(AppContext& ctx);
        ftxui::Component renderer() const {return renderer_;}
    private:
        AppContext& ctx_;

        ftxui::Component info_container_;
        ftxui::Component renderer_;
};

// Экран 4: главное меню банкомата
class MainMenuScreen {
    public:
        MainMenuScreen(AppContext& ctx, StartMenuScreen& start_menu_screen);
        ftxui::Component renderer() const {return renderer_;}
    private:
        AppContext& ctx_;
        StartMenuScreen& start_menu_screen_;

        int main_selected_opt_ = 0;
        int main_focus_ind_ = 0; // 0 - menu, 1 - back button
        std::vector<std::string> main_menu_items_;
        ftxui::Component main_menu_;
        ftxui::Component main_menu_hotkey_;
        ftxui::Component back_to_menu_btn_main_;
        ftxui::Component main_container_;
        ftxui::Component renderer_;
};

// Экран 5: регистрация карты
class RegCardScreen {
    public:
        explicit RegCardScreen(AppContext& ctx);
        ftxui::Component renderer() const {return renderer_;}
    private:
        AppContext& ctx_;

        std::vector<std::string> bank_items_;
        int bank_index_ = 0;
        int reg_card_focus_ind_ = 0;
        BankName selected_bank_ = BankName::RofloBank;
        std::string data_card_;
        ftxui::Component bank_choose_;
        ftxui::Component pincode_input_;
        ftxui::Component btn_generate_card_;
        ftxui::Component btn_confirm_card_;
        ftxui::Component btn_cancel_card_;
        ftxui::Component reg_card_container_;
        ftxui::Component renderer_;
};

// Экран 6: вставка карты
class CardInsertScreen {
    public:
        explicit CardInsertScreen(AppContext& ctx);
        ftxui::Component renderer() const {return renderer_;}
    private:
        AppContext& ctx_;

        int card_choose_idx_ = 0;
        int cardInsert_focus_ind_ = 0;
        std::vector<std::string> insert_items_;
        ftxui::Component choose_card_toInsert_;
        ftxui::Component check_pincode_;
        ftxui::Component btn_confirm_cardInsert_;
        ftxui::Component btn_cancel_cardInsert_;
        ftxui::Component btn_back_cardInsert_;
        ftxui::Component cardInsert_container_;
        ftxui::Component renderer_;
};

// Экран 7: пополнение / снятие денег
class TopupWithdrawScreen {
    public:
        explicit TopupWithdrawScreen(AppContext& ctx);
        ftxui::Component renderer() const {return renderer_;}
    private:
        AppContext& ctx_;

        int oper_choose_idx_ = 0;
        int topup_focus_ind_ = 0;
        std::vector<std::string> topup_withdrew_items_;
        int money_selected_ = 0;
        std::vector<std::string> money_menu_items_;
        std::string withdrew_cash_;
        std::string status_;
        ftxui::Component topup_confirm_;
        ftxui::Component withdrew_confirm_;
        ftxui::Component topup_backmenu_;
        ftxui::Component topup_cancel_;
        ftxui::Component withdrew_inut_;
        ftxui::Component topup_withdrew_choose_;
        ftxui::Component money_menu_;
        ftxui::Component topup_withdrew_container_;
        ftxui::Component renderer_;
};

// Экран 8: история операций
class HistoryScreen {
    public:
        explicit HistoryScreen(AppContext& ctx);
        ftxui::Component renderer() const {return renderer_;}
    private:
        AppContext& ctx_;

        std::vector<std::string> history_items_;
        int history_selected_ = 0;
        int history_focus_ind_ = 0;
        ftxui::Component history_menu_;
        ftxui::Component back_to_menu_btn_history_;
        ftxui::Component history_container_;
        ftxui::Component renderer_;
};
