#ifdef __cplusplus
extern "C" {
#endif
#ifndef LASERSHOW_H_
#define LASERSHOW_H_

#include <stdlib.h>
#include <stdbool.h>

// generates the image at time 'time'. Array size is 'numElements'. r,g,b are arrays of size 'numElements'.
typedef void LasershowGenerateFunc(float timePosition, size_t numPixels, unsigned char *r, unsigned char *g, unsigned char *b);
typedef void LasershowResetFunc();

struct LaserShowGenerator
{
    char *name;
    int duration;
    LasershowGenerateFunc *generate;
    LasershowResetFunc *reset;
};

void registerLasershow(struct LaserShowGenerator gen);
struct LaserShowGenerator *getLasershows(size_t *size);

#define REGISTER_LASERSHOW(name_, duration_, generateFunc_, resetFunc_)\
    static void __attribute__((constructor)) name_##registerLasershow_()\
    {\
        struct LaserShowGenerator gen; \
        gen.name = #name_; \
        gen.duration = duration_; \
        gen.generate = generateFunc_; \
        gen.reset = resetFunc_; \
        registerLasershow(gen); \
    }

#endif
#ifdef __cplusplus
}
#endif
