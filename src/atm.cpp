#include "atm.h"
#include <vector>

ATM::ATM(){
    init_money_bag();
}

void ATM::init_money_bag(){
    atm_money_bag[50] = 5;
    atm_money_bag[100] = 5;
    atm_money_bag[200] = 5;
    atm_money_bag[500] = 5;
    atm_money_bag[1000] = 5;
    atm_money_bag[2000] = 5;
    atm_money_bag[5000] = 5;
}

int ATM::insertCard(const Card& card_, const std::string& pin_code_){
    if (!card_.registered()) {};
    if (isCardInsert) {}; //TODO: обработать ошибки
    if (!card_.checkPin(pin_code_)) {return 1;}

    isCardInsert = true;
    card = card_;  
    return 0;
}
void ATM::eraseCard(){
    isCardInsert = false;
    card = Card();
}

long long ATM::getBalance(){
    if (!isCardInsert) {return -1;}
    
    return card.chekBalance();
}

void ATM::log(Account& account_,const std::string& pass){
    if (account_.GetPass() == pass) {
        account = account_;
    }
}

std::string ATM::getStatus(){
    std::string result = "Account: " + account.GetName() + '\n';
    if (isCardInsert) {
        result += "Card: " + card.getCardNum() + '\n' + "isCardInsert: True";
    } else result += "isCardInsert: False";

    return result;
}
long long ATM::put_money_transaction(int type_money){
    transaction_map[type_money]++; 
    return type_money;
}
void ATM::topup_account_transaction(){
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> roll_luck(1, 100); 
    long long result = 0;

    for (auto[type,cnt] : transaction_map){
        for (int i=0;i<cnt;i++){
            int chance = roll_luck(gen);
            if (chance<6){
                result +=type;
            } else transaction_error_map[type] +=1;
        }
    }
    transaction_map.clear();
    
    int status = card.push_request_tupup(result);
}
void ATM::get_rejected_banknotes(std::string& rejected){
    for (auto[type,cnt] : transaction_error_map){
        rejected += std::to_string(type) + "x" + std::to_string(cnt) + ' ';
    }
    transaction_error_map.clear();
}


int ATM::get_banknote_cnt(int type_money){
    return transaction_map[type_money];
}

bool ATM::pick_banknotes(long long money, std::map<int,int>& out){
    out.clear();

    if (money <= 0) return false;
    if (money % 50 != 0) return false;

    long long total = 0;
    for (const auto&[type,cnt] : atm_money_bag){
        total += 1LL * type * cnt;
    }
    if (money > total) return false;

    // can[s]  - сумму s можно собрать из купюр
    // from[s] - предыдущая сумма на пути к s
    // bill[s] - купюра, которой получили сумму s
    std::vector<bool> can(money + 1, false);
    std::vector<long long> from(money + 1, -1);
    std::vector<int> bill(money + 1, -1);
    can[0] = true;

    // сначала крупные номиналы - чтобы предпочтение отдавалось меньшему числу купюр
    for (auto it = atm_money_bag.rbegin(); it != atm_money_bag.rend(); ++it){
        const auto&[type,avail] = *it;
        // used[s] - сколько купюр данного номинала на пути к сумме s
        std::vector<int> used(money + 1, 0);
        for (long long s = type; s <= money; ++s){
            if (can[s] || !can[s - type]) continue;
            if (used[s - type] >= avail) continue;

            can[s] = true;
            used[s] = used[s - type] + 1;
            from[s] = s - type;
            bill[s] = type;
        }
    }
    if (!can[money]) return false;

    for (long long s = money; s > 0; s = from[s]){
        out[bill[s]]++;
    }

    for (const auto&[type,cnt] : out){
        if (cnt > atm_money_bag.at(type)) {
            out.clear();
            return false;
        }
    }
    return true;
}

bool ATM::can_withdraw(long long money){
    std::map<int,int> tmp;
    return pick_banknotes(money, tmp);
}

void ATM::withdrew_transaction(long long money){
    if (!pick_banknotes(money, dispensed_map)) {
        dispensed_map.clear();
        return;
    }
    for (const auto&[type,cnt] : dispensed_map){
        atm_money_bag[type] -= cnt;
    }
}

void ATM::get_dispensed_banknotes(std::string& dispensed){
    for (const auto&[type,cnt] : dispensed_map){
        dispensed += std::to_string(type) + "x" + std::to_string(cnt) + ' ';
    }
    dispensed_map.clear();
}