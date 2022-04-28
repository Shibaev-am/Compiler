
//
// Created by Spanch bob on 12.03.2022.
//
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>

int count_of_lexems = -1;
struct lexem{
    int type;
    std::string lex;
};
lexem curr_lexem;
bool is_now_func = false;
std::string func_type = "";

std::vector<lexem> Lexems;
struct Variable{
    std::string name = "int";
    std::string type;
};

struct Array{
    std::string name = "int";
    std::string type;
    int dimension;
    std::vector<int> sizes;
};

struct Function{
    std::string name = "int";
    std::string type;
    int count_of_parameters;
    std::vector<std::string> types;
    //std::map<std::string, std::string> types;
    //std::vector<std::string> types;
};

struct TID{
    TID* prev_tid;
    std::map<std::string, Variable> vars;
    std::map<std::string, Array> arrays;
    std::map<std::string, Function> functions;

};



TID* curr_tid = nullptr;
Variable curr_variable;
Array curr_array;
Function curr_function, temp_func;
std::string get_type(Array arr);
Function find_function(std::string name);

void add_tid() {
    //
    TID* new_tid = new TID;
    new_tid->prev_tid = curr_tid;
    curr_tid = new_tid;
    //
}
void delete_tid() {
    //
    TID* new_tid = curr_tid->prev_tid;
    delete curr_tid;
    curr_tid = new_tid;
    //
}


bool check_array() {
    if (curr_tid->arrays[curr_array.name].name == "int" &&
        curr_tid->vars[curr_array.name].name == "int" &&
        curr_tid->functions[curr_array.name].name == "int") {
        return true;
    } else {
        return false;
    }

}
bool check_variable() {
    if (curr_tid->arrays[curr_variable.name].name == "int" &&
        curr_tid->vars[curr_variable.name].name == "int" &&
            curr_tid->functions[curr_variable.name].name == "int") {
        return true;
    } else {
        return false;
    }
}
bool check_function() {
    if (curr_tid->arrays[curr_function.name].name == "int" &&
        curr_tid->vars[curr_function.name].name == "int" &&
        curr_tid->functions[curr_function.name].name == "int") {
        return true;
    } else {
        return false;
    }
}
void add_variable() {
    if (check_variable()) {
        curr_tid->vars[curr_variable.name] = curr_variable;
        curr_variable.name = "int";
        curr_variable.type = "";
    } else {
        std::string error = "Переменная " + curr_variable.name +
                            " несколько раз инициализирована в одном блоке";
        throw error;
    }
}
void add_array() {
    if (check_array()) {
        curr_tid->arrays[curr_array.name] = curr_array;
        curr_array.name = "int";
        curr_array.type = "";
        curr_array.dimension = -1;
        curr_array.sizes.clear();

    } else {
        std::string error = "Переменная " + curr_array.name +
                            " несколько раз инициализирована в одном блоке";
        throw error;
    }
}
void add_function() {
    if (check_function()) {
        curr_function.count_of_parameters = curr_function.types.size();
        curr_tid->functions[curr_function.name] = curr_function;
        curr_function.name = "int";
        curr_function.type = "";
        curr_function.count_of_parameters = 0;
        curr_function.types.clear();
    } else {
        std::string error = "Переменная " + curr_function.name +
                            " несколько раз инициализирована в одном блоке";
        throw error;
    }
}

void check_name(std::string name) {
    TID* temp_tid = curr_tid;
    while (temp_tid != nullptr) {
        if (temp_tid->vars[name].name == "int" and
            temp_tid->arrays[name].name == "int" and
            temp_tid->functions[name].name == "int") {
            temp_tid = temp_tid->prev_tid;
        } else {
            return;
        }
    }
    std::string error = "Переменная " + name + " не инициализирована";
    throw error;
}

std::stack<std::string> types;
std::string curr_type;

void add_to_stack_of_types(std::string name) {
    TID* temp_tid = curr_tid;
    std::string type = "";
    while (temp_tid != nullptr) {
        if (temp_tid->vars[name].name != "int") {
            type = temp_tid->vars[name].type;
            types.push(type);
            return;
        } else if (temp_tid->arrays[name].name != "int") {
            type = get_type(temp_tid->arrays[name]);
            types.push(type);
            return;
        } else if (temp_tid->functions[name].name != "int") {
            type = temp_tid->functions[name].type;
            types.push(type);
            return;
        } else {
            temp_tid = temp_tid->prev_tid;
        }
    }
}



bool is_array(std::string s) {
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '[') {
            return true;
        }
    }
    return false;
}

std::string array_after_indexing(std::string s) {
    std::string ans = "";
    int i = 0;
    for (; i < s.size(); ++i) {
        if (s[i] == '[') {
            break;
        } else {
            ans += s[i];
        }
    }
    i += 2;
    std::string num = "";
    for (; i < s.size(); ++i) {
        num += s[i];
    }
    int number = 0;
    int pow = 1;
    for (int i = num.size() - 1; i >= 0; --i) {
        number += (num[i] - '0') * pow;
        pow *= 10;
    }
    --number;
    if (number == 0) {
        //types.push(ans);
        return ans;
    }
    ans += "[]";
    ans += std::to_string(number);
    //types.push(ans);
    return ans;
}


