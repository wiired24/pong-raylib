#include <iostream>
#include <raylib.h>

using namespace std;

// Global Vars
    int playerScore;
    int cpuScore;

class Ball {
    public:
    float x, y; 
    int speed_x, speed_y;
    int radius;
    Sound ball_bounce;
    
    // Constructor for Ball Class (Needed for loading sound)
    Ball() {
        ball_bounce = LoadSound("ping_pong_8bit_beeep.ogg");
    }

    // Destructor (Needed for unloading sound)
    ~Ball() {
        UnloadSound(ball_bounce);
    }

    void Draw() {
          // Draw the ball on to the game screen
        DrawCircle(x, y, radius, RED);
    }

    void Update() {
        // Increase the x,y cordinates each frame to 
        // so the ball is re-drawn in a new place each frame
        x += speed_x;
        y += speed_y;

        // Y + Radius >= GetScreenHeight(): Explained
        //==============================================================================================================
        // If the y position of the ball + the radius is greater
        // than or equal to the screen height we know that the
        // ball has reached the bottom so lets' reverse it's course
        // by multiplying the y speed by negative 1. Remember the Y Position increases as 
        // we move down. If we hit the bottom on the Y Axis we're at or above 720 also known
        // as the screen height. So ypos + radius >= screenHeight = Ball has reached the bottom. 
        // We include the radius here because we need to check if the edge of the ball has hit bottom
        // and since the x,y position of the ball lays in the center of the ball, we need to know the radius in addition
        // to it's current position to know where the edge of the ball is touching. 

        // Y - Radius <= 0: Explained 
        //===============================================================================================================
        // If the y position on the ball (Where the ball is at on the Y Axis) is less than or equal to 0 we know the ball is touching
        // the top of the screen because the top of the game screen starts at 0. We subtract the radius. Why? Because if we start at the center of the ball
        // and move down to get to the edge we're increasing as we move down so we have to add the radius. However if the ball is at the top
        // of the screen we decrease the radius because we move up to the top of the ball starting at the center. 

        // Putting it all together: 
        // ===============================================================================================================
        // Essentailly this if condition the ball is touching the bottom then bounce. If the ball is touching the top then bounce.

        if(y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
            PlaySound(ball_bounce);
            
            
        }

        // Everything is the same here as the Y logic except this is on the X axis so this applies to left to right instead of up/down. 
        if (x + radius >= GetScreenWidth() || x - radius <= 0) {
            speed_x *= -1;
            PlaySound(ball_bounce);

            // If the x position of the ball is 0 or less we know
            // the ball has reached the edge of the left side of
            // the screen, therefore CPU has scored.
            if (x - radius <= 0) {
                cpuScore++;
            }

            // If the x position of the ball + the radius is greater than or equal to 1280 we know the ball has landed
            // on the right side of the screen and the player scored so let's increase their score. This holds true because
            // the x cordinates start at 0 on the very left (Player Side) moving all the way to 1280 on the far right (Enemy Side) 
            if (x + radius >= 1280) {
                playerScore++;
            }

            
        }
    }
};

class Paddle {
    public:
    float x, y;
    int speed;
    float width, height;

    // Draw the paddle on to the screen at the given x,y position with given width/height
    void Draw() {
        DrawRectangle(x, y, width, height, BLUE);
    }
    
    // Check if the Up key is pressed if it is AND if the y position of the paddle is greater than 0 (Meaning it's not already at the top of the screen)
    // Then we move the paddle up by applying y -1 * speed. Remember we decrease going up so it's go -1 -1 -1 -1 as it's moving up by the speed. 
    void Update() {
        if (IsKeyDown(KEY_UP) && y > 0) {
            y = y -1 * speed;
        }
    
    // Check if the Down key is being pressed AND if the y position of the paddle is less than the screen height - paddle height. We subtract paddle height because
    // oddly enough the paddle orgin is at the top so if we want to check the bottom of the paddle to see if it's hit bottom we need to subtract it's height. If both
    // of these conditions hold true we know we've hit bottom so we can increase y + 1. This will move the paddle upwards by the speed.
        if (IsKeyDown(KEY_DOWN) && y < GetScreenHeight() - height) {
            y = y +1 * speed;
        }

    }
};

