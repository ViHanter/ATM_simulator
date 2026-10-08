#pragma once

#include "card.h"
#include "account.h"
#include <string>
#include <iostream>
#include <map>
#include <random>

class ATM {
    private:
        Account account;
        Card card;

        bool isCardInsert = false;
        bool isLog = false;
        std::map<int,int> atm_money_bag;
        std::map<int,int> transaction_map;
        std::map<int,int> transaction_error_map;
        std::map<int,int> dispensed_map;

        void init_money_bag();
    public:
        ATM();

        int insertCard(const Card& card_, const std::string& pin_code_);
        void eraseCard();
        void log(Account& account_, const std::string& pass);
        bool checkCardInsert() {return isCardInsert;}

        long long getBalance();
        void getCard(Card &card_) {card_=card;}
        bool isLogged(){return account.isRegistred();}
        std::string getStatus();

        

        long long put_money_transaction(int type_money);
        void clear_transaction_map() {transaction_map.clear();}
        int get_banknote_cnt(int type_money);
        void withdrew_transaction(long long money);
        bool can_withdraw(long long money);
        bool pick_banknotes(long long money, std::map<int,int>& out);
        void topup_account_transaction();
        void get_rejected_banknotes(std::string& rejected);
        void get_dispensed_banknotes(std::string& dispensed);
};