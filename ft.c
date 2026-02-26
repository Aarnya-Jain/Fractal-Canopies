#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 950
#define HEIGHT 650
#define BRANCH_ANGLE 20

void DrawTree(double leftAngle , double rightAngle, double ratio) {

}

void DrawBranch(Vector2 start,float length,float angle,float thickness) {
  if(length < 1) return;
  float x_end = start.x + sinf(angle)*length;
  float y_end = start.y - cosf(angle)*length;
  Vector2 end = {x_end,y_end};

  float new_length = length*0.7;
  float new_thickness = thickness*0.7;
  float new_angle_l = angle + BRANCH_ANGLE;
  float new_angle_r = angle - BRANCH_ANGLE;
  DrawLineEx(start,end,thickness,BLACK);
  DrawBranch(end,new_length,new_angle_l,new_thickness);
  DrawBranch(end,new_length,new_angle_r,new_thickness);
}

int main(int argc,char *argv[]) {
  InitWindow(WIDTH, HEIGHT, "Fractal Tree Visualiser");

    SetTargetFPS(1); // Setting to 1 because only loading the generated tree once each time
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RED);
            float length = 250;
            Vector2 start = {WIDTH/2,HEIGHT-20};
            DrawBranch(start,length,0,20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}