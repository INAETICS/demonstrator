#ifndef SAMPLEDCPS_H
#define SAMPLEDCPS_H

#include <dds_dcps.h>


#ifndef _queue_sample_defined
#define _queue_sample_defined
#ifdef __cplusplus
struct queue_sample;
#else /* __cplusplus */
typedef struct queue_sample queue_sample;
#endif /* __cplusplus */
#endif /* _queue_sample_defined */
 queue_sample *queue_sample__alloc (void);

struct queue_sample {
    DDS_unsigned_long_long sequence_number;
    DDS_unsigned_long_long time;
    DDS_double value1;
    DDS_double value2;
};

#endif
