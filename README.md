# Mandelbrot optimization
## General information
In this lab I am drawing a Mandelbrot set with sfml library. First, I wrote the simpliest algorithm. Then optimized it, using SSE instructions.

## Picture of Mandelbrot set
![Picture](img/MandelbrotSet.png)
## Simple algorithm
To calculate the Mandelbrot set, you need to iterate a simple formula on each point in the complex plane and
determine whether the resulting sequence of numbers remains bounded or not.

The formula for iteration is:

$z_{n+1} = z_n^2 + c$

To create a visualization of the Mandelbrot set, I colored the points based on how quickly the sequence of numbers diverges. 
Points that are part of the set are colored black, while points outside the set are colored based on the number of iterations it takes for the sequence to become unbounded.
So the main part of calcualtion is:
~~~C++
while (n < 256 && r2 < 100)
{
    float x2 = x * x;
    float y2 = y * y;
    float xy = x * y;

    x  = x2 - y2 + X0;
    y  = xy + xy + Y0;
    r2 = x2 + y2;
    n += 1;
}
if (n >= 256)
    image.setPixel(xi, yi, Color::Black);
else
    image.setPixel(xi, yi, pickColor(n));
~~~
As you can see, it is very complicated algorithm, that need to be optimized.

## Optimization

The main idea of optimization, is to use the SIMD (single instruction, multiple data) instructions. By using this instruction, 
we can calculate multiple points in one iteration.

In my case, I was using SSE set of instruction, so I could calculate four points in once.
The code after optimisation:
~~~C++
for (int i = 0; i < 256; i++)
{
        __m128 X2 = _mm_mul_ps (X, X);
        __m128 Y2 = _mm_mul_ps (Y, Y);
        __m128 XY = _mm_mul_ps (X, Y);
        __m128 R2 = _mm_add_ps (X2, Y2);

        dN = _mm_cmplt_ps (R2, MAX_R2);

        if (!_mm_movemask_ps(dN))
            break;

        X  = _mm_add_ps (_mm_sub_ps (X2, Y2), X0);
        Y  = _mm_add_ps (_mm_add_ps (XY, XY), Y0);

        dN = _mm_and_ps(dN, Mask1);

        N = _mm_add_ps (dN, N);
}
~~~
## Measurements

To compare the performance, I use ```sf::Clock```. Dividing 1 by iteration time, I get the FPS. 
As the perfomance depends from rendering, I turned off it. The results you can see in table below.

| Compilation flags | Optimization | FPS |
|-------------------|--------------|-----|
| -O0               |     None     | 4   |
| -O2               |     None     | 8   |
| -Ofast            |     None     | 8.5 |
| -O2               |     SSE      | 27.5|
| -Ofast            |     SSE      | 30  |

So the perfomance boost $30/8.5 = 3.53$ time, which I think is pretty good result.

## Conclusion

Compilation flags are great at speeding up programs, but in some cases they are not enough. This lab work shows this. 
One such way of optimizing is SSE instructions, which can speed up performance by a factor of 4.
