#include "board.hpp"
#include "match.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <vector>

void Draw(const Board& board) {
    std::cout << " ";
    for (int j = 0; j < board.size; j++) {
        std::cout << " " << j;
    }
    std::cout << std::endl;
    for (int i = 0; i < board.size; i++) {
        std::cout << i << " ";
        for (int j = 0; j < board.size; j++) {
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

BoardState ReadMove(const BoardState& bs) {
    std::cout << "> ";
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
    Element e = board.cells[y][x]; 
    board.cells[y][x] = board.cells[y1][x1];  
    board.cells[y1][x1] = e;
    return BoardState(board, bs.score());
}

Element RandomElement() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution dist(0, 5);
    constexpr char symbols[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    return Element(symbols[dist(gen)]);
}

BoardState FillEmptySpaces(const BoardState& currentState) {
    if (currentState.board().cells.empty())
        return currentState;

    std::vector<std::vector<Element>> newCells = currentState.board().cells;

    for (int row = 0; row < currentState.board().size; row++) {
        for (int col = 0; col < currentState.board().size; col++) {
            if (newCells[row][col].Symbol == Element::EMPTY) {
                newCells[row][col] = RandomElement();
            }
        }
    }

    return BoardState(
        Board(currentState.board().size, newCells),
        currentState.score());
}

BoardState RemoveAllMatches(const BoardState& currentState) {
    auto matches = FindMatches(currentState.board());
    if (matches.empty()) {
        return BoardState(currentState);
    }
    return currentState
        | [&](auto bs) { return RemoveMatches(bs, matches); }
        | FillEmptySpaces
        | RemoveAllMatches;
}

BoardState ZeroScore(const BoardState& currentState) {
    return BoardState(currentState.board(), 0);
}

BoardState InitializeGame(int size) {
    return BoardState(Board(size), 0)
        | FillEmptySpaces
        | RemoveAllMatches
        | ZeroScore;
}

BoardState ProcessCascade(const BoardState& bs) {
    std::cout << std::endl << "Score: " << bs.score() << std::endl;
    Draw(bs.board());
    return bs | ReadMove | RemoveAllMatches;
}

int main() {
    BoardState bs = InitializeGame(5);
    while (true) {
        bs |= ProcessCascade;
    }
    return 0;
}