void match_types(std::vector<std::string> arr_types, std::string operation) {
    if (operation == "[]") {
       std::string temp = arr_types[0];
       if (temp == "string") {
           types.push("char");
           return;
       } else if (is_array(temp)) {
           std::string new_arr = array_after_indexing(temp);
           types.push(new_arr);
           return;
       } else {
           std::string error = "Attempt to index a non-indexed type";
           throw error;
       }
    } else if (operation == "=") {
        std::string first = arr_types[1];
        std::string second = arr_types[0];
        if (first == second) {
            types.push(first);
            return;
        } else if ( (first == "int" and second == "double")
                or (first == "double" and second == "int")
                or (first == "int" and second == "bool")
                or (first == "bool" and second == "int")
                or (first == "int" and second == "char")
                or (first == "char" and second == "int")) {
            types.push(first);
            return;
        } else if (first == "string" and second == "anystring") {
            types.push(first);
        } else if (first == "char" and second == "symbol") {
            types.push(first);
        } else {
            std::string error = "Attempt to index a non-indexed type";
            throw error;
        }
    } else if (operation == "++") {
        std::string first = arr_types[0];
        if (first == "int" or first == "double" or first == "char" or first == "bool") {
            types.push(first);
            return;
        }  else {
            std::string error = "Attempt to index a non-indexed type";
            throw error;
        }
    } else if (operation == "!") {
        std::string first = arr_types[0];
        if (first == "bool") {
            types.push(first);
            return;
        }  else {
            std::string error = "Attempt to index a non-indexed type";
            throw error;
        }
    } else if (operation == "in") {
        std::string first = arr_types[1];
        std::string second = arr_types[0];
        if (first == "char" && second == "string") {
            return;
        } else if (is_array(second) and first == array_after_indexing(second)) {
            return;
        } else {
            std::string error = "Attempt to index a non-indexed type";
            throw error;
        }
    } else if (operation == "%=" || operation == "^=" || operation == "%"
        || operation == "^") {
        std::string first = arr_types[1];
        std::string second = arr_types[0];
        if (first == "int" and second == "int") {
            types.push("int");
            return;
        } else {
            std::string error = "Attempt to index a non-indexed type";
            throw error;
        }
    } else if (operation == "*=" || operation == "*"
        || operation == "/=" || operation == "/" || operation == "-="
        || operation == "-") {
        std::string first = arr_types[1];
        std::string second = arr_types[0];
        if ( (first == "int" or first == "double" or first == "bool") and
                (second == "int" or second == "double" or second == "bool")){
            types.push(first);
            return;
        } else {
            std::string error = "Attempt to index a non-indexed type";
            throw error;
        }

    } else if (operation == "+=" || operation == "+") {
        std::string first = arr_types[1];
        std::string second = arr_types[0];
        if (first == "string" and (second == "string" or
            second == "char" or second == "anystring" or second == "symbol")) {
            types.push("string");
            return;
        } else if ( (first == "int" or first == "double" or first == "bool") and
                    (second == "int" or second == "double" or
                    second == "bool" or second == "char" or second == "symbol")){
            types.push(first);
            return;
        } else {
            std::string error = "Attempt to index a non-indexed type";
            throw error;
        }
    } else if (operation == ">=" || operation == ">" ||
        operation == "<=" || operation == "<" || operation == "==" ||
        operation == "!=") {
        std::string first = arr_types[1];
        std::string second = arr_types[0];

        if (first == second) {
            types.push("bool");
            return;
        } else if ( (first == "int" or first == "double" or first == "bool") and
             (second == "int" or second == "double" or second == "bool")){
            types.push("bool");
            return;
        } else {
            std::string error = "Attempt to index a non-indexed type";
            throw error;
        }
    } else if (operation == "()") {
        std::string name = arr_types[0];
        temp_func = find_function(name);
        int i = 0;
        int j = arr_types.size() - 1;
        for (int k = 1; k < arr_types.size(); ++k) {
            if (temp_func.types[i] == "int" and
                (arr_types[j] == "double" or arr_types[j] == "char"
                or arr_types[j] == "bool")) {

            } else if (temp_func.types[i] == "double" and
                       (arr_types[j] == "int" or arr_types[j] == "char"
                        or arr_types[j] == "bool")) {

            } else if (temp_func.types[i] == "bool" and
                       (arr_types[j] == "int" or arr_types[j] == "char"
                        or arr_types[j] == "double")) {

            } else if (temp_func.types[i] == "char" and
                       (arr_types[j] == "int" or arr_types[j] == "double")) {

            } else if (arr_types[j] != temp_func.types[i]) {
                std::string err = "function parameter type mismatch";
                throw err;
            }
            ++i;
            --j;
        }
        types.push(temp_func.type);
    }
}


std::string get_type(Array arr) {
    std::string type = "";
    type = arr.type;
    type += "[]";
    type += std::to_string(arr.dimension);
    return type;
}

std::string type(int num) {
    if (num == 1) return "SERVICE_NAMES";
    if (num == 2) return "IDENTIFIERS";
    if (num == 3) return "CONSTANTS";
    if (num == 4) return "OPERATIONS";
    if (num == 5) return "PUNCTUATION";
    if (num == 6) return "SQUARE_BRACKETS";
    if (num == 7) return "ROUND_BRACKETS";
}

Function find_function(std::string name) {
    TID* temp = curr_tid;
    while (temp != nullptr) {
        if (temp->functions[name].name != "int") {
            return temp->functions[name];
        } else {
            temp = temp->prev_tid;
        }
    }
}

void get_next() {
    ++count_of_lexems;
    if (count_of_lexems == Lexems.size()) {
        //
        curr_lexem.lex = "end";
        curr_lexem.type = -1;
        return;
    }
    curr_lexem = Lexems[count_of_lexems];
    //std::cout << "\n" << curr_lexem.lex << "\n";

}

