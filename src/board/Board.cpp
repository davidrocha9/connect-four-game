#include "Board.h"
#include <iostream>
#include <climits>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Utils.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

Board::Board(SDL_Renderer *renderer)
    : renderer(renderer), rows(6), cols(7), cellSize(100), playerTurn(PLAYER_1), selectedCol(-1),
      bgColor({0x00, 0x00, 0xFF, 0xFF}), lineColor({0x00, 0x00, 0x00, 0xFF}),
      player1Color({0xFF, 0x00, 0x00, 0xFF}), player2Color({0x00, 0xFF, 0x00, 0xFF}), textColor({0x00, 0x00, 0x00, 0xFF})
{
    grid.resize(rows, vector<int>(cols, 0));

    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("../fonts/font.ttf", 25);
    if (!font)
    {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    LoadTexture("../assets/red.png", renderer, redDiscTexture);
    LoadTexture("../assets/blue.png", renderer, blueDiscTexture);
}

Board::~Board()
{
    if (redDiscTexture)
    {
        SDL_DestroyTexture(redDiscTexture);
        redDiscTexture = nullptr;
    }
    if (blueDiscTexture)
    {
        SDL_DestroyTexture(blueDiscTexture);
        blueDiscTexture = nullptr;
    }

    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }

    grid.clear();

    TTF_Quit();
}

void Board::SetWinnerCallback(const std::function<void(int)> &callback)
{
    winnerCallback = callback;
}

void Board::Render()
{
    DrawSelectedCol();
    DrawGrid();
    DrawDiscs();

    if (winner != 0)
        ShowGameEndMessage(winner);
}

void Board::DrawGrid()
{
    SDL_SetRenderDrawColor(renderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    int x1, y1, x2, y2;

    // Draw horizontal lines
    for (int row = 0; row <= rows; ++row)
    {
        x1 = GetGridLeftX(windowWidth);
        x2 = x1 + cols * cellSize;
        y1 = GetGridTopY(windowHeight) + row * cellSize;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y1);
    }

    // Draw vertical lines
    for (int col = 0; col <= cols; ++col)
    {
        x1 = GetGridLeftX(windowWidth) + col * cellSize;
        y1 = GetGridTopY(windowHeight);
        y2 = y1 + rows * cellSize;
        SDL_RenderDrawLine(renderer, x1, y1, x1, y2);
    }
}

void Board::DrawSelectedCol()
{
    if (selectedCol == -1)
        return;

    SDL_SetRenderDrawColor(renderer, 95, 166, 202, 128);

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    SDL_Rect rect;
    rect.x = windowWidth / 2 - cols * cellSize / 2 + selectedCol * cellSize;
    rect.y = windowHeight / 2 - rows * cellSize / 2;
    rect.w = cellSize;
    rect.h = rows * cellSize;

    SDL_RenderFillRect(renderer, &rect);
}

void Board::HandleEvents(SDL_Event &event)
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            HandleClick(event.button.x);
        }
        break;
    case SDL_MOUSEMOTION:
        HandleMouseMove(event.motion.x);
        break;
    }
}

void Board::HandleClick(int XClicked)
{
    if (winner != 0)
    {
        winnerCallback(winner);
        return;
    }

    if (playerTurn != PLAYER_1)
        return;

    int col = GetColumnFromMousePosition(XClicked);

    if (!CheckIfValidCol(col))
        return;

    int row = GetEmptySlotFromCol(col);
    if (row == -1)
        return;

    PlacePlayerDisc(row, col);
    playerTurn = PLAYER_2;
    selectedCol = -1;

    if (CountConsecutive(CONNECT4, PLAYER_1))
    {
        winner = PLAYER_1;
    }
    else
    {
        AiTurn();
    }
}

void Board::HandleMouseMove(int XHovered)
{
    if (winner != 0)
        return;

    int col = GetColumnFromMousePosition(XHovered);

    if (!CheckIfValidCol(col))
    {
        selectedCol = -1;
        return;
    }

    selectedCol = col;
}

int Board::GetEmptySlotFromCol(int col)
{
    for (int row = rows - 1; row >= 0; --row)
    {
        if (grid[row][col] == 0)
            return row;
    }
    return -1;
}

bool Board::CheckIfValidCol(int col) const
{
    return col >= 0 && col < cols;
}

bool Board::CheckIfValidRow(int row) const
{
    return row >= 0 && row < rows;
}

void Board::PlacePlayerDisc(int row, int col)
{
    grid[row][col] = PLAYER_1;
}

void Board::PlaceAiDisc(int row, int col)
{
    grid[row][col] = PLAYER_2;
}

void Board::DrawDisc(int row, int col, SDL_Texture *texture)
{
    if (!texture)
    {
        cerr << "No texture available to draw!" << endl;
        return;
    }

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    SDL_Rect destRect = {
        GetGridLeftX(windowWidth) + col * cellSize,
        GetGridTopY(windowHeight) + row * cellSize,
        cellSize,
        cellSize};

    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

void Board::DrawDiscs()
{
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            switch (grid[row][col])
            {
            case PLAYER_1:
                DrawDisc(row, col, redDiscTexture);
                break;
            case PLAYER_2:
                DrawDisc(row, col, blueDiscTexture);
                break;
            default:
                break;
            }
        }
    }
}

