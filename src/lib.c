#include <malloc.h>
#include "collector.h"

#ifdef DEBUG
# include <stdio.h>
#endif

#ifdef DEBUG
void collector____collect__(void* object)
{
    printf("[collector] DEBUG: collected '%p'\n", object);
}
#endif

Collector* collector__new()
{
    Collector* collector = (Collector*) malloc(sizeof(Collector));

    if (collector == NULL)
    {
        #ifdef DEBUG
            fprintf(stderr, "[collector] WARNING: collector__new: allocation failed.\n");
        #endif

        return NULL;
    }

    collector->next = NULL;
    collector->previous = NULL;

    collector->keepers = 0;
    collector->object = NULL;

    #ifdef DEBUG
        collector->__collect__ = collector____collect__;
    #else
        collector->__collect__ = NULL;
    #endif

    return collector;
}

bool collector__del(Collector* self)
{
    if (self == NULL)
        return false;
        
    Collector* current;
    Collector* current_next;

    for (current=self; current != NULL; current = current_next)
    {
        current_next = current->next;
        
        if (current->object != NULL)
        {
            if (current->__collect__ != NULL)
                current->__collect__(current->object);
            
            free(current->object);

            #ifdef DEBUG
                if (current->keepers > 0)
                    fprintf(stderr, "[collector] WARNING: freed alive object, this may cause undefined behaviour.\n");
            #endif
        }
        
        free(current);
    }

    return true;
}

bool collector__keep(Collector* self, void* object)
{
    if (self == NULL)
        return false;
    else if (object == NULL)
        return false;

    Collector* current;

    for (current=self; current != NULL; current = current->next)
    {
        #ifdef DEBUG
            if (current->object == NULL && current->keepers > 0)
                fprintf(stderr, "[collector] WARNING: collector__keep: found an alive NULL object.\n");
        #endif

        if (current->object == object)
        {
            current->keepers++;
            return true;
        }
    }

    Collector* last;

    for (last=self; last->next != NULL; last = last->next)
        continue;

    if (last->object == NULL)
    {
        last->keepers = 1;
        last->object = object;
        return true;
    }

    Collector* last_next = collector__new();

    if (last_next == NULL)
        return false;
    
    last_next->previous = last;
    last_next->keepers = 1;
    last_next->object = object;

    last->next = last_next;

    return true;
}

bool collector__collect(Collector* self)
{
    if (self == NULL)
        return false;

    Collector* current;

    for (current=self; current != NULL; current = current->next)
    {
        if (current->keepers-- == 0)
        {
            if (current->object != NULL)
            {
                if (current->__collect__ != NULL)
                    current->__collect__(current->object);

                free(current->object);
            }
            else
                #ifdef DEBUG
                    fprintf(stderr, "[collector] WARNING: collector__collect: attempt to free a NULL object.\n");
                #else
                    continue;
                #endif
        }
            
    }

    return true;
}