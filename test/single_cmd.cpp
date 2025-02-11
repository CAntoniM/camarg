#include "camarg.h"
#include <iostream>
#include <memory>

int main(int arg, char** argv)
{
    int arg_c = 2;
    const char* arg_v[2] = { "test", "ls" };

    const char* name = "test";

    cam::arg::Parser parser("test", "test");

    std::unique_ptr<cam::arg::Parser> subcommand(
        new cam::arg::Parser("ls", "list al files"));

    subcommand->arg("dir", "directory you want to parse", ".");

    parser.cmd(std::move(subcommand));

    parser.parse(arg_c, arg_v);

    std::unique_ptr<cam::arg::Parser> result = parser.subcommand();

    if (result != nullptr) {
        std::cout << "PASS" << std::endl;
        return 0;
    } else {
        std::cout << "FAIL" << std::endl;
        return 1;
    }
}
