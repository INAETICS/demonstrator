#if defined (__cplusplus)
extern "C" {
#endif

#include "sampleSacDcps.h"
#include "sampleSplDcps.h"

#include <dds_dcps.h>
#include <dds_dcps_private.h>

DDS_sequence_queue_sample *DDS_sequence_queue_sample__alloc (void)
{
    return (DDS_sequence_queue_sample *)DDS__malloc (DDS_sequence_free, 0, sizeof(DDS_sequence_queue_sample));
}

queue_sample *DDS_sequence_queue_sample_allocbuf (DDS_unsigned_long len)
{
    return (queue_sample *)DDS_sequence_allocbuf (NULL, sizeof (queue_sample), len);
}

extern c_bool __queue_sample__copyIn(c_base base, void *from, void *to);
extern void __queue_sample__copyOut(void *from, void *to);
extern const char *queue_sample_metaDescriptor[];
extern const c_ulong queue_sample_metaDescriptorArrLength;

queue_sampleTypeSupport
queue_sampleTypeSupport__alloc (
    void
    )
{
    c_ulong i;
    os_size_t strlength =0;
    char * metaDescriptor;
    queue_sampleTypeSupport result;
    for (i = 0; i < queue_sample_metaDescriptorArrLength; i++) {
        strlength +=strlen(queue_sample_metaDescriptor[i]);
    }

    metaDescriptor = (char*)malloc(strlength+1);
    metaDescriptor[0] = '\0';
    for (i = 0; i < queue_sample_metaDescriptorArrLength; i++) {
        strcat(metaDescriptor,queue_sample_metaDescriptor[i]);
    }
	result = DDS__FooTypeSupport__alloc (
	    __queue_sample__name(),
            __queue_sample__keys(),
            metaDescriptor,
            NULL,
            (DDS_copyIn)__queue_sample__copyIn,
            (DDS_copyOut)__queue_sample__copyOut,
            (DDS_unsigned_long)(sizeof(queue_sample)),
            (DDS_typeSupportAllocBuffer)DDS_sequence_queue_sample_allocbuf
        );
    free(metaDescriptor);
    return result;
}

DDS_ReturnCode_t
queue_sampleTypeSupport_register_type (
    queue_sampleTypeSupport _this,
    const DDS_DomainParticipant domain,
    const DDS_string name
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooTypeSupport_register_type (
	    (DDS_TypeSupport)_this,
	    domain,
	    name
	);
    return result;
}

DDS_string
queue_sampleTypeSupport_get_type_name (
    queue_sampleTypeSupport _this
    )
{
    DDS_string result = (DDS_string)
        DDS__FooTypeSupport_get_type_name (
	    (DDS_TypeSupport)_this
	);
    return result;
}

DDS_InstanceHandle_t
queue_sampleDataWriter_register_instance (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data
    )
{
    DDS_InstanceHandle_t result = (DDS_InstanceHandle_t)
        DDS__FooDataWriter_register_instance (
	    (DDS_DataWriter)_this,
	    (const DDS_sample)instance_data
	);
    return result;
}

DDS_InstanceHandle_t
queue_sampleDataWriter_register_instance_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_Time_t *source_timestamp
    )
{
    DDS_InstanceHandle_t result = (DDS_InstanceHandle_t)
        DDS__FooDataWriter_register_instance_w_timestamp (
	    (DDS_DataWriter)_this,
	    (const DDS_sample)instance_data,
	    source_timestamp
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataWriter_unregister_instance (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t handle
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataWriter_unregister_instance (
	    (const DDS_DataWriter)_this,
	    (DDS_sample)instance_data,
	    handle
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataWriter_unregister_instance_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t handle,
    const DDS_Time_t *source_timestamp
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataWriter_unregister_instance_w_timestamp (
	    (DDS_DataWriter)_this,
	    (const DDS_sample)instance_data,
	    handle,
	    source_timestamp
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataWriter_write (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t handle
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataWriter_write (
	    (DDS_DataWriter)_this,
	    (const DDS_sample)instance_data,
	    handle
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataWriter_write_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t handle,
    const DDS_Time_t *source_timestamp
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataWriter_write_w_timestamp (
	    (DDS_DataWriter)_this,
	    (const DDS_sample)instance_data,
	    handle,
	    source_timestamp
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataWriter_dispose (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t instance_handle
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataWriter_dispose (
	    (DDS_DataWriter)_this,
	    (const DDS_sample)instance_data,
	    instance_handle
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataWriter_dispose_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t instance_handle,
    const DDS_Time_t *source_timestamp
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataWriter_dispose_w_timestamp (
	    (DDS_DataWriter)_this,
	    (const DDS_sample)instance_data,
	    instance_handle,
	    source_timestamp
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataWriter_writedispose (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t instance_handle
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataWriter_writedispose (
	    (DDS_DataWriter)_this,
	    (const DDS_sample)instance_data,
	    instance_handle
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataWriter_writedispose_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t instance_handle,
    const DDS_Time_t *source_timestamp
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataWriter_writedispose_w_timestamp (
	    (DDS_DataWriter)_this,
	    (const DDS_sample)instance_data,
	    instance_handle,
	    source_timestamp
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataWriter_get_key_value (
    queue_sampleDataWriter _this,
    queue_sample *key_holder,
    const DDS_InstanceHandle_t handle
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataWriter_get_key_value (
	    (DDS_DataWriter)_this,
	    (DDS_sample)key_holder,
	    handle
	);
    return result;
}

DDS_InstanceHandle_t
queue_sampleDataWriter_lookup_instance (
    queue_sampleDataWriter _this,
    const queue_sample *key_holder
    )
{
    DDS_InstanceHandle_t result = (DDS_InstanceHandle_t)
        DDS__FooDataWriter_lookup_instance (
        (DDS_DataWriter)_this,
        (DDS_sample)key_holder
    );
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_read (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_read (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_take (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_take (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_read_w_condition (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_ReadCondition a_condition
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_read_w_condition (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_condition
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_take_w_condition (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_ReadCondition a_condition
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_take_w_condition (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_condition
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_read_next_sample (
    queue_sampleDataReader _this,
    queue_sample *received_data,
    DDS_SampleInfo *sample_info
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_read_next_sample (
	    (DDS_DataReader)_this,
	    (DDS_sample)received_data,
	    sample_info
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_take_next_sample (
    queue_sampleDataReader _this,
    queue_sample *received_data,
    DDS_SampleInfo *sample_info
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_take_next_sample (
	    (DDS_DataReader)_this,
	    (DDS_sample)received_data,
	    sample_info
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_read_instance (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_read_instance (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_take_instance (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_take_instance (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_read_next_instance (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_read_next_instance (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_take_next_instance (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_take_next_instance (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_read_next_instance_w_condition (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_ReadCondition a_condition
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_read_next_instance_w_condition (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    a_condition
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_take_next_instance_w_condition (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_ReadCondition a_condition
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_take_next_instance_w_condition (
	    (DDS_DataReader)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    a_condition
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_return_loan (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_return_loan (
            (DDS_DataReader)_this,
            (DDS_sequence)received_data,
            info_seq
    );
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReader_get_key_value (
    queue_sampleDataReader _this,
    queue_sample *key_holder,
    const DDS_InstanceHandle_t handle
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReader_get_key_value (
	    (DDS_DataReader)_this,
	    (DDS_sample)key_holder,
	    handle
	);
    return result;
}

DDS_InstanceHandle_t
queue_sampleDataReader_lookup_instance (
    queue_sampleDataReader _this,
    const queue_sample *key_holder
    )
{
    DDS_InstanceHandle_t result = (DDS_InstanceHandle_t)
        DDS__FooDataReader_lookup_instance (
	    (DDS_DataReader)_this,
	    (DDS_sample)key_holder
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReaderView_read (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_read (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReaderView_take (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_take (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReaderView_read_next_sample (
    queue_sampleDataReaderView _this,
    queue_sample *received_data,
    DDS_SampleInfo *sample_info
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_read_next_sample (
	    (DDS_DataReaderView)_this,
	    (DDS_sample)received_data,
	    sample_info
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReaderView_take_next_sample (
    queue_sampleDataReaderView _this,
    queue_sample *received_data,
    DDS_SampleInfo *sample_info
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_take_next_sample (
	    (DDS_DataReaderView)_this,
	    (DDS_sample)received_data,
	    sample_info
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReaderView_read_instance (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_read_instance (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReaderView_take_instance (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_take_instance (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReaderView_read_next_instance (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_read_next_instance (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReaderView_take_next_instance (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_take_next_instance (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
	    sample_states,
	    view_states,
	    instance_states
	);
    return result;
}

DDS_ReturnCode_t
queue_sampleDataReaderView_return_loan (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_return_loan (
            (DDS_DataReaderView)_this,
            (DDS_sequence)received_data,
            info_seq
    );

    if ( result == DDS_RETCODE_OK ) {
        DDS__free(received_data->_buffer);
        received_data->_length  = 0;
        received_data->_maximum = 0;
        received_data->_buffer  = NULL;

        DDS__free(info_seq->_buffer);
        info_seq->_length  = 0;
        info_seq->_maximum = 0;
        info_seq->_buffer  = NULL;
    }

    return result;
}


DDS_ReturnCode_t
queue_sampleDataReaderView_read_w_condition (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_ReadCondition a_condition
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_read_w_condition (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
            a_condition
	);
    return result;
}


DDS_ReturnCode_t
queue_sampleDataReaderView_take_w_condition (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_ReadCondition a_condition
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_take_w_condition (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
            a_condition
	);
    return result;
}


DDS_ReturnCode_t
queue_sampleDataReaderView_read_next_instance_w_condition (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_ReadCondition a_condition
   )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_read_next_instance_w_condition (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
            a_condition
	);
    return result;
}


DDS_ReturnCode_t
queue_sampleDataReaderView_take_next_instance_w_condition (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_ReadCondition a_condition
   )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_take_next_instance_w_condition (
	    (DDS_DataReaderView)_this,
	    (DDS_sequence)received_data,
	    info_seq,
	    max_samples,
	    a_handle,
            a_condition
	);
    return result;
}


DDS_ReturnCode_t
queue_sampleDataReaderView_get_key_value (
    queue_sampleDataReaderView _this,
    queue_sample *key_holder,
    const DDS_InstanceHandle_t handle
    )
{
    DDS_ReturnCode_t result = (DDS_ReturnCode_t)
        DDS__FooDataReaderView_get_key_value (
	    (DDS_DataReaderView)_this,
	    (DDS_sample)key_holder,
	    handle
	);
    return result;
}


DDS_InstanceHandle_t
queue_sampleDataReaderView_lookup_instance (
    queue_sampleDataReaderView _this,
    queue_sample *key_holder
    )
{
    DDS_InstanceHandle_t result = (DDS_InstanceHandle_t)
        DDS__FooDataReaderView_lookup_instance (
	    (DDS_DataReaderView)_this,
	    (DDS_sample)key_holder
	);
    return result;
}

#if defined (__cplusplus)
}
#endif
