#include "stdio.h"
#include "stdlib.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include "MandelBrot.hpp"

using namespace sf;

void InitPixelArray (sf::Uint8** array, size_t size);
void MandelbrotSet (RenderWindow &window);
Color pickColor (int n);

int main()
{
	sf::RenderWindow window(sf::VideoMode(InitMb.width, InitMb.height), "Mandelbrot");

    sf::Texture texture;
    sf::Sprite sprite(texture);

	while (window.isOpen())
	{
        window.clear();
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			switch (Event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
			}
		}

        MandelbrotSet(window);

		window.display();
	}

	return 0;
}

Color pickColor (int n)
{
    return Color(256 - n*5, 5*n, 128 - n*5, 255);
}

void MandelbrotSet (RenderWindow &window)
{
    int yi = 0;
    RectangleShape rectangle(Vector2f(1, 1));

    for (;yi < InitMb.height; ++yi)
    {
        float Y0 = InitMb.min_y + yi * InitMb.dy;

        int xi = 0;
        for (;xi < InitMb.width; ++xi)
        {
            float X0 = InitMb.min_x + xi * InitMb.dx;

            float x  = X0;
            float y  = Y0;
            float r2 = x*x + y*y;

            int n = 0;

            while (n < InitMb.max_N && r2 < InitMb.max_R2)
            {
                float x2 = x*x;
                float y2 = y*y;
                float xy = x*y;

                x  = x2 - y2 + X0;
                y  = xy + xy + Y0;
                r2 = x2 + y2;
                n += 1;
            }

            rectangle.setPosition((float) xi, (float) yi);

            if (n >= InitMb.max_N)
                rectangle.setFillColor(Color::Black);
            else
                rectangle.setFillColor(pickColor(n));

            window.draw(rectangle);
        }
    }
}
