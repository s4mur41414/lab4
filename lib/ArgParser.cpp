#include "ArgParser.h"

using namespace ArgumentParser;

ArgParser::ArgParser(const std::string& pars_name)
        :parser_name{pars_name}{}

bool ArgParser::Parse(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        return true;
    }
    if (arguments.size() == 1) {
        if (string_arguments.empty() && int_arguments.empty() && flag_arguments.empty()) {
            return true;
        }
        for (int64_t i = 0; i < string_arguments.size(); i++) {
            if (!string_arguments[i]->default_value) {
                return false;
            }
        }
        for (int64_t i = 0; i < int_arguments.size(); i++) {
            if (!int_arguments[i]->default_value) {
                return false;
            }
        }
        for (int64_t i = 0; i < flag_arguments.size(); i++) {
            if (!flag_arguments[i]->default_value) {
                return false;
            }
        }

        return true;
    }
    for (int64_t i = 1; i < arguments.size(); i++) {
        std::string value;
        if (arguments[i][0] == '-') {
            if (arguments[i].size() == 1) {
                continue;
            }
            if (arguments[i][1] == '-') {
                std::string current_param;
                int64_t index = arguments[i].size();
                for (int64_t j = 2; j < arguments[i].size(); j++) {
                    if (arguments[i][j] == '=') {
                        index = j;
                        break;
                    }
                }
                if (index == arguments[i].size()) {
                    for (int64_t j = 2; j < arguments[i].size(); j++) {
                        current_param += arguments[i][j];
                    }
                    if (having_help_argument) {
                        if (current_param == long_name_help_argument) {
                            use_have_argument = true;
                            std::cout << HelpDescription();
                            return true;
                        }
                    }

                    // не help, возможно flag
                    bool findflag = false;
                    for (int64_t j = 0; j < flag_arguments.size(); j++) {
                        if (flag_arguments[j]->long_name == current_param) {
                            flag_arguments[j]->key = true;
                            for (int64_t k = 0; k < flag_arguments[j]->partner.size(); k++) {
                                *flag_arguments[j]->partner[k] = true;
                            }
                            findflag = true;
                            break;
                        }
                    }

                    if (findflag) {
                        continue;
                    } else {
                        return false;
                    }
                } else {
                    for (int64_t j = 2; j < index; j++) {
                        current_param += arguments[i][j];
                    }
                    if (index == arguments[i].size() - 1) {
                        // неверный формат ввода
                        return false;
                    }
                    for (int64_t j = index + 1; j < arguments[i].size(); j++) {
                        value += arguments[i][j];
                    }
                }

                for (int64_t j = 0; j < string_arguments.size(); j++) {
                    if (string_arguments[j]->long_name == current_param) {
                        string_arguments[j]->key = value;
                        for (int64_t m = 0; m < string_arguments[j]->partner.size(); m++) {
                            *string_arguments[j]->partner[m] = value;
                        }
                        break;
                    }
                }

                for (int64_t j = 0; j < int_arguments.size(); j++) {
                    if (int_arguments[j]->long_name == current_param) {
                        if (!int_arguments[j]->multi_value) {
                            int_arguments[j]->key = std::stoi(value);
                            for (int64_t m = 0; m < int_arguments[j]->partner.size(); m++) {
                                *(int_arguments[j]->partner[m]) = std::stoi(value);
                            }
                        } else {
                            int_arguments[j]->keys.push_back(std::stoi(value));
                            for (int64_t m = 0; m < int_arguments[j]->partner_multi.size(); m++) {
                                (*(int_arguments[j]->partner_multi[m])) = int_arguments[j]->keys;
                            }
                        }
                        break;
                    }
                }
            } else {
                char current = arguments[i][1];

                if (having_help_argument) {
                    if (current == short_name_help_argument) {
                        std::cout << HelpDescription();
                        return true;
                    }
                }

                bool findflag = false;
                for (int64_t j = 0; j < flag_arguments.size(); j++) {
                    if (flag_arguments[j]->short_name == current) {
                        flag_arguments[j]->key = true;
                        for (int64_t k = 0; k < flag_arguments[j]->partner.size(); k++) {
                            *(flag_arguments[j]->partner[k]) = true;
                        }
                        findflag = true;
                        break;
                    }
                }
                if (findflag) {
                    for (int64_t l = 2; l < arguments[i].length(); l++) {
                        for (int64_t j = 0; j < flag_arguments.size(); j++) {
                            if (flag_arguments[j]->short_name == arguments[i][l]) {
                                flag_arguments[j]->key = true;
                                for (int64_t k = 0; k < flag_arguments[j]->partner.size(); k++) {
                                    *(flag_arguments[j]->partner[k]) = true;
                                }
                                break;
                            }
                        }
                    }
                    continue;
                }
                if (arguments[i].length() == 2 || arguments[i].length() >= 3 && arguments[i][2] != '=' ||
                    arguments[i].length() < 4) {
                    return false;
                }
                for (int64_t j = 3; j < arguments[i].size(); j++) {
                    value += arguments[i][j];
                }
                for (int64_t j = 0; j < string_arguments.size(); j++) {
                    if (string_arguments[j]->short_name == current) {
                        string_arguments[j]->key = value;
                        for (int64_t m = 0; m < string_arguments[j]->partner.size(); m++) {
                            *(string_arguments[j]->partner[m]) = value;
                        }
                        break;
                    }
                }

                for (int64_t j = 0; j < int_arguments.size(); j++) {
                    if (int_arguments[j]->short_name == current) {
                        if (!int_arguments[j]->multi_value) {
                            int_arguments[j]->key = std::stoi(value);
                            for (int64_t m = 0; m < int_arguments[j]->partner.size(); m++) {
                                *(int_arguments[j]->partner[m]) = std::stoi(value);
                            }
                        } else {
                            int_arguments[j]->keys.push_back(std::stoi(value));
                            for (int64_t m = 0; m < int_arguments[j]->partner_multi.size(); m++) {
                                (*(int_arguments[j]->partner_multi[m])) = int_arguments[j]->keys;
                            }
                        }


                        break;
                    }
                }
            }
            // свободные аргументы
        } else {
            for (int64_t j = 0; j < int_arguments.size(); j++) {
                if (int_arguments[j]->positional) {
                    if (int_arguments[j]->multi_value) {
                        int_arguments[j]->keys.push_back(std::stoi(arguments[i]));
                        for (int64_t m = 0; m < int_arguments[j]->partner_multi.size(); m++) {
                            (*(int_arguments[j]->partner_multi[m])) = int_arguments[j]->keys;
                        }
                    } else {
                        int_arguments[j]->key = std::stoi(arguments[i]);
                        for (int64_t m = 0; m < int_arguments[j]->partner.size(); m++) {
                            *(int_arguments[j]->partner[m]) = std::stoi(arguments[i]);
                        }
                    }
                }
                break;
            }
        }
    }
    // проверка введено ли минимальное количество данных для MultiValues
    for (int64_t i = 0; i < int_arguments.size(); i++) {
        if (int_arguments[i]->multi_value) {
            if (int_arguments[i]->keys.size() < int_arguments[i]->min_args_count) {
                return false;
            }
        }
    }
    for (int64_t i = 0; i < string_arguments.size(); i++) {
        if (string_arguments[i]->multi_value) {
            if (string_arguments[i]->keys.size() < string_arguments[i]->min_args_count) {
                return false;
            }
        }
    }
    return true;
}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> parse;
    for (int i = 0; i < argc; i++) {
        parse.push_back(argv[i]);
    }
    return Parse(parse);
}

