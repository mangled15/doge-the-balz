#include <iostream>
#include <cmath>
#include <raylib.h>
#include <ctime>
#include "resources/gameOver.h"
#include "resources/mainMusic.h"
// ####################################################
// player stuff
// Constructer for the player
struct Player{
    int radius;
    int speed;
    Color color;
    Vector2 playerPosition;
};

// function for putting a player in a variable
Player spawnPlayer(Vector2 pos, float radiuss, Color color)
{
    Player p;
    p.speed = 2.5f;
    p.radius = radiuss;
    p.color = color;
    p.playerPosition = pos;
    return p;
}
// update the players movement
void UpdatePlayer(Player& player, int windowW, int windowH)
{
    // Player movement
    if (IsKeyDown(KEY_UP))    player.playerPosition.y -= player.speed;
    if (IsKeyDown(KEY_DOWN))  player.playerPosition.y += player.speed;
    if (IsKeyDown(KEY_LEFT))  player.playerPosition.x -= player.speed;
    if (IsKeyDown(KEY_RIGHT)) player.playerPosition.x += player.speed;
    // Borders around the window
    if(player.playerPosition.x <= 0.0f + player.radius){
        player.playerPosition.x = 0.0f + player.radius;
    }
    if(player.playerPosition.x >= windowW - player.radius){
        player.playerPosition.x = windowW - player.radius;
    }
    if(player.playerPosition.y <= 0 + player.radius){
        player.playerPosition.y = 0 + player.radius;
    }
    if(player.playerPosition.y >= windowH - player.radius){
        player.playerPosition.y = windowH - player.radius;
    }
}
// Draw the player
void DrawPlayer(const Player& player)
{
    // Draws the player with a position, radius and a color
    DrawCircleV(player.playerPosition, player.radius, player.color);
}

// ####################################################
// enemy stuff
// constuctor for the enemy
struct Enemy{
    int radius;
    Color color;
    Vector2 enemyPosition;
};
bool checkEnemyCollision(Player& player, Enemy& enemy){
    return CheckCollisionCircles(player.playerPosition, player.radius, enemy.enemyPosition, enemy.radius);
}
Enemy spawnEnemy(Vector2 position, int radius, Color color){
    Enemy e;
    e.radius = radius;
    e.color = color;
    e.enemyPosition = position;
    return e;
}
void drawEnemy(const Enemy& enemy){
    DrawCircleV(enemy.enemyPosition, enemy.radius, enemy.color);
}
void updateEnemy(Enemy& enemy, int windowHeight){
    // Move the enemy down
    enemy.enemyPosition.y += 6.0f;
    // Check if the player is outside of the window
    if(enemy.enemyPosition.y >= windowHeight + 30){
        enemy.enemyPosition.y = -30.0f; // puts the enemy on top again
        // generates a random number
        int randomNumber = std::rand() % 7 + 1; // between 10 and 790
        int xPos = randomNumber * 100;

        // sets the enemys position to a random player on the x-axis
        enemy.enemyPosition.x = xPos;
    }
}
// ####################################################

int main(){
    //random number prep
    std::srand(std::time(NULL));

    // GAMEPLAYE STATE
    bool isAlive = true;

    // GAME OVER TIMER
    float timer = 0.0f;

    // A static bool to check if the game over audio has been played before
    static bool playedBefore = false;

    // Window setup
    int windowHeight = 400.0f;
    int windowWidth = 800.0f;
    InitWindow(windowWidth, windowHeight, "Doge the balz");

    // Initialize audio
    InitAudioDevice();

    // setup audio variables
    Wave gameOverWave;
    gameOverWave.frameCount = gameOver_sampleCount;
    gameOverWave.sampleRate = gameOver_sampleRate;
    gameOverWave.sampleSize = 8;
    gameOverWave.channels = 1;
    gameOverWave.data = (void*)gameOver_samples;

    Sound gameOver = LoadSoundFromWave(gameOverWave);
    SetSoundVolume(gameOver, 0.01f);

    Wave musicWave;
    musicWave.frameCount = mainMusic_sampleCount;
    musicWave.sampleRate = mainMusic_sampleRate;
    musicWave.sampleSize = 8;
    musicWave.channels = 1;
    musicWave.data = (void*)mainMusic_samples;

    Sound mainMusic = LoadSoundFromWave(musicWave);

    SetSoundVolume(mainMusic, 0.01f);
    PlaySound(mainMusic);

    // Middle of the screen stuff cus why not
    int middleX = windowWidth / 2;
    int middleY = windowHeight / 2;

    // Center of the window
    Vector2 center = {(float)windowWidth / 2, (float)windowHeight / 2};

    // Enemy start position
    Vector2 enemyStartPosition = {-100, 100};

    // Give the Player strucs params(properties like stats)
    Player player = spawnPlayer(center, 30.0f, BLUE);

    // Give the enemys params
    Enemy enemy1 = spawnEnemy(enemyStartPosition, 20.0f, RED);
    Enemy enemy2 = spawnEnemy(enemyStartPosition, 20.0f, ORANGE);
    Enemy enemy3 = spawnEnemy(enemyStartPosition, 20.0f, PURPLE);
    Enemy enemy4 = spawnEnemy(enemyStartPosition, 20.0f, PINK);

    // Sets the target fps(duhh!!)
    SetTargetFPS(60);
    // Update loop
    while(!WindowShouldClose() && timer < 4.0f){
        //UpdateMusicStream(mainMusic);

        // check for collision and if so shows a game over screen
        if (checkEnemyCollision(player, enemy1) == true){
            isAlive = false;
        }
        if (checkEnemyCollision(player, enemy2) == true){
            isAlive = false;
        }
        if (checkEnemyCollision(player, enemy3) == true){
            isAlive = false;
        }
        if (checkEnemyCollision(player, enemy4) == true){
            isAlive = false;
        }
        // if the player is alive this loop will run
        if (isAlive == true){
            // Update the players movement and logic
            UpdatePlayer(player, windowWidth, windowHeight);

            // Update the enemys position and logic
            updateEnemy(enemy1, windowHeight);
            updateEnemy(enemy2, windowHeight);
            updateEnemy(enemy3, windowHeight);
            updateEnemy(enemy4, windowHeight);

            // #########################################################

            // Start putting stuff on the screen
            BeginDrawing();
            DrawText("I dont know why the audio is so bad on god not my intention", 10, 10, 20, BLACK);
            // Clears the image of the previous frame
            ClearBackground(RAYWHITE);

            // Makes the player appear on the screen
            DrawPlayer(player);

            // Makes the enemy appear on the screen
            drawEnemy(enemy1);
            drawEnemy(enemy2);
            drawEnemy(enemy3);
            drawEnemy(enemy4);

            // Stops Drawing
            EndDrawing();
        }

        if(!isAlive){

            if (!playedBefore){
                playedBefore = true;
                PlaySound(gameOver);
                StopSound(mainMusic);
            }
            // Counts for 3 seconds and then closes the game
            timer += GetFrameTime();

            // Draws the "GAME OVER" on the screen
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("I dont know why the audio is so bad on god not my intention", 10, 10, 20, BLACK);


            // Gets the width of "GAME OVER" with a font size of 24
            int textWidth = MeasureText("GAME OVER", 24);

            // Draws "GAME OVER" in the middle of the screen
            DrawText("GAME OVER", middleX - textWidth / 2, middleY - 24/ 2, 24, RED);
            EndDrawing();
        }

    }
    // Closes the window if WindowShouldClose() == true or if timer => 3.0f
    StopSound(mainMusic);
    UnloadSound(mainMusic);

    UnloadSound(gameOver);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}