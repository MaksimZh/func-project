#include "board.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <random>

void Draw(const Board& board) {
    std::cout << "  0 1 2 3 4 5 6 7" << std::endl;
    for(int i = 0; i < 8; i++) {
        std::cout << i << " ";
        for(int j = 0; j < 8; j++) {
            std::cout << board.cells[i][j].Symbol << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<std::string> SplitWords(const std::string& text) {
    std::istringstream s(text);
    std::vector<std::string> words;
    std::string word;
    while (s >> word) {
        words.push_back(word);
    }
    return words;
}

static BoardState ReadMove(const BoardState& bs) {
    std::cout << ">" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    if (input == "q") 
        std::exit(0);  // TODO: find better way to signal program termination

    Board board = bs.board();
    auto coords = SplitWords(input);
    int x = std::stoi(coords[1]);
    int y = std::stoi(coords[0]);
    int x1 = std::stoi(coords[3]);
    int y1 = std::stoi(coords[2]);
    Element e = board.cells[x][y]; 
    board.cells[x][y] = board.cells[x1][y1];  
    board.cells[x1][y1] = e;
    return BoardState(board, bs.score());
}

BoardState InitializeGame() {
    
}

int main() {
    BoardState bs = InitializeGame();
    return 0;
}
