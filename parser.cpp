
//
// Created by Spanch bob on 12.03.2022.
//
#include <iostream>
#include <vector>
#include <map>

int count_of_lexems = -1;
struct lexem{
    int type;
    std::string lex;
};
lexem curr_lexem;

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
    std::map<std::string, std::string> types;
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
Function curr_function;

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
    } else {
        std::string error = "Переменная " + curr_variable.name +
                            " несколько раз инициализирована в одном блоке";
        throw error;
    }
}
void add_array() {
    if (check_array()) {
        curr_tid->arrays[curr_array.name] = curr_array;
    } else {
        std::string error = "Переменная " + curr_array.name +
                            " несколько раз инициализирована в одном блоке";
        throw error;
    }
}
void add_function() {
    if (check_function()) {
        curr_tid->functions[curr_function.name] = curr_function;
    } else {
        std::string error = "Переменная " + curr_function.name +
                            " несколько раз инициализирована в одном блоке";
        throw error;
    }
}

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

    Type();

    //
    prev_lexem();
    curr_function.type = curr_lexem.lex;
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
        add_array();
        curr_function.types[curr_array.name] = get_type(curr_array);
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
    curr_function.types[curr_variable.name] = curr_variable.type;
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
    while (true) {
        if (curr_lexem.lex != ",") {
            break;
        }
        get_next();
        Name();
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
    if (curr_lexem.type == 2) {
        //имя
        get_next();
        if (curr_lexem.lex == "=") {
            Initialization();
            return;
        }
        if (curr_lexem.lex == "[") {
            prev_lexem();
            Indexing();
            return;
        }
        prev_lexem();
        Expression1();
        return;
    }
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
        get_next();
        Expression2();
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
        get_next();
        Expression3();
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
        get_next();
        Expression4();
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
        get_next();
        Expression5();
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
        get_next();
        Expression6();
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
        get_next();
        if (curr_lexem.lex == "[") {
            prev_lexem();
            Indexing();
            return;
        }
        prev_lexem();
    }
    if (curr_lexem.lex == "++" || curr_lexem.lex == "--") {
        get_next();
        Atom();
        return;
    }
    Atom();
    if (curr_lexem.lex == "++" || curr_lexem.lex == "--") {
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
    if (curr_lexem.lex == "(") {
        prev_lexem();
        FunctionCall();
        return;
    }
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
        get_next();
    }
    return;
}

void Const() {
    if (curr_lexem.lex == "!") {
        get_next();
        Atom();
        return;
    }
    if (curr_lexem.lex == "true" || curr_lexem.lex == "false") {
        Boolean();
        return;
    }
    Constant();
}

void FunctionCall() {
    Name();
    if (curr_lexem.lex != "(") {
        curr_lexem.lex = "Function parameters must be in '(' and ')'";
        throw curr_lexem;
    }
    get_next();
    if (curr_lexem.lex == ")") {
        get_next();
        return;
    }
    Expression();
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
    }
    if (curr_lexem.lex == ")") {
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
    get_next();
    if (curr_lexem.lex == ".") {
        get_next();
        if (!is_number()) {
            curr_lexem.lex = "Incorrect constant";
            throw curr_lexem;
        }
        get_next();
        return;
    }
    return;
}

void Boolean() {
    if (!(curr_lexem.lex == "true" || curr_lexem.lex == "false")) {
        curr_lexem.lex = "Expected boolean";
        throw curr_lexem;
    }
    get_next();
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
    return;
}

void AnySymbol() {
    if (!(curr_lexem.lex.size() == 3 && curr_lexem.lex[0] == '\''
          && curr_lexem.lex[2] == '\'')) {
        curr_lexem.lex = "Expected one symbol and two brackets:', like 'a'";
        throw curr_lexem;
    }
    get_next();
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
        } else {
            Expression();
            if (curr_lexem.lex != ";") {
                curr_lexem.lex = "Incorrect structure of the 'for' operator";
                throw curr_lexem;
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
    std::string type, name;

    Type();

    prev_lexem();
    curr_variable.type = curr_lexem.lex;
    get_next();

    Name();

    prev_lexem();
    curr_variable.name = curr_lexem.lex;
    get_next();

    add_variable();

    if (curr_lexem.lex != "in") {
        curr_lexem.lex = "Incorrect structure of the 'foreach' operator, "
                         "missing 'in'";
        throw curr_lexem;
    }
    get_next();
    Name();
    if (curr_lexem.lex != ")") {
        curr_lexem.lex = "Operator execution conditions must be in '(' and ')'";
        throw curr_lexem;
    }
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



