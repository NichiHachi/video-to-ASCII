#include <iostream>
#include <string>
using namespace std;

void loading_bar(int value_max, int value_at_the_moment){
    string loading_bar = to_string(value_at_the_moment*100/value_max)+"% ["+string(value_at_the_moment*60/value_max,'|')+string(60-value_at_the_moment*60/value_max,' ')+']';
    cout << "\r" << loading_bar;
}