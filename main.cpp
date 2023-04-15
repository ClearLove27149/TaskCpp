#include <iostream>
#include <variant>
#include <string>

struct Visitor{
    void operator() (int x) {

    }
    void operator() (std::string s) {

    }
};
int main() {
    std::cout << "Hello, World!" << std::endl;
    std::variant<int, std::string> vv;
    vv = std::to_string(1);
    int x = 2;
    std::visit(Visitor(),
               vv);
    return 0;
}