void prev_lexem() {
    if (count_of_lexems <= 0) {
        return;
    }
    --count_of_lexems;
    curr_lexem = Lexems[count_of_lexems];
    return;
}

bool is_type() {
    if ((curr_lexem.lex == "int" || curr_lexem.lex == "double" || curr_lexem.lex == "bool" ||
         curr_lexem.lex == "string" || curr_lexem.lex == "char")) {
        return true;
    }
    return false;
}

bool is_number() {
    if (curr_lexem.type != 3) {
        return false;
    }
    int i = 0;
    while(true) {
        if (i >= curr_lexem.lex.size()) {
            return true;
        }
        if (curr_lexem.lex[i] == '.') {
            ++i;
            break;
        }
        if ( !(curr_lexem.lex[i] >= '0' && curr_lexem.lex[i] <= '9')) {
            return false;
        }
        ++i;
    }
    while(true) {
        if (i >= curr_lexem.lex.size()) {
            return true;
        }
        if (!(curr_lexem.lex[i] >= '0' && curr_lexem.lex[i] <= '9')) {
            return false;
        }
        ++i;
    }
}

void Program();
void GlobalDescription();
void Block();
void Description();
void Function();
void FormalParameters ();
void Type();
void Initialization();
void DescriptionOfArray();
void Operator();
void ExpressionOperator();
void LoopOperator();
void ConditionalOperator();
void InputOperator();
void OutputOperator();
void Expression();
void Expression1();
void Expression2();
bool is_AssigmentOperation();
void Expression3();
bool is_ComparisonOperation();
void Expression4();
bool is_AdditionOperation();
void Expression5();
bool is_MultiplicationOperation();
void Expression6();
bool is_PowOperation();
void Atom();
void Increment();
void Indexing();
void Const();
void FunctionCall();
void Constant();
void Boolean();
void Sign();
void Digit();
void AnyString();
void AnySymbol();
void FOR();
void WHILE();
void DOWHILE();
void RETURN();
void BREAK();
void FOREACH();
void ELSEIF();
void ELSE();
void OutputElement();
void Name();




/*if (curr_lexem.type == -1) {
curr_lexem.lex = "Missing function 'int main'";
throw curr_lexem;
}*/


void Program() {
    GlobalDescription();
    if (curr_lexem.lex != "int") {
        curr_lexem.lex = "Missing function 'int main'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "main") {
        curr_lexem.lex = "Missing function 'int main'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Missing function 'int main'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != ")") {
        curr_lexem.lex = "'int main' must be without accepted parameters";
        throw curr_lexem;
    }
    get_next();
    add_tid();
    Block();
    delete_tid();
    return;
}

void GlobalDescription() {
    while (true) {
        if (curr_lexem.lex == "int") {
            get_next();
            if (curr_lexem.lex == "main") {
                prev_lexem();
                return;
            }
            prev_lexem();
        }
        std::string type, name;
        Type();

        //
        prev_lexem();
        type = curr_lexem.lex;
        get_next();
        //

        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Missing function 'int main'";
            throw curr_lexem;
        }
        Name();

        prev_lexem();
        name = curr_lexem.lex;
        get_next();

        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Missing function 'int main'";
            throw curr_lexem;
        }
        if (curr_lexem.lex == "(") {
            prev_lexem();
            prev_lexem();
            Function();
            //
            add_function();
            //
            //curr_tid->functions[curr_function.name] = curr_function;
        } else {
            prev_lexem();
            prev_lexem();
            Description();
            //curr_tid->functions[curr_function.name] = curr_function;
        }
    }

}

void Block() {

    if (curr_lexem.lex != "{") {
        curr_lexem.lex = "Block must start with '{'";
        throw curr_lexem;
    }
    get_next();
    while (true) {
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Block must end with '}'";
            throw curr_lexem;
        }
        if (curr_lexem.lex == "}") {
            break;
        }
        Operator();

    }
    get_next();

}

void Description() {
    std::string type, name;
    if (!is_type()) {
        curr_lexem.lex = "Missing type when initializing variable";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex == "[") {
        prev_lexem();
        DescriptionOfArray();
        if (curr_lexem.lex != ";") {
            curr_lexem.lex = "Expected ';' after initialization if array";
            throw curr_lexem;
        }
        get_next();
        //
        add_array();
        //
        //curr_tid->arrays[arr]
        return;
    }

    //
    prev_lexem();
    type = curr_lexem.lex;
    get_next();
    //

    Name();

    //
    prev_lexem();
    name = curr_lexem.lex;
    get_next();
    curr_variable.name = name;
    curr_variable.type = type;
    add_variable();
    //

    if (curr_lexem.lex == "=") {
        get_next();
        Expression();
        if (types.empty()) {
            std::string error = "error";
            throw error;
        }
        std::string second = types.top();
        types.pop();
        match_types({second, type}, "=");
        if (types.empty()) {
            std::string error = "error";
            throw error;
        } else {
            types.pop();
        }
    }


    while (true) {
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Expected ';' after initialization if variable(s)";
            throw curr_lexem;
        }
        if (curr_lexem.lex != ",") {
            break;
        }
        get_next();
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Expected ';' instead if ','";
            throw curr_lexem;
        }
        Name();

        //
        prev_lexem();
        name = curr_lexem.lex;
        get_next();
        curr_variable.name = name;
        curr_variable.type = type;
        add_variable();
        //

        if (curr_lexem.lex == "=") {
            get_next();
            Expression();
            if (types.empty()) {
                std::string error = "error";
                throw error;
            }
            std::string second = types.top();
            types.pop();
            match_types({second, type}, "=");
            if (types.empty()) {
                std::string error = "error";
                throw error;
            } else {
                types.pop();
            }
        }
    }
    if (curr_lexem.lex == ";") {
        get_next();
        return;
    } else {
        curr_lexem.lex = "Expected ';' after initialization if variable(s)";
        throw curr_lexem;
    }
}


