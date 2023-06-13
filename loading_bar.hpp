#include <iostream>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#endif

void loading_bar(int percent){
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    #endif
    //Color YELLOW
    if(percent!=100){
        std::cout << "\033[33m";
    }
    //Color GREEN
    else{
        std::cout << "\033[32m";
    }
    std::string loading_bar = std::to_string(percent)+"% ["+std::string(percent/2,'|')+std::string(50-percent/2,' ')+']';
    std::cout << "\r" << loading_bar << "\033[0m";
}