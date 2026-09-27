#pragma once

#include "card.h"
#include "account.h"
#include <string>
#include <iostream>

class ATM {
    private:
        Account account;
        Card card;

        bool isCardInsert = false;
        bool isLog = false;
    public:
        void insertCard(const Card& card_, const std::string& pin_code_);
        void log(Account& account_, const std::string& pass);
        bool checkCardInsert() {return isCardInsert;}

        double getBalance();
        bool isLogged(){return account.isRegistred();}
        std::string getStatus();
};