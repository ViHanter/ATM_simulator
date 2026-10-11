#include "screens.h"

#include <cctype>

using namespace ftxui;

// ==================
// START MENU WINDOW
// ==================

StartMenuScreen::StartMenuScreen(AppContext& ctx) : ctx_(ctx) {
    start_menu_items_ = {
        "1. Войти в систему",
        "2. Регистрация",
        "3. Инфо",
        "4. Начать работу",
        "5. Выйти"
    };

    MenuOption start_menu_option;

    start_menu_option.on_enter = [&] {
        switch (start_selected_opt){
            case 0:
            // login_screen
            ctx_.active_screen = 1;
            break;
            case 1:
            // register_screen
            ctx_.active_screen = 2;
            break;
            case 2:
            // information
            ctx_.active_screen = 3;
            break;
            case 3:
            // Начать работу
            if (ctx_.isLog) {
                ctx_.go_main_menu();
            } else ctx_.error_log = "Вы не авторизованы.";
            break;
            case 4:
            //exit
            ctx_.screen.Exit();
            break;
        }
    };
    start_menu_option.entries_option.transform = [&](const EntryState& state){
        auto element = text(state.label);
        if (state.active){
            return element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return element | color(ctx_.main_color);
    };
    start_menu_ = Menu(&start_menu_items_,&start_selected_opt,start_menu_option);
    start_menu_hotkey_ = CatchEvent(start_menu_, [&](Event event){
        bool changed = false;

        if (event == Event::Character('1')){
            start_selected_opt = 0;
            changed = true;
        }
        if (event == Event::Character('2')){
            start_selected_opt = 1;
            changed = true;
        }
        if (event == Event::Character('3')){
            start_selected_opt = 2;
            changed = true;
        }
        if (event == Event::Character('4')){
            start_selected_opt = 3;
            changed = true;
        }
        if (event == Event::Character('5')){
            start_selected_opt = 4;
            changed = true;
        }

        if (changed){
            ctx_.screen.PostEvent(Event::Custom);
            return true;
        }
        return false;
    });

    start_menu_container_ = Container::Vertical({
        start_menu_hotkey_
    }) | color(ctx_.main_color);

    ctx_.reset_on_start_menu.push_back([this]{ start_selected_opt = 0; });

    renderer_ = Renderer(start_menu_container_, [&] {
        std::string login_status = ctx_.isLog ? ctx_.account.GetName() : "неавторизован";
        std::string error_status = ctx_.error_log != "" ? ctx_.error_log : "ничего";

        return window(text("VICTOR ATM - main menu"), vbox({
            separator(),
            text("Аккаунт: " + login_status) | color(ctx_.isLog ? Color::Green : Color::Red),
            text("\nВыберите действие: ") | bold,
            separator(),
            start_menu_->Render(),
            separator(),
            text("Текущий выбор: " + start_menu_items_[start_selected_opt]),
            text("Error log: " + error_status) | color(ctx_.error_log == "" ? Color::Green : Color::Red)
        }))
        | size(WIDTH, EQUAL, 60)
        | size(HEIGHT, EQUAL, 15)
        | center
        | color(ctx_.main_color)
        | bgcolor(ctx_.main_bgcolor);
    });
}

// ===================
// LOGIN WINDOW
// ====================

LoginScreen::LoginScreen(AppContext& ctx) : ctx_(ctx) {
    // Настройка стилей полей ввода
    InputOption input_option_log;
    InputOption input_option_pass;

    input_option_log.multiline = false;
    input_option_log.on_enter = [&] {login_focus_ind_ = 1;};

    input_option_pass.multiline = false;
    input_option_pass.on_enter = [&] {login_focus_ind_ = 2;};
    input_option_pass.password = true;

    input_option_log.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return state.element | color(ctx_.main_color);
    };
    input_option_pass.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return state.element | color(ctx_.main_color);
    };

    input_login_ = Input(&ctx_.login,"Введите логин: ", input_option_log);
    input_password_ = Input(&ctx_.password,"Введите пароль: ", input_option_pass);

    auto log_confirm_btn = Button("Подтвердить", [&] {
        if (ctx_.login == "" || ctx_.password == "") {
            ctx_.error_code = 1;
            ctx_.error_log = "Неккоректный ввод.";
        } // неккоректный ввод
        else {
            //TODO: доделать вход с бд
            ctx_.password = "";
            ctx_.login = "";

            ctx_.go_start_menu();
        }
    },ctx_.btn_config);
    auto back_to_menu_btn_log = Button("Назад в меню", [&] {ctx_.go_start_menu();},ctx_.btn_config);

    log_btns_ = Container::Horizontal({
        log_confirm_btn,
        back_to_menu_btn_log
    });
    login_container_ = Container::Vertical({
        input_login_,
        input_password_,
        log_btns_
    }, &login_focus_ind_);

    ctx_.reset_on_start_menu.push_back([this]{ login_focus_ind_ = 0; });

    renderer_ = Renderer(login_container_, [&]{
        return window(text("Login session"), vbox({
            separator(),
            input_login_->Render(),
            input_password_->Render(),
            separator(),

            log_btns_->Render()  | size(WIDTH, EQUAL, 30) | center
        }))
        | size(WIDTH, EQUAL, 60)
        | size(HEIGHT, EQUAL, 12)
        | center
        | color(ctx_.main_color)
        | bgcolor(ctx_.main_bgcolor);
    });
}

