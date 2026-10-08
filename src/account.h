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
        Card regNewCard(BankName Bankname_,std::string& data_ ,std::string& pincode_);
        bool isCard_listNull() {return card_list.size() == 0;}


        void init_money_bag();
        bool get_banknotes_from_bag(int type_money, int cnt_banknotes);
};