void Function() {
    is_now_func = true;
    Type();

    //
    prev_lexem();
    curr_function.type = curr_lexem.lex;
    func_type = curr_lexem.lex;
    get_next();
    //

    Name();

    //
    prev_lexem();
    curr_function.name = curr_lexem.lex;
    get_next();
    //

    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Function parameters must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex == ")") {
        get_next();
        add_tid();
        Block();
        delete_tid();
        return;
    }
    add_tid();
    FormalParameters();
    while (true) {
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Function parameters must be in '(' and ')'";
            throw curr_lexem;
        }
        if (curr_lexem.lex != ",") {
            break;
        }
        get_next();
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Expected ')' instead of ','";
            throw curr_lexem;
        }
        FormalParameters();
    }
    if (curr_lexem.lex == ")") {
        get_next();
        Block();
        delete_tid();
        return;
    }
    curr_lexem.lex = "Function parameters must be in '(' and ')'";
    throw curr_lexem;

}

void FormalParameters () {
    std::string type;
    if ( !(curr_lexem.lex == "int" || curr_lexem.lex == "double" || curr_lexem.lex == "bool" ||
           curr_lexem.lex == "string" || curr_lexem.lex == "char")) {
        curr_lexem.lex = "Formal parameter has no type";
        throw  curr_lexem;
    }
    get_next();

    prev_lexem();
    type = curr_lexem.lex;
    get_next();

    if (curr_lexem.lex == "[") {
        prev_lexem();
        DescriptionOfArray();
        Array ar = curr_array;
        add_array();
        curr_function.types.push_back(get_type(ar));
        //curr_function.types[curr_array.name] = get_type(curr_array);
        //curr_function.types.push_back(get_type(curr_array));
        return;
    }

    Name();

    prev_lexem();
    curr_variable.name = curr_lexem.lex;
    curr_variable.type = type;
    get_next();



    if (curr_lexem.type == -1) {
        curr_lexem.lex = "Function parameters must be in '(' and ')'";
        throw curr_lexem;
    }

    add_variable();
    curr_function.types.push_back(type);
    //curr_function.types[curr_variable.name] = curr_variable.type;
    //curr_function.types.push_back()
    if (curr_lexem.lex == "=") {
        get_next();
        Expression();

    }
}

void Type() {
    if ( !(curr_lexem.lex == "int" || curr_lexem.lex == "double" || curr_lexem.lex == "bool" ||
           curr_lexem.lex == "string" || curr_lexem.lex == "char")) {
        curr_lexem.lex = "Missing variable type";
        throw curr_lexem;
    }
    get_next();
    return;
}

void Initialization() {
    if (curr_lexem.lex != "=") {
        curr_lexem.lex = "Initialization must start with the '=' operator";
        throw curr_lexem;
    }
    get_next();
    Expression();
    return;
}

void DescriptionOfArray() {
    Type();

    prev_lexem();
    curr_array.type = curr_lexem.lex;
    get_next();
    int k = 0;
    if (curr_lexem.lex != "[") {
        throw curr_lexem;
    }
    ++k;
    get_next();
    Expression();
    if (curr_lexem.lex != "]") {
        throw curr_lexem;
    }
    get_next();
    if (types.empty()) {
        std::string s = "Error";
        throw s;
    } else {
        types.pop();
    }
    while (true) {
        if (curr_lexem.type == -1) {
            throw curr_lexem;
        }
        if (curr_lexem.lex != "[") {
            break;
        }
        ++k;
        get_next();
        Expression();
        if (curr_lexem.lex != "]") {
            throw curr_lexem;
        }
        get_next();
        if (types.empty()) {
            std::string s = "Error";
            throw s;
        } else {
            types.pop();
        }
    }
    Name();
    prev_lexem();
    curr_array.name = curr_lexem.lex;
    get_next();
    curr_array.dimension = k;
    return;
}

void Operator() {
    if (curr_lexem.lex == "for" || curr_lexem.lex == "while" || curr_lexem.lex == "do"
        || curr_lexem.lex == "foreach") {
        LoopOperator();
        return;
    }
    if (curr_lexem.lex == "if") {
        ConditionalOperator();
        return;
    }
    if (curr_lexem.lex == "get") {
        InputOperator();
        if (curr_lexem.lex != ";") {
            curr_lexem.lex = "Expected ';' after 'get' operator";
            throw curr_lexem;
        }
        get_next();
        return;
    }
    if (curr_lexem.lex == "show") {
        OutputOperator();
        if (curr_lexem.lex != ";") {
            curr_lexem.lex = "Expected ';' after 'show' operator";
            throw curr_lexem;
        }
        get_next();
        return;
    }
    if (curr_lexem.lex == "return") {
        RETURN();

        if (curr_lexem.lex != ";") {
            curr_lexem.lex = "Expected ';' after 'return' operator";
            throw curr_lexem;
        }
        get_next();
        return;
    }
    if (curr_lexem.lex == "break") {
        BREAK();
        if (curr_lexem.lex != ";") {
            curr_lexem.lex = "Expected ';' after 'break' operator";
            throw curr_lexem;
        }
        get_next();
        return;
    }
    if (is_type()) {
        Description();
        return;
    }
    ExpressionOperator();
    if (types.empty()) {
        std::string er = "Error";
        throw er;
    } else {
        types.pop();
    }
    if (curr_lexem.lex != ";") {
        curr_lexem.lex = "Expected ';' after expression operator";
        throw curr_lexem;
    }
    get_next();
    return;
}

