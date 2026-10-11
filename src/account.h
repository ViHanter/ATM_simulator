#pragma once

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <map>
#include "card.h"

class Database;

struct TransactionHistoryEntry {
    std::string created_at;
    std::string type;
    std::string card_num;
    long long   amount = 0;
    std::string detail;
};

class Account{
    private:
        std::string password;
        std::map<std::string,Card> card_list;
        std::string name;
        bool registred = false;
        std::map<int,int> money_bag;
        int db_id_ = -1;
    
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
        void reset();

        // работа с бд (psql)
        bool dbRegister(Database& db, const std::string& name_, const std::string& password_);
        bool dbLogin(Database& db, const std::string& name_, const std::string& password_);
        bool dbAddCard(Database& db, const Card& card);
        bool dbUpdateBalance(Database& db, const std::string& card_num, long long balance);
        bool dbLoadMoneyBag(Database& db);
        bool dbSaveMoneyBag(Database& db);
        bool dbAddTransaction(Database& db, const std::string& card_num,
                              const std::string& type, long long amount,
                              const std::string& detail);
        std::vector<TransactionHistoryEntry> getTransactionHistory(Database& db);

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
