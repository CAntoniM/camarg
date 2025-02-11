#include "camarg.h"
#include <iostream>

int main(int argc, const char* argv[])
{

    int arg_c = 2;
    const char* arg_v[2] = { "test", "test" };

    const char* name = "flag";

    cam::arg::Parser parser("test", "a test command");
    parser.arg(name, "test");

    parser.parse(arg_c, arg_v);

    if (parser.get(name) == "test") {
        std::cout << "PASS" << std::endl;
        return 0;
    } else {
        std::cout << "FAIL" << std::endl;
        return 1;
    }
}
