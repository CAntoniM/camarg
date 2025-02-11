#include "camarg.h"
#include <algorithm>
#include <cctype>
#include <memory>
#include <sstream>
#include <string>

namespace cam {
namespace arg {
    using namespace std;
    const char* long_flag_prefix = "--";
    const char* short_flag_prefix = "-";

    // trim from start (in place)
    inline void ltrim(std::string& s)
    {
        s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                [](unsigned char ch) { return !std::isspace(ch); }));
    }

    // trim from end (in place)
    inline void rtrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                    [](unsigned char ch) { return !std::isspace(ch); })
                    .base(),
            s.end());
    }

    inline void trim(std::string& s)
    {
        rtrim(s);
        ltrim(s);
    }

    bool startsWith(std::string text, std::string t)
    {
        return text.find(t) == 0;
    }

    bool isValidName(std::string text)
    {
        bool is_short_flag = startsWith(text, short_flag_prefix);
        bool is_long_flag = startsWith(text, long_flag_prefix);
        bool is_empty = text.empty() || text.size() == 0;

        return !(is_short_flag || is_long_flag || is_empty);
    }

    Parser::Parser(std::string name, std::string help)
        : _name(name)
        , _desc(help)
    {
    }

    void Parser::flag(
        string name, string help, bool takes_value, string default_value)
    {
        trim(name);

        if (_has_parsed)
            throw AlreadyParsed();

        if (!isValidName(name))
            throw InvalidName();

        if (exists(name))
            throw AlreadyExsists();

        _flags[name] = { name, help, takes_value, default_value };
    }

    void Parser::cmd(std::unique_ptr<Parser> parser)
    {
        if (_has_parsed)
            throw AlreadyParsed();

        std::string name = parser->name();

        if (!isValidName(name))
            throw InvalidName();

        if (_cmds.find(name) != _cmds.end())
            throw AlreadyExsists();

        if (!_args.empty())
            throw InvalidParserConfig();

        _cmds[name]
            = { name, parser->parserDescription(), true, std::move(parser) };
    }

    void Parser::arg(string name, string help, string default_value)
    {
        trim(name);

        if (_has_parsed)
            throw AlreadyParsed();

        if (!isValidName(name))
            throw InvalidName();

        if (exists(name))
            throw AlreadyExsists();

        if (!_cmds.empty())
            throw InvalidParserConfig();

        _args.push_back({ name, help, true, default_value });
    }

    void Parser::parse(int argc, const char* argv[])
    {

        if (_has_parsed == true)
            throw AlreadyParsed();

        _has_parsed = true;

        if (_args.empty() && _flags.empty() && _cmds.empty())
            throw InvalidParserConfig();

        int argument_position = 0;

        for (int index = 1; index < argc; index++) {

            if (startsWith(argv[index], long_flag_prefix)
                || startsWith(argv[index], short_flag_prefix)) {

                const char* name = argv[index];

                while (*name == '-')
                    name++;

                if (*name == '\0')
                    throw InvalidCommand();

                if (_flags.find(name) == _flags.end())
                    throw InvalidCommand();

                if (_flags[name].takes_value) {
                    if (index + 1 >= argc)
                        throw InvalidCommand();

                    index++;
                    _flags[name].data = argv[index];
                } else {
                    _flags[name].data = FLAG_USED();
                }
            } else if (!_args.empty()) {

                if (argument_position >= _args.size())
                    throw InvalidCommand();

                _args[argument_position].data = argv[index];
                argument_position++;

            } else if (!_cmds.empty()) {

                if (_cmds.find(argv[index]) == _cmds.end())
                    throw InvalidCommand();

                _sub_command = argv[index];

                _cmds[_sub_command].data->parse(argc - index, &argv[index]);
                return;
            } else
                throw InvalidCommand();
        }
    }

    std::string Parser::help()
    {
        std::stringstream sstr;

        int longest_name = 0;
        const char* spacer = "         ";
        auto max = [](int a, int b) { return a > b ? a : b; };
        auto gen_spacer = [&](const std::string& name) {
            std::string return_value;

            for (int i = 0; i < (longest_name - name.size()); i++)
                return_value.push_back(' ');

            return return_value;
        };

        sstr << "Info:\n\t" << _desc << '\n';
        if (!_flags.empty()) {
            sstr << "\nFlags:\n";

            for (auto flag = _flags.begin(); flag != _flags.end(); flag++)
                longest_name = max(flag->first.length(), longest_name);

            for (auto flag = _flags.begin(); flag != _flags.end(); flag++) {
                sstr << '\t' << '-' << flag->first << " | --" << flag->first
                     << gen_spacer(flag->first) << gen_spacer(flag->first)
                     << spacer << flag->second.help << '\n';
            }
        }

        if (!_args.empty()) {
            longest_name = 0;

            sstr << "\nArgs:\n";

            for (auto arg = _args.begin(); arg != _args.end(); arg++) {
                longest_name = max(arg->name.length(), longest_name);
            }

            for (auto arg = _args.begin(); arg != _args.end(); arg++) {
                sstr << '\t' << '[' << arg->name << ']' << gen_spacer(arg->name)
                     << spacer << arg->help << '\n';
            }
        }

        if (!_cmds.empty()) {
            longest_name = 0;

            sstr << "\nCommands:\n";

            for (auto cmd = _cmds.begin(); cmd != _cmds.end(); cmd++)
                longest_name = max(cmd->first.length(), longest_name);

            for (auto cmd = _cmds.begin(); cmd != _cmds.end(); cmd++) {
                sstr << '\t' << cmd->first << gen_spacer(cmd->first) << spacer
                     << cmd->second.help << '\n';
            }
        }

        return sstr.str();
    }

    std::unique_ptr<Parser> Parser::subcommand()
    {

        if (!_has_parsed)
            throw AlreadyParsed();

        if (_cmds.find(_sub_command) == _cmds.end())
            throw InvalidParserConfig();

        if (_sub_command.empty())
            return nullptr;

        return std::move(_cmds[_sub_command].data);
    }

    std::string Parser::get(std::string name)
    {
        if (!_has_parsed)
            throw AlreadyParsed();

        if (name.empty() || !isValidName(name))
            throw InvalidName();

        auto find = [](std::vector<ArgumentData<string>>& args, string name) {
            auto itr = args.begin();

            for (; itr != args.end(); itr++)
                if (itr->name == name)
                    return itr;

            return itr;
        };

        bool is_flag = _flags.find(name) != _flags.end();
        auto arg = find(_args, name);
        bool is_arg = arg != _args.end();

        if (is_flag)
            return _flags[name].data;
        else if (is_arg)
            return arg->data;
        else
            throw NoSuchValue();
    }

    bool Parser::exists(std::string name)
    {

        bool is_in_args = false;

        for (auto itr = _args.begin(); itr != _args.end(); itr++) {
            if (itr->name == name) {
                is_in_args = true;
                break;
            }
        }

        bool is_in_flags = _flags.find(name) != _flags.end();

        return is_in_flags || is_in_args;
    };

    std::string Parser::name() { return _name; }

    std::string Parser::parserDescription() { return _desc; }

}
}
