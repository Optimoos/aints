#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
//#define RAYGUI_CUSTOM_ICONS     // It requires providing gui_icons.h in the same directory
//#include "gui_icons.h"          // External icons data provided, it can be generated with rGuiIcons tool
#include "raygui.h"
#include "aints.h"
#include <entt/entt.hpp>
#include <string>
#include <iostream>

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

    uint32_t camera_x = 0;
    uint32_t camera_y = 0;

    InitWindow(screenWidth, screenHeight, "Aints");

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    //--------------------------------------------------------------------------------------
    bool Button000Pressed = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        if (IsKeyDown(KEY_RIGHT)) { if ( (camera_x + screenWidth) < world.WORLD_X ) camera_x += 1; }
        if (IsKeyDown(KEY_LEFT)) { if ( camera_x > 0 ) camera_x -= 1; }
        if (IsKeyDown(KEY_UP)) { if ( camera_y > 0 ) camera_y -= 1; }
        if (IsKeyDown(KEY_DOWN)) { if ( (camera_y + screenHeight) < world.WORLD_Y ) camera_y += 1; }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 movement = GetMouseDelta();
            if (movement.x > 0) {
                if ((camera_x - movement.x) > 0) camera_x -= movement.x;
                if (movement.y > 0) {
                    if ((camera_y - movement.y) > 0) camera_y -= movement.y;
                } else {
                    if ((camera_y + screenHeight - movement.y) < world.WORLD_Y) camera_y -= movement.y;
                }
            } else {
                if ((camera_x + screenWidth - movement.x) < world.WORLD_X) camera_x -= movement.x;
                if (movement.y > 0) {
                    if ((camera_y - movement.y) > 0) camera_y -= movement.y;
                } else {
                    if ((camera_y + screenHeight - movement.y) < world.WORLD_Y) camera_y -= movement.y;
                }
            }

        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int ypos = 0; ypos < screenHeight; ypos++) {
            for (int xpos = 0; xpos < screenWidth; xpos++) {

                std::vector<world::worldtile>& tile_column = world.worldtiles.at((xpos + camera_x)/world::worldtile::TILE_X);

                world::worldtile& tile = tile_column.at((ypos + camera_y)/world::worldtile::TILE_Y);


//                uint32_t calculated_position = xpos+(ypos*1600);
//
//                uint8_t greyscale = world.noise_output.at(calculated_position) * 255;
//                uint8_t alpha = 255;
//                Color color = {greyscale, greyscale, greyscale, alpha};
//
//                DrawPixel(xpos, ypos, color);

//                switch (world.get_world_position(calculated_position)){
                switch (tile.blocks.at(xpos % world::worldtile::TILE_X + ((ypos*1600) % world::worldtile::TILE_Y))){
                    case 0:
                        DrawPixel(xpos, ypos, SKYBLUE);
                        break;
                    case 1:
                        DrawPixel(xpos, ypos, BROWN);
                        break;
                    case 2:
                        DrawPixel(xpos, ypos, GREEN);
                        break;
                    case 3:
                        DrawPixel(xpos, ypos, RED);
                        break;
                    case 4:
                        DrawPixel(xpos, ypos, DARKGRAY);
                        break;
                    case 5:
                        DrawPixel(xpos, ypos, BLUE);
                        break;
                    case 6:
                        DrawPixel(xpos, ypos, YELLOW);
                        break;
                    case 7:
                        DrawPixel(xpos, ypos, DARKBROWN);
                        break;
                    case 8:
                        DrawPixel(xpos, ypos, PURPLE);
                        break;
                }

            }
        }

        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        for (auto entity: antview){
            auto &anAnt = antview.get<aints>(entity);
            anAnt.tick();
            // No need to draw if the ant is not on camera
            if ((camera_x < anAnt.getX() < (camera_x + screenWidth)) &&
            (camera_y < anAnt.getY() < (camera_y + screenHeight))) {
                DrawPixel(anAnt.getX() - camera_x, anAnt.getY() - camera_y, BLACK);
            }
        }

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        //Button000Pressed = GuiButton((Rectangle){ 430, 276, 120, 24 }, "SAMPLE TEXT");
        //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

