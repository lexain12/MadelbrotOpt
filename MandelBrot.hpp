#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

static struct
{
    const int   max_N  = 255;
    const float max_R  = 10.0;
    const float max_R2 = max_R * max_R;

    const float min_x = -2.0;
    const float max_x =  2.0;
    const float min_y = -2.0;
    const float max_y =  2.0;

    const unsigned int width  = 640;
    const unsigned int height = 640;

    const float dx = (max_x - min_x) / width;
    const float dy = (max_y - min_y) / height;
} InitMb;

#define ShowXmmReg(Name) \
    printf ("-----" # Name  "-----\n");            \
    for (int Name##_ = 0;Name##_ < 4; ++ Name##_)\
{                                               \
    printf ("%f\n", ((float*) &Name)[Name##_]);        \
}                                               \
    printf ("----------\n");

void InitPixelArray (sf::Uint8** array, size_t size);
void MandelbrotSet (sf::Image &image);
sf::Color pickColor (int n);
