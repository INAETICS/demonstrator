#if defined (__cplusplus)
extern "C" {
#endif

#include "sampleSplDcps.h"
#include "sampleDcps.h"

const char *
__queue_sample__name(void)
{
    return (const char*)"queue::sample";
}

const char *
__queue_sample__keys(void)
{
    return (const char*)"sequence_number";
}

/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 138 */
#include <v_kernel.h>
#include <v_topic.h>
#include <string.h>
#include <os_report.h>

/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 212 */
c_bool
__queue_sample__copyIn(c_base base, void *_from, void *_to)
{
    c_bool result = TRUE;

    queue_sample *from = (queue_sample *)_from;
    struct _queue_sample *to = (struct _queue_sample *)_to;
/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 491 */
/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 292 */
    to->sequence_number = (c_ulonglong)from->sequence_number;
/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 491 */
/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 292 */
    to->time = (c_ulonglong)from->time;
/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 491 */
/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 292 */
    to->value1 = (c_double)from->value1;
/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 491 */
/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 292 */
    to->value2 = (c_double)from->value2;
/* Code generated in file: /home/dds/OvernightTests/tmp.j3XyO86N0W/build/src/tools/idlpp/code/idl_genCorbaCCopyin.c at line: 244 */
    (void) base;
    return result;
}

#include <dds_dcps_private.h>

void
__queue_sample__copyOut(void *_from, void *_to)
{
    struct _queue_sample *from = (struct _queue_sample *)_from;
    queue_sample *to = (queue_sample *)_to;
    to->sequence_number = (DDS_unsigned_long_long)from->sequence_number;
    to->time = (DDS_unsigned_long_long)from->time;
    to->value1 = (DDS_double)from->value1;
    to->value2 = (DDS_double)from->value2;
}

#include <dds_dcps_private.h>

queue_sample *queue_sample__alloc (void)
{
    return (queue_sample *)DDS__malloc (NULL, 0, sizeof(queue_sample));
}

const char * queue_sample_metaDescriptor[] = {"<MetaData version=\"1.0.0\"><Module name=\"queue\"><Struct name=\"sample\"><Member name=\"sequence_number\">",
"<ULongLong/></Member><Member name=\"time\"><ULongLong/></Member><Member name=\"value1\"><Double/></Member>",
"<Member name=\"value2\"><Double/></Member></Struct></Module></MetaData>"};
const c_ulong  queue_sample_metaDescriptorArrLength = 3;
#if defined (__cplusplus)
}
#endif