// =====================
// REGISTER WINDOW
// =====================

RegisterScreen::RegisterScreen(AppContext& ctx) : ctx_(ctx) {
    // Настройка стилей полей ввода
    InputOption reg_input_option_log;
    InputOption reg_input_option_pass;
    InputOption reg_input_option_pass_rep;

    reg_input_option_log.multiline = false;
    reg_input_option_log.on_enter = [&] {reg_focus_ind_ = 1;};

    reg_input_option_pass.multiline = false;
    reg_input_option_pass.on_enter = [&] {reg_focus_ind_ = 2;};
    reg_input_option_pass.password = true;

    reg_input_option_pass_rep.multiline = false;
    reg_input_option_pass_rep.on_enter = [&] {reg_focus_ind_ = 3;};
    reg_input_option_pass_rep.password = true;

    reg_input_option_pass.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return state.element | color(ctx_.main_color);
    };
    reg_input_option_log.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return state.element | color(ctx_.main_color);
    };
    reg_input_option_pass_rep.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return state.element | color(ctx_.main_color);
    };

    input_login_reg_ = Input(&ctx_.login,"Введите логин: ", reg_input_option_log);
    input_password_reg_ = Input(&ctx_.password,"Введите пароль: ", reg_input_option_pass);
    repeat_password_ = Input(&repeat_pass_, "Повторите пароль:", reg_input_option_pass_rep);

    auto reg_confirm_btn = Button("Подтвердить", [&] {
        if (ctx_.login == "" || ctx_.password == "" || repeat_pass_ == "") {
            ctx_.error_code = 1;
            ctx_.error_log = "Неккоректный ввод.";
        } // неккоректный ввод
        else if (ctx_.password != repeat_pass_) {
            ctx_.error_code = 1;
            ctx_.error_log = "Пароли не совпадают.";
        }
        else {
            ctx_.account.regAccount(ctx_.login,ctx_.password);
            ctx_.atm.log(ctx_.account,ctx_.password);

            ctx_.isLog = true;

            ctx_.login = "";
            ctx_.password = "";
            repeat_pass_ = "";
            ctx_.go_start_menu();
        }
    },ctx_.btn_config);

    auto back_to_menu_btn_reg = Button("Назад в меню", [&] {ctx_.go_start_menu();},ctx_.btn_config);

    reg_btns_ = Container::Horizontal({
        reg_confirm_btn,
        back_to_menu_btn_reg
    });
    reg_container_ = Container::Vertical({
        input_login_reg_,
        input_password_reg_,
        repeat_password_,
        reg_btns_
    }, &reg_focus_ind_);

    ctx_.reset_on_start_menu.push_back([this]{ reg_focus_ind_ = 0; });

    renderer_ = Renderer(reg_container_, [&]{
        return window(text("Registration session"), vbox({
            separator(),
            input_login_reg_->Render(),
            input_password_reg_->Render(),
            repeat_password_->Render(),
            separator(),

            reg_btns_->Render()  | size(WIDTH, EQUAL, 30) | center
        }))
        | size(WIDTH, EQUAL, 60)
        | size(HEIGHT, EQUAL, 12)
        | center
        | color(ctx_.main_color)
        | bgcolor(ctx_.main_bgcolor);
    });
}