void ExpressionOperator() {
    Expression();
    return;
}

void LoopOperator() {
    if (curr_lexem.lex == "for") {
        FOR();
        return;
    }
    if (curr_lexem.lex == "while") {
        WHILE();
        return;
    }
    if (curr_lexem.lex == "do") {
        DOWHILE();
        return;
    }
    if (curr_lexem.lex == "foreach") {
        FOREACH();
        return;
    }
    curr_lexem.lex = "Unknown loop operator";
    throw curr_lexem;

}

void ConditionalOperator() {
    if (curr_lexem.lex != "if") {
        curr_lexem.lex = "Unknown conditional operator";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    Expression();
    if (curr_lexem.lex != ")") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    add_tid();
    Block();
    delete_tid();
    while (true) {
        if (curr_lexem.lex != "elseif") {
            break;
        }
        ELSEIF();
    }
    if (curr_lexem.lex == "else") {
        ELSE();
        return;
    }
    return;

}

void InputOperator() {
    if (curr_lexem.lex != "get") {
        curr_lexem.lex = "Unknown input operator";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Operator parameters must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();

    Name();
    std::string name;

    prev_lexem();
    name = curr_lexem.lex;
    get_next();
    check_name(name);

    while (true) {
        if (curr_lexem.lex != ",") {
            break;
        }
        get_next();
        Name();

        prev_lexem();
        name = curr_lexem.lex;
        get_next();
        check_name(name);
    }
    if (curr_lexem.lex != ")") {
        curr_lexem.lex = "Operator parameters must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    return;
}

void OutputOperator() {
    if (curr_lexem.lex != "show") {
        curr_lexem.lex = "Unknown input operator";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Operator parameters must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    OutputElement();
    while (true) {
        if (curr_lexem.lex != ",") {
            break;
        }
        get_next();
        OutputElement();
    }
    if (curr_lexem.lex != ")") {
        curr_lexem.lex = "Operator parameters must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    return;
}

void Expression() {
    /*if (is_type()) {
        //это описание массива
        DescriptionOfArray();
        return;
    }*/
    /*if (curr_lexem.type == 2) {
        //имя
        std::string name;
        //prev_lexem();
        name = curr_lexem.lex;
        //get_next();
        check_name(name);

        add_to_stack_of_types(name);

        get_next();
        if (curr_lexem.lex == "=") {
            Initialization();

            try{
                if (types.empty()) {
                    std::string s = "Error";
                    throw s;
                }
                std::string up1_type = types.top();
                types.pop();
                if (types.empty()) {
                    std::string s = "Error";
                    throw s;
                }
                std::string up2_type = types.top();
                types.pop();
                std::vector<std::string> temp;
                temp.push_back(up1_type);
                temp.push_back(up2_type);
                match_types(temp, "=");
            } catch(...) {
                std::string s = "Error";
                throw s;
            }

            //std::string str = types.
            //type_matching();
            return;
        }
        types.pop();
        if (curr_lexem.lex == "[") {
            prev_lexem();
            Indexing();
            if (curr_lexem.lex == "=") {
                Initialization();

                try{
                    if (types.empty()) {
                        std::string s = "Error";
                        throw s;
                    }
                    std::string up1_type = types.top();
                    types.pop();
                    if (types.empty()) {
                        std::string s = "Error";
                        throw s;
                    }
                    std::string up2_type = types.top();
                    types.pop();
                    std::vector<std::string> temp;
                    temp.push_back(up1_type);
                    temp.push_back(up2_type);
                    match_types(temp, "=");
                } catch(...) {
                    std::string s = "Error";
                    throw s;
                }

                //std::string str = types.
                //type_matching();
                return;
            }
            return;

        } else {
            prev_lexem();
            Expression1();
            return;
        }

    }*/
    Expression1();
    return;
}



void Expression1() {
    Expression2();
    while (true) {
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Expected '}' in the end of block";
            throw curr_lexem;
        }
        if (!is_AssigmentOperation()) {
            return;
        }
        std::string oper = curr_lexem.lex;

        get_next();
        Expression2();

        try{
            std::vector<std::string> temp;
            std::string s1, s2;
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s1 = types.top();
            types.pop();
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s2 = types.top();
            types.pop();
            temp.push_back(s1);
            temp.push_back(s2);
            match_types(temp, oper);
        } catch(...) {
            std::string s = "Error";
            throw s;
        }

    }
}

void Expression2() {
    Expression3();
    while (true) {
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Expected '}' in the end of block";
            throw curr_lexem;
        }
        if (!is_ComparisonOperation()) {
            return;
        }
        std::string oper = curr_lexem.lex;
        get_next();
        Expression3();

        try{
            std::vector<std::string> temp;
            std::string s1, s2;
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s1 = types.top();
            types.pop();
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s2 = types.top();
            types.pop();
            temp.push_back(s1);
            temp.push_back(s2);
            match_types(temp, oper);
        } catch(...) {
            std::string s = "Error";
            throw s;
        }


    }
}

bool  is_AssigmentOperation() {
    if ( !(curr_lexem.lex == "+=" || curr_lexem.lex == "-=" || curr_lexem.lex == "*="
           || curr_lexem.lex == "/=" || curr_lexem.lex == "%=" || curr_lexem.lex == "^="
           || curr_lexem.lex == "=") ) {
        return false;
    }
    return true;
}

void Expression3() {
    Expression4();
    while (true) {
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Expected '}' in the end of block";
            throw curr_lexem;
        }
        if (!is_AdditionOperation()) {
            return;
        }

        std::string oper = curr_lexem.lex;

        get_next();
        Expression4();

        try{
            std::vector<std::string> temp;
            std::string s1, s2;
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s1 = types.top();
            types.pop();
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s2 = types.top();
            types.pop();
            temp.push_back(s1);
            temp.push_back(s2);
            match_types(temp, oper);
        } catch(...) {
            std::string s = "Error";
            throw s;
        }

    }
}

bool  is_ComparisonOperation() {
    if ( !(curr_lexem.lex == "!=" || curr_lexem.lex == ">" || curr_lexem.lex == "<"
           || curr_lexem.lex == ">=" || curr_lexem.lex == "<=" || curr_lexem.lex == "==") ) {
        return false;
    }
    return true;
}

void Expression4() {
    Expression5();
    while (true) {
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Expected '}' in the end of block";
            throw curr_lexem;
        }
        if (!is_MultiplicationOperation()) {
            return;
        }
        std::string oper = curr_lexem.lex;
        get_next();
        Expression5();

        try{
            std::vector<std::string> temp;
            std::string s1, s2;
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s1 = types.top();
            types.pop();
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s2 = types.top();
            types.pop();
            temp.push_back(s1);
            temp.push_back(s2);
            match_types(temp, oper);
        } catch(...) {
            std::string s = "Error";
            throw s;
        }

    }
}

bool  is_AdditionOperation() {
    if ( !(curr_lexem.lex == "+" || curr_lexem.lex == "-")) {
        return false;
    }
    return true;
}

void Expression5() {
    Expression6();
    while (true) {
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Expected '}' in the end of block";
            throw curr_lexem;
        }
        if (!is_PowOperation()) {
            return;
        }
        std::string oper = curr_lexem.lex;

        get_next();
        Expression6();
        try{
            std::vector<std::string> temp;
            std::string s1, s2;
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s1 = types.top();
            types.pop();
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s2 = types.top();
            types.pop();
            temp.push_back(s1);
            temp.push_back(s2);
            match_types(temp, oper);
        } catch(...) {
            std::string s = "Error";
            throw s;
        }

    }
}

