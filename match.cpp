#include "match.hpp"
#include "board.hpp"

#include <numeric>
#include <vector>

void AddMatchIfValid(
    std::vector<Match>& matches, int row, int col, 
    int length, MatchDirection direction
) {
    // Учитываем только комбинации из 3 и более элементов (ТЗ)
    if (length >= 3) {
        matches.emplace_back(direction, row, col, length);
    }
}

std::vector<Match> FindMatches(const Board& board) {
    std::vector<Match> matches;

    // Горизонтальные комбинации
    for (int row = 0; row < board.size; row++) {
        int startCol = 0;
        for (int col = 1; col < board.size; col++) {
            // Пропускаем пустые ячейки в начале строки
            if (board.cells[row][startCol].Symbol == Element::EMPTY) {
                startCol = col;
                continue;
            }

            // Если текущая ячейка пустая, обрываем текущую последовательность
            if (board.cells[row][col].Symbol == Element::EMPTY) {
                AddMatchIfValid(
                    matches, row, startCol,
                    col - startCol, MatchDirection::Horizontal);
                startCol = col + 1;
                continue;
            }

            // Проверяем совпадение символов для непустых ячеек
            if (board.cells[row][col] != board.cells[row][startCol]) {
                AddMatchIfValid(
                    matches, row, startCol,
                    col - startCol, MatchDirection::Horizontal);
                startCol = col;
            } else if (col == board.size - 1) {
                AddMatchIfValid(
                    matches, row, startCol,
                    col - startCol + 1, MatchDirection::Horizontal);
            }
        }
    }

    // Вертикальные комбинации
    for (int col = 0; col < board.size; col++) {
        int startRow = 0;
        for (int row = 1; row < board.size; row++) {
            // Пропускаем пустые ячейки в начале столбца
            if (board.cells[startRow][col].Symbol == Element::EMPTY)
            {
                startRow = row;
                continue;
            }

            // Если текущая ячейка пустая, обрываем текущую последовательность
            if (board.cells[row][col].Symbol == Element::EMPTY)
            {
                AddMatchIfValid(
                    matches, startRow, col,
                    row - startRow, MatchDirection::Vertical);
                startRow = row + 1;
                continue;
            }

            // Проверяем совпадение символов для непустых ячеек
            if (board.cells[row][col] != board.cells[startRow][col]) {
                AddMatchIfValid(
                    matches, startRow, col,
                    row - startRow, MatchDirection::Vertical);
                startRow = row;
            } else if (row == board.size - 1) {
                AddMatchIfValid(
                    matches, startRow, col,
                    row - startRow + 1, MatchDirection::Vertical);
            }
        }
    }

    return matches;
}


std::vector<std::vector<Element>> MarkCellsForRemoval(
    const Board& board, std::vector<Match> matches
) {
    std::vector<std::vector<Element>> newCells(board.cells);

    for (const auto& match : matches) {
        for (int i = 0; i < match.Length(); i++) {
            int row = (match.Direction() == MatchDirection::Horizontal)
                ? match.Row()
                : match.Row() + i;
            int col = (match.Direction() == MatchDirection::Horizontal)
                ? match.Col() + i
                : match.Col();
            newCells[row][col] = Element(Element::EMPTY);
        }
    }

    return newCells;
}


std::vector<std::vector<Element>> ApplyGravity(
    const std::vector<std::vector<Element>>& cells, int size
) {
    std::vector<std::vector<Element>> newCells(
        size, std::vector<Element>(size, Element::EMPTY));

    for (int col = 0; col < size; col++) {
        int newRow = size - 1;
        for (int row = size - 1; row >= 0; row--) {
            if (cells[row][col].Symbol != Element::EMPTY) {
                newCells[newRow][col] = cells[row][col];
                newRow--;
            }
        }
    }

    return newCells;
}


int CalculateScore(int removedCount) {
    // Базовая система подсчета очков: 10 за каждый элемент
    return removedCount * 10;
}


BoardState RemoveMatches(
    const BoardState& currentState, std::vector<Match> matches
) {
    if (matches.empty())
        return currentState;

    // Шаг 1: Помечаем ячейки для удаления 
    std::vector<std::vector<Element>> markedCells =
        MarkCellsForRemoval(currentState.board, matches);

    // Шаг 2: Применяем гравитацию
    std::vector<std::vector<Element>> gravityAppliedCells =
        ApplyGravity(markedCells, currentState.board.size);

    // Шаг 3: Подсчитываем очки
    int removedCount = std::accumulate(
        matches.begin(), matches.end(), 0,
        [](int acc, const Match& m) { return acc + m.Length(); });
    int newScore = currentState.score + CalculateScore(removedCount);

    // Возвращаем НОВОЕ состояние
    return BoardState(
        Board(currentState.board.size, gravityAppliedCells),
        newScore);
}
