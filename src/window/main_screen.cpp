#include "main_screen.h"

int main(){
    using namespace ftxui;
    Account account;
    int program_status = 0;
    std::string error_log;
    
    auto screen = ScreenInteractive::TerminalOutput();
    int active_screen = 0;
    

    // MENU WINDOW
    // ==================

    int selected_opt = 0;
    std::vector<std::string> menu_items = {
        "1. Войти в систему",
        "2. Регистрация",
        "3. Инфо",
        "4. Выйти"
    };

    MenuOption menu_option;
    menu_option.entries_option.transform = [](const EntryState& state){
        auto element = text(state.label);
        if (state.focused){
            return element | color(Color::DarkOrange3) | bgcolor(Color::DarkRed);
        } else return element | color(Color::DarkRed);
    };
    menu_option.on_enter = [&] {
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
                //exit
                screen.Exit();
                break;
        }
    };
    auto menu = Menu(&menu_items,&selected_opt,menu_option);
    
    auto menu_container = Container::Vertical({
        menu
    }) | color(Color::DarkRed);

    // ===================
    // LOGIN WINDOW
    // ====================
    std::string login;
    std::string password;
    int login_focus_ind = 0; // 0 - login, 1 - password, 2 - btns

    InputOption input_option_log;
    InputOption input_option_pass;

    input_option_log.multiline = false;
    input_option_log.on_enter = [&] {login_focus_ind = 1;};

    input_option_pass.multiline = false;
    input_option_pass.on_enter = [&] {login_focus_ind = 2;};
    input_option_pass.password = true;

    auto input_login = Input(&login,"Введите логин: ", input_option_log);
    auto input_password = Input(&password,"Введите пароль: ", input_option_pass);

    // стиль кнопки
    ButtonOption btn_config;
    btn_config.transform = [](const EntryState& state) {
        auto element = text(state.label);
        if (state.focused){
        return element | center | borderRounded | color(Color::DarkOrange3);
        }
        else return element | center | borderRounded | color(Color::DarkRed);
    };


    auto log_confirm_btn = Button("Подтвердить", [&] {
        if (login == "" || password == "") {
            program_status = 1;
            error_log = "Неккоректный ввод.";
        } // неккоректный ввод
        else {
            account.regAccount(login,password);
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

    // экран menu, active_screen = 0
    auto menu_screen = Renderer(menu_container, [&] {
        std::system("clear");
        return window(text("VICTOR ATM - main menu"), vbox({
            separator(),
            text("Выберите действие: ") | bold,
            separator(),
            menu->Render(),
            separator(),
            text("Текущий выбор: " + menu_items[selected_opt])
        }))
        | size(WIDTH, EQUAL, 60) 
        | size(HEIGHT, EQUAL, 15)
        | center;
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
        | center;
    });

    auto main_tabs = Container::Tab({
        menu_screen,
        login_screen
    },&active_screen);

    screen.Loop(main_tabs);
    std::system("clear");
    return 0;
}