int Board::CountConsecutive(int length, int playerDisc) const
{
    int count = 0;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            if (CheckDirection(row, col, 1, 0, playerDisc, length) ||
                CheckDirection(row, col, -1, 0, playerDisc, length) ||
                CheckDirection(row, col, 0, 1, playerDisc, length) ||
                CheckDirection(row, col, 0, -1, playerDisc, length) ||
                CheckDirection(row, col, 1, 1, playerDisc, length) ||
                CheckDirection(row, col, 1, -1, playerDisc, length) ||
                CheckDirection(row, col, -1, 1, playerDisc, length) ||
                CheckDirection(row, col, -1, -1, playerDisc, length))
            {
                count++;
            }
        }
    }

    return count;
}

bool Board::CheckDirection(int startRow, int startCol, int deltaRow, int deltaCol, int playerDisc, int targetCount) const
{
    int count = 0;

    for (int row = startRow, col = startCol;
         count < targetCount && CheckIfValidRow(row) && CheckIfValidCol(col);
         row += deltaRow, col += deltaCol)
    {
        if (grid[row][col] != playerDisc)
            return false;

        count++;
    }

    return count == targetCount;
}

void Board::AiTurn()
{
    auto [bestMove, bestScore] = Minimax(4, false);

    if (bestMove != -1)
    {
        int row = GetAvailableRow(bestMove);
        PlaceAiDisc(row, bestMove);
        playerTurn = PLAYER_1;

        if (CountConsecutive(CONNECT4, PLAYER_2))
        {
            winner = PLAYER_2;
        }
    }
}

int Board::EvaluateBoard() const
{
    int score = 0;

    score += CountConsecutive(4, PLAYER_1) * 1000;
    score += CountConsecutive(3, PLAYER_1) * 100;
    score += CountConsecutive(2, PLAYER_1) * 10;

    score -= CountConsecutive(4, PLAYER_2) * 1000;
    score -= CountConsecutive(3, PLAYER_2) * 100;
    score -= CountConsecutive(2, PLAYER_2) * 10;

    return score;
}

std::pair<int, int> Board::Minimax(int depth, bool isMaximizing)
{
    int score = EvaluateBoard();

    if (CountConsecutive(4, PLAYER_1) || CountConsecutive(4, PLAYER_2) || IsBoardFull() || depth == 0)
    {
        return {-1, score * (depth + 1)};
    }

    if (isMaximizing)
    {
        return Maximize(depth);
    }
    else
    {
        return Minimize(depth);
    }
}

std::pair<int, int> Board::Maximize(int depth)
{
    int bestScore = -INT_MAX;
    int bestCol = -1;

    for (int col = 0; col < cols; ++col)
    {
        if (IsColumnAvailable(col))
        {
            int row = GetAvailableRow(col);
            PlacePlayerDisc(row, col);
            auto [colResult, eval] = Minimax(depth - 1, false);
            UndoMove(row, col);

            if (eval > bestScore)
            {
                bestScore = eval;
                bestCol = col;
            }
        }
    }

    return {bestCol, bestScore};
}

std::pair<int, int> Board::Minimize(int depth)
{
    int bestScore = INT_MAX;
    int bestCol = -1;

    for (int col = 0; col < cols; ++col)
    {
        if (IsColumnAvailable(col))
        {
            int row = GetAvailableRow(col);
            PlaceAiDisc(row, col);
            auto [colResult, eval] = Minimax(depth - 1, true);
            UndoMove(row, col);

            if (eval < bestScore)
            {
                bestScore = eval;
                bestCol = col;
            }
        }
    }

    return {bestCol, bestScore};
}

bool Board::IsColumnAvailable(int col) const
{
    return grid[0][col] == 0;
}

int Board::GetAvailableRow(int col) const
{
    for (int row = rows - 1; row >= 0; --row)
    {
        if (grid[row][col] == 0)
            return row;
    }

    return -1;
}

void Board::UndoMove(int row, int col)
{
    grid[row][col] = 0;
}

bool Board::IsBoardFull() const
{
    for (int col = 0; col < cols; ++col)
    {
        if (IsColumnAvailable(col))
            return false;
    }
    return true;
}

int Board::GetColumnFromMousePosition(int XClicked) const
{
    int windowWidth;
    SDL_GetRendererOutputSize(renderer, &windowWidth, nullptr);
    int topLeftX = GetGridLeftX(windowWidth);

    return floor(static_cast<double>(XClicked - topLeftX) / cellSize);
}

int Board::GetGridLeftX(int windowWidth) const
{
    return windowWidth / 2 - cols * cellSize / 2;
}

int Board::GetGridTopY(int windowHeight) const
{
    return windowHeight / 2 - rows * cellSize / 2;
}

void Board::ShowGameEndMessage(int winner)
{
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    RenderText("Click anywhere to go the Menu", windowWidth / 2 - 175, 20);

    switch (winner)
    {
    case PLAYER_1:
        RenderText("You win!", 40, windowHeight / 2 - 15);
        RenderText("You win!", 0.875 * windowWidth, windowHeight / 2 - 15);
        break;
    case PLAYER_2:
        RenderText("You lose!", 40, windowHeight / 2 - 15);
        RenderText("You lose!", 0.875 * windowWidth, windowHeight / 2 - 15);
        break;
    default:
        break;
    }
}

void Board::RenderText(const std::string &text, int x, int y)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!textSurface)
    {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!textTexture)
    {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    int textWidth = 0, textHeight = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = {x, y, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
}