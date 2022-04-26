//
// Created by Spanch bob on 18.12.2021.
//
#include <map>
#include <vector>


/*
 * Добавить:
 *  , и .
 *  %, !=, %=, !, ^
 *  main, else, and, or, not, true, false - системное слово
 */


/*
 * curr_lexeme == "if" || curr_lexeme == "for" || curr_lexeme == "while" || curr_lexeme == "int" ||
                            curr_lexeme == "string" || curr_lexeme == "char" || curr_lexeme == "double" || curr_lexeme == "foreach" ||
                                curr_lexeme == "return"*/
std::vector<std::string> all_service_names = {
        "if", "elseif", "else", "for", "while",  "do", "foreach", "int", "string",
        "char", "double", "main", "or", "and", "not", "true", "false", "return",
        "break", "in"
};
bool is_service_name(std::string& s) {
    for (auto el : all_service_names) {
        if (s == el) return true;
    }
    return false;
}


std::vector<char> kinds_of_operations = {'+', '-', '*', '/', '%', '^'
                                         , '!', '=', '>', '<'};
bool is_operation(char c) {
    for (auto el : kinds_of_operations) {
        if (c == el) return true;
    }
    return false;
}

std::vector<char> kinds_of_punctuation = {',', '{', '}', ';'};
bool is_punctuation(char c) {
    for (auto el : kinds_of_punctuation) {
        if (c == el) return true;
    }
    return false;
}

//////////////////////////////итоговые лексемы
enum{
    SERVICE_NAMES = 1,
    IDENTIFIERS,
    CONSTANTS,
    OPERATIONS,
    PUNCTUATION,
    SQUARE_BRACKETS,
    ROUND_BRACKETS,
};


/////////////////////символы для перехода в состояние
enum{
    number = 1,
    letter,
    symbol,
    punctuation,
    square_bracket,
    round_bracket,
    operation,
    double_tick,
    single_quote,
    dot,
    plus,
    minus,
    divide,
    equals,
    multiply,
    space,
    comparison,
    pow,
};

int get_type(char c) {
    if (c >= '0' && c <= '9') return number;
    if (c >= 'a' && c <= 'z') return letter;
    if (c >= 'A' && c <= 'Z') return letter;
    if (c == '_') return letter;
    if (is_punctuation(c)) return punctuation;
    if (c == ']' || c == '[') return square_bracket;
    if (c == '(' || c == ')') return round_bracket;
    if (c == '.') return dot;
    if (is_operation(c)) return operation;
    if (c == ' ') return space;
    if (c == '\'') return single_quote;
    if (c == '"') return double_tick;
    //if (c == '+') return plus;
    //if (c == '-') return minus;
    //if (c == '*') return multiply;
    //if (c == '/') return divide;
    //if (c == '=') return equals;
    //if (c == '<' || c == '>') return comparison;
    //if (c == '^') return pow;
    return symbol;
}



///////////////////////////состояния
enum{
    Start = 1,
    Number1,
    Number2,
    Number3,
    String,
    Symbol,
    Punctuation,
    Round_brackets,
    Square_brackets,
    Id,
    Plus,
    Minus,
    Divide,
    Equals,
    Multiply,
    Operation,
};



std::map<int, std::map<int, int>> finite_state_machine = {
        {Start, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Number1}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
        {Number1, { {space, Start}, {number, Number1}, {dot, Number2}, {operation, Operation},
                {round_bracket, Round_brackets}, {square_bracket, Square_brackets}, {punctuation, Punctuation},
                {double_tick, String}, {single_quote, Symbol}, {letter, Id} } },
        {Number2, { {dot, Number3} } },
        {Number3, { {space, Start}, {number, Number3}, {operation, Operation}, {round_bracket, Round_brackets},
                 {square_bracket, Square_brackets}, {punctuation, Punctuation}, {double_tick, String},
                 {single_quote, Symbol}, {letter, Id} } },
        {String, { {symbol, String}, {double_tick, Start} } },
        {Punctuation, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Number1}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
        {Round_brackets, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Number1}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
        {Square_brackets, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Number1}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
        {Id, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Id}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
        {Operation, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Number1}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
        {Minus, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Number1}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
        {Divide, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Number1}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
        {Equals, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Number1}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
        {Multiply, {{space, Start}, {letter, Id}, {double_tick, String}, {single_quote, Symbol},
                 {number, Number1}, {operation, Operation}, {round_bracket, Round_brackets}, {square_bracket, Square_brackets},
                 {punctuation, Punctuation} } },
};

/*

std::vector<std::pair<int, std::string>> lexer(std::vector<std::string> text) {
    std::vector<std::map<int, std::string>> answer(text.size());
    for (int j = 0; j < text.size(); ++j) {

    }
}*/

