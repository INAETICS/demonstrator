#ifndef SAMPLESPLTYPES_H
#define SAMPLESPLTYPES_H

#include "sampleDcps.h"

#include <c_base.h>
#include <c_misc.h>
#include <c_sync.h>
#include <c_collection.h>
#include <c_field.h>

extern c_metaObject __sample_queue__load (c_base base);

extern c_metaObject __queue_sample__load (c_base base);
extern const char * __queue_sample__keys (void);
extern const char * __queue_sample__name (void);
struct _queue_sample ;
extern  c_bool __queue_sample__copyIn(c_base base, void *from, void *to);
extern  void __queue_sample__copyOut(void *_from, void *_to);
struct _queue_sample {
    c_ulonglong sequence_number;
    c_ulonglong time;
    c_double value1;
    c_double value2;
};

#endif
