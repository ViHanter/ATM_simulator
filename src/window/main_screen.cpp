#include "main_screen.h"

int main(){
    using namespace ftxui;

    Element document = hbox({
    text("left")   | border,
    text("middle") | border | flex,
    text("right")  | border,
    });

    auto screen = Screen::Create(
        Dimension::Full(),       // ширина
        Dimension::Fit(document) // высота
    );

    ftxui::Render(screen, document);
    screen.Print();

    return 0;
}