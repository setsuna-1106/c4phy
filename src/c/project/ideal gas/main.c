#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 6400 
#define HEIGHT 4000
#define FPS 60

#define R 1
#define N 10000

Vector2 p[N], v[N];

double momentum;
int count;

void init() {
  srand(time(NULL));
  for (int i = 0; i < N; i++) {
    p[i].x = GetRandomValue(R, WIDTH - R);
    p[i].y = GetRandomValue(R, HEIGHT - R);
    v[i].x = (float)GetRandomValue(-300, 300) * 5;
    v[i].y = (float)GetRandomValue(-300, 300) * 5;
    /*速度分布需要优化*/
  }
  momentum = 0;
  count = 0;
}

void step(double dt) {
  momentum = 0;
  count++;
  for (int i = 0; i < N; i++) {
    p[i].x += v[i].x * dt;
    p[i].y += v[i].y * dt;

    if (p[i].x > WIDTH - R) {
      p[i].x = WIDTH - R;
      momentum += v[i].x;
      v[i].x = -v[i].x;
    }
    if (p[i].x < R) {
      p[i].x = R;
      v[i].x = -v[i].x;
    }
    if (p[i].y > HEIGHT - R) {
      p[i].y = HEIGHT - R;
      v[i].y = -v[i].y;
    }
    if (p[i].y < R) {
      p[i].y = R;
      v[i].y = -v[i].y;
    }
    // 与容器壁碰撞判定
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (j == i)
        continue;
      if (CheckCollisionCircles(p[i], R, p[j], R)) {
        double dx = p[i].x - p[j].x;
        double dy = p[i].y - p[j].y;
        double dist = sqrt(dx * dx + dy * dy);

        // 位置分离
        double overlap = 2.0 * R - dist;
        if (dist > 0.001) {
          double nx = dx / dist, ny = dy / dist;
          p[i].x += nx * overlap * 0.5;
          p[i].y += ny * overlap * 0.5;
          p[j].x -= nx * overlap * 0.5;
          p[j].y -= ny * overlap * 0.5;
        }

        // 速度响应（同 bug 1 修正）
        double dvx = v[i].x - v[j].x;
        double dvy = v[i].y - v[j].y;
        double dot = dvx * dx + dvy * dy;
        double dist2 = dx * dx + dy * dy;

        v[i].x -= (dot / dist2) * dx;
        v[i].y -= (dot / dist2) * dy;
        v[j].x += (dot / dist2) * dx; // ← 注意：+
        v[j].y += (dot / dist2) * dy;
      }
    }
  }
  // 相互碰撞判定
}

void Drawparticles() {
  for (int i = 0; i < N; i++) {
    DrawCircleV(p[i], R, WHITE);
  }
}

int main() {
  FILE *fp = fopen("the_froce.csv", "w");
  // InitWindow(WIDTH, HEIGHT, "ideal gas");
  // SetTargetFPS(FPS);
  init();
  // while (!WindowShouldClose()) {
  //   step((double)1 / FPS);
  //   fprintf(fp, "%d,%lf\n", count,momentum);
  //   ClearBackground(BLACK);

  //   BeginDrawing();
  //   Drawparticles();
  //   EndDrawing();
  // }
  for(int i=0;i<1e3;i++){
    step((double)1 / FPS);
    fprintf(fp, "%d,%lf\n", count,momentum);

  }
  fclose(fp);
  return 0;
}
