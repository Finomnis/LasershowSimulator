#include "lasershow.h"

#include <stdlib.h>
#include <stdio.h>

static size_t arraySize = 0;
static size_t numGenerators = 0;
static struct LaserShowGenerator *generators = NULL;

void registerLasershow(struct LaserShowGenerator gen){
    printf("Registering '%s' ...\n", gen.name);
    if(arraySize == 0){
        arraySize = 1;
        generators = malloc(sizeof(struct LaserShowGenerator)*arraySize);
    }
    if(numGenerators >= arraySize){
        arraySize = arraySize * 2;
        generators = realloc(generators, sizeof(struct LaserShowGenerator)*arraySize);
    }

    generators[numGenerators] = gen;
    numGenerators++;
}

struct LaserShowGenerator *getLasershows(size_t *size){
    if(size != NULL){
        *size = numGenerators;
    }
    return generators;
}


