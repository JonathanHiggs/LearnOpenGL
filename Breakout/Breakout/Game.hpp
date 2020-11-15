#pragma once


#include <Breakout/SpriteRenderer.hpp>


namespace Breakout
{

    enum class GameState
    {
        Active,
        Menu,
        Win
    };

    class Game
    {
    public:
        GameState State;
        bool Keys[2014];
        unsigned int Width, Height;

        Game(unsigned int width, unsigned int height);
        ~Game();

        void Init();
        void ProcessInput(float deltaTime);
        void Update(float deltaTime);
        void Render();

    private:
        SpriteRenderer * renderer;
    };

}