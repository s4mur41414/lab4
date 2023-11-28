#pragma once
#include <cstdint>
#include <iostream>
#include <vector>

namespace ArgumentParser {

    class StringArgument {
    public:
        StringArgument(char short_n){
            short_name = short_n;
        }

        StringArgument(std::string long_n){
            long_name = long_n;
        }

        StringArgument(char short_n, std::string long_n){
            short_name = short_n;
            long_name = long_n;
        }

        StringArgument& Default(const std::string& value) {
            key = value;
            default_value = true;
            return *this;
       }

        StringArgument& StoreValue(std::string& value) {
            value = key;
            partner.push_back(&value);
            return *this;
       }

        StringArgument& MultiValue(int64_t min_count) {
            multi_value = true;
            min_args_count = min_count;
            return *this;
        }

        char short_name = '\t';
        bool default_value = false;
        bool multi_value = false;
        int64_t min_args_count = 0;
        std::vector<std::string> keys;
        std::vector<std::string *> partner;
        std::string long_name;
        std::string key;
        std::string description;
    };

    class IntArgument {
    public:
        IntArgument(char short_n) {
            short_name = short_n;
        }

        IntArgument(std::string long_n){
            long_name = long_n;
        }

        IntArgument(char short_n, std::string long_n) {
            short_name = short_n;
            long_name = long_n;
        }

        IntArgument& Default(int value) {
            key = value;
            default_value = true;
            return *this;
       }

        IntArgument& StoreValue(int& value) {
            value = key;
            partner.push_back(&value);
            return *this;
       }

        IntArgument& StoreValues(std::vector<int>& int_values) {
            int_values = keys;
            partner_multi.push_back(&int_values);
            return *this;
       }

        IntArgument& MultiValue() {
            multi_value = true;
            return *this;
        }

        IntArgument& MultiValue(int64_t min_count) {
            multi_value = true;
            min_args_count = min_count;
            return *this;
        }

        IntArgument& Positional() {
            positional = true;
            return *this;
        }


        char short_name = '\t';
        bool default_value = false;
        bool positional = false;
        bool multi_value = false;
        int64_t min_args_count = 0;
        std::vector<int *> partner;
        std::vector<std::vector<int> *> partner_multi;
        std::vector<int> keys;
        std::string description;
        std::string long_name;
        int key;
    };

    class FlagArgument {
    public:
        FlagArgument(char short_n) {
            short_name = short_n;
        }

        FlagArgument(std::string long_n){
            long_name = long_n;
        }

        FlagArgument(char short_n, std::string long_n) {
            short_name = short_n;
            long_name = long_n;
        }

        FlagArgument& Default(bool value) {
            key = value;
            default_value = true;
            return *this;
       }

        FlagArgument& StoreValue(bool &value) {
            value = key;
            partner.push_back(&value);
            return *this;
       }

        char short_name = '\t';
        bool default_value = false;
        std::string long_name;
        std::string description;
        bool key = false;
        std::vector<bool *> partner;
    };

    class ArgParser {
    private:

        std::vector<StringArgument*> string_arguments;
        std::vector<IntArgument*> int_arguments;
        std::vector<FlagArgument*> flag_arguments;

        std::string long_name_help_argument;
        char short_name_help_argument{};
        std::string description_help_argument;
        bool having_help_argument = false;
        bool use_have_argument = false;

    public:
        std::string parser_name;

        ArgParser(const std::string& s);

        bool Parse(int argc, char **argv);

        bool Parse(std::vector<std::string> arguments);

        StringArgument& AddStringArgument(const std::string& long_n, const std::string& description = "");

        StringArgument& AddStringArgument(char short_n,  const std::string& long_n, const std::string& description = "");

        std::string GetStringValue(char short_n, int64_t index = 0);

        std::string GetStringValue(const std::string& long_n, int64_t index = 0);

        IntArgument& AddIntArgument(const std::string& long_n, const std::string& description = "");

        IntArgument& AddIntArgument(char short_n, const std::string& long_n, const std::string& description = "");

        int64_t GetIntValue(char short_n, int64_t index = 0);

        int64_t GetIntValue(const std::string& long_n, int64_t index = 0);

        FlagArgument& AddFlag(const std::string& long_n, const std::string& description = "");

        FlagArgument& AddFlag(char short_n, const std::string& long_n, const std::string& description = "");

        bool GetFlag(char short_n);

        bool GetFlag(const std::string& long_n);

        void AddHelp(char short_n, const std::string& long_n, const std::string& description);

        std::string HelpDescription();

        bool Help();
    };

} // namespace ArgumentParser