class Paddle2 {
    public:
    float x, y;
    int speed;
    float width, height;
    Ball ball;

    void Draw() {
        DrawRectangle(x, y, width, height, BLUE);
    }

    // This is the AI Code for the game. Basically this looks at the Y Position of the paddle + Height of the paddle 
    // we divide by 2 to get the center of the paddle. We then compare the center of the paddle position to the current
    // y position of the ball. If it turns out that the ball has a y position less than the paddle we know the ball is currently above the paddle
    // therefore we need to move the paddle up by decreasing the Y position of the paddle. Remember in Game Programming y decreasing means up. So if an object
    // has a Y Position less than another object you can safely bet the object with the lower number Y Position is above other object. 
    void Update(int ball_y) {
        if (y + height/2 > ball_y) {
            y = y -1 * 5;
        }

    // Here we just do the oppsoite. We again look to see if the center of the paddle y position is less than the balls y position. In the case where the center of the paddle
    // y position is a lower value than the balls y position, we know that we increase as we move down, therefore the ball is below the paddle so we need to move the paddle down
    // by decreasing y. So we add +1 to y times the speed.
        if (y + height/2 <= ball_y) {
            y = y +1 * 5;
        }
    } 
};




int main() 
{
    // Initialize the Game Window
    cout << "Starting the game" << endl;
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const int line_X_Begin = 640;
    const int line_x_End = 640;
    const int line_y_Begin = 0;
    const int line_y_End = 720;
    string lastHitBall;
    InitWindow(screenWidth, screenHeight, "Pong");
    InitAudioDevice();
    SetTargetFPS(60);

    Ball ball;
    Paddle paddle;
    Paddle2 paddle2;
    
    ball.radius = 20;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.speed_x = 8;
    ball.speed_y = 8;
    
    paddle.height = 120;
    paddle.width = 25;
    paddle.x = 10;
    paddle.y = screenHeight / 2 - 60;
    paddle.speed = 10;

    paddle2.width = 25;
    paddle2.height = 120;
    paddle2.x = 1240;
    paddle2.y = screenHeight / 2 - 60;
    paddle2.speed = 10;

    // Run the Game loop as long as the windows hasn't been closed
    while(WindowShouldClose() == false) {
        BeginDrawing();

        // Update the position of the ball
        ball.Update();

        // Update position of paddles as they move
        paddle.Update();
        paddle2.Update(ball.y);

        // Check for collisions on ball hitting player paddle
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle.x, paddle.y, paddle.width, paddle.height})) 
        {
            ball.speed_x *= -1;
            
        }

        // Check for collisons on ball hitting cpu paddle
         if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle2.x, paddle2.y, paddle2.width, paddle2.height})) 
        {
            ball.speed_x *= -1;

        }

        //Reset the screen background to black so that objects
        // being drawn do not leave a trace
        ClearBackground(BLACK);

        // Draw the ball on to the screen, centered
        ball.Draw();

        // Draw the Paddles on to the screen
        paddle.Draw();
        paddle2.Draw();

        // Draw a line down the middle of the screen to seperate player and cpu
        DrawLine(line_X_Begin, line_y_Begin, line_x_End, line_y_End, WHITE);

        // Display how many "Frames Per Second" the game is running at on the top left position of the screen
        DrawFPS(0, 0);

        // Draw Player Score to the screen
        std::string scoreTextPlayer = "Player Score: " + std::to_string(playerScore);
        DrawText(scoreTextPlayer.c_str(), 200, 0, 25, WHITE);

        // Draw CPU Score to the screen
        std::string scoreTextCpu = "Cpu Score: " + std::to_string(cpuScore);
        DrawText(scoreTextCpu.c_str(), 800, 0, 25, WHITE);


        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}