#include "stdio.h"
#include "stdlib.h"
#include "MandelBrot.hpp"
#include "emmintrin.h"
#include <SFML/Graphics/RenderWindow.hpp>

using namespace sf;

#ifdef VidMode
int main()
{
	sf::RenderWindow window(sf::VideoMode(InitMb.width, InitMb.height), "Mandelbrot");

    sf::Texture texture;
    sf::Sprite sprite(texture);

    Clock clock;

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

        sf::Time elapsed = clock.restart();
        printf ("FPS %f\n", 1/elapsed.asSeconds());

	}

	return 0;
}
#else

int main ()
{
    RenderWindow window;


    while (true)
    {
        MandelbrotSet(window);
        printf ("FPS \n");
    }
}

#endif

Color pickColor (int n)
{
    return Color(255 - n*5, 5*n, 128 - n*5, 255);
}

#ifndef SSE
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


            volatile int n = 0;

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

#ifdef VidMode
            rectangle.setPosition((float) xi, (float) yi);

            if (n >= InitMb.max_N)
                rectangle.setFillColor(Color::Black);
            else
                rectangle.setFillColor(pickColor(n));

            window.draw(rectangle);
#endif
        }
    }
}
#else

void MandelbrotSet (RenderWindow &window)
{
    float dx    = InitMb.dx;
    float dy    = InitMb.dy;
    float min_y = InitMb.min_y;
    float min_x = InitMb.min_x;

    int yi = 0;
    RectangleShape rectangle(Vector2f(1, 1));

    __m128 DY = _mm_set1_ps (dy);
    __m128 DX = _mm_set_ps (0, dx, dx*2, dx*3);
    __m128 Y0 = _mm_set1_ps (min_y - dy);
    __m128 MAX_R2 = _mm_set1_ps (InitMb.max_R2);
    __m128 MAX_N  = _mm_set1_ps (InitMb.max_N);

    for (;yi < InitMb.height; yi++)
    {
        Y0 = _mm_add_ps (Y0, DY);

        int xi = 0;
        for (;xi < InitMb.width; xi+=4)
        {
            __m128 X0 = _mm_set1_ps(dx * xi - InitMb.max_x);
            X0 = _mm_add_ps (X0, DX);

            __m128 X  = X0;
            __m128 Y  = Y0;

            __m128 N      = _mm_set1_ps (0);
            __m128 dN     = _mm_set1_ps (1);
            __m128 Mask1  = _mm_set1_ps (0x0001);


            for (int i = 0; i < InitMb.max_N; i++)
            {
                __m128 X2 = _mm_mul_ps (X, X);
                __m128 Y2 = _mm_mul_ps (Y, Y);
                __m128 XY = _mm_mul_ps (X, Y);
                __m128 R2 = _mm_add_ps (X2, Y2);

                dN = _mm_cmplt_ps (R2, MAX_R2);

                if (!_mm_movemask_ps(dN))
                {
                    break;
                }

                X  = _mm_add_ps (_mm_sub_ps (X2, Y2), X0);
                Y  = _mm_add_ps (_mm_add_ps (XY, XY), Y0);

                dN = _mm_and_ps(dN, Mask1);

                N = _mm_add_ps (dN, N);
            }

#ifdef VidMode
            for (int i = 0; i < 4; i++)
            {

                float n = ((float*) &N)[3 - i];

                rectangle.setPosition((float) xi + i, (float) yi);

                if (n >= InitMb.max_N)
                    rectangle.setFillColor(Color::Black);
                else
                    rectangle.setFillColor(pickColor(n));

                window.draw(rectangle);
            }
#endif
        }
    }
}
#endif