StringArgument& ArgParser::AddStringArgument(const std::string& long_n, const std::string& description) {
    StringArgument* x;
    if(long_n.size() == 1){
        x = new StringArgument(long_n[0]);
    }
    else {
        x = new StringArgument(long_n);
    }
    x->description = description;
    string_arguments.push_back(x);
    return *x;
}

StringArgument&
ArgParser::AddStringArgument(char short_n, const std::string& long_n, const std::string& description) {
    StringArgument* x = new StringArgument(short_n, long_n);
    x->description = description;
    string_arguments.push_back(x);
    return *x;
}

std::string ArgParser::GetStringValue(char short_n, int64_t index) {
    for (int64_t i = 0; i < string_arguments.size(); i++) {
        if (string_arguments[i]->short_name == short_n) {
            if (string_arguments[i]->multi_value) {
                return string_arguments[i]->keys[index];
            } else {
                return string_arguments[i]->key;
            }
        }
    }
    return "";
}

std::string ArgParser::GetStringValue(const std::string& long_n, int64_t index) {
    for (int64_t i = 0; i < string_arguments.size(); i++) {
        if (string_arguments[i]->long_name == long_n) {
            if (string_arguments[i]->multi_value) {
                return string_arguments[i]->keys[index];
            } else {
                return string_arguments[i]->key;
            }
        }
    }
    return "";
}

IntArgument& ArgParser::AddIntArgument(const std::string& long_n, const std::string& description) {
    IntArgument* x;
    if(long_n.size() == 1){
        x = new IntArgument(long_n[0]);
    }
    else {
        x = new IntArgument(long_n);
    }
    x->description = description;
    int_arguments.push_back(x);
    return *x;
}

IntArgument&
ArgParser::AddIntArgument(char short_n, const std::string& long_n, const std::string& description) {
    IntArgument* x = new IntArgument(short_n, long_n);
    x->description = description;
    int_arguments.push_back(x);
    return *x;
}

int64_t ArgParser::GetIntValue(char short_n, int64_t index) {
    for (int64_t i = 0; i < int_arguments.size(); i++) {
        if (int_arguments[i]->short_name == short_n) {
            if (int_arguments[i]->multi_value) {
                return int_arguments[i]->keys[index];
            } else {
                return int_arguments[i]->key;
            }
        }
    }
    return 0;
}

int64_t ArgParser::GetIntValue(const std::string& long_n, int64_t index) {
    for (int64_t i = 0; i < int_arguments.size(); i++) {
        if (int_arguments[i]->long_name == long_n) {
            if (int_arguments[i]->multi_value) {
                return int_arguments[i]->keys[index];
            } else {
                return int_arguments[i]->key;
            }
        }
    }
    return 0;
}

