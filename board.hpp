#pragma once

#include <vector>
#include <type_traits>

struct Element {
    static constexpr char EMPTY = '.';
    char Symbol;
    Element(char c) : Symbol(c) {}

    bool operator==(const Element&) const = default;
};

struct Board {
    int size;
    std::vector<std::vector<Element>> cells;
    
    explicit Board(int s)
    : size(s),
    cells(s, std::vector<Element>(s, Element::EMPTY)) {}

    explicit Board(int s, std::vector<std::vector<Element>> c)
    : size(s), cells(c) {}
};


struct BoardState {
    Board board;
    int score;
};
