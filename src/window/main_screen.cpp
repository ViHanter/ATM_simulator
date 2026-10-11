#include "main_screen.h"

#include "app_context.h"
#include "screens.h"

int main(){
    using namespace ftxui;
    std::system("clear");

    AppContext ctx;

    // экраны в порядке active_screen: 0..7
    StartMenuScreen start_menu_screen(ctx);
    LoginScreen login_screen(ctx);
    RegisterScreen register_screen(ctx);
    InformationScreen information_screen(ctx);
    MainMenuScreen main_menu_screen(ctx, start_menu_screen);
    RegCardScreen reg_card_screen(ctx);
    CardInsertScreen card_insert_screen(ctx);
    TopupWithdrawScreen topup_withdrew_screen(ctx);

    auto main_tabs = Container::Tab({
        start_menu_screen.renderer(),
        login_screen.renderer(),
        register_screen.renderer(),
        information_screen.renderer(),
        main_menu_screen.renderer(),
        reg_card_screen.renderer(),
        card_insert_screen.renderer(),
        topup_withdrew_screen.renderer()
    },&ctx.active_screen);

    ctx.screen.Loop(main_tabs);
    std::system("clear");
    return 0;
}
