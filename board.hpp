#pragma once

#include <vector>

struct Element {
    static constexpr char EMPTY = '.';
    char Symbol;
    Element(char c) : Symbol(c) {}

    bool operator==(const Element&) const = default;
};

struct Board {
    int size;
    std::vector<std::vector<Element>> cells;
    
    Board(int s)
    : size(s),
    cells(s, std::vector<Element>(s, Element::EMPTY)) {}

    Board(int s, std::vector<std::vector<Element>> c)
    : size(s), cells(c) {}
};

class BoardState {
public:
    BoardState(Board board, int score)
    : board_(board), score_(score) {}

    const Board& Board() const {
        return board_;
    }

    int Score() const {
        return score_;
    }

private:
    struct Board board_;
    int score_;
};
