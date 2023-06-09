#include <future>

#include "aints_types.h"
#include "raylib.h"
#include "BS_thread_pool.hpp"

#define RAYGUI_IMPLEMENTATION
// #define RAYGUI_CUSTOM_ICONS     // It requires providing gui_icons.h in the
// same directory #include "gui_icons.h"          // External icons data
// provided, it can be generated with rGuiIcons tool
#include <cmath>
#include <entt/entt.hpp>
#include <iostream>
#include <string>

#include "aints.h"
#include "raygui.h"


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth= 1600;
  const int screenHeight= 900;

  InitWindow(screenWidth, screenHeight, "Aints");

  auto world= std::make_shared<World>();


  entt::registry registry;

  BS::thread_pool ant_pool;



  bool paused{false};
  // FIXME: This should default false before release
  bool debug{true};

  for (int i= 0; i < 1; i++)
  {
    auto entity= registry.create();
    auto *ant1= new aints(world);
    ant1->setId(i);
    ant1->updateLocation(4000, 400);
    ant1->tn.ConnectNeuron(ant1->cn);
    ant1->tn.ConnectNeuron(ant1->taskn);
    ant1->cn.ConnectNeuron(ant1->mn);
    ant1->tn.ConnectNeuron(ant1->fn);
    ant1->tn.ConnectNeuron(ant1->gn);
    ant1->mn.ConnectNeuron(ant1->dan);
    registry.emplace<aints *>(entity, ant1);
  }

  auto antview= registry.view<aints *>();

  for (auto entity : antview)
  {
    auto &anAnt= antview.get<aints *>(entity);
  }

  auto food1= PosXY{4000, 500};
  auto food2= PosXY{3500, 400};

  world->AddFood(food1, 5);
//  world->AddFood(food2, 20);

  // Initial position should be midway through world on X and at the top of Y
  Vector2 camera_position= {(float)kWorldX / 2, (float)screenHeight/2 };

  Camera2D camera= {0};

  camera.target= camera_position;
  camera.offset= (Vector2){screenWidth * 0.5f, screenHeight * 0.5f};
  camera.zoom= 1.0f;

  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

  // Main game loop
  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------

    if (IsKeyDown(KEY_RIGHT))
    {
      if ((camera_position.x + screenWidth) < kWorldX) camera_position.x+= 1;
    }
    if (IsKeyDown(KEY_LEFT))
    {
      if (camera_position.x > 0) camera_position.x-= 1;
    }
    if (IsKeyDown(KEY_UP))
    {
      if (camera_position.y > 0) camera_position.y-= 1;
    }
    if (IsKeyDown(KEY_DOWN))
    {
      if ((camera_position.y + screenHeight) < kWorldY) camera_position.y+= 1;
    }
    if (IsKeyReleased(KEY_P))
    {
      paused= !paused;
    }
    if (IsKeyReleased(KEY_D))
    {
      debug= !debug;
    }

    // FIXME: Camera zoom should center on mouse position
    camera.zoom+= ((float)GetMouseWheelMove() * 0.1f);

    if (camera.zoom > 3.0f)
      camera.zoom= 3.0f;
    else if (camera.zoom < 1.0f)
      camera.zoom= 1.0f;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
      Vector2 movement= GetMouseDelta();
      camera_position.x-= movement.x;
      camera_position.y-= movement.y;

      // FIXME: Cannot push the camera to edges of the world with camera zoomed in
      if (camera_position.x <= camera.offset.x ) {camera_position.x= camera.offset.x; }
      else if (camera_position.x >= (kWorldX - camera.offset.x)) { camera_position.x= kWorldX - camera.offset.x; }
      else { camera_position.x-= movement.x; }
      if (camera_position.y <= camera.offset.y) { camera_position.y= camera.offset.y; }
      else if (camera_position.y >= (kWorldY - camera.offset.y)) { camera_position.y= kWorldY - camera.offset.y; }
      else { camera_position.y-= movement.y; }
    }

    camera.target= camera_position;
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    BeginMode2D(camera);

    ClearBackground(RAYWHITE);

    for (uint16_t y_tile= 0; y_tile < WorldTileRatioY; y_tile++)
    {
      for (uint16_t x_tile= 0; x_tile < WorldTileRatioX; x_tile++)
      {
        PosXY pos= PosXY{x_tile, y_tile};
        auto tile= world->GetTile(pos.TilePosToInt());
        DrawTexture(*tile->TileTexture(), x_tile * kTileX, y_tile * kTileY,
                    WHITE);
      }
    }

    Brain::TaskType debug_task_record{};
    PosXY debug_ant_location{};
    PosXY debug_destination{};
    uint64_t debug_ant_path_length{};
    BlockTypes debug_block_type{};
    PosXY debug_sensed_food{};
    for (auto entity : antview)
    {
      auto &anAnt= antview.get<aints *>(entity);
      if (!paused)
      {
        anAnt->tick();
      }
      if (debug)
      {
        debug_ant_location= anAnt->GetPosition();
        debug_destination= anAnt->brain.current_destination.position;
        debug_task_record= anAnt->brain.current_task;
        debug_ant_path_length= anAnt->brain.path_to_target.size();
        debug_block_type= anAnt->brain.carrying;
        debug_sensed_food= anAnt->brain.sensed_food.position;
      }
      // No need to draw if the ant is not on camera
      if ((camera_position.x < anAnt->getX() <
           (camera_position.x + screenWidth)) &&
          (camera_position.y < anAnt->getY() <
           (camera_position.y + screenHeight)))
      {
        // DrawPixel(anAnt->getX(), anAnt->getY(), BLACK);
        DrawRectangle(anAnt->getX(), anAnt->getY(), 2, 2, BLACK);
      }
    }
    // raygui: controls drawing
    //----------------------------------------------------------------------------------
    // Button000Pressed = GuiButton((Rectangle){ 430, 276, 120, 24 }, "SAMPLE
    // TEXT");
    //----------------------------------------------------------------------------------

    EndMode2D();

    DrawFPS(10, 10);
    if (debug)
    {
      std::string debug_text=
          std::string("Cam X: ") + std::to_string(camera_position.x) +
          std::string("; Y: ") + std::to_string(camera_position.y) +
          std::string(" Action: ") + std::to_string(debug_task_record) +
          std::string(" Position: ") + std::to_string(debug_ant_location.x) +
          std::string(", ") + std::to_string(debug_ant_location.y) +
          std::string(" Destination: ") + std::to_string(debug_destination.x) +
          std::string(", ") + std::to_string(debug_destination.y) +
          std::string(" Sensed Food: ") + std::to_string(debug_sensed_food.x) +
          std::string(", ") + std::to_string(debug_sensed_food.y) +
          std::string(" Path Length: ") +
          std::to_string(debug_ant_path_length) + std::string(" Block Type: ") +
          std::to_string(debug_block_type);
      DrawText(debug_text.c_str(), 10, 40, 10, RED);
    }
    EndDrawing();

    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  for (auto entity : antview)
  {
    auto &anAnt= antview.get<aints *>(entity);
    delete anAnt;
  }
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
