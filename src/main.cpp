#include "game/Game.h"

int main(int argc, char *argv[])
{
    Game game;
    if (!game.Init())
    {
        return 1;
    }
    game.Run();
    return 0;
}