bool  is_MultiplicationOperation() {
    if (!(curr_lexem.lex == "*" || curr_lexem.lex == "/" || curr_lexem.lex == "%")) {
        return false;
    }
    return true;
}

void Expression6() {
    if (curr_lexem.type == 2) {
        //имя

        std::string name;
        //prev_lexem();
        name = curr_lexem.lex;
        //get_next();
        check_name(name);

        get_next();
        if (curr_lexem.lex == "[") {
            prev_lexem();
            Indexing();
            return;
        }
        prev_lexem();
        //add_to_stack_of_types(name);
        //return;
    }
    if (curr_lexem.lex == "++" || curr_lexem.lex == "--") {
        get_next();
        Atom();
        try{
            std::vector<std::string> temp;
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            temp.push_back(types.top());
            types.pop();
            match_types(temp, "++");
        } catch(...) {
            std::string s = "Error";
            throw s;
        }

        return;
    }
    Atom();
    if (curr_lexem.lex == "++" || curr_lexem.lex == "--") {
        try{
            std::vector<std::string> temp;
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            temp.push_back(types.top());
            types.pop();
            match_types(temp, "++");
        } catch(...) {
            std::string s = "Error";
            throw s;
        }

        get_next();
    }
    return;
}

bool  is_PowOperation() {
    if (curr_lexem.lex != "^") {
        return false;
    }
    return true;
}

void Atom() {
    if (curr_lexem.lex == "(") {
        get_next();
        Expression();
        if (curr_lexem.lex != ")") {
            curr_lexem.lex = "The balance of brackets in the expression is broken";
            throw curr_lexem;
        }
        get_next();
        return;
    }
    if (curr_lexem.lex == "+" || curr_lexem.lex == "-" ||
            (curr_lexem.lex[0] == '\'' && curr_lexem.lex[curr_lexem.lex.size() - 1] == '\'' )||
            (curr_lexem.lex[0] == '\"' && curr_lexem.lex[curr_lexem.lex.size() - 1] == '\"') ||
            curr_lexem.lex == "true" || curr_lexem.lex == "false" ||
        curr_lexem.lex == "!" || is_number()) {
        Const();
        return;
    }
    Name();

    std::string name;
    prev_lexem();
    name = curr_lexem.lex;
    get_next();
    check_name(name);

    if (curr_lexem.lex == "(") {
        prev_lexem();
        FunctionCall();
        return;
    }

    add_to_stack_of_types(name);
    return;
}

void Increment() {
    if (!(curr_lexem.lex == "++" || curr_lexem.lex == "--")) {
        throw curr_lexem;
    }
    get_next();
    return;
}

