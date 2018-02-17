#include <iostream>
#include <SDL.h>

#include <vector>
using std::vector;

#include "XmlReader.h"
using XmlReader::LoadXmlFile;

#include "Matrix.h"
using Geometry::Matrix2;

#include "Color.h"
using Color::HSVAColor;

#include "Renderable.h"
using Geometry::Renderable;

#include "Matrix.h"
using Geometry::Vector2;
using Geometry::Vector3;

#include "Triangle.h"
using Geometry::Triangle;

#include "LineSegment.h"
using Geometry::LineSegment;

#include "Model.h"
using Models::Model;
using Models::Primitive;

#include "Threading.h"
using Threads::Thread;
using Threads::Mutex;
using Threads::LockHold;

#include "Glitching.h"
using RNG::CoinFlip;
using RNG::Perlin;

#include "Font.h"
using Models::Font;

#include <ctime>

static int windowX = 800;
static int windowY = 600;

class Animator : public Thread
{
public:
	Animator(Model& logo, Model& text) : logo(logo), text(text), frameCount(0), done(false), paused(false)
	{
	}

	virtual void StopGraceful()
	{
		done = true;
	}

	bool isDone()
	{
		return done;
	}

	Mutex modelGuard;

	bool paused;

private:
	Model& logo;
	Model& text;
	int frameCount;
	bool done;
	const unsigned long sleepDuration = 1000.0 / 60.0;

	virtual void Execute()
	{
		vector<Primitive*> letterStrokes = text.GetAllPrimitives();
		while (!done)
		{
			clock_t begin = clock();
			if (!paused)
			{
				LockHold hold(modelGuard);

				if (CoinFlip(0.025))
				{
					logo.vertexDistortion = 50;
					logo.hueDistortion = 60;
					text.hueRotation = (frameCount * 25) % 360;
					text.sLerp = 1.0f;
				}
				else
				{
					logo.vertexDistortion = 0;
					logo.hueDistortion *= 0.90;
					text.sLerp = 0.0f;
				}

				int letterState = frameCount % (letterStrokes.size() * 2);
				if (letterState < letterStrokes.size())
				{
					letterStrokes[letterState]->disable = true;
				}
				else
				{
					letterStrokes[letterState - letterStrokes.size()]->disable = false;
				}

				frameCount++;
			}
			clock_t end = clock();

			float timeElapsed = (end - begin) / 1000.0;

			_sleep(sleepDuration - timeElapsed);
		}
	}
};

void ScreenShot(SDL_Renderer* renderer)
{
	static int number = 0;
	//char fileName[] = "..\\screenshots\\screenshot_000.bmp";
	//sprintf_s(fileName, "..\\screenshots\\screenshot_%03d.bmp", number++);
	char fileName[] = "screenshot_000.bmp";
	sprintf_s(fileName, "screenshot_%03d.bmp", number++);

	SDL_Surface *sshot = SDL_CreateRGBSurface(
		0, windowX, windowY, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	SDL_SaveBMP(sshot, fileName);
	SDL_FreeSurface(sshot);
}

int main(int argc, char* argv[])
{
	Vector3 c(0, 0, 100);
	Vector3 nw(-100, -100, 0);
	Vector3 ne(100, -100, 0);
	Vector3 se(100, 100, 0);
	Vector3 sw(-100, 100, 0);

	Model sun;

	HSVAColor sunWhite = HSVAColor(0, 0, 1, 1);
	sun.AddLineSegment("nwse", LineSegment(nw, se), sunWhite);
	sun.AddLineSegment("nesw", LineSegment(ne, sw), sunWhite);
	sun.scale = 0.1;
	sun.offset = Vector2(windowX - 15, windowY - 15);

	Model polybian = Model(LoadXmlFile("polybian.xml"));

	Font playerFont = Font(LoadXmlFile("playerfont.xml"));
	Model pressStart = playerFont.ModelText("PRESS START");

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;

		Animator animator(polybian, pressStart);

		//if (SDL_CreateWindowAndRenderer(windowX, windowY, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer) == 0)
		if (SDL_CreateWindowAndRenderer(windowX, windowY, SDL_WINDOW_BORDERLESS, &window, &renderer) == 0)
		{
			SDL_GetWindowSize(window, &windowX, &windowY);
			polybian.offset = Vector2(windowX / 2, windowY / 2);
			polybian.scale = windowX / 800;
			pressStart.offset = Vector2(windowX / 2, 13 * windowY / 16);
			pressStart.scale = windowX / 50;

			SDL_ShowCursor(SDL_DISABLE);
			bool pause = false;
			int frameCount = 0;
			SDL_bool done = SDL_FALSE;
			animator.Start();
			while (!done)
			{
				SDL_Event event;

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderClear(renderer);

				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				sun.offset = Vector2(mouseX, mouseY);

				// render stuff here
				animator.modelGuard.Lock();
				polybian.Render(renderer, sun.offset);
				pressStart.Render(renderer, sun.offset);
				animator.modelGuard.Unlock();

				if (!animator.isDone())
				{
					//ScreenShot(renderer);
				}

				sun.Render(renderer, Vector2::Origin);

				SDL_RenderPresent(renderer);

				while (SDL_PollEvent(&event)) 
				{
					if (event.type == SDL_QUIT)
					{
						done = SDL_TRUE;
					}
					else if (event.type == SDL_KEYDOWN)
					{
						switch (event.key.keysym.scancode)
						{
						case SDL_SCANCODE_ESCAPE:
							done = SDL_TRUE;
							break;
						case SDL_SCANCODE_P:
							animator.paused = !animator.paused;
							break;
						case SDL_SCANCODE_PRINTSCREEN:
							ScreenShot(renderer);
							break;
						}
					}
				}

				frameCount++;
			}
			animator.StopGraceful();
			animator.Wait();
		}

		if (renderer)
		{
			SDL_DestroyRenderer(renderer);
		}

		if (window)
		{
			SDL_DestroyWindow(window);
		}
	}
	SDL_Quit();
	return 0;
}
