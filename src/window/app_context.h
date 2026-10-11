#pragma once

#include <ctime>
#include <functional>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "account.h"
#include "atm.h"
#include "card.h"
#include "db.h"

// ==================
// Общее состояние приложения (разделяется всеми экранами)
// ==================

struct AppContext {
    // палитра
    ftxui::Color main_bgcolor = ftxui::Color::RGBA(245, 246, 248,0); //rgb(0, 163, 16)
    ftxui::Color main_color = ftxui::Color::RGB(0, 163, 16);
    ftxui::Color second_color = ftxui::Color::RGB(10, 34, 64);
    ftxui::Color mark_color = ftxui::Color::RGB(91, 254, 153); //

    //глобал переменные
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::TerminalOutput();
    int active_screen = 0;

    Account account;
    ATM atm;
    Card current_card;
    Database db;

    std::string login;
    std::string password;
    bool isLog = false;
    long long transaction = 0;
    std::string reject_banknotes = "";

    // Error map filling

    // TODO: организация ошибок

    int error_code = -1;
    std::string error_log;
    std::vector<std::string> error_vector;

    int day;
    int month;
    int year;
    std::string today;

    // общие для регистрации карты и вставки карты
    std::string pincode;
    Card last_card;

    // стиль кнопки
    ftxui::ButtonOption btn_config;

    // сброс выбранных пунктов меню при возврате в меню
    std::vector<std::function<void()>> reset_on_start_menu;
    std::vector<std::function<void()>> reset_on_main_menu;

    void go_start_menu() {
        for (auto& reset : reset_on_start_menu) reset();
        active_screen = 0;
    }
    void go_main_menu() {
        for (auto& reset : reset_on_main_menu) reset();
        active_screen = 4;
    }

    // Сбросить вставленную в банкомат карту (например, при смене аккаунта)
    void reset_inserted_card() {
        atm.eraseCard();
        current_card = Card();
        last_card = Card();
        transaction = 0;
        reject_banknotes = "";
        atm.clear_transaction_map();
    }

    AppContext() {
        std::time_t now = std::time(nullptr);
        std::tm* local_time = std::localtime(&now);
        day = local_time->tm_mday;
        month = local_time->tm_mon + 1;
        year = local_time->tm_year + 1900;
        today = std::to_string(day) + '.' + std::to_string(month) + '.' + std::to_string(year);

        btn_config.transform = [this](const ftxui::EntryState& state) {
            auto element = ftxui::text(state.label);
            if (state.focused){
                return element | ftxui::center | ftxui::borderRounded | ftxui::color(mark_color);
            }
            else return element | ftxui::center | ftxui::borderRounded | ftxui::color(main_color);
        };

        if (!db.ok()) {
            error_log = "Нет соединения с БД: " + db.error();
        } else if (!db.ensureSchema()) {
            error_log = "Ошибка инициализации БД: " + db.error();
        } else {
            atm.dbLoadMoneyBag(db);
        }
    }
};
