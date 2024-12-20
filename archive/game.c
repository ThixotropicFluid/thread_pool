
#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "math.h"


#include "thread_pool/thread_pool.h"


#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)

#define MAX_ENEMIES 100

#define WINDOW_TITLE "Window title"


typedef struct {
    Vector2 player_position;
    float player_rotaion;
    int player_health;
    int invc;
    int invc_counter;
} Player;
typedef enum {
    playing, // update enemies 
    paused, // wait for unpause
    dead // wait for restart
} GameStatus;
typedef struct {
    float player_speed;
    int invc_max;
    int enemy_spawn_time;
} Settings;
typedef struct {
    int exists;
    Vector2 target;
    float speed;
    float health;
    Vector2 position;
    float size;
    Color color;
} Enemy;
typedef struct {
    GameStatus game_status;
    Settings settings;
    Player player;
    Enemy enemies[MAX_ENEMIES];
    int enemy_count;
    int enemy_spawn_timer;
} GameState;


static const GameState GameStateDefault = {
    .game_status = paused,
    .settings = {
        .player_speed = 2.0,
        .invc_max = 30,
        .enemy_spawn_time = 50,
    },
    .player = {
        .player_position.x = SCREEN_WIDTH / 2,
        .player_position.y = SCREEN_HEIGHT / 2,
        .player_rotaion = 0,
        .player_health = 3,
        .invc = 0,
        .invc_counter = 0,
        
    },
    .enemy_count = 0,
    .enemy_spawn_timer = 0,
};

int main(void)
{

    srand(0);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    thread_test(12);
    

    GameState game_state = GameStateDefault;

    Texture2D texture = LoadTexture("assets/test.png"); // Check README.md for how this works

    for (int i = 0; i < MAX_ENEMIES; i++) {
        game_state.enemies[i].exists = 0;
    }
    
    

    game_state.enemy_count = 1;

    game_state.enemies[0].color = RED;
    game_state.enemies[0].position.x = 100;
    game_state.enemies[0].position.y = 100;
    game_state.enemies[0].size = 10;
    game_state.enemies[0].exists = 1;


    while (!WindowShouldClose())
    {
        // update state before draw command

        // player movement
        {
            //determine velocity vector for player 
            Vector2 velocity = { 0, 0 };
            if (IsKeyDown(KEY_D)) {
                velocity.x += 1;
            }
            if (IsKeyDown(KEY_A)) {
                velocity.x += -1;
            }
            if (IsKeyDown(KEY_W)) {
                velocity.y += 1;
            }
            if (IsKeyDown(KEY_S)) {
                velocity.y += -1;
            }
            velocity = Vector2Normalize(velocity);

            game_state.player.player_position.x += velocity.x * game_state.settings.player_speed;
            game_state.player.player_position.y += velocity.y * game_state.settings.player_speed;
        }
        // enemy movement 
        {
            
        }
        // enemy spawining
        {
            if (game_state.enemy_spawn_timer++ > game_state.settings.enemy_spawn_time) {
                game_state.enemy_spawn_timer = 0;
                for (int i = 0; i < MAX_ENEMIES; i++) {
                    if (game_state.enemies[i].exists != 0) continue; // look for open spot
                    game_state.enemies[i].exists = 1;
                    game_state.enemies[i].position.x = 150 + rand() % (100 + 1);
                    game_state.enemies[i].position.y = 150 + rand() % (100 + 1);
                    game_state.enemies[i].size = 10;
                    game_state.enemies[i].color = BLUE;
                    game_state.enemies[i].health = 5;
                    break;
                }
                
            }
        }

        // colision detection
        {
            if (game_state.player.invc) {
                if (game_state.player.invc_counter++ > game_state.settings.invc_max) {
                    game_state.player.invc_counter = 0;
                    game_state.player.invc = 0;
                }
            }
            else {

                for (int i = 0; i < MAX_ENEMIES; i++) {
                    if (game_state.enemies[i].exists == 0) continue; // fuckass tombstoneing cuz i'm lazy
                    float dist = Vector2Distance(game_state.player.player_position, game_state.enemies[i].position);
                    if (dist < (10 + game_state.enemies[i].size)) {
                        game_state.enemies[i].exists = 0;
                        game_state.enemy_count = 0;
                        game_state.player.player_health -= 1;
                        game_state.player.invc = 1;
                    }
                }
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        // draw player
        DrawCircle(game_state.player.player_position.x, SCREEN_HEIGHT - game_state.player.player_position.y, 10, game_state.player.invc ? RED : GREEN);

        // draw enemies
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (game_state.enemies[i].exists == 0) continue;
            DrawCircle(game_state.enemies[i].position.x, SCREEN_HEIGHT - game_state.enemies[i].position.y, game_state.enemies[i].size, game_state.enemies[i].color);
        }
        
        
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