// =====================
// INFO WINDOW
// =====================

InformationScreen::InformationScreen(AppContext& ctx) : ctx_(ctx) {
    if (ctx_.isLog) auto infromation = text(
        "Профиль: " + ctx_.account.GetName() + "\n"
        "Версия: 0.3\n"
        "Это супер-мега крутой банкомат Виктора, который может много чего");
    else {
        auto infromation = text(
        "Профиль: неавторизован\n"
        "Версия: 0.3\n"
        "Это супер-мега крутой банкомат Виктора, который может много чего");
    }

    auto back_to_menu_btn_inf = Button("Назад в меню", [&] {ctx_.go_start_menu();},ctx_.btn_config);
    info_container_ = Container::Vertical({
        back_to_menu_btn_inf
    });

    renderer_ = Renderer(info_container_, [&, back_to_menu_btn_inf]{
        std::string login_status = ctx_.isLog ? ctx_.account.GetName() : "неавторизован";
        return window(text("Information"), vbox({
            separator(),
            text("Login: " + login_status) | color(ctx_.isLog ? Color::Green : Color::Red),
            text("Vesion: 0.3") | color(ctx_.main_color),
            text("Это супер-мега-крутой банкомат Виктора, да-да") | color(ctx_.main_color),
            separator(),
            back_to_menu_btn_inf->Render() | center
        }))
        | size(WIDTH, EQUAL, 60)
        | size(HEIGHT, EQUAL, 13)
        | center
        | color(ctx_.main_color)
        | bgcolor(ctx_.main_bgcolor);
    });
}

// =====================
// MAIN WINDOW
// =====================

