#pragma once

#include <string>

enum class BankName {
    RofloBank,
    SbeerBank,
    Tankoff,
    MusorBank
};
class Bank{
    private:
        std::string name;
        int balance;
    public:
        void setName(std::string name_);
        void setBalance(int cash);
        int getBalance();
};