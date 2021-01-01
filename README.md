# G-W_Fire-Guy

This is a game gone into a game engine project.

Basically, it's a game engine for the Game and Watch.

## Compilation

You can compile your program in four ways: \
- `make` or `make sdl` will compile your code to be ran on your desktop.\
- `make gnw-flash` will compile your code to be ran on your G&W via flashing.\
- `make gnw-ram` will compile your code to be ran on your G&W via it's RAM.

## Structure

Here's a basic  app made with the engine:

```C++
#include "GameAndWatchEngine.h"

//Basic entity setup
class Square : public Entity{
public:
	Square()
	: Entity(0, 0)
	{
		spriteID = 0;
		tag = "None";
	}
};

//Basic game class
class Game: public GameAndWatchEngine{
public:
	Game()
	: GameAndWatchEngine()
	{
		//Initialise the color palette for rendering
		uint16_t colors[16] = {0x0000, 0xD596, 0xE1E1, 0xC041, 
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
		palette.SetColors(colors);
		renderer->SetPalette(&palette);

		//Create a sprite of a fire of size 8x8 at sprite slot 0 (256 slots total)
		spriteManager->CreateSprite(0, "\x00\x30\x00\x00\x00\x03\x33\x00\x00\x32\x33\x30\x00\x11\x23\x30\x03\x11\x21\x33\x03\x22\x22\x33\x03\x22\x22\x33\x00\x33\x33\x00", 4, 8);

		//Create a basic square at entity slot 0 (256 slots total) and setup its position to be (10;10)
		Entity* square = entityManager->CreateEntity<Square>(0);
		Transform* trans = square->GetTransform();
		trans->x = 10;
		trans->y = 10;
	}
};

//Initialising the Game class and starting it
int main(void)
{
	Game game;
	game.Start();
}
```

This code produces a black screen with a little fire sprite 10 pixels from the side and top of the screen.

*Note: There will be features such as tilemaps, and layers (I think) down the line, and as such the structure of a program will change slightly overtime*

## Sprite structure

The sprites in this game engine have a specific structure, and must be hard-coded in order to be able to be put onto the actual console. This part will describe how the sprites are structured, and the different ways you can render them.

First off, the game engine uses a color palette for rendering, so sprites don't have a full color range, but you can easily change the color palette during the game's execution, and the sprite require less spaces. 
The palette can only contain 16 different colors that are in the RGB565 format. ([You can use this converter](http://drakker.org/convert_rgb565.html))

The palette can be defined like so:
```C++
uint16_t colors[16] = {
0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
palette.SetColors(colors);
renderer->SetPalette(&palette);
```
*Note: this is the default palette defined by the engine.*

Thanks to the palette size, we can encode two pixels into a single byte.
For instance `0x01` decodes to the colors `0` and `1` of the palette.

Knowing this we can start encoding sprites :smiley:.

For instance, we want to encode this sprite:
![](images/exampleSprite.png)

We can take each pixel, and assign them a number based on their color.
![](images/ExampleEncoding1.gif)

We can then create a palette out of all the pixels:
![](images/ExampleEncoding2.gif)

And convert the palette from RGB888 to RGB565:
![](images/ExampleEncoding3.gif)

And we can pair the numbers together to make bytes:
![](images/ExampleEncoding4.gif)

Finally, we get this data:
![](images/FinalEncoding.png)

And thus we finally can enter this code to create the sprite within the engine:
```C++
//Code to set the palette (palette is shared by all sprites)
uint16_t colors[16] = {
0x0000, 0xC041, 0xE1E1, 0xD596, 
0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000};
palette.SetColors(colors);
renderer->SetPalette(&palette);

//Code to create the sprite on slot 0
spriteManager->CreateSprite(0, "\x00\x30\x00\x00\x00\x03\x33\x00\x00\x32\x33\x30\x00\x11\x23\x30\x03\x11\x21\x33\x03\x22\x22\x33\x03\x22\x22\x33\x00\x33\x33\x00", 4, 8);
```
