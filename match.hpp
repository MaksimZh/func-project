#pragma once

#include "board.hpp"

#include <vector>

enum class MatchDirection { Horizontal, Vertical };

class Match {
public:
    Match(MatchDirection direction, int row, int col, int length)
    : direction_(direction), row_(row), col_(col), length_(length) {}

    MatchDirection Direction() const {
        return direction_;
    }

    int Row() const {
        return row_;
    }

    int Col() const {
        return col_;
    }

    int Length() const {
        return length_;
    }

private:
    MatchDirection direction_;
    int row_;
    int col_;
    int length_;
};

std::vector<Match> FindMatches(const Board& board);
BoardState RemoveMatches(
    const BoardState& currentState, std::vector<Match> matches);
