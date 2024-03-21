#ifndef _ARGZ_HPP_X
#define _ARGZ_HPP_X

#include<iostream>
#include<string>
#include<type_traits>
#include<vector>
#include<unordered_map>

template<typename T>
concept StringType = std::is_class_v<T> && requires(T type) {
    { type.length() } -> std::same_as<typename T::size_type>;
    { type[0] } -> std::same_as<typename T::value_type&>;
    { type += T{} } -> std::same_as<T&>;
};

enum class ArgType {
    ARG_SINGLE,
    ARG_SINGLE_VALUE,
    ARG_DOUBLE,
    ARG_DOUBLE_VALUE,
    ARG_NONE
};

template<StringType String>
struct Argument {
    String arg_name;
    int arg_letter;
    String arg_value;
    ArgType arg_type;
    String desc;
    Argument() : arg_name{}, arg_letter{}, arg_value{}, arg_type{}, desc{} {}
    Argument(const Argument &a) : arg_name{a.arg_name}, arg_letter{a.arg_letter}, arg_value{a.arg_value}, arg_type{a.arg_type}, desc{a.desc} {}
    Argument &operator=(const Argument<String> &a) {
        arg_name = a.arg_name;
        arg_letter = a.arg_letter;
        arg_value = a.arg_value;
        arg_type = a.arg_type;
        desc = a.desc;
        return *this;
    }
    auto operator<=>(const Argument<String> &a) const {
        return (arg_letter <=> a.arg_letter);
    }
};

template<StringType String>
struct ArgumentData {
    std::vector<String> args;
    int argc;
    ArgumentData() = default;
    ArgumentData(const ArgumentData<String> &a) : args{a.args}, argc{a.argc} {}
    ArgumentData &operator=(const ArgumentData<String> &a) {
        if(!args.empty()) {
            args.erase(args.begin(), args.end());
        }
        std::copy(a.args.begin(), a.args.end(), std::back_inserter(args));
        return *this;
    }
};

template<StringType String>
class ArgException {
public:
    ArgException() = default; 
    ArgException(const String &s) : value{s} {}
    String text() const { return value; }
private:
    String value;
};

template<StringType String>
class Argz {
public:
    
    Argz() = default;
    Argz(int argc, char **argv) {
        initArgs(argc, argv);
    }
    Argz(const Argz<String> &a) : arg_data{a.arg_data}, arg_info{a.arg_info} {}
    
    void initArgs(int argc, char **argv) {
        arg_data.argc = argc;
        for(int i = 1; i < argc; ++i) {
            arg_data.args.push_back(argv[i]);
        }
        reset();
    }
    
    void reset() {
        index = 0;
        cindex = 1;
    }
    
    void addOptionSingle(const int &c, const String &description) {
        Argument<String> a {};
        a.arg_letter = c;
        a.arg_type = ArgType::ARG_SINGLE;
        a.desc = description;
        arg_info[c] = a;
    }
    
    void addOptionSingleValue(const int &c, const String &description) {
        Argument<String> a {};
        a.arg_letter = c;
        a.arg_type = ArgType::ARG_SINGLE_VALUE;
        a.desc = description;
        arg_info[c] = a;
    }
    
    void addOptionDouble(const int &code, const String &value, const String &description) {
        Argument<String> a {};
        a.arg_letter = code;
        a.arg_type = ArgType::ARG_DOUBLE;
        a.desc = description;
        a.arg_name = value;
        arg_info[code] = a;
    }
    
    void addOptionDoubleValue(const int &code, const String &value, const String &description) {
        Argument<String> a {};
        a.arg_letter = code;
        a.arg_type = ArgType::ARG_DOUBLE_VALUE;
        a.desc = description;
        a.arg_name = value;
        arg_info[code] = a;
    }
    
    
    int lookUpCode(const String &value) {
        for(const auto &i : arg_info) {
            if(i.second.arg_name == value) {
                return i.second.arg_letter;
            }
        }
        return -1;
    }
    
    int proc(Argument<String> &a) {
        if(index < arg_data.args.size()) { // START
            const String &type {arg_data.args[index]};
            if(type.length() > 3 && type[0] == '-' && type[1] == '-') { 
                String name{};
                for(int z = 2; z < type.length(); ++z)
                    name += type[z];
                int code = lookUpCode(name);
                if(code != -1) {
                    auto pos = arg_info.find(code);
                    if(pos != arg_info.end()) {
                        if(pos->second.arg_type == ArgType::ARG_DOUBLE) {
                            a = pos->second;
                            index++;
                            return code;
                        } else {
                            a = pos->second;
                            if(++index < arg_data.args.size() && arg_data.args[index][0] != '-') {
                                a.arg_value = arg_data.args[index];
                                index++;
                                return code;
                            } else {
                                throw ArgException<String>("Expected Value");
                            }
                        }
                    } 
                } else {
                    String value = "Error argument: ";
                    value += name;
                    value += " switch not found";
                    throw ArgException<String>(value);
                }
            } else if(type.length() > 1 && type[0] == '-') {
                
                const int c {type[cindex]};
                const auto pos {arg_info.find(c)};
                cindex++;
                if(cindex >= type.length()) {
                    cindex = 1;
                    index++;
                }  
                if(pos != arg_info.end()) {
                    if(pos->second.arg_type == ArgType::ARG_SINGLE) {
                        a = pos->second;
                        return c;
                    } else if(pos->second.arg_type == ArgType::ARG_SINGLE_VALUE) {
                        
                        if(index < arg_data.args.size()) {
                            const String &s {arg_data.args[index]};
                            if(s.length() > 0 && s[0] == '-')
                                throw ArgException<String>("Expected value");
                            
                            a = pos->second;
                            a.arg_value = s;
                            index++;
                            return c;
                        }
                    }
                } else {
                    String value;
                    value = "Error argument ";
                    value += static_cast<typename String::value_type>(c);
                    value += " switch not found.";
                    throw ArgException<String>(value);
                }
            } else {
                a = Argument<String>();
                a.arg_type = ArgType::ARG_NONE;
                a.arg_name = a.arg_value = arg_data.args.at(index);
                index++;
                return '-';
            } 
        }
        return -1;
    }
    
    void help() {
        std::vector<Argument<String>> v;
        for(const auto &i : arg_info) {
            v.push_back(i.second);
        }
        std::sort(v.begin(), v.end());
        for(auto a = v.begin(); a != v.end(); ++a) {
            if(a->arg_type == ArgType::ARG_SINGLE || a->arg_type == ArgType::ARG_SINGLE_VALUE)
                std::cout << "-" << static_cast<char>(a->arg_letter) << "\t" << a->desc << "\n";
            else
                std::cout << "--" << a->arg_name << "\t\t" << a->desc << "\n";
        }
    }
protected:
    ArgumentData<String> arg_data;
    std::unordered_map<int,Argument<String>> arg_info;
private:
    int index = 0, cindex = 1;    
};


#endif