int curr_line = 0;

std::vector<std::pair<int, std::string>> parsing(std::string& input) {
    std::vector<std::pair<int, std::string>> answer;
    int curr_condition = Start;
    int type_for_ans = 0;
    std::string curr_lexeme = "";
    bool is_now_string = false, is_now_symbol = false;
    int i = 0;
    while (true) {
        if (i == input.size() - 1 && input[i] == '.') {
            std::string s = "Unexpected character ";
            s += input[i];
            s += " (";
            s += std::to_string(curr_line);
            s += " line, ";
            s += std::to_string(i);
            s += " position)";
            throw s;
        }
        if (i >= input.size()) {
            if (is_now_string) {
                std::string s = "Expected \" ";
                //s += input[i];
                s += "(";
                s += std::to_string(curr_line);
                s += " line, ";
                s += std::to_string(i);
                s += " position)";
                throw s;
            }
            return answer;
        }

        char c = input[i];
        curr_lexeme += c;

        int symbol = get_type(c);

        if (finite_state_machine[curr_condition][symbol] != 0 || (is_now_string)) {
            if (!is_now_string) {
                if (symbol == operation) {
                    type_for_ans = OPERATIONS;
                    if (i < input.size() - 1) {
                        if (input[i + 1] == '+') {
                            if (input[i] != '+') {
                                //ошибка
                                std::string s = "Double operation ";
                                s += " (";
                                s += std::to_string(curr_line);
                                s += " line, ";
                                s += std::to_string(i + 1);
                                s += " position)";
                                throw s;
                            } else {
                                curr_lexeme = "++";
                                answer.push_back({OPERATIONS, curr_lexeme});
                                curr_lexeme = "";
                                type_for_ans = 0;
                                curr_condition = Start;
                                i = i + 2;
                            }
                        } else if (input[i + 1] == '-') {
                            if (input[i] != '-') {
                                //ошибка
                                std::string s = "Double operation ";
                                s += " (";
                                s += std::to_string(curr_line);
                                s += " line, ";
                                s += std::to_string(i + 1);
                                s += " position)";
                                throw s;
                            } else {
                                curr_lexeme = "--";
                                answer.push_back({OPERATIONS, curr_lexeme});
                                curr_lexeme = "";
                                type_for_ans = 0;
                                curr_condition = Start;
                                i = i + 2;
                            }
                        } else if (input[i + 1] == '=') {
                            curr_lexeme += "=";
                            answer.push_back({OPERATIONS, curr_lexeme});
                            curr_lexeme = "";
                            type_for_ans = 0;
                            curr_condition = Start;
                            i = i + 2;
                        } else {
                            answer.push_back({OPERATIONS, curr_lexeme});
                            curr_lexeme = "";
                            type_for_ans = 0;
                            curr_condition = Start;
                            ++i;
                        }
                    } else {
                        answer.push_back({OPERATIONS, curr_lexeme});
                        curr_lexeme = "";
                        type_for_ans = 0;
                        curr_condition = Start;
                        ++i;
                    }
                } else if (symbol == letter) {
                    type_for_ans = IDENTIFIERS;
                    if (curr_condition == Start) {
                        curr_condition = Id;
                    }
                    if (i < input.size() - 1) {
                        if (get_type(input[i + 1]) != number && get_type(input[i + 1]) != letter) {
                            if (is_service_name(curr_lexeme)) {
                                answer.push_back({SERVICE_NAMES, curr_lexeme});
                            } else {
                                answer.push_back({type_for_ans, curr_lexeme});
                            }
                            curr_lexeme = "";
                            type_for_ans = 0;
                            ++i;
                            curr_condition = Start;
                        } else {
                            ++i;
                        }
                    } else {
                        if (is_service_name(curr_lexeme)) {
                            answer.push_back({SERVICE_NAMES, curr_lexeme});
                        } else {
                            answer.push_back({type_for_ans, curr_lexeme});
                        }
                        curr_lexeme = "";
                        type_for_ans = 0;
                        ++i;
                        curr_condition = Start;
                    }
                } else if (symbol == punctuation) {
                    answer.push_back({PUNCTUATION, curr_lexeme});
                    curr_lexeme = "";
                    type_for_ans = 0;
                    ++i;
                    curr_condition = Start;
                } else if (symbol == square_bracket) {
                    answer.push_back({SQUARE_BRACKETS, curr_lexeme});
                    curr_lexeme = "";
                    type_for_ans = 0;
                    ++i;
                    curr_condition = Start;
                } else if (symbol == round_bracket) {
                    answer.push_back({ROUND_BRACKETS, curr_lexeme});
                    curr_lexeme = "";
                    type_for_ans = 0;
                    ++i;
                    curr_condition = Start;
                } else if (symbol == number) {
                    if (type_for_ans == 0) {
                        type_for_ans = CONSTANTS;
                    }
                    if (curr_condition == Start) {
                        curr_condition = Number1;
                    }
                    if (i < input.size() - 1) {
                        if ( (curr_condition == Number1 && get_type(input[i + 1]) == dot) ||
                             (curr_condition == Number1 && get_type(input[i + 1]) == number) ||
                             (curr_condition == Number3 && get_type(input[i + 1]) == number)) {
                            ++i;
                        } else if (curr_condition == Id){
                            if (get_type(input[i + 1]) != number && get_type(input[i + 1]) != letter) {
                                if (is_service_name(curr_lexeme)) {
                                    answer.push_back({SERVICE_NAMES, curr_lexeme});
                                } else {
                                    answer.push_back({type_for_ans, curr_lexeme});
                                }
                                curr_lexeme = "";
                                type_for_ans = 0;
                                ++i;
                                curr_condition = Start;
                            } else {
                                ++i;
                            }
                        } else {
                            answer.push_back({type_for_ans, curr_lexeme});
                            curr_lexeme = "";
                            type_for_ans = 0;
                            ++i;
                            curr_condition = Start;
                        }
                    } else {
                        answer.push_back({type_for_ans, curr_lexeme});
                        type_for_ans = 0;
                        curr_lexeme = "";
                        curr_condition = Start;
                        ++i;
                    }
                } else if (symbol == single_quote) {
                    if (i + 2 <= input.size() - 1) {
                        if (input[i + 2] == '\'') {
                            curr_lexeme += input[i + 1];
                            curr_lexeme += input[i + 2];
                            answer.push_back({CONSTANTS, curr_lexeme});
                            curr_lexeme = "";
                            i = i + 3;
                            curr_condition = Start;
                        }  else {
                            std::string s = "Expected \' ";
                            s += "(";
                            s += std::to_string(curr_line);
                            s += " line, ";
                            s += std::to_string(i + 2);
                            s += " position)";
                            throw s;
                        }
                    } else {
                        std::string s = "Expected \' ";
                        s += "(";
                        s += std::to_string(curr_line);
                        s += " line, ";
                        if (i == input.size() - 1)
                            s += std::to_string(i + 1);
                        else
                            s += std::to_string(i + 2);
                        s += " position)";
                        throw s;
                    }
                } else if (symbol == dot) {
                    if (curr_condition == Number1) {
                        if (get_type(input[i + 1]) == number) {
                            curr_condition = Number3;
                            i = i + 1;
                        } else {
                            std::string s = "Unexpected character ";
                            s += input[i];
                            s += " (";
                            s += std::to_string(curr_line);
                            s += " line, ";
                            s += std::to_string(i);
                            s += " position)";
                            throw s;
                        }
                    } else {
                        std::string s = "Unexpected character ";
                        s += input[i];
                        s += " (";
                        s += std::to_string(curr_line);
                        s += " line, ";
                        s += std::to_string(i);
                        s += " position)";
                        throw s;
                    }
                } else if (symbol == space){
                    ++i;
                    curr_condition = Start;
                    curr_lexeme = "";
                    type_for_ans = 0;
                } else if (input[i] != '"'){
                    std::string s = "Unexpected character ";
                    s += input[i];
                    s += " (";
                    s += std::to_string(curr_line);
                    s += " line, ";
                    s += std::to_string(i);
                    s += " position)";
                    throw s;
                }
            }
            if (symbol == double_tick) {
                if (is_now_string == true) {
                    //сейчас конец строки
                    answer.push_back({type_for_ans, curr_lexeme});
                    curr_lexeme = "";
                    type_for_ans = 0;
                    ++i;
                    curr_condition = Start;
                    is_now_string = false;
                } else {
                    if (type_for_ans == 0) {
                        type_for_ans = CONSTANTS;
                    }
                    curr_condition = String;
                    is_now_string = true;
                    ++i;
                }
            } else if (is_now_string) {
                ++i;
            }
        } else {
            std::string s = "Unexpected character ";
            s += input[i];
            s += " (";
            s += std::to_string(curr_line);
            s += " line, ";
            s += std::to_string(i);
            s += " position)";
            throw s;
            answer.clear();
            return answer;
        }
    }
}

std::vector<std::vector<std::pair<int, std::string>>> lexer(std::vector<std::string>& text) {
    std::vector<std::vector<std::pair<int, std::string>>> answer;
    for (std::string el : text) {
        answer.push_back(parsing(el));
        ++curr_line;
    }
    return answer;
}


