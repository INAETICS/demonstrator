/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
#ifndef __INAETICS_DEMONSTRATOR_API__SAMPLE_H_
#define __INAETICS_DEMONSTRATOR_API__SAMPLE_H_

#include <stdint.h>

struct sample {
        uint64_t time; //milliseconds since unix epoch
        double value1;
        double value2;
};

#endif
