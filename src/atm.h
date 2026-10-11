#pragma once

#include "card.h"
#include "account.h"
#include <string>
#include <iostream>
#include <map>
#include <random>

enum class WithdrawResult {
    Success,
    NoCard,
    InvalidAmount,
    NotEnoughBalance,
    NotEnoughCash
};

std::string withdrawResultText(WithdrawResult result);

class Database;

class ATM {
    private:
        Account* account = nullptr;
        Card* card = nullptr;

        bool isCardInsert = false;
        bool isLog = false;
        std::map<int,int> atm_money_bag;
        std::map<int,int> transaction_map;
        std::map<int,int> transaction_error_map;
        std::map<int,int> dispensed_map;

        void init_money_bag();
    public:
        ATM();

        int insertCard(Card& card_, const std::string& pin_code_);
        void eraseCard();
        void log(Account& account_, const std::string& pass);
        bool checkCardInsert() {return isCardInsert;}

        long long getBalance();
        void getCard(Card &card_);
        bool isLogged();
        std::string getStatus();

        // Пополнение
        long long put_money_transaction(int type_money);
        void clear_transaction_map() {transaction_map.clear();}
        int get_banknote_cnt(int type_money);
        long long topup_account_transaction();
        void get_rejected_banknotes(std::string& rejected);

        // Снятие
        bool can_withdraw(long long money);
        bool pick_banknotes(long long money, std::map<int,int>& out);
        WithdrawResult withdrew_transaction(long long money);
        void get_dispensed_banknotes(std::string& dispensed);

        // ---- Работа с БД (касса банкомата) ----
        bool dbLoadMoneyBag(Database& db);
        bool dbSaveMoneyBag(Database& db);
};