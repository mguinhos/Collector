#ifndef COLLECTOR__H
#define COLLECTOR__H

#include <stdbool.h>
#include <malloc.h>

typedef struct Collector
{
    struct Collector* next;
    struct Collector* previous;
    
    unsigned long keepers;
    void* object;

    void (*__collect__)(void* object);
}
Collector;

Collector* collector__new();
bool collector__del(Collector* self);
bool collector__keep(Collector* self, void* object);
bool collector__collect(Collector* self);

#endif