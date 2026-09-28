#include "main_screen.h"

int main(){
    using namespace ftxui;
    std::system("clear");
    
    
    // палитра
    auto main_bgcolor = Color::RGBA(245, 246, 248,0); //rgb(0, 163, 16)
    auto main_color = Color::RGB(0, 163, 16);
    auto second_color = Color::RGB(10, 34, 64);
    auto mark_color = Color::RGB(91, 254, 153); //
    
    //глобал переменные
    auto screen = ScreenInteractive::TerminalOutput();
    int active_screen = 0;

    Account account;
    ATM atm;

    std::string login;
    std::string password;
    bool isLog = false;

    int error_code = -1;
    std::string error_log;
    std::vector<std::string> error_vector;

    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);
    int day = local_time->tm_mday;
    int month = local_time->tm_mon + 1;
    int year = local_time->tm_year + 1900;
    std::string today = std::to_string(day) + '.' + std::to_string(month) + '.' + std::to_string(year);

    // Error map filling

    // TODO: организация ошибок

    // ==================
    // START MENU WINDOW
    // ==================

    int start_selected_opt = 0;
    std::vector<std::string> start_menu_items = {
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
            active_screen = 1;
            break;
            case 1:
            // register_screen
            active_screen = 2;
            break;
            case 2:
            // information
            active_screen = 3;
            break;
            case 3:
            // Начать работу
            if (isLog) {
                active_screen = 4;
            } else error_log = "Вы не авторизованы.";
            break;
            case 4:
            //exit
            screen.Exit();
            break;
        }
    };
    start_menu_option.entries_option.transform = [&](const EntryState& state){
        auto element = text(state.label);
        if (state.active){
            return element | color(second_color) | bgcolor(mark_color);
        } else return element | color(main_color);
    };
    auto start_menu = Menu(&start_menu_items,&start_selected_opt,start_menu_option);
    auto start_menu_hotkey = CatchEvent(start_menu, [&](Event event){
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
            screen.PostEvent(Event::Custom);
            return true;
        }
        return false;
    });
    
    auto start_menu_container = Container::Vertical({
        start_menu_hotkey
    }) | color(main_color);

    // ===================
    // LOGIN WINDOW
    // ====================
    int login_focus_ind = 0; // 0 - login, 1 - password, 2 - btns

    // Настройка стилей полей ввода
    InputOption input_option_log;
    InputOption input_option_pass;

    input_option_log.multiline = false;
    input_option_log.on_enter = [&] {login_focus_ind = 1;};

    input_option_pass.multiline = false;
    input_option_pass.on_enter = [&] {login_focus_ind = 2;};
    input_option_pass.password = true;

    input_option_log.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(second_color) | bgcolor(mark_color);
        } else return state.element | color(main_color);
    };
    input_option_pass.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(second_color) | bgcolor(mark_color);
        } else return state.element | color(main_color);
    };

    auto input_login = Input(&login,"Введите логин: ", input_option_log);
    auto input_password = Input(&password,"Введите пароль: ", input_option_pass);

    // стиль кнопки
    ButtonOption btn_config;
    btn_config.transform = [&](const EntryState& state) {
        auto element = text(state.label);
        if (state.focused){
        return element | center | borderRounded | color(mark_color);
        }
        else return element | center | borderRounded | color(main_color);
    };


    auto log_confirm_btn = Button("Подтвердить", [&] {
        if (login == "" || password == "") {
            error_code = 1;
            error_log = "Неккоректный ввод.";
        } // неккоректный ввод
        else {
            //TODO: доделать вход с бд
            password = "";
            login = "";

            active_screen = 0;
        }
    },btn_config);
    auto back_to_menu_btn_log = Button("Назад в меню", [&] {active_screen = 0;},btn_config);

    auto log_btns = Container::Horizontal({
        log_confirm_btn,
        back_to_menu_btn_log
    });
    auto login_container = Container::Vertical({
        input_login,
        input_password,
        log_btns
    }, &login_focus_ind);

    // =====================
    // REGISTER WINDOW
    // =====================
    
    std::string repeat_pass;
    int reg_focus_ind = 0; // 0 - login, 1 - password, 2 - btns

    // Настройка стилей полей ввода
    InputOption reg_input_option_log;
    InputOption reg_input_option_pass;
    InputOption reg_input_option_pass_rep;

    reg_input_option_log.multiline = false;
    reg_input_option_log.on_enter = [&] {reg_focus_ind = 1;};

    reg_input_option_pass.multiline = false;
    reg_input_option_pass.on_enter = [&] {reg_focus_ind = 2;};
    reg_input_option_pass.password = true;

    reg_input_option_pass_rep.multiline = false;
    reg_input_option_pass_rep.on_enter = [&] {reg_focus_ind = 3;};
    reg_input_option_pass_rep.password = true;

    reg_input_option_pass.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(second_color) | bgcolor(mark_color);
        } else return state.element | color(main_color);
    };
    reg_input_option_log.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(second_color) | bgcolor(mark_color);
        } else return state.element | color(main_color);
    };
    reg_input_option_pass_rep.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(second_color) | bgcolor(mark_color);
        } else return state.element | color(main_color);
    };

    auto input_login_reg = Input(&login,"Введите логин: ", reg_input_option_log);
    auto input_password_reg = Input(&password,"Введите пароль: ", reg_input_option_pass);
    auto repeat_password = Input(&repeat_pass, "Повторите пароль:", reg_input_option_pass_rep);

    auto reg_confirm_btn = Button("Подтвердить", [&] {
        if (login == "" || password == "" || repeat_pass == "") {
            error_code = 1;
            error_log = "Неккоректный ввод.";
        } // неккоректный ввод
        else if (password != repeat_pass) {
            error_code = 1;
            error_log = "Пароли не совпадают.";
        }
        else {
            account.regAccount(login,password);
            atm.log(account,password);

            active_screen = 0;
            isLog = true;

            login = "";
            password = "";
            repeat_pass = "";
        }
    },btn_config);

    auto back_to_menu_btn_reg = Button("Назад в меню", [&] {active_screen = 0;},btn_config);

    auto reg_btns = Container::Horizontal({
        reg_confirm_btn,
        back_to_menu_btn_reg
    });
    auto reg_container = Container::Vertical({
        input_login_reg,
        input_password_reg,
        repeat_password,
        reg_btns
    }, &reg_focus_ind);    

    // =====================
    // INFO WINDOW
    // =====================

    if (isLog) auto infromation = text(
        "Профиль: " + account.GetName() + "\n"
        "Версия: 0.3\n"
        "Это супер-мега крутой банкомат Виктора, который может много чего");
    else {
        auto infromation = text(
        "Профиль: неавторизован\n"
        "Версия: 0.3\n"
        "Это супер-мега крутой банкомат Виктора, который может много чего");
    }

    auto back_to_menu_btn_inf = Button("Назад в меню", [&] {active_screen = 0;},btn_config);
    auto info_container = Container::Vertical({
        back_to_menu_btn_inf
    });

    // =====================
    // MAIN WINDOW
    // =====================

    int main_selected_opt = 0;
    std::vector<std::string> main_menu_items = {
        "1. Зарегестрировать карту",
        "2. Вставить карту"
    };

    MenuOption main_menu_option;
    
    main_menu_option.on_enter = [&] {
        switch (main_selected_opt){
            case 0:
            // registration window
            active_screen = 5;
            break;
            case 1:
            // insert card
            active_screen = 6;
            break;
        }
    };
    main_menu_option.entries_option.transform = [&](const EntryState& state){
        auto element = text(state.label);
        if (state.active){
            return element | color(second_color) | bgcolor(mark_color);
        } else return element | color(main_color);
    };
    auto main_menu = Menu(&main_menu_items,&main_selected_opt,main_menu_option);
    auto main_menu_hotkey = CatchEvent(main_menu, [&](Event event){
        bool changed = false;
        
        if (event == Event::Character('1')){
            start_selected_opt = 0;
            changed = true;
        }
        return false;
    });
    
    auto back_to_menu_btn_main = Button("Назад в меню", [&] {active_screen = 0;},btn_config);
    auto main_container = Container::Vertical({
        main_menu_hotkey,
        back_to_menu_btn_main
    }) | color(main_color);
    
    // =====================
    // REGIST CARD WINDOW
    // =====================

    std::vector<std::string> bank_items = {
        "RofloBank",
        "SbeerBank",
        "Tankoff",
        "MusorBank"
    };
    int bank_index = 0; 
    BankName selected_bank = BankName::RofloBank;
    
    std::string pincode = "";
    std::string data_card = std::to_string(month) + '/' + std::to_string(year%100 + 8);
    Card last_card;

    RadioboxOption bank_option;
    bank_option.on_change = [&] {
        selected_bank = static_cast<BankName>(bank_index);
    };
    auto bank_choose = Radiobox(&bank_items, &bank_index,bank_option);   

    auto pincode_input_config = InputOption();
    pincode_input_config.password = true;
    pincode_input_config.multiline = false;

    pincode_input_config.on_change = [&] {

        if (pincode.length() > 4) {pincode.pop_back();}
        
        if (!pincode.empty() && !std::isdigit(pincode.back())) {
            pincode.pop_back();
        }
    };
    pincode_input_config.transform = [&](InputState state) {

        if (state.focused) {
            return state.element | color(second_color) | bgcolor(mark_color);
        } else return state.element | color(main_color);
    };

    auto pincode_input = Input(&pincode,"Придумайте пинкод",pincode_input_config);
    auto btn_generate_card = Button("Сгенерировать карту", [&]{
        if (pincode.length() == 4) {
            last_card = account.regNewCard(selected_bank,data_card,pincode);
        } 
    }, btn_config);

    auto btn_confirm_card = Button("Подтвердить", [&]{
        account.addCard(last_card);

        last_card = Card();
        pincode = "";
        bank_index = 0;
        selected_bank = BankName::RofloBank;

        active_screen = 4;
    }, btn_config);
    auto btn_cancel_card = Button("Отмена", [&]{
        last_card = Card();
        pincode = "";
        bank_index = 0;
        selected_bank = BankName::RofloBank;

        active_screen = 4;
    }, btn_config);

    auto reg_card_container = Container::Vertical({
        bank_choose,
        pincode_input,
        btn_generate_card,
        btn_confirm_card,
        btn_cancel_card
    });

    // ====================
    // INSERT CARD
    // ====================

    int card_choose_idx = 0;
    std::vector<std::string> insert_items;
    for (auto&[card_num,card] : account.getCard_map()){
        insert_items.push_back(card_num);
    }
    auto pincode_check_config = InputOption();
    pincode_input_config.password = true;
    pincode_input_config.multiline = false;

    auto choose_card_toInsert = Radiobox(&insert_items,&card_choose_idx);
    auto check_pincode = Input("Введите пинкод",&pincode,pincode_check_config);
    auto btn_confirm_cardInsert = Button("Вставить карту", [&]{
        if (!account.isCard_listNull()){
            last_card = account.getCard_map()[insert_items[card_choose_idx]];
            atm.insertCard(last_card,pincode);

            last_card = Card();
        } else {} //TODO: обработка ошибок
    },btn_config);
    auto btn_cancel_cardInsert = Button("Отмена", [&]{
        card_choose_idx = 0;
        last_card = Card();
        atm.eraseCard();
        pincode = "";
    }, btn_config);
    auto btn_back_cardInsert = Button("Выйти в меню", [&]{
        card_choose_idx = 0;
        last_card = Card();
        pincode = "";

        active_screen = 4;
    });

    auto cardInsert_container = Container::Vertical({
        choose_card_toInsert,
        check_pincode,
        btn_confirm_cardInsert,
        btn_cancel_cardInsert,
        btn_back_cardInsert
    });

    // экран menu, active_screen = 0
    auto start_menu_screen = Renderer(start_menu_container, [&] {
        std::string login_status = isLog ? account.GetName() : "неавторизован";
        std::string error_status = error_log != "" ? error_log : "ничего";

        return window(text("VICTOR ATM - main menu"), vbox({
            separator(),
            text("Аккаунт: " + login_status) | color(isLog ? Color::Green : Color::Red),
            text("\nВыберите действие: ") | bold,
            separator(),
            start_menu->Render(),
            separator(),
            text("Текущий выбор: " + start_menu_items[start_selected_opt]),
            text("Error log: " + error_status) | color(error_log == "" ? Color::Green : Color::Red)
        }))
        | size(WIDTH, EQUAL, 60) 
        | size(HEIGHT, EQUAL, 15)
        | center
        | color(main_color)
        | bgcolor(main_bgcolor);
    });

    // экран login, active_screen = 1
    auto login_screen = Renderer(login_container, [&]{
        return window(text("Login session"), vbox({
            separator(),
            input_login->Render(),
            input_password->Render(),
            separator(),
            
            log_btns->Render()  | size(WIDTH, EQUAL, 30) | center
        }))
        | size(WIDTH, EQUAL, 60) 
        | size(HEIGHT, EQUAL, 15)
        | center
        | color(main_color)
        | bgcolor(main_bgcolor);
    });

    // экран registraion, active_screen = 2
    auto register_screen = Renderer(reg_container, [&]{
        return window(text("Registration session"), vbox({
            separator(),
            input_login_reg->Render(),
            input_password_reg->Render(),
            repeat_password->Render(),
            separator(),
            
            reg_btns->Render()  | size(WIDTH, EQUAL, 30) | center
        }))
        | size(WIDTH, EQUAL, 60) 
        | size(HEIGHT, EQUAL, 15)
        | center
        | color(main_color)
        | bgcolor(main_bgcolor);
    });

    // экран информации, active_screen = 3
    auto information_screen = Renderer(info_container, [&]{
        std::string login_status = isLog ? account.GetName() : "неавторизован";
        return window(text("Information"), vbox({
            separator(),
            text("Login: " + login_status) | color(isLog ? Color::Green : Color::Red),
            text("Vesion: 0.3") | color(main_color), 
            text("Это супер-мега-крутой банкомат Виктора, да-да") | color(main_color),
            separator(),
            back_to_menu_btn_inf->Render() | center 
        }))
        | size(WIDTH, EQUAL, 60) 
        | size(HEIGHT, EQUAL, 15)
        | center
        | color(main_color)
        | bgcolor(main_bgcolor);
    });

    //экран главный, acrive_screen = 4
    auto main_screen = Renderer(main_container, [&]{
        return window(text("ATM VICTOR"), vbox({
            separator(),
            text("Привет, " + account.GetName()) | color(main_color),
            text("Сегодня " + today) | color(main_color),
            separator(),
            text("Ваши карты: " + account.getCard_list()) | color(account.isCard_listNull() ? Color::Red : main_color),
            separator(),
            main_menu->Render(),
            separator(),
            back_to_menu_btn_main->Render() 
        }))
        | size(WIDTH, EQUAL, 60) 
        | size(HEIGHT, EQUAL, 15)
        | center
        | color(main_color)
        | bgcolor(main_bgcolor);
    });

    // экран регистрации карты, active_screen = 5
    auto reg_card_screen = Renderer(reg_card_container, [&]{

        std::string card_num = last_card.registered() ? last_card.getCardNum() : "незарегестрирована";
        std::string card_data = last_card.registered() ? last_card.getData() : "незарегестрирована";
        std::string card_cvv = last_card.registered() ? last_card.getCVV() : "незарегестрирована";

        return window(text("Card registration"), vbox({
            separator(),
            text("Выберите банк.") | color(main_color),
            bank_choose->Render() | color(main_color),
            pincode_input->Render(),
            btn_generate_card->Render() | center,
            separator(),
            text("Номер карты: " + card_num) | color(last_card.registered() ? Color::Green : Color::Red),
            text("Дата: " + card_data) | color(last_card.registered() ? Color::Green : Color::Red),
            text("CVV: " + card_cvv) | color(last_card.registered() ? Color::Green : Color::Red),
            separator(),
            btn_confirm_card->Render(),
            btn_cancel_card->Render()
        }))
        | size(WIDTH, EQUAL, 70) 
        | size(HEIGHT, EQUAL, 25)
        | center
        | color(main_color)
        | bgcolor(main_bgcolor);
    });

    // экран вставки карты, active_screen = 6
    auto cardInsert_screen = Renderer(cardInsert_container, [&]{
        static constexpr std::string_view err = "карта не вставлена";

        std::string card_num {atm.checkCardInsert() ? last_card.getCardNum() : err};
        std::string card_data {atm.checkCardInsert() ? last_card.getData() : err};
        std::string card_cvv {atm.checkCardInsert() ? last_card.getCVV() : err};
        std::string card_bank {atm.checkCardInsert() ? last_card.getBankName() : err};
        return window(text("Insert card"), vbox({
            separator(),
            text("Выберите карту."),
            choose_card_toInsert->Render(),
            check_pincode->Render(),
            separator(),
            btn_confirm_cardInsert->Render(),
            btn_cancel_cardInsert->Render(),
            separator(),
            text("Карта: " + card_num) | color(last_card.registered() ? Color::Green : Color::Red),
            text("Банк: " + card_bank) | color(last_card.registered() ? Color::Green : Color::Red),
            text("Дата: " + card_data) | color(last_card.registered() ? Color::Green : Color::Red),
            text("CVV: " + card_cvv) | color(last_card.registered() ? Color::Green : Color::Red),
            separator(),
            btn_back_cardInsert->Render() | center
        }));
    });

    auto main_tabs = Container::Tab({
        start_menu_screen,
        login_screen,
        register_screen,
        information_screen,
        main_screen,
        reg_card_screen,
        cardInsert_screen
    },&active_screen);

    screen.Loop(main_tabs);
    std::system("clear");
    return 0;
}