#include <iostream>
#include <SDL.h>

#include <vector>
using std::vector;

#include "XmlReader.h"
using XmlReader::LoadXmlFile;

#include "Matrix.h"
using Geometry::Matrix2;

#include "Color.h"
using Models::CompileHSVA;
using Models::DecompileHSVA;

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

const static int windowX = 800;
const static int windowY = 600;

int main(int argc, char* argv[])
{
	Vector3 c(0, 0, 100);
	Vector3 nw(-100, -100, 0);
	Vector3 ne(100, -100, 0);
	Vector3 se(100, 100, 0);
	Vector3 sw(-100, 100, 0);

	Model sun;
	sun.prims.push_back(Primitive(LineSegment(se, nw), CompileHSVA(0, 0, 1, 1)));
	sun.prims.push_back(Primitive(LineSegment(sw, ne), CompileHSVA(0, 0, 1, 1)));
	sun.scale = 0.1;

	Vector3 p0(0, -3, 0);
	Vector3 p1(-3, 0, 0);
	Vector3 p2(0, 7, -2);
	Vector3 p3(0, 3, 0);
	Vector3 p4(3, 0, 0);

	Vector3 pc0(0, -1, 1);
	Vector3 pc1(1, 0, 1);
	Vector3 pc2(0, 1, 1);
	Vector3 pc3(-1, 0, 1);

	SDL_Color pPaint = CompileHSVA(-15, 0.90, 1, 1);
	SDL_Color pLine = CompileHSVA(1, .9, 1, 1);
	SDL_Color pInnerLine = CompileHSVA(1, .9, 1, 0.25);

	Model p;
	p.prims.push_back(Primitive(Triangle(pc0, pc3, p1), pPaint));
	p.prims.push_back(Primitive(Triangle(pc0, p1, p0), pPaint));
	p.prims.push_back(Primitive(Triangle(pc1, pc0, p0), pPaint));
	p.prims.push_back(Primitive(Triangle(pc1, p0, p4), pPaint));
	p.prims.push_back(Primitive(Triangle(pc2, pc1, p4), pPaint));
	p.prims.push_back(Primitive(Triangle(pc2, p4, p3), pPaint));
	p.prims.push_back(Primitive(Triangle(pc3, pc2, p2), pPaint));
	p.prims.push_back(Primitive(Triangle(pc3, p2, p1), pPaint));
	p.prims.push_back(Primitive(LineSegment(p0, p1), pLine));
	p.prims.push_back(Primitive(LineSegment(p1, p2), pLine));
	p.prims.push_back(Primitive(LineSegment(p2, p3), pLine));
	p.prims.push_back(Primitive(LineSegment(p3, p4), pLine));
	p.prims.push_back(Primitive(LineSegment(p4, p0), pLine));
	p.prims.push_back(Primitive(LineSegment(pc0, pc1), pLine));
	p.prims.push_back(Primitive(LineSegment(pc1, pc2), pLine));
	p.prims.push_back(Primitive(LineSegment(pc2, pc3), pLine));
	p.prims.push_back(Primitive(LineSegment(pc3, pc0), pLine));
	p.prims.push_back(Primitive(LineSegment(pc0, p0), pInnerLine));
	p.prims.push_back(Primitive(LineSegment(pc1, p4), pInnerLine));
	p.prims.push_back(Primitive(LineSegment(pc2, p3), pInnerLine));
	p.prims.push_back(Primitive(LineSegment(pc3, p1), pInnerLine));
	p.scale = 30;
	p.offset = Vector2(windowX / 2, windowY / 2);


	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;

		if (SDL_CreateWindowAndRenderer(windowX, windowY, SDL_WINDOW_BORDERLESS, &window, &renderer) == 0)
		{
			SDL_ShowCursor(SDL_DISABLE);
			bool pause = false;
			int frameCount = 0;
			SDL_bool done = SDL_FALSE;
			while (!done)
			{
				SDL_Event event;

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderClear(renderer);

				// render stuff here
				//int x, y;
				//SDL_GetMouseState(&x, &y);
				//sun.offset.x = (cos(frameCount / 377.0) * 300) + 400;
				//sun.offset.y = (sin(frameCount / 377.0) * 300) + 300;
				//sun.offset = Vector2(x, y);
				//pyramid.Render(renderer, sun.offset);
				sun.offset = Vector2(windowX - 15, windowY - 15);
				p.rotation = frameCount;
				p.Render(renderer, sun.offset);
				sun.Render(renderer, Vector2::origin);

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
							pause = !pause;
							break;
						}
					}
				}

				//_sleep(10);

				if (!pause)
				{
					char fileName[] = "screenshots/screenshot_000.bmp";
					sprintf_s(fileName, "screenshots/screenshot_%03d.bmp", frameCount);

					SDL_Surface *sshot = SDL_CreateRGBSurface(
						0, windowX, windowY, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
					SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
					SDL_SaveBMP(sshot, fileName);
					SDL_FreeSurface(sshot);

					frameCount++;
				}

				if (frameCount >= 360)
					done = SDL_TRUE;
			}
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
