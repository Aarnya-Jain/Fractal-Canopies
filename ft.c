#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>



#define WIDTH 950
#define HEIGHT 650

void DrawTree(double leftAngle , double rightAngle, double ratio) {

}

void DrawBranch(Vector2 start,float length,float angle,float thickness,float ratio,float branch_angle) {
  if(length < 1) return;
  float x_end = start.x + sinf(angle)*length;
  float y_end = start.y - cosf(angle)*length;
  Vector2 end = {x_end,y_end};

  float new_length = length*ratio;
  float new_thickness = thickness*ratio;
  float new_angle_l = angle + branch_angle;
  float new_angle_r = angle - branch_angle;
  DrawLineEx(start,end,thickness,RAYWHITE);
  DrawBranch(end,new_length,new_angle_l,new_thickness,ratio,branch_angle);
  DrawBranch(end,new_length,new_angle_r,new_thickness,ratio,branch_angle);
}

int main(int argc,char *argv[]) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WIDTH, HEIGHT, "Fractal Tree Visualiser");

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x181818FF);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x2A2A2AFF);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x3A3A3AFF);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x505050FF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x808080FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xE0E0E0FF);

    GuiSetStyle(SLIDER, SLIDER_WIDTH, 8);
    GuiSetStyle(SLIDER, BORDER_WIDTH, 1);

    float length = 250.0f;
    float thickness = 20.0f;
    float ratio = 0.7f;
    float angleDeg = 20.0f;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            GuiLabel((Rectangle){20, 20, 200, 20}, "Tree Parameters");

            GuiSliderBar(
                (Rectangle){20, 60, 200, 20},
                "Length", TextFormat("%.1f", length),
                &length, 50.0f, 350.0f
            );

            GuiSliderBar(
                (Rectangle){20, 90, 200, 20},
                "Thickness", TextFormat("%.1f", thickness),
                &thickness, 1.0f, 30.0f
            );

            GuiSliderBar(
                (Rectangle){20, 120, 200, 20},
                "Ratio", TextFormat("%.2f", ratio),
                &ratio, 0.5f, 0.75f
            );

            GuiSliderBar(
                (Rectangle){20, 150, 200, 20},
                "Angle", TextFormat("%.1f", angleDeg),
                &angleDeg, 0.0f, 180.0f
            );

            Vector2 start = {WIDTH/2,HEIGHT-20};
            DrawBranch(start,length,0,thickness,ratio,(angleDeg)*DEG2RAD);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}