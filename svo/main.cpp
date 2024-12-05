#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>
struct SVO {
	int scale;
	SVO* children[2][2];
	int x;
	int y;
	bool has[2][2] = { 0 };
	SVO() {
		this->x = 0;
		this->y = 0;
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				this->has[x][y] = false;
			}
		}
	}
	void Draw() {
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				Rectangle rec;
				float childSize = 800 / pow(2, scale);
				rec.x = this->x + x * childSize;
				rec.y = this->y + y * childSize;
				rec.width = childSize;
				rec.height = childSize;
				if (has[x][y]) DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, BLACK);

				if (CheckCollisionRecs(rec, { float(GetMouseX()), float(GetMouseY()), 1, 1 })) {
					if (IsMouseButtonDown(0) && !this->has[x][y] && scale != 7) {
						this->children[x][y] = new SVO();
						this->children[x][y]->x = rec.x;
						this->children[x][y]->y = rec.y;
						this->children[x][y]->scale = this->scale + 1;
						this->has[x][y] = true;
					}
				}
				if (has[x][y]) {
					children[x][y]->Draw();
				}
			}
		}
	

	}
};
struct LightSource {
	Vector2 pos;
	Vector2 goTo;
	void Setup(int x, int y) {
		this->pos = { float(x),float(y) };
		this->goTo = { float(GetRandomValue(0,800)),float(GetRandomValue(0,800)) };
	}
	void SendRay(SVO svo, int angle) {
		
		float directionalX = sin(angle);
		float directionalY = cos(angle);
		Vector2 ray = this->pos;
		SVO* currentSVO = &svo;
		int finalLength = 0;
		for (int i = 0; i < 100; i++) {
			bool redified = false;
			bool canconitnue = true;
			int scale = 1;
			ray.x += directionalX * 3;
			ray.y += directionalY * 3;
			currentSVO = &svo;
			while (canconitnue && scale < 7) {

				float childSize = 800 / pow(2, scale);
				int hasX = floor((ray.x - currentSVO->x) / childSize);
				int hasY = floor((ray.y - currentSVO->y) / childSize);

				if (!currentSVO->has[hasX][hasY]) {
					redified = true;
					finalLength = i;
					break;
				}
				else {

					currentSVO = currentSVO->children[hasX][hasY];

				}
				scale++;
			}
			if (!redified) {
				DrawRectangle(ray.x, ray.y,3,3, BLACK);
				break;
			}
		}
		DrawLine(this->pos.x, this->pos.y, ray.x, ray.y, RED);
	}
	void Update(SVO svo) {
		this->pos = Vector2MoveTowards(this->pos, this->goTo, 3);
		if (Vector2Distance(this->pos, this->goTo) < 1) {
			this->goTo = { float(GetRandomValue(0,800)),float(GetRandomValue(0,800)) };

		}
		DrawCircle(this->pos.x, this->pos.y, 5, RED);
		for (int i = -360; i < 360; i++) {
			SendRay(svo, i/2);
		}
	}
};
class App {
	SVO svo;
	std::vector<LightSource> sources;
public:
	App() {
		InitWindow(800, 800,"a");
		
		this->svo.scale = 1;
		for (int i = 0; i < 1; i++) {
			LightSource light;
			light.Setup(GetRandomValue(0, 800), GetRandomValue(0, 800));
			sources.push_back(light);
		}
		
	}
	void Run() {
		while (!WindowShouldClose()) {
			BeginDrawing();
			ClearBackground(WHITE);
			svo.Draw();
			this->sources[0].pos = GetMousePosition();
			for (auto &t : this->sources) {
				t.Update(svo);
			}
			DrawFPS(0, 0);
			EndDrawing();
		}
	}
};

void main() {
	App app;
	app.Run();
}