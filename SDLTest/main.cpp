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

#include "Perturbation.h"

static int windowX = 800;
static int windowY = 600;

class Animator : public Thread
{
public:
	Animator(Model& glyph) : glyph(glyph), frameCount(0), done(false), paused(false)
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

	Mutex glyphGuard;

	bool paused;

private:
	Model& glyph;
	int frameCount;
	bool done;
	const unsigned long sleepDuration = 1000.0 / 60.0;

	float pvalue()
	{
		const int sinPeriod = 360;
		const int flatPeriod = 180;
		const int cosPeriod = 180;

		int modFrames = frameCount % (cosPeriod + flatPeriod + sinPeriod);

		if (modFrames < sinPeriod)
		{
			return 1.0f - abs(sin(modFrames * (M_PI / 360.0)));
		}
		else if (modFrames < (sinPeriod + flatPeriod))
		{
			return 0;
		}
		else
		{
			return 1.0f - abs(sin(modFrames * (M_PI / 360.0)));
		}
	}

	virtual void Execute()
	{
		while (!done)
		{
			if (!paused)
			{
				LockHold hold(glyphGuard);

				//glyph.rotation = frameCount;
				//glyph.huePerturbation = pvalue() * 45;
				//glyph.vertexPerturbation = pvalue() * 25;

				if (Perturb::PerturbPoisson(1) == 4)
					glyph.vertexPerturbation = 25;
				else
					glyph.vertexPerturbation = 0;

				frameCount++;
			}

			_sleep(sleepDuration);
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

static Perturb::PerturbPerlin perlin = Perturb::PerturbPerlin(25);

void filledRectRGBA(SDL_Renderer* renderer, HSVAColor color, int x, int y, int w, int h)
{
	SDL_Color comp = color.Compile();
	Vector2 topleft = perlin.PerturbVector(Vector2(x, y));
	Vector2 topright = perlin.PerturbVector(Vector2(x + w, y));
	Vector2 bottomleft = perlin.PerturbVector(Vector2(x, y + h));
	Vector2 bottomright = perlin.PerturbVector(Vector2(x + w, y + h));

	filledTrigonRGBA(
		renderer,
		topleft.x, topleft.y,
		bottomleft.x, bottomleft.y,
		topright.x, topright.y,
		comp.r, comp.g, comp.b, comp.a);

	filledTrigonRGBA(
		renderer,
		bottomright.x, bottomright.y,
		topright.x, topright.y,
		bottomleft.x, bottomleft.y,
		comp.r, comp.g, comp.b, comp.a);
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

	Model polybian;
	polybian.LoadXml(LoadXmlFile("polybian.xml"));
	//polybian.scale = 2;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		Animator animator(polybian);

		if (SDL_CreateWindowAndRenderer(windowX, windowY, SDL_WINDOW_BORDERLESS, &window, &renderer) == 0)
		{
			SDL_GetWindowSize(window, &windowX, &windowY);
			polybian.offset = Vector2(windowX / 2, windowY / 2);
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

				/*HSVAColor colorList[] = {
					HSVAColor::Red,
					HSVAColor::Yellow,
					HSVAColor::Green,
					HSVAColor::Blue,
					HSVAColor::Cyan,
					HSVAColor::Magenta,
					HSVAColor::White,
					HSVAColor::Black,
					HSVAColor::DkRed,
					HSVAColor::DkYellow,
					HSVAColor::DkGreen,
					HSVAColor::DkBlue,
					HSVAColor::DkCyan,
					HSVAColor::DkMagenta,
					HSVAColor::DkWhite,
					HSVAColor::DkBlack,
				};

				perlin = Perturb::PerturbPerlin(5);
				for (int idx = 0; idx < 48; idx++)
				{
					filledRectRGBA(
						renderer,
						colorList[idx % 16], 
						(idx % 8) * 100,
						(idx / 8) * 100,
						100, 100);
				}

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				for (int x = 0; x < windowX; x += 25)
				{
					for (int y = 0; y < windowY; y += 25)
					{
						Vector2 offset = perlin.PerturbVector(Vector2(x, y));
						SDL_RenderDrawLine(renderer, x, y, offset.x, offset.y);
					}
				}*/

				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				sun.offset = Vector2(mouseX, mouseY);

				// render stuff here
				animator.glyphGuard.Lock();
				polybian.Render(renderer, sun.offset);
				animator.glyphGuard.Unlock();

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
