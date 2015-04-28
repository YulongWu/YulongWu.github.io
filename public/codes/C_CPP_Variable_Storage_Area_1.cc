#include <iostream>
using namespace std;

int main() {
    char *s_literal = "This is literal string.";
    char s_array[] = "This is literal string.";
    s_array[0] = 'A';
    cout << s_array << endl;
    s_literal[0] = 'A';
    cout << s_literas_literal << endl;
}