void Indexing() {
    Name();

    std::string name;
    prev_lexem();
    name = curr_lexem.lex;
    get_next();
    check_name(name);

    add_to_stack_of_types(name);

    if (curr_lexem.lex != "[") {
        curr_lexem.lex = "To refer to an element of an array, "
                         "use the operator '[' and ']'";
        throw curr_lexem;
    }
    get_next();
    Expression();

    if (curr_lexem.lex != "]") {
        curr_lexem.lex = "To refer to an element of an array, "
                         "use the operator '[' and ']'";
        throw curr_lexem;
    }
    std::string s;
    try{
        if (types.empty()) {
            std::string s = "Error";
            throw s;
        }
        s = types.top();
        types.pop();
    } catch(...) {
        std::string s = "Error";
        throw s;
    }

    if (s != "int") {
        std::string error = "Inside [ ] must be an integer";
        throw error;
    }
    try{
        if (types.empty()) {
            std::string s = "Error";
            throw s;
        }
        s = types.top();
        types.pop();
        match_types({s}, "[]");
    } catch(...) {
        std::string s = "Error";
        throw s;
    }


    get_next();
    while (true) {
        if (curr_lexem.lex != "[") {
            break;
        }
        get_next();
        Expression();
        if (curr_lexem.lex != "]") {
            curr_lexem.lex = "To refer to an element of an array, "
                             "use the operator '[' and ']'";
            throw curr_lexem;
        }
        try{
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s = types.top();
            types.pop();
        } catch (...) {
            std::string s = "Error";
            throw s;
        }

        if (s != "int") {
            std::string error = "Inside [ ] must be an integer";
            throw error;
        }
        try{
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            s = types.top();
            types.pop();
            match_types({s}, "[]");
        } catch (...) {
            std::string s = "Error";
            throw s;
        }

        get_next();
    }
    return;
}

void Const() {
    if (curr_lexem.lex == "!") {
        get_next();
        Atom();
        try{
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            std::string s = types.top();
            types.pop();
            match_types({s}, "!");
        } catch (...) {
            std::string s = "Error";
            throw s;
        }

        return;
    }
    if (curr_lexem.lex == "true" || curr_lexem.lex == "false") {
        Boolean();
        return;
    }
    Constant();
}

void FunctionCall() {
    //is_now_func = true;
    Name();
    int count_of_params = 0;
    std::string name;
    prev_lexem();
    name = curr_lexem.lex;
    get_next();
    check_name(name);

    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Function parameters must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex == ")") {
        match_types({name}, "()");
        get_next();
        return;
    }
    Expression();
    ++count_of_params;
    while (true) {
        if (curr_lexem.lex != ",") {
            break;
        }
        get_next();
        if (curr_lexem.type == -1) {
            curr_lexem.lex = "Function parameters must be in '(' and ')'";
            throw curr_lexem;
        }
        Expression();
        ++count_of_params;
    }
    if (curr_lexem.lex == ")") {
        //if (count_of_params)
        //Function c;
        //Function curr_func;
        if (find_function(name).count_of_parameters != count_of_params) {
            std::string s = "Incorrect number of function params";
            throw s;
        }
        std::vector<std::string> temp;
        temp.push_back(name);
        for (int i = 0; i < count_of_params; ++i) {
            std::string s;
            if (types.empty()) {
                std::string error = "Missing function params";
                throw s;
            } else {
                s = types.top();
                types.pop();
                temp.push_back(s);
            }
        }
        match_types(temp, "()");
        get_next();
        return;
    }
    curr_lexem.lex = "Function parameters must be in '(' and ')'";
    throw curr_lexem;
}

void Constant() {
    if (curr_lexem.lex[0] == '\'') {
        AnySymbol();
        return;
    }
    if (curr_lexem.lex[0] == '\"') {
        AnyString();
        return;
    }
    if (curr_lexem.lex == "+" || curr_lexem.lex == "-") {
        get_next();
    }
    if (!is_number()) {
        curr_lexem.lex = "Incorrect constant";
        throw curr_lexem;
    }

    std::string tp = "int";
    if (curr_lexem.lex.find(".") != -1) {
        tp = "double";
        types.push(tp);
        get_next();
        return;
    }
    get_next();
    types.push("int");
    return;
}

void Boolean() {
    if (!(curr_lexem.lex == "true" || curr_lexem.lex == "false")) {
        curr_lexem.lex = "Expected boolean";
        throw curr_lexem;
    }
    get_next();
    types.push("bool");
    return;
}

void Sign() {

}

void Digit() {

}

void AnyString() {
    if (curr_lexem.type != 3) {
        curr_lexem.lex = "Expected string";
        throw curr_lexem;
    }
    get_next();
    types.push("anystring");
    return;
}

void AnySymbol() {
    if (!(curr_lexem.lex.size() == 3 && curr_lexem.lex[0] == '\''
          && curr_lexem.lex[2] == '\'')) {
        curr_lexem.lex = "Expected one symbol and two brackets:', like 'a'";
        throw curr_lexem;
    }
    get_next();
    types.push("symbol");
    return;
}

