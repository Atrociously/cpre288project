/*
 * objects.h
 *
 *  Created on: May 1, 2023
 *      Author: jackmorr
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <stdint.h>
#include <stddef.h>

typedef struct {
    int right;
    int left;
    float arc_len;
    float distance;
} Obj_t;

size_t scan_objects(Obj_t objects[], size_t max);

#endif /* OBJECTS_H_ */
