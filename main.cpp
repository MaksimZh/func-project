#include "board.hpp"
#include "match.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <vector>

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

    Board board = bs.Board();
    auto coords = SplitWords(input);
    int x = std::stoi(coords[1]);
    int y = std::stoi(coords[0]);
    int x1 = std::stoi(coords[3]);
    int y1 = std::stoi(coords[2]);
    Element e = board.cells[x][y]; 
    board.cells[x][y] = board.cells[x1][y1];  
    board.cells[x1][y1] = e;
    return BoardState(board, bs.Score());
}

Element RandomElement() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution dist(0, 5);
    constexpr char symbols[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    return Element(symbols[dist(gen)]);
}

Board RandomBoard(int size) {
    Board board(size);
    for (auto& row : board.cells) {
        std::generate(row.begin(), row.end(), RandomElement);
    }
    return board;
}

BoardState InitializeGame() {
    return BoardState(RandomBoard(8), 0);
}

BoardState FillEmptySpaces(BoardState currentState) {
    if (currentState.Board().cells.empty())
        return currentState;

    std::vector<std::vector<Element>> newCells = currentState.Board().cells;

    for (int row = 0; row < currentState.Board().size; row++) {
        for (int col = 0; col < currentState.Board().size; col++) {
            if (newCells[row][col].Symbol == Element::EMPTY) {
                newCells[row][col] = RandomElement();
            }
        }
    }

    return BoardState(
        Board(currentState.Board().size, newCells),
        currentState.Score());
}

int main() {
    BoardState bs = InitializeGame();
    Draw(bs.Board());
    bs = RemoveMatches(bs, FindMatches(bs.Board()));
    Draw(bs.Board());
    bs = FillEmptySpaces(bs);
    Draw(bs.Board());
    return 0;
}
