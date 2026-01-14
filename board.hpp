#pragma once

#include <vector>

struct Element {
    static constexpr char EMPTY = '0';
    char Symbol;
    Element(char c) : Symbol(c) {}
};

struct Board {
    int size;
    std::vector<std::vector<Element>> cells;
    
    Board(int s)
    : size(s),
    cells(s, std::vector<Element>(s, Element::EMPTY)) {}
};

class BoardState {
public:
    BoardState(Board board, int score)
    : board_(board), score_(score) {}

    const Board& board() const {
        return board_;
    }

    int score() const {
        return score_;
    }

private:
    Board board_;
    int score_;
};
