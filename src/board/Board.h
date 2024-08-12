#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <functional>

using namespace std;

const int PLAYER_1 = 1;
const int PLAYER_2 = 2;
const int CONNECT4 = 4;

class Board
{
public:
    Board(SDL_Renderer *renderer);
    ~Board();

    void Render();
    void HandleEvents(SDL_Event &event);
    void HandleClick(int XClicked);
    void HandleMouseMove(int XHovered);

    void SetWinnerCallback(const function<void(int)> &callback);

private:
    void DrawGrid();
    void DrawDiscs();
    void DrawDisc(int row, int col, SDL_Texture *texture);
    void DrawSelectedCol();
    void PlacePlayerDisc(int row, int col);
    void PlaceAiDisc(int row, int col);
    void AiTurn();
    void UndoMove(int row, int col);
    void ShowGameEndMessage(int winner);
    void RenderText(const string &text, int x, int y);

    function<void(int)> winnerCallback;

    int GetEmptySlotFromCol(int col);
    int CountConsecutive(int length, int playerDisc) const;
    int EvaluateBoard() const;
    int GetAvailableRow(int col) const;

    bool CheckIfValidCol(int col) const;
    bool CheckIfValidRow(int row) const;
    bool CheckDirection(int startRow, int startCol, int deltaRow, int deltaCol, int playerDisc, int targetCount) const;
    bool IsColumnAvailable(int col) const;
    bool IsBoardFull() const;

    pair<int, int> Minimax(int depth, bool isMaximizing);
    pair<int, int> Maximize(int depth);
    pair<int, int> Minimize(int depth);

    int GetColumnFromMousePosition(int XClicked) const;
    int GetGridLeftX(int windowWidth) const;
    int GetGridTopY(int windowHeight) const;

    SDL_Renderer *renderer;

    vector<vector<int>> grid;
    int rows, cols, cellSize;
    int playerTurn;
    int selectedCol;
    int winner;

    SDL_Color bgColor, lineColor, player1Color, player2Color;
    SDL_Texture *redDiscTexture;
    SDL_Texture *blueDiscTexture;
    TTF_Font *font;
    SDL_Color textColor;
};

#endif // BOARD_H
