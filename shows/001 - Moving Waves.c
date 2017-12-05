#include <lasershow.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int enableR, enableG, enableB;
static float speedR, speedG, speedB;
static float sizeR, sizeG, sizeB;
static int oldEnabled = 0;

static void generate(float timePosition, size_t numPixels, float *r, float *g, float *b)
{
    //printf("%f\n", timePosition);
    //fflush(stdout);
    float pct = timePosition/4;

    float mult = 1.0f;
    if(timePosition < 0.5f){
        mult = timePosition/0.5f;
    }
    if(timePosition > 15.5f){
        mult = (16-timePosition)/0.5f;
    }

    for(int i = 0; i < numPixels; i++){
        float pos = i/(float)(numPixels-1);
        r[i] = enableR ? mult*(0.5f + 0.5f * sinf(2*M_PI*(15* pos * sizeR + 1 * pct * speedR))) : 0;
        g[i] = enableG ? mult*(0.5f + 0.5f * sinf(2*M_PI*(15* pos * sizeG + 1 * pct * speedG))) : 0;
        b[i] = enableB ? mult*(0.5f + 0.5f * sinf(2*M_PI*(15* pos * sizeB + 1 * pct * speedB))) : 0;
    }

}

static void reset()
{
    int enabled = rand()%3;
    if(enabled >= oldEnabled)
        enabled++;
    oldEnabled = enabled;

    float speed1 = 0.3f + rand() / (float)RAND_MAX;
    float speed2 = - rand() / (float)RAND_MAX;
    float speed3 = 0;

    if(rand()%2){
        float tmp = speed1;
        speed1 = speed2;
        speed2 = tmp;
    }

    if(enabled == 0){
        speedR = 0;
        speedG = speed1;
        speedB = speed2;
        enableR = 0;
        enableG = 1;
        enableB = 1;
    }
    else if(enabled == 1){
        speedR = speed1;
        speedG = 0;
        speedB = speed2;
        enableR = 1;
        enableG = 0;
        enableB = 1;
    }
    else if(enabled == 2){
        speedR = speed1;
        speedG = speed2;
        speedB = 0;
        enableR = 1;
        enableG = 1;
        enableB = 0;
    }
    else{
        int order1 = rand()%3;
        int order2 = rand()%2;
        if(order2 >= order1)
            order2++;

        speedR = 0;
        speedG = 0;
        speedB = 0;

        if(order1 == 0){
            speedR = speed1;
        } else if(order1 == 1){
            speedG = speed1;
        } else {
            speedB = speed1;
        }

        if(order2 == 0){
            speedR = speed2;
        } else if(order2 == 1){
            speedG = speed2;
        } else {
            speedB = speed2;
        }

        enableR = 1;
        enableG = 1;
        enableB = 1;
    }

    sizeR = 0.3f + rand() / (float)RAND_MAX;
    sizeG = 0.3f + rand() / (float)RAND_MAX;
    sizeB = 0.3f + rand() / (float)RAND_MAX;

}

REGISTER_LASERSHOW("Moving Waves", 16, generate, reset)