FlagArgument& ArgParser::AddFlag(const std::string& long_n, const std::string& description) {
    FlagArgument* x;
    if(long_n.size() == 1){
        x = new FlagArgument(long_n[0]);
    }
    else {
        x = new FlagArgument(long_n);
    }
    x->description = description;
    flag_arguments.push_back(x);
    return *x;
}

FlagArgument& ArgParser::AddFlag(char short_n, const std::string& long_n, const std::string& description) {
    FlagArgument* x = new FlagArgument(short_n, long_n);
    x->description = description;
    flag_arguments.push_back(x);
    return *x;
}

bool ArgParser::GetFlag(char short_n) {
    for (int i = 0; i < flag_arguments.size(); i++) {
        if (flag_arguments[i]->short_name == short_n) {
            return flag_arguments[i]->key;
        }
    }
    return false;
}

bool ArgParser::GetFlag(const std::string& long_n) {
    for (int i = 0; i < flag_arguments.size(); i++) {
        if (flag_arguments[i]->long_name == long_n) {
            return flag_arguments[i]->key;
        }
    }
    return false;
}

void ArgParser::AddHelp(char short_n, const std::string& long_n, const std::string& description) {
    having_help_argument = true;
    long_name_help_argument = long_n;
    short_name_help_argument = short_n;
    description_help_argument = description;
}

std::string ArgParser::HelpDescription() {
    std::string help_field = parser_name + '\n' + description_help_argument + '\n' + '\n';

    for (int i = 0; i < string_arguments.size(); i++) {
        if(string_arguments[i]->short_name!='\t'){
            help_field.append("-");
            help_field += string_arguments[i]->short_name;
            if(!string_arguments[i]->long_name.empty() || !string_arguments[i]->description.empty()) {
                help_field.append(",   ");
            }
            else{
                help_field.append("=<string>");
            }
        }
        else {
            help_field += string_arguments[i]->short_name;
            help_field.append(" ");
        }
        if(!string_arguments[i]->long_name.empty()){
            help_field.append("--" + string_arguments[i]->long_name + "=<string>");
        }
        if(!string_arguments[i]->description.empty()){
            help_field.append(",   " + string_arguments[i]->description);
        }
        if (string_arguments[i]->min_args_count != 0) {
            help_field.append( ",   [repeated, min args = " + std::to_string(string_arguments[i]->min_args_count) + "]");
            if(!string_arguments[i]->default_value){
                help_field+='\n';
            }
        }
        else if (string_arguments[i]->default_value) {
            help_field.append(",   [default = " + string_arguments[i]->key + "]\n");
        }
        else{
            help_field+='\n';
        }
    }

    for (int i = 0; i < int_arguments.size(); i++) {
        if(int_arguments[i]->short_name!='\t'){
            help_field.append("-");
            help_field += int_arguments[i]->short_name;
            if(!int_arguments[i]->long_name.empty() || !int_arguments[i]->description.empty()) {
                help_field.append(",   ");
            }
            else{
                help_field.append("=<int>");
            }
        }
        else {
            help_field += int_arguments[i]->short_name;
            help_field.append(" ");
        }
        if(!int_arguments[i]->long_name.empty()){
            help_field.append("--" + int_arguments[i]->long_name + "=<int>");
        }
        if(!int_arguments[i]->description.empty()){
            help_field.append(",   " + int_arguments[i]->description);
        }
        if (int_arguments[i]->min_args_count != 0) {
            help_field.append( ", [repeated, min args = " + std::to_string(int_arguments[i]->min_args_count) + "]");
            if(!int_arguments[i]->default_value){
                help_field+='\n';
            }
        }
        else if (int_arguments[i]->default_value) {
            help_field.append(", [default = " + std::to_string(int_arguments[i]->key) + "]\n");
        }
        else{
            help_field+='\n';
        }
    }

    for (int i = 0; i < flag_arguments.size(); i++) {
        if(flag_arguments[i]->short_name!='\t'){
            help_field.append("-");
            help_field += flag_arguments[i]->short_name;
            if(!flag_arguments[i]->long_name.empty() || !flag_arguments[i]->description.empty()) {
                help_field.append(",   ");
            }
            else{
                help_field.append("=<flag>");
            }
        }
        else {
            help_field += flag_arguments[i]->short_name;
            help_field.append(" ");
        }
        if(!flag_arguments[i]->long_name.empty()){
            help_field.append("--" + flag_arguments[i]->long_name);
        }
        if(!flag_arguments[i]->description.empty()){
            help_field.append(",   " + flag_arguments[i]->description);
        }
        if (flag_arguments[i]->default_value) {
            help_field.append(",   [default = " + std::to_string(flag_arguments[i]->key) + "]\n");
        }
        else{
            help_field+='\n';
        }
    }

    help_field += "-h, --help Display this help and exit\n";

    return help_field;
}

bool ArgParser::Help() {
    if (!use_have_argument) {
        return false;
    }
    return true;
}
