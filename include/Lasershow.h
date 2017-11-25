#ifdef __cplusplus
extern "C" {
#endif
#ifndef LASERSHOW_H_
#define LASERSHOW_H_

#include <stdlib.h>
#include <stdbool.h>

// generates the image at time 'time'. Array size is 'numElements'. r,g,b are arrays of size 'numElements'.
typedef void LasershowGeneratorFunc(float timePosition, size_t numPixels, unsigned char *r, unsigned char *g, unsigned char *b);

struct LaserShowGenerator
{
    char *name;
    int duration;
    LasershowGeneratorFunc *run;
};

void registerLasershow(struct LaserShowGenerator gen);
struct LaserShowGenerator *getLasershows(size_t *size);

#define LASERSHOW(name_, duration_)\
    static LasershowGeneratorFunc name_##laserShowFunc;\
    static void __attribute__((constructor)) name_##registerLasershow_()\
    {\
        struct LaserShowGenerator gen; \
        gen.name = #name_; \
        gen.duration = duration_; \
        gen.run = name_##laserShowFunc; \
        registerLasershow(gen); \
    }\
    static void name_##laserShowFunc (float timePosition, size_t numPixels, unsigned char *r, unsigned char *g, unsigned char *b)

#endif
#ifdef __cplusplus
}
#endif
