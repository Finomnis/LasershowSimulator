#include <lasershow.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

LASERSHOW(Test, 4)
{
    printf("%f\n", timePosition);
    fflush(stdout);
    float pct = timePosition/4;

    for(int i = 0; i < numPixels; i++){
        float pos = i/(float)(numPixels-1);
        r[i] = 255 * (0.5f + 0.5f * sinf(2*M_PI*(8*pos - 1 * pct)));
        g[i] = 255 * (0.5f + 0.5f * cosf(2*M_PI*(10*pos + 1 * pct)));
        b[i] = 255 * (0.5f + 0.5f * sinf(2*M_PI*(5*pos)));
    }

}


LASERSHOW(Test2, 4)
{
    bool showEven = ((int)timePosition) & 0x1;

    for(int i = 0; i < numPixels; i++){
        if ((i&0x1) == showEven){
            r[i] = 0;
            g[i] = 0;
            b[i] = 0;
        } else {
            r[i] = 255;
            g[i] = 255;
            b[i] = 255;
        }
    }
}