MainMenuScreen::MainMenuScreen(AppContext& ctx, StartMenuScreen& start_menu_screen)
    : ctx_(ctx), start_menu_screen_(start_menu_screen) {

    main_menu_items_ = {
        "1. Зарегестрировать карту",
        "2. Вставить карту",
        "3. Пополнение / снятие"
    };

    MenuOption main_menu_option;

    main_menu_option.on_enter = [&] {
        switch (main_selected_opt_){
            case 0:
            // registration window
            ctx_.active_screen = 5;
            break;
            case 1:
            // insert card
            ctx_.active_screen = 6;
            break;
            case 2:
            // topup / withdraw
            if (ctx_.atm.checkCardInsert()) {
                ctx_.active_screen = 7;
            } else ctx_.error_log = "Сначала вставьте карту.";
            break;
        }
    };
    main_menu_option.entries_option.transform = [&](const EntryState& state){
        auto element = text(state.label);
        if (state.active){
            return element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return element | color(ctx_.main_color);
    };
    main_menu_ = Menu(&main_menu_items_,&main_selected_opt_,main_menu_option);
    main_menu_hotkey_ = CatchEvent(main_menu_, [&](Event event){
        bool changed = false;

        if (event == Event::Character('1')){
            main_selected_opt_ = 0;
            changed = true;
        }
        if (event == Event::Character('2')){
            main_selected_opt_ = 1;
            changed = true;
        }
        if (event == Event::Character('3')){
            main_selected_opt_ = 2;
            changed = true;
        }

        if (changed){
            ctx_.screen.PostEvent(Event::Custom);
            return true;
        }
        return false;
    });

    back_to_menu_btn_main_ = Button("Назад в меню", [&] {ctx_.go_start_menu();},ctx_.btn_config);

    main_container_ = Container::Vertical({
        main_menu_hotkey_,
        back_to_menu_btn_main_
    }, &main_focus_ind_) | color(ctx_.main_color);

    ctx_.reset_on_main_menu.push_back([this]{
        main_selected_opt_ = 0;
        main_focus_ind_ = 0;
    });

    renderer_ = Renderer(main_container_, [&]{
        static constexpr std::string_view err = "карта не вставлена";
        std::string card_num {ctx_.atm.checkCardInsert() ? ctx_.current_card.getCardNum() : std::string(err)};
        std::string balance {ctx_.atm.checkCardInsert() ? std::to_string(ctx_.atm.getBalance()) + " руб." : std::string(err)};
        std::string error_status = ctx_.error_log.empty() ? "ничего" : ctx_.error_log;

        return window(text("ATM VICTOR"), vbox({
            separator(),
            text("Привет, " + ctx_.account.GetName()) | color(ctx_.main_color),
            text("Сегодня " + ctx_.today) | color(ctx_.main_color),
            separator(),
            text("Ваши карты: " + ctx_.account.getCard_list()) | color(ctx_.account.isCard_listNull() ? Color::Red : ctx_.main_color),
            text("Купюры: " + ctx_.account.getMoneyBagList()) | color(ctx_.main_color),
            separator(),
            text("Карта: " + card_num) | color(ctx_.atm.checkCardInsert() ? Color::Green : Color::Red),
            text("Баланс: " + balance) | color(ctx_.atm.checkCardInsert() ? Color::Green : Color::Red),
            main_menu_->Render(),
            separator(),
            text("Error log: " + error_status) | color(ctx_.error_log.empty() ? Color::Green : Color::Red),
            back_to_menu_btn_main_->Render()
        }))
        | size(WIDTH, EQUAL, 66)
        | size(HEIGHT, EQUAL, 22)
        | center
        | color(ctx_.main_color)
        | bgcolor(ctx_.main_bgcolor);
    });
}

// =====================
// REGIST CARD WINDOW
// =====================

RegCardScreen::RegCardScreen(AppContext& ctx) : ctx_(ctx) {
    bank_items_ = {
        "RofloBank",
        "SbeerBank",
        "Tankoff",
        "MusorBank"
    };
    data_card_ = std::to_string(ctx_.month) + '/' + std::to_string(ctx_.year%100 + 8);

    RadioboxOption bank_option;
    bank_option.on_change = [&] {
        selected_bank_ = static_cast<BankName>(bank_index_);
    };
    bank_choose_ = Radiobox(&bank_items_, &bank_index_,bank_option);

    auto pincode_input_config = InputOption();
    pincode_input_config.password = true;
    pincode_input_config.multiline = false;

    pincode_input_config.on_change = [&] {

        if (ctx_.pincode.length() > 4) {ctx_.pincode.pop_back();}

        if (!ctx_.pincode.empty() && !std::isdigit(ctx_.pincode.back())) {
            ctx_.pincode.pop_back();
        }
    };
    pincode_input_config.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return state.element | color(ctx_.main_color);
    };

    pincode_input_ = Input(&ctx_.pincode,"Придумайте пинкод",pincode_input_config);
    btn_generate_card_ = Button("Сгенерировать карту", [&]{
        if (ctx_.pincode.length() == 4) {
            ctx_.last_card = ctx_.account.regNewCard(selected_bank_,data_card_,ctx_.pincode);
        }
    }, ctx_.btn_config);

    btn_confirm_card_ = Button("Подтвердить", [&]{
        ctx_.account.addCard(ctx_.last_card);

        ctx_.last_card = Card();
        ctx_.pincode = "";

        ctx_.go_main_menu();
    }, ctx_.btn_config);
    btn_cancel_card_ = Button("Отмена", [&]{
        ctx_.last_card = Card();
        ctx_.pincode = "";

        ctx_.go_main_menu();
    }, ctx_.btn_config);

    reg_card_container_ = Container::Vertical({
        bank_choose_,
        pincode_input_,
        btn_generate_card_,
        btn_confirm_card_,
        btn_cancel_card_
    }, &reg_card_focus_ind_);

    ctx_.reset_on_main_menu.push_back([this]{
        bank_index_ = 0;
        selected_bank_ = BankName::RofloBank;
        reg_card_focus_ind_ = 0;
    });

    renderer_ = Renderer(reg_card_container_, [&]{

        std::string card_num = ctx_.last_card.registered() ? ctx_.last_card.getCardNum() : "незарегестрирована";
        std::string card_data = ctx_.last_card.registered() ? ctx_.last_card.getData() : "незарегестрирована";
        std::string card_cvv = ctx_.last_card.registered() ? ctx_.last_card.getCVV() : "незарегестрирована";

        return window(text("Card registration"), vbox({
            separator(),
            text("Выберите банк.") | color(ctx_.main_color),
            bank_choose_->Render() | color(ctx_.main_color),
            pincode_input_->Render(),
            btn_generate_card_->Render() | center,
            separator(),
            text("Номер карты: " + card_num) | color(ctx_.last_card.registered() ? Color::Green : Color::Red),
            text("Дата: " + card_data) | color(ctx_.last_card.registered() ? Color::Green : Color::Red),
            text("CVV: " + card_cvv) | color(ctx_.last_card.registered() ? Color::Green : Color::Red),
            separator(),
            btn_confirm_card_->Render(),
            btn_cancel_card_->Render()
        }))
        | size(WIDTH, EQUAL, 72)
        | size(HEIGHT, EQUAL, 25)
        | center
        | color(ctx_.main_color)
        | bgcolor(ctx_.main_bgcolor);
    });
}

