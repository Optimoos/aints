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

    world world;

    entt::registry registry;


    for (int i = 0; i < 10; i++){
        auto entity = registry.create();
        aints ant1;
        //movementNeuron mn(ant1);
        //ant1.addNeuron();
        ant1.setId(i);
        ant1.updateLocation(800, 800);
        registry.emplace<aints>(entity, ant1);
    }

    auto antview = registry.view<aints>();

    for (auto entity: antview){
        auto &anAnt = antview.get<aints>(entity);
        //std::cout << "Ant Entity ID " << anAnt.getId() << std::endl;
    }

    Vector2 camera_position = { 0, 0 };
    Camera2D camera = { 0 };

    camera.target = camera_position;
    camera.zoom = 1.0f;

    InitWindow(screenWidth, screenHeight, "Aints");

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    //--------------------------------------------------------------------------------------
    bool Button000Pressed = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        if (IsKeyDown(KEY_RIGHT)) { if ( (camera_position.x + screenWidth) < world.WORLD_X ) camera_position.x += 1; }
        if (IsKeyDown(KEY_LEFT)) { if ( camera_position.x > 0 ) camera_position.x -= 1; }
        if (IsKeyDown(KEY_UP)) { if ( camera_position.y > 0 ) camera_position.y -= 1; }
        if (IsKeyDown(KEY_DOWN)) { if ( (camera_position.y + screenHeight) < world.WORLD_Y ) camera_position.y += 1; }

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
                    if ((camera_position.y + screenHeight - movement.y) < world.WORLD_Y) camera_position.y -= movement.y;
                }
            } else {
                if ((camera_position.x + screenWidth - movement.x) < world.WORLD_X) camera_position.x -= movement.x;
                if (movement.y > 0) {
                    if ((camera_position.y - movement.y) > 0) camera_position.y -= movement.y;
                } else {
                    if ((camera_position.y + screenHeight - movement.y) < world.WORLD_Y) camera_position.y -= movement.y;
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        BeginMode2D(camera);


        ClearBackground(RAYWHITE);

        for (uint16_t y_pixel = 0; y_pixel < screenHeight; y_pixel++) {
            for (uint16_t x_pixel = 0; x_pixel < screenWidth; x_pixel++) {

                const uint8_t x_tile = std::min((x_pixel + (int)camera_position.x), world.WORLD_X - 1) / world::worldtile::TILE_X;
                const uint8_t y_tile = std::min((y_pixel + (int)camera_position.y), world.WORLD_Y - 1) / world::worldtile::TILE_Y;
                world::worldtile& tile = world.worldtiles[y_tile][x_tile];

                float& blockinfo = tile.blocks[((x_pixel + (int)camera_position.x) % world::worldtile::TILE_X) + (((y_pixel + (int)camera_position.y) % world::worldtile::TILE_Y) * 256)];
                uint8_t greyscale = blockinfo * 255;
                Color color = {greyscale, greyscale, greyscale, 255};
                DrawPixel(x_pixel, y_pixel, color);

//                switch (tile.blocks.at(x_pixel % world::worldtile::TILE_X + ((y_pixel*1600) % world::worldtile::TILE_Y))){
//                    case 0:
//                        DrawPixel(x_pixel, y_pixel, SKYBLUE);
//                        break;
//                    case 1:
//                        DrawPixel(x_pixel, y_pixel, BROWN);
//                        break;
//                    case 2:
//                        DrawPixel(x_pixel, y_pixel, GREEN);
//                        break;
//                    case 3:
//                        DrawPixel(x_pixel, y_pixel, RED);
//                        break;
//                    case 4:
//                        DrawPixel(x_pixel, y_pixel, DARKGRAY);
//                        break;
//                    case 5:
//                        DrawPixel(x_pixel, y_pixel, BLUE);
//                        break;
//                    case 6:
//                        DrawPixel(x_pixel, y_pixel, YELLOW);
//                        break;
//                    case 7:
//                        DrawPixel(x_pixel, y_pixel, DARKBROWN);
//                        break;
//                    case 8:
//                        DrawPixel(x_pixel, y_pixel, PURPLE);
//                        break;
//                }

            }
        }

        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        for (auto entity: antview){
            auto &anAnt = antview.get<aints>(entity);
            anAnt.tick();
            // No need to draw if the ant is not on camera
            if ((camera_position.x < anAnt.getX() < (camera_position.x + screenWidth)) &&
            (camera_position.y < anAnt.getY() < (camera_position.y + screenHeight))) {
                DrawPixel(anAnt.getX() - camera_position.x, anAnt.getY() - camera_position.y, BLACK);
            }
        }

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        //Button000Pressed = GuiButton((Rectangle){ 430, 276, 120, 24 }, "SAMPLE TEXT");
        //----------------------------------------------------------------------------------

        EndMode2D();

        DrawFPS(10,10);
        EndDrawing();
        //----------------------------------------------------------------------------------

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

