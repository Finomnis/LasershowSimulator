#include <lasershow.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static void generate(float timePosition, size_t numPixels, float *r, float *g, float *b)
{
    //printf("%f\n", timePosition);
    //fflush(stdout);
    float pct = timePosition/4;

    for(int i = 0; i < numPixels; i++){
        float pos = i/(float)(numPixels-1);
        r[i] = 0.5f + 0.5f * sinf(2*M_PI*(8*pos - 1 * pct));
        g[i] = 0.5f + 0.5f * cosf(2*M_PI*(10*pos + 1 * pct));
        b[i] = 0.5f + 0.5f * sinf(2*M_PI*(5*pos));
    }

}

static void generate2(float timePosition, size_t numPixels, float *r, float *g, float *b)
{
    bool showEven = ((int)timePosition) & 0x1;

    for(int i = 0; i < numPixels; i++){
        if (((i/10)&0x1) == showEven){
            r[i] = 0;
            g[i] = 0;
            b[i] = 0;
        } else {
            r[i] = 1;
            g[i] = 1;
            b[i] = 1;
        }
        if(i / (float)numPixels > timePosition / 4.0f){
            r[i] = 0;
            g[i] = 1;
            b[i] = 0;
        }
    }
}

static void reset()
{

}

REGISTER_LASERSHOW("Test", 16, generate, reset)
REGISTER_LASERSHOW("Test2", 4, generate2, reset)