// ====================
// INSERT CARD
// ====================

CardInsertScreen::CardInsertScreen(AppContext& ctx) : ctx_(ctx) {
    auto pincode_check_config = InputOption();
    pincode_check_config.password = true;
    pincode_check_config.multiline = false;

    choose_card_toInsert_ = Radiobox(&insert_items_,&card_choose_idx_);
    check_pincode_ = Input(&ctx_.pincode,"Введите пинкод",pincode_check_config);
    btn_confirm_cardInsert_ = Button("Вставить карту", [&]{
        if (!ctx_.account.isCard_listNull() && card_choose_idx_ >= 0 && card_choose_idx_ < (int)insert_items_.size()){
            ctx_.last_card = ctx_.account.getCard_map().at(insert_items_[card_choose_idx_]);
            Card& real_card = ctx_.account.getCard(insert_items_[card_choose_idx_]);

            int status = ctx_.atm.insertCard(real_card, ctx_.pincode);
            ctx_.atm.getCard(ctx_.current_card);

            if (status == 0) ctx_.error_log = "";
            else if (status == 1) ctx_.error_log = "Неверный пинкод.";
            else if (status == 2) ctx_.error_log = "Карта не зарегистрирована.";
        } else ctx_.error_log = "Выберите карту.";
    },ctx_.btn_config);
    btn_cancel_cardInsert_ = Button("Вытащить", [&]{
        card_choose_idx_ = 0;
        ctx_.last_card = Card();
        ctx_.current_card = Card();
        ctx_.atm.eraseCard();
        ctx_.pincode = "";
    }, ctx_.btn_config);
    btn_back_cardInsert_ = Button("Выйти в меню", [&]{
        ctx_.last_card = Card();
        ctx_.pincode = "";

        ctx_.go_main_menu();
    }, ctx_.btn_config);

    cardInsert_container_ = Container::Vertical({
        choose_card_toInsert_,
        check_pincode_,
        btn_confirm_cardInsert_,
        btn_cancel_cardInsert_,
        btn_back_cardInsert_
    }, &cardInsert_focus_ind_);

    ctx_.reset_on_main_menu.push_back([this]{
        card_choose_idx_ = 0;
        cardInsert_focus_ind_ = 0;
    });

    renderer_ = Renderer(cardInsert_container_, [&]{
        static constexpr std::string_view err = "карта не вставлена";
        insert_items_.clear();
        for (auto&[card_num,card] : ctx_.account.getCard_map()){
            insert_items_.push_back(card_num);
        }

        std::string card_num {ctx_.atm.checkCardInsert() ? ctx_.current_card.getCardNum() : err};
        std::string card_data {ctx_.atm.checkCardInsert() ? ctx_.current_card.getData() : err};
        std::string card_cvv {ctx_.atm.checkCardInsert() ? ctx_.current_card.getCVV() : err};
        std::string card_bank {ctx_.atm.checkCardInsert() ? ctx_.current_card.getBankName() : err};
        return window(text("Insert card"), vbox({
            separator(),
            text("Выберите карту.") | color(ctx_.main_color),
            choose_card_toInsert_->Render() | color(ctx_.main_color),
            check_pincode_->Render(),
            separator(),
            btn_confirm_cardInsert_->Render(),
            btn_cancel_cardInsert_->Render(),
            separator(),
            text("Карта: " + card_num) | color(ctx_.current_card.registered() ? Color::Green : Color::Red),
            text("Банк: " + card_bank) | color(ctx_.current_card.registered() ? Color::Green : Color::Red),
            text("Дата: " + card_data) | color(ctx_.current_card.registered() ? Color::Green : Color::Red),
            text("CVV: " + card_cvv) | color(ctx_.current_card.registered() ? Color::Green : Color::Red),
            separator(),
            btn_back_cardInsert_->Render() | center
        }))
        | size(WIDTH, EQUAL, 72)
        | size(HEIGHT, EQUAL, 34)
        | center
        | color(ctx_.main_color)
        | bgcolor(ctx_.main_bgcolor);
    });
}

