#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <string.h>
#include "collector.h"

Collector* collector;

int main()
{
    collector = collector__new();
    
    // first we create a buffer
    char* buffer = malloc(6);
    strcpy(buffer, "hello");

    // now we put our buffer in the collector
    collector__keep(collector, buffer);
    
    // now we attempt a collection
    // nothing will be collected, because we kept buffer
    // this will decrement the .keepers count
    collector__collect(collector);

    // now buffer will be collected
    collector__collect(collector);
}