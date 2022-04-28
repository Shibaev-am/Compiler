#include <iostream>
#include <map>
#include <vector>


std::map<int, int> a = {{2, 3}};
std::vector<std::vector<std::pair<int, std::string>>> lexer(std::vector<std::string>& text);
std::string type(int num);
void Parser(std::vector<std::vector<std::pair<int, std::string>>>);

enum{
    SERVICE_NAMES = 1,
    IDENTIFIERS,
    CONSTANTS,
    OPERATIONS,
    PUNCTUATION,
    SQUARE_BRACKETS,
    ROUND_BRACKETS,
};

struct lexem{
    int type;
    std::string lex;
};



int main() {
    //std::cout << "Hello, World!" << std::endl;
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    setlocale(LC_ALL, "Russian");
    std::string s;
    std::vector<std::string> text;
    while (std::getline(std::cin, s)) {
        text.push_back(s);
    }
    std::vector<std::vector<std::pair<int, std::string>>> ans;
    try{
        ans = lexer(text);
        for (int i = 0; i < ans.size(); ++i) {
            for (int j = 0; j < ans[i].size(); ++j) {
                std::cout << "<" << type(ans[i][j].first) << ", " << ans[i][j].second << ">" << " ";
            }
            std::cout << "\n";
        }
        try {
            Parser(ans);
        } catch (lexem error) {
            std::cout << error.lex;
        }
    } catch (int e) {
        std::cout << "Ошибка ввода";
    } catch (std::string s) {
        std::cout << s;
    } catch(...) {
        std::cout << "ERROR!!!!!";
    }

    return 0;
}
