#ifndef _camarg_h_
#define _camarg_h_

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <vector>

#ifndef CAMEXPORT
#ifdef WIN32
#define CAMEXPORT __declspec(dllexport)
#else
#define CAMEXPORT
#endif
#endif

namespace cam {
namespace arg {

    using std::string;
    using std::unique_ptr;

    /**
     * @brief this is an error that is throw in an value already exists in the
     *        in the Parser;
     *
     */
    class CAMEXPORT AlreadyExsists : public std::exception {
        const char* what() const noexcept { return "AlreadyExists"; }
    };

    /**
     * @brief this is an error that is thrown in the case where the user calls
     *        a function that is only valid prethe parse function being called
     *
     */
    class CAMEXPORT AlreadyParsed : public std::exception {
        const char* what() const noexcept { return "AlreadyParsed"; }
    };

    /**
     * @brief this is an error that is thorwn if the name of a value does not
     * pass our validation checks
     *
     */
    class CAMEXPORT InvalidName : public std::exception {
        const char* what() const noexcept { return "InvalidName"; }
    };

    /**
     * @brief this is an error that is thrown if the user tries to configure the
     * the parser in such a way that is unsupported
     *
     */
    class CAMEXPORT InvalidParserConfig : public std::exception {
        const char* what() const noexcept { return "InvalidParserConfig"; }
    };

    /**
     * @brief this is error that is thrown if the user tried to retreve a value
     *        that has not been defined in the parser.
     *
     */
    class CAMEXPORT NoSuchValue : public std::exception {
        const char* what() const noexcept { return "NoSuchValue"; }
    };

    /**
     * @brief this is an error when the user provides an input that does not
     * match the configuration that was set up before parsing;
     *
     */
    class CAMEXPORT InvalidCommand : public std::exception {
        const char* what() const noexcept { return "InvalidCommand"; }
    };

    /**
     * @brief A parser that handles the converting values from standard unix
     * style arguments to a keyvalue store
     *
     */
    class CAMEXPORT Parser {

    public:
        Parser(std::string name, std::string help);
        /**
         * @brief Add a flag to the parser
         *
         * @throws AlreadyExists if a flag or argument already exists in the
         * parser
         * @throws AlreadyParsed if the parser has already parsed so is not
         * excepting new flags
         * @throws InvalidName if the name of the flag does not pass the
         * validation checks
         *
         *
         * @param name the name of the flag with out the preceeding -
         * @param help the description of the flag provided to the users
         * @param takes_value defines if the flag expects to have a value
         * provided
         * @param default_value defines the default value that will be places in
         *                      to the flag if no value is provided
         */
        void flag(string name, string help, bool takes_value,
            string default_value = "");
        /**
         * @brief Add a subcommand to this parser
         *
         * @throws AlreadyExists if their is a subcommand with that name
         * regiesterd in the parser
         * @throws AlreadyParsed if the parser has already parsed so it is not
         * excepting new subcommmands
         * @throws InvalidName if the name provided does not pass our validation
         * checks
         * @throws InvalidParserConfig if a postional argument (arg) is alredy
         * defined
         *
         * @warning this feature is mutaly exclusive with the arg function on a
         * single instance of the class
         *
         * @param parser a pointer to the parser you want to parse
         */
        void cmd(unique_ptr<Parser> parser);
        /**
         * @brief add a positonal argument
         *
         * @throws AlreadyExists if their is a subcommand with that name
         * regiesterd in the parser
         * @throws AlreadyParsed if the parser has already parsed so it is not
         * excepting new subcommmands
         * @throws InvalidName if the name provided does not pass our validation
         * checks
         * @throws InvalidParserConfig if a postional argument (arg) is alredy
         * defined
         *
         * @param name the name of the argument will be printed in the help text
         * @param help the description of the argument
         * @param default_value the value that will be returned if no value of
         * provided
         */
        void arg(string name, string help, string default_value = "");
        /**
         * @brief parses the commandline argument and enabled get* methods
         *
         * @throws InvalidCommand if the user provides commands that can not be
         * mapped onto the Configuration of this parser;
         * @throws AlreadyParsed if the parse function has already been called
         * @throws InvalidParserConfig if no configuration has been provided to
         * the parser;
         *
         *
         * @param argc
         * @param argv
         */
        void parse(int argc, const char* argv[]);

        /**
         * @brief return a string contain the help message that is returned;
         *
         * @return string
         */
        string help();

        /**
         * @brief returns a pointer to the subcommand
         *
         * @throws AlreadyParsed if the parse() function has not been called
         * @throws InvalidParserConfig if the subcommand that is in the parser
         * does not exist
         *
         * @return unique_ptr<Parser> points to the subparser that was called by
         *         the user or a nullptr_t if their is no subcommand called
         */
        unique_ptr<Parser> subcommand();

        /**
         * @brief returns a copy of the value of the argument with the following
         * name
         *
         * @throws AlreadyParsed if the parse function has not been called
         * @throws InvalidName if the name does not meet the naming critera
         * @throws NoSuchValue if their is no arg or flag with that name
         * regiesterd;
         *
         * @return string
         */
        string get(string);

        /**
         * @brief tests if the name provided alredy exits in the namespace of
         * parser
         *
         */
        bool exists(std::string);

        /**
         * @brief returns the name of the command the parser is for;
         *
         * @return std::string
         */
        std::string name();

        /**
         * @brief return the help message for this parser
         *
         * @return std::string
         */
        std::string parserDescription();

        /**
         * @brief returns the value that is set if a flag that does not provide
         * a value is chosen
         *
         * @return constexpr const char*
         */
        constexpr const char* FLAG_USED() { return "true"; }

    private:
        /**
         * @brief Helper type used to store infomation regarding each argument
         *
         * @tparam t
         */
        template <typename t> struct ArgumentData {
            string name;
            string help;
            bool takes_value;
            t data;
        };
        std::string _name;
        std::string _desc;
        std::string _sub_command;
        bool _has_parsed = false;
        std::map<string, ArgumentData<string>> _flags;
        std::vector<ArgumentData<string>> _args;
        std::map<string, ArgumentData<unique_ptr<Parser>>> _cmds;
    };

} // namespace arg
} // namespace cam

#endif