void FOR() {
    add_tid();
    if (curr_lexem.lex != "for") {
        curr_lexem.lex = "Unknown loop operator, expected 'for' operator";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();

    if (curr_lexem.lex == ";") {
        get_next();
    } else {
        if (is_type()) {
            Description();
            /*try{
                if (types.empty()) {
                    std::string s = "Error";
                    throw s;
                }
                types.pop();
            } catch (...){
                std::string s = "Error";
                throw s;
            }*/
        } else {
            Expression();
            if (curr_lexem.lex != ";") {
                curr_lexem.lex = "Incorrect structure of the 'for' operator";
                throw curr_lexem;
            }
            try{
                if (types.empty()) {
                    std::string s = "Error";
                    throw s;
                }
                types.pop();
            } catch (...){
                std::string s = "Error";
                throw s;
            }
            get_next();
        }

    }

    if (curr_lexem.lex == ";") {
        get_next();
    } else {
        Expression();
        if (curr_lexem.lex != ";") {
            curr_lexem.lex = "Incorrect structure of the 'for' operator";
            throw curr_lexem;
        }
        try{
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            types.pop();
        } catch (...){
            std::string s = "Error";
            throw s;
        }
        get_next();
    }
    if (curr_lexem.lex == ")") {
        get_next();
    } else {
        Expression();
        if (curr_lexem.lex != ")") {
            curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
            throw curr_lexem;
        }
        try{
            if (types.empty()) {
                std::string s = "Error";
                throw s;
            }
            types.pop();
        } catch (...){
            std::string s = "Error";
            throw s;
        }
        get_next();
    }
    Block();
    delete_tid();
}

void WHILE() {
    add_tid();
    if (curr_lexem.lex != "while") {
        curr_lexem.lex = "Unknown loop operator, expected 'while' operator";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex == ")") {
        get_next();
        Block();
        return;
    }
    Expression();
    try{
        if (types.empty()) {
            std::string s = "Error";
            throw s;
        }
        types.pop();
    } catch (...){
        std::string s = "Error";
        throw s;
    }
    if (curr_lexem.lex == ")") {
        get_next();
        Block();
        delete_tid();
        return;
    }
    curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
    throw curr_lexem;
}

void DOWHILE() {
    if (curr_lexem.lex != "do") {
        curr_lexem.lex = "Unknown loop operator, expected 'dowhile' operator";
        throw curr_lexem;
    }
    get_next();
    add_tid();
    Block();
    delete_tid();
    if (curr_lexem.lex != "while") {
        curr_lexem.lex = "Unknown loop operator, expected 'dowhile' operator";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex == ")") {
        get_next();
        if (curr_lexem.lex != ";") {
            curr_lexem.lex = "Incorrect structure of the 'dowhile' operator, "
                             "expected ';' after 'while' condition";
            throw curr_lexem;
        }
        get_next();
        return;
    }
    Expression();
    try{
        if (types.empty()) {
            std::string s = "Error";
            throw s;
        }
        types.pop();
    } catch (...){
        std::string s = "Error";
        throw s;
    }
    if (curr_lexem.lex != ")") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != ";") {
        curr_lexem.lex = "Incorrect structure of the 'dowhile' operator, "
                         "expected ';' after 'while' condition";
        throw curr_lexem;
    }
    get_next();
    return;
}

void RETURN() {
    if (curr_lexem.lex != "return") {
        curr_lexem.lex = "Unknown operator, expected 'return' operator";
        throw curr_lexem;
    }
    get_next();
    Expression();
    if (is_now_func) {
        is_now_func = false;
        if (types.empty()) {
            std::string er = "No return";
            throw er;
        } else {
            std::string s = types.top();
            types.pop();
            if (s != func_type) {
                std::string er = "Bad return type";
                throw er;
            }
        }
    }
    return;
}

void BREAK() {
    if (curr_lexem.lex != "break") {
        curr_lexem.lex = "Unknown operator, expected 'return' operator";
        throw curr_lexem;
    }
    get_next();
    return;
}

void FOREACH() {
    add_tid();
    if (curr_lexem.lex != "foreach") {
        curr_lexem.lex = "Unknown loop operator, expected 'foreach' operator";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    std::string type, name, first, second;

    Type();

    prev_lexem();
    curr_variable.type = curr_lexem.lex;
    get_next();

    Name();

    prev_lexem();
    curr_variable.name = curr_lexem.lex;
    first = curr_variable.name;
    get_next();

    add_variable();


    if (curr_lexem.lex != "in") {
        curr_lexem.lex = "Incorrect structure of the 'foreach' operator, "
                         "missing 'in'";
        throw curr_lexem;
    }
    get_next();
    Name();


    //std::string name;
    prev_lexem();
    name = curr_lexem.lex;
    get_next();
    check_name(name);
    second = name;



    if (curr_lexem.lex != ")") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    match_types({first, second}, "in");
    get_next();
    Block();
    delete_tid();
    return;
}

void ELSEIF() {
    add_tid();
    if (curr_lexem.lex != "elseif") {
        curr_lexem.lex = "Unknown conditional operator, expected 'elseif'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    Expression();
    try{
        if (types.empty()) {
            std::string s = "Error";
            throw s;
        }
        types.pop();
    } catch (...){
        std::string s = "Error";
        throw s;
    }

    if (curr_lexem.lex != ")") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    Block();
    delete_tid();
    return;
}

void ELSE() {
    add_tid();
    if (curr_lexem.lex != "else") {
        curr_lexem.lex = "Unknown conditional operator, expected 'else'";
        throw curr_lexem;
    }
    get_next();
    Block();
    delete_tid();
    return;
}

void OutputElement() {
    if (curr_lexem.lex == "endl") {
        get_next();
        return;
    }
    Expression();
    try{
        if (types.empty()) {
            std::string s = "Error";
            throw s;
        }
        types.pop();
    } catch (...){
        std::string s = "Error";
        throw s;
    }
    return;
}

void Name() {
    if (type(curr_lexem.type) != "IDENTIFIERS") {
        curr_lexem.lex = "Expected Identifier";
        throw curr_lexem;
    }
    get_next();
    return;
}





void Parser(std::vector<std::vector<std::pair<int, std::string>>> ans) {
    for (int i = 0; i < ans.size(); ++i) {
        for (int j = 0; j < ans[i].size(); ++j) {
            lexem new_lexem;
            new_lexem.type = ans[i][j].first;
            new_lexem.lex = ans[i][j].second;
            Lexems.push_back(new_lexem);
        }
    }
    get_next();
    curr_tid = new TID;
    curr_tid->prev_tid = nullptr;
    Program();
    delete curr_tid;
}