// ========================
// Пополнение / снятие денег
// ========================

TopupWithdrawScreen::TopupWithdrawScreen(AppContext& ctx) : ctx_(ctx) {
    topup_withdrew_items_ = {
        "Пополнение",
        "Снятие"
    };
    money_menu_items_ = {
        "50",
        "100",
        "200",
        "500",
        "1000",
        "2000",
        "5000"
    };
    MenuOption money_menu_option;


    money_menu_option.on_enter = [&] {
        int type_money = 0;
        switch (money_selected_){
            case 0: type_money = 50;   break;
            case 1: type_money = 100;  break;
            case 2: type_money = 200;  break;
            case 3: type_money = 500;  break;
            case 4: type_money = 1000; break;
            case 5: type_money = 2000; break;
            case 6: type_money = 5000; break;
        }
        int in_wallet = ctx_.account.get_banknote_cnt(type_money);
        int already = ctx_.atm.get_banknote_cnt(type_money);

        if (already >= in_wallet) {
            status_ = "Купюр " + std::to_string(type_money) + " руб. больше нет.";
            return;
        }

        ctx_.transaction += type_money;
        ctx_.atm.put_money_transaction(type_money);
        status_ = "";
    };

    money_menu_option.entries_option.transform = [&](const EntryState& state){
        auto element = text(state.label);
        if (state.active){
            return element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return element | color(ctx_.main_color);
    };

    RadioboxOption oper_option;
    oper_option.on_change = [&] {
        ctx_.transaction = 0;
        ctx_.atm.clear_transaction_map();
        std::string rejected;
        ctx_.atm.get_rejected_banknotes(rejected);
        ctx_.reject_banknotes = "";
        withdrew_cash_ = "";
        status_ = "";
    };

    InputOption withdrew_input_config;
    withdrew_input_config.on_change = [&] {

        if (withdrew_cash_.length() > 10) { withdrew_cash_.pop_back(); }

        if (!withdrew_cash_.empty() && !std::isdigit((unsigned char)withdrew_cash_.back())) {
            withdrew_cash_.pop_back();
        }
    };
    withdrew_input_config.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(ctx_.second_color) | bgcolor(ctx_.mark_color);
        } else return state.element | color(ctx_.main_color);
    };
    withdrew_input_config.multiline = false;

    topup_confirm_ = Button("Пополнить", [&]{
        if (!ctx_.atm.checkCardInsert()) { status_ = "Сначала вставьте карту."; return; }

        long long accepted = ctx_.atm.topup_account_transaction();

        std::string rejected;
        ctx_.atm.get_rejected_banknotes(rejected);
        ctx_.atm.getCard(ctx_.current_card);

        status_ = "Зачислено: " + std::to_string(accepted) + " руб.";
        if (!rejected.empty()) status_ += " | не принято: " + rejected;

        ctx_.transaction = 0;
        ctx_.reject_banknotes = rejected;
    },ctx_.btn_config);

    withdrew_confirm_ = Button("Снять", [&]{
        if (withdrew_cash_.empty()) { status_ = "Введите сумму."; return; }

        long long amount = 0;
        try {
            amount = std::stoll(withdrew_cash_);
        } catch (...) {
            status_ = "Некорректная сумма.";
            return;
        }

        WithdrawResult result = ctx_.atm.withdrew_transaction(amount);

        if (result == WithdrawResult::Success) {
            std::string dispensed;
            ctx_.atm.get_dispensed_banknotes(dispensed);
            ctx_.atm.getCard(ctx_.current_card);

            status_ = "Выдано: " + std::to_string(amount) + " руб. (" + dispensed + ")";
            withdrew_cash_ = "";
        } else {
            status_ = withdrawResultText(result);
        }
    },ctx_.btn_config);

    topup_backmenu_ = Button("Назад в меню", [&]{
        ctx_.go_main_menu();
    },ctx_.btn_config);
    topup_cancel_ = Button("Забрать купюры", [&]{
        ctx_.transaction = 0;

        ctx_.atm.clear_transaction_map();
        std::string rejected;
        ctx_.atm.get_rejected_banknotes(rejected);
        ctx_.reject_banknotes = "";
        status_ = "Купюры возвращены.";
    },ctx_.btn_config);


    withdrew_inut_ = Input(&withdrew_cash_,"Введите сумму снятия",withdrew_input_config);
    topup_withdrew_choose_ = Radiobox(&topup_withdrew_items_,&oper_choose_idx_,oper_option);
    money_menu_ = Menu(&money_menu_items_,&money_selected_, money_menu_option);

    auto deposit_panel = Container::Vertical({
        money_menu_,
        Container::Horizontal({
            topup_cancel_,
            topup_confirm_
        })
    });
    auto withdrew_panel = Container::Vertical({
        withdrew_inut_,
        withdrew_confirm_
    });
    auto oper_tab = Container::Tab({deposit_panel, withdrew_panel}, &oper_choose_idx_);

    topup_withdrew_container_ = Container::Vertical({
        topup_withdrew_choose_,
        oper_tab,
        topup_backmenu_
    }, &topup_focus_ind_);

    ctx_.reset_on_main_menu.push_back([this]{
        oper_choose_idx_ = 0;
        money_selected_ = 0;
        topup_focus_ind_ = 0;
        withdrew_cash_ = "";
        status_ = "";
        ctx_.transaction = 0;
        ctx_.atm.clear_transaction_map();
        std::string notes;
        ctx_.atm.get_rejected_banknotes(notes);
        ctx_.reject_banknotes = "";
    });

    renderer_ = Renderer(topup_withdrew_container_, [&]{
        static constexpr std::string_view no_card = "карта не вставлена";
        static const int nominals[7] = {50, 100, 200, 500, 1000, 2000, 5000};

        std::string card_num = ctx_.atm.checkCardInsert() ? ctx_.current_card.getCardNum() : std::string(no_card);
        std::string balance = ctx_.atm.checkCardInsert() ? std::to_string(ctx_.atm.getBalance()) + " руб." : std::string(no_card);

        std::string inserted_list;
        for (int nominal : nominals){
            int cnt = ctx_.atm.get_banknote_cnt(nominal);
            if (cnt > 0) inserted_list += std::to_string(nominal) + "x" + std::to_string(cnt) + ' ';
        }
        if (inserted_list.empty()) inserted_list = "нет купюр";

        Element oper_panel;
        if (oper_choose_idx_ == 0){
            oper_panel = vbox({
                text("Вставьте купюры (Enter - вставить купюру):") | color(ctx_.main_color),
                money_menu_->Render(),
                text("Вставлено: " + std::to_string(ctx_.transaction) + " руб. (" + inserted_list + ")"),
                separator(),
                hbox({
                    topup_cancel_->Render(),
                    topup_confirm_->Render()
                })
            });
        } else {
            oper_panel = vbox({
                text("Введите сумму снятия (кратно 50):") | color(ctx_.main_color),
                withdrew_inut_->Render(),
                separator(),
                withdrew_confirm_->Render() | center
            });
        }

        return window(text("Пополнение / снятие"), vbox({
            separator(),
            text("Карта: " + card_num) | color(ctx_.atm.checkCardInsert() ? Color::Green : Color::Red),
            text("Баланс: " + balance) | color(ctx_.atm.checkCardInsert() ? Color::Green : Color::Red),
            text("Купюры аккаунта: " + ctx_.account.getMoneyBagList()) | color(ctx_.main_color),
            separator(),
            topup_withdrew_choose_->Render(),
            separator(),
            oper_panel,
            separator(),
            text("Статус: " + (status_.empty() ? std::string("ничего") : status_))
                | color(status_.empty() ? ctx_.main_color : Color::Yellow),
            separator(),
            topup_backmenu_->Render() | center
        }))
        | size(WIDTH, EQUAL, 72)
        | size(HEIGHT, EQUAL, 30)
        | center
        | color(ctx_.main_color)
        | bgcolor(ctx_.main_bgcolor);
    });
}
