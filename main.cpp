#include "board.hpp"
#include "match.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <sstream>
#include <random>
#include <vector>
#include <type_traits>


// Конвеерная магия ------------------------------------------------------------
template <typename T>
auto piped(void (*f)(const T&)) {
    return [f](const T& x) -> T { f(x); return x; };
}

template <typename F>
requires std::is_invocable_v<F, BoardState>
auto operator|(const BoardState& bs, F f) {
    return f(bs);
}

template <typename F>
requires std::is_invocable_r_v<Board, F, Board>
auto operator|(const BoardState& bs, F f) {
    return BoardState{f(bs.board), bs.score};
}

template<typename>   constexpr bool is_optional = false;
template<typename T> constexpr bool is_optional<std::optional<T>> = true;

template <typename T>
auto ensureOptional(T x) {
    if constexpr (is_optional<T>) {
        return x;
    } else {
        return std::make_optional(x);
    }
}

// Теперь std::optional - это монада.
template <typename F>
auto operator|(const std::optional<BoardState>& bs, F f) {
    return bs ? ensureOptional(bs.value() | f) : std::nullopt;
}
// -----------------------------------------------------------------------------

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
    std::cout << std::endl << std::endl;
}

void DrawScore(const BoardState& bs) {
    std::cout << "Score: " << bs.score << std::endl;
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

std::optional<BoardState> ReadMove(const BoardState& bs) {
    std::cout << "> ";
    std::string input;
    std::getline(std::cin, input);
    if (input == "q") {
        // Вот для этого понадобилась монада.
        // Теперь выход из игры будет обрабатываться в основном цикле.
        return std::nullopt;
    }

    Board board = bs.board;
    auto coords = SplitWords(input);
    int x = std::stoi(coords[1]);
    int y = std::stoi(coords[0]);
    int x1 = std::stoi(coords[3]);
    int y1 = std::stoi(coords[2]);
    Element e = board.cells[y][x]; 
    board.cells[y][x] = board.cells[y1][x1];  
    board.cells[y1][x1] = e;
    return BoardState{board, bs.score};
}

Element RandomElement() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution dist(0, 5);
    constexpr char symbols[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    return Element(symbols[dist(gen)]);
}

BoardState FillEmptySpaces(const BoardState& currentState) {
    if (currentState.board.cells.empty())
        return currentState;

    std::vector<std::vector<Element>> newCells = currentState.board.cells;

    for (int row = 0; row < currentState.board.size; row++) {
        for (int col = 0; col < currentState.board.size; col++) {
            if (newCells[row][col].Symbol == Element::EMPTY) {
                newCells[row][col] = RandomElement();
            }
        }
    }

    return BoardState(
        Board(currentState.board.size, newCells),
        currentState.score);
}

struct BoardStateMatches {
    BoardState state;
    std::vector<Match> matches;

    template <typename F>
    auto operator|(F f) const {
        return f(*this);
    }
};

BoardStateMatches CollectMatches(const BoardState& bs) {
    return BoardStateMatches(bs, FindMatches(bs.board));
}

BoardState RemoveCollectedMatches(const BoardStateMatches& bsm) {
    return RemoveMatches(bsm.state, bsm.matches);
}

BoardState RemoveAllMatches(const BoardStateMatches& bsm) {
    return bsm.matches.empty()
        ? bsm.state
        : bsm
            | RemoveCollectedMatches
            | FillEmptySpaces
            | CollectMatches
            | RemoveAllMatches;
}

BoardState ProcessCascade(const BoardState& bs) {
    return bs | CollectMatches | RemoveAllMatches;
}

BoardState ZeroScore(const BoardState& currentState) {
    return BoardState(currentState.board, 0);
}

BoardState InitializeGame(int size) {
    return BoardState{Board(size), 0}
        | FillEmptySpaces
        | ProcessCascade
        | ZeroScore;
}

int main() {
    std::optional<BoardState> bs = InitializeGame(5);
    while (bs) {
        bs = bs
            | piped(DrawScore)
            | piped(Draw)
            | ReadMove
            | ProcessCascade;
    }
    return 0;
}
