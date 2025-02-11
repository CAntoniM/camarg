#include "camarg.h"
#include <iostream>

int main(int argc, char** argv)
{
    int arg_c = 3;
    const char* arg_v[3] = { "test", "--flag", "test" };
    cam::arg::Parser parser("single_flag",
        "the most basic test of adding a single flag and correctly parsing it");

    const char* name = "flag";

    parser.flag(name, "a test flag", true);

    parser.parse(arg_c, arg_v);

    if (parser.get(name) == "test") {
        std::cout << "PASS" << std::endl;
        return 0;
    } else {
        std::cout << "FAIL" << std::endl;
        return 1;
    }
}