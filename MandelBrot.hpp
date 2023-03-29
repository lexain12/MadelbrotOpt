static struct
{
    const int   max_N  = 256;
    const float max_R  = 10.0;
    const float max_R2 = max_R * max_R;

    const float min_x = -2.0;
    const float max_x =  2.0;
    const float min_y = -2.0;
    const float max_y =  2.0;

    const unsigned int width  = 1000;
    const unsigned int height = 1000;

    const float dx = (max_x - min_x) / width;
    const float dy = (max_y - min_y) / height;
} InitMb;
