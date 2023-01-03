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

    uint16_t camera_x = screenWidth * 0.5;
    uint16_t camera_y = screenHeight * 0.5;

    InitWindow(screenWidth, screenHeight, "Aints");

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    //--------------------------------------------------------------------------------------
    bool Button000Pressed = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int ypos = 0; ypos < 900; ypos++) {
            for (int xpos = 0; xpos < 1600; xpos++) {

                std::vector<world::worldtile>& tile_column = world.worldtiles.at(xpos/world::worldtile::TILE_X);

                world::worldtile& tile = tile_column.at(ypos/world::worldtile::TILE_Y);


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
            DrawPixel(anAnt.getX(), anAnt.getY(), BLACK);
        }

        //DrawPixel(ant1.getX(), ant1.getY(), BLACK );
        //int64_t newAntX = ant1.getX() - 1;
        //ant1.updateLocation(newAntX, ant1.getY());


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

