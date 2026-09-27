#include "main_screen.h"

int main(){
    using namespace ftxui;
    
    
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

    int selected_opt = 0;
    std::vector<std::string> start_menu_items = {
        "1. Войти в систему",
        "2. Регистрация",
        "3. Инфо",
        "4. Начать работу",
        "5. Выйти"
    };

    MenuOption start_menu_option;
    
    start_menu_option.on_enter = [&] {
        switch (selected_opt){
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
    auto start_menu = Menu(&start_menu_items,&selected_opt,start_menu_option);
    auto start_menu_hotkey = CatchEvent(start_menu, [&](Event event){
        bool changed = false;
        
        if (event == Event::Character('1')){
            selected_opt = 0;
            changed = true;
        }
        if (event == Event::Character('2')){
            selected_opt = 1;
            changed = true;
        }
        if (event == Event::Character('3')){
            selected_opt = 2;
            changed = true;
        }
        if (event == Event::Character('4')){
            selected_opt = 3; 
            changed = true;
        }
        if (event == Event::Character('5')){
            selected_opt = 4; 
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
        }
    },btn_config);
    auto back_to_menu_btn = Button("Назад в меню", [&] {active_screen = 0;},btn_config);

    auto log_btns = Container::Horizontal({
        log_confirm_btn,
        back_to_menu_btn
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

    auto input_login_reg = Input(&login,"Введите логин: ", input_option_log);
    auto input_password_reg = Input(&password,"Введите пароль: ", input_option_pass);
    auto repeat_password = Input(&repeat_pass, "Повторите пароль:", input_option_pass);

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

    auto reg_btns = Container::Horizontal({
        reg_confirm_btn,
        back_to_menu_btn
    });
    auto reg_container = Container::Vertical({
        input_login,
        input_password,
        repeat_password,
        reg_btns
    }, &login_focus_ind);    

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

    auto info_container = Container::Vertical({
        back_to_menu_btn
    });

    // =====================
    // MAIN WINDOW
    // =====================

    auto main_container = Container::Vertical({
        back_to_menu_btn
    });

    // экран menu, active_screen = 0
    auto start_menu_screen = Renderer(start_menu_container, [&] {
        std::system("clear");
        std::string login_status = isLog ? account.GetName() : "неавторизован";
        std::string error_status = error_log != "" ? error_log : "ничего";

        return window(text("VICTOR ATM - main menu"), vbox({
            separator(),
            text("Аккаунт: " + login_status) | color(isLog ? Color::Green : Color::Red),
            text("\nВыберите действие: ") | bold,
            separator(),
            start_menu->Render(),
            separator(),
            text("Текущий выбор: " + start_menu_items[selected_opt]),
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
        std::system("clear");
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
        std::system("clear");
        return window(text("Registration session"), vbox({
            separator(),
            input_login->Render(),
            input_password->Render(),
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
        std::system("clear");
        std::string login_status = isLog ? account.GetName() : "неавторизован";
        return window(text("Information"), vbox({
            separator(),
            text("Login: " + login_status) | color(isLog ? Color::Green : Color::Red),
            text("Vesion: 0.3") | color(main_color), 
            text("Это супер-мега-крутой банкомат Виктора, да-да") | color(main_color),
            separator(),
            back_to_menu_btn->Render() | center 
        }))
        | size(WIDTH, EQUAL, 60) 
        | size(HEIGHT, EQUAL, 15)
        | center
        | color(main_color)
        | bgcolor(main_bgcolor);
    });

    //экран главный, acrive_screen = 4
    auto main_screen = Renderer(main_container, [&]{
        std::system("clear");

        return window(text("ATM VICTOR"), vbox({
            separator(),
            text("Привет, " + account.GetName()) | color(main_color),
            text("Сегодня " + today) | color(main_color),
            separator(),
            text("Ваши карты: " + account.getCard_list()) | color(account.isCard_listNull() ? Color::Red : main_color),
            separator(),
            back_to_menu_btn->Render() 
        }))
        | size(WIDTH, EQUAL, 60) 
        | size(HEIGHT, EQUAL, 15)
        | center
        | color(main_color)
        | bgcolor(main_bgcolor);
    });

    auto main_tabs = Container::Tab({
        start_menu_screen,
        login_screen,
        register_screen,
        information_screen,
        main_screen
    },&active_screen);

    screen.Loop(main_tabs);
    std::system("clear");
    return 0;
}