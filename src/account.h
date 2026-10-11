#pragma once

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <map>
#include "card.h"

class Account{
    private:
        std::string password;
        std::map<std::string,Card> card_list;
        std::string name;
        bool registred = false;
        std::map<int,int> money_bag;
    
    public:
        Account();

        void regAccount(
            const std::string& name_,
            const std::string& password_);
        void addCard(Card& card);
        void printInfo();
        std::string GetPass();
        std::string GetName();
        bool isRegistred(){return registred;}

        std::string getCard_list();
        const std::map<std::string,Card>& getCard_map();
        Card& getCard(const std::string& card_num) { return card_list.at(card_num); }
        Card regNewCard(BankName Bankname_,std::string& data_ ,std::string& pincode_);
        bool isCard_listNull() {return card_list.size() == 0;}


        void init_money_bag();
        int get_banknote_cnt(int type_money);
        void take_banknotes(int type_money, int cnt_banknotes);
        void add_banknotes(int type_money, int cnt_banknotes);
        std::string getMoneyBagList();
};
