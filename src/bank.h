#pragma once

#include <string>

enum class BankName {
    RofloBank,
    SbeerBank,
    Tankoff,
    MusorBank
};
class Bank_account{
    private:
        std::string name;
        long long balance = 0;
    public:
        void setName(std::string name_);
        void setBalance(int cash);
        long long getBalance();
        std::string getName() {return name;}
};