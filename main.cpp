#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
//#define RAYGUI_CUSTOM_ICONS     // It requires providing gui_icons.h in the same directory
//#include "gui_icons.h"          // External icons data provided, it can be generated with rGuiIcons tool
#include "raygui.h"
#include "aints.h"
#include <entt/entt.hpp>
#include <string>
#include <iostream>
#include <cmath>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Aints");

    World world;

    entt::registry registry;


    for (int i = 0; i < 10; i++){
        auto entity = registry.create();
        aints ant1(&world);
        //movementNeuron mn(ant1);
        //ant1.addNeuron();
        ant1.setId(i);
        ant1.updateLocation(4000, 400);
        registry.emplace<aints>(entity, ant1);
    }

    auto antview = registry.view<aints>();

    for (auto entity: antview){
        auto &anAnt = antview.get<aints>(entity);
    }

    // Initial position should be midway through world on X and at the top of Y
    Vector2 camera_position = { static_cast<float>((world.kWorldX/2) - (screenWidth/2)) , 0 };
    Camera2D camera = { 0 };

    camera.target = camera_position;
    camera.offset = (Vector2) {0,0};
    camera.zoom = 1.0f;

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    //--------------------------------------------------------------------------------------
    bool Button000Pressed = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        if (IsKeyDown(KEY_RIGHT)) { if ( (camera_position.x + screenWidth) < world.kWorldX ) camera_position.x += 1; }
        if (IsKeyDown(KEY_LEFT)) { if ( camera_position.x > 0 ) camera_position.x -= 1; }
        if (IsKeyDown(KEY_UP)) { if ( camera_position.y > 0 ) camera_position.y -= 1; }
        if (IsKeyDown(KEY_DOWN)) { if ( (camera_position.y + screenHeight) < world.kWorldY ) camera_position.y += 1; }

        camera.zoom += ((float)GetMouseWheelMove()*0.05f);

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 movement = GetMouseDelta();
            if (movement.x > 0) {
                if ((camera_position.x - movement.x) > 0) camera_position.x -= movement.x;
                if (movement.y > 0) {
                    if ((camera_position.y - movement.y) > 0) camera_position.y -= movement.y;
                } else {
                    if ((camera_position.y + screenHeight - movement.y) < world.kWorldY) camera_position.y -= movement.y;
                }
            } else {
                if ((camera_position.x + screenWidth - movement.x) < world.kWorldX) camera_position.x -= movement.x;
                if (movement.y > 0) {
                    if ((camera_position.y - movement.y) > 0) camera_position.y -= movement.y;
                } else {
                    if ((camera_position.y + screenHeight - movement.y) < world.kWorldY) camera_position.y -= movement.y;
                }
            }
        }

        camera.target = camera_position;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        BeginMode2D(camera);


        ClearBackground(RAYWHITE);

        for (uint16_t y_tile = 0; y_tile < (world.kWorldY / World::Tile::kTileY); y_tile++) {
            for (uint16_t x_tile = 0; x_tile < (world.kWorldX / World::Tile::kTileX); x_tile++) {
                World::Tile& tile = world.world_tiles_[y_tile][x_tile];
                DrawTexture(tile.tile_texture_, (x_tile * World::Tile::kTileX), (y_tile * World::Tile::kTileY), WHITE);
            }
        }

        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        for (auto entity: antview){
            auto &anAnt = antview.get<aints>(entity);
            anAnt.tick();
            // No need to draw if the ant is not on camera
            if ((camera_position.x < anAnt.getX() < (camera_position.x + screenWidth)) &&
            (camera_position.y < anAnt.getY() < (camera_position.y + screenHeight))) {
                //DrawPixel(anAnt.getX(), anAnt.getY(), BLACK);
                DrawRectangle(anAnt.getX(), anAnt.getY(), 2, 2, BLACK);
            }
        }

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        //Button000Pressed = GuiButton((Rectangle){ 430, 276, 120, 24 }, "SAMPLE TEXT");
        //----------------------------------------------------------------------------------

        EndMode2D();

        DrawFPS(10,10);
        std::string debug_text = std::string("Cam X: ") + std::to_string(camera_position.x) + std::string("; Y: ") + std::to_string(camera_position.y);
        DrawText(debug_text.c_str(), 10, 40, 10, RED);
        EndDrawing();
        //----------------------------------------------------------------------------------

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

