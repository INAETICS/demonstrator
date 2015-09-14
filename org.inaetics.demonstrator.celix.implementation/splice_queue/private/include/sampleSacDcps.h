#ifndef SAMPLESACDCPS_H
#define SAMPLESACDCPS_H

#include "sampleDcps.h"

#ifndef DDS_API
#define DDS_API
#endif


#define queue_sampleTypeSupport DDS_TypeSupport

 queue_sampleTypeSupport
queue_sampleTypeSupport__alloc (
    void
    );

 DDS_ReturnCode_t
queue_sampleTypeSupport_register_type (
    queue_sampleTypeSupport _this,
    const DDS_DomainParticipant domain,
    const DDS_string name
    );

 DDS_string
queue_sampleTypeSupport_get_type_name (
    queue_sampleTypeSupport _this
    );

#ifndef _DDS_sequence_queue_sample_defined
#define _DDS_sequence_queue_sample_defined
typedef struct {
    DDS_unsigned_long _maximum;
    DDS_unsigned_long _length;
    queue_sample *_buffer;
    DDS_boolean _release;
} DDS_sequence_queue_sample;

 DDS_sequence_queue_sample *DDS_sequence_queue_sample__alloc (void);
 queue_sample *DDS_sequence_queue_sample_allocbuf (DDS_unsigned_long len);
#endif /* _DDS_sequence_queue_sample_defined */

#define queue_sampleDataWriter DDS_DataWriter

#define queue_sampleDataWriter_enable DDS_Entity_enable

#define queue_sampleDataWriter_get_status_changes DDS_Entity_get_status_changes

#define queue_sampleDataWriter_get_statuscondition DDS_Entity_get_statuscondition

#define queue_sampleDataWriter_get_instance_handle DDS_Entity_get_instance_handle

#define queue_sampleDataWriter_assert_liveliness DDS_DataWriter_assert_liveliness

#define queue_sampleDataWriter_get_listener DDS_DataWriter_get_listener

#define queue_sampleDataWriter_get_liveliness_lost_status DDS_DataWriter_get_liveliness_lost_status

#define queue_sampleDataWriter_get_matched_subscription_data DDS_DataWriter_get_matched_subscription_data

#define queue_sampleDataWriter_get_matched_subscriptions DDS_DataWriter_get_matched_subscriptions

#define queue_sampleDataWriter_get_offered_deadline_missed_status DDS_DataWriter_get_offered_deadline_missed_status

#define queue_sampleDataWriter_get_offered_incompatible_qos_status DDS_DataWriter_get_offered_incompatible_qos_status

#define queue_sampleDataWriter_get_publication_match_status DDS_DataWriter_get_publication_match_status

#define queue_sampleDataWriter_get_publisher DDS_DataWriter_get_publisher

#define queue_sampleDataWriter_get_qos DDS_DataWriter_get_qos

#define queue_sampleDataWriter_get_topic DDS_DataWriter_get_topic

#define queue_sampleDataWriter_set_listener DDS_DataWriter_set_listener

#define queue_sampleDataWriter_set_qos DDS_DataWriter_set_qos

 DDS_InstanceHandle_t
queue_sampleDataWriter_register_instance (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data
    );

 DDS_InstanceHandle_t
queue_sampleDataWriter_register_instance_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_Time_t *source_timestamp
    );

 DDS_ReturnCode_t
queue_sampleDataWriter_unregister_instance (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t handle
    );

 DDS_ReturnCode_t
queue_sampleDataWriter_unregister_instance_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t handle,
    const DDS_Time_t *source_timestamp
    );

 DDS_ReturnCode_t
queue_sampleDataWriter_write (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t handle
    );

 DDS_ReturnCode_t
queue_sampleDataWriter_write_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t handle,
    const DDS_Time_t *source_timestamp
    );

 DDS_ReturnCode_t
queue_sampleDataWriter_dispose (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t instance_handle
    );

 DDS_ReturnCode_t
queue_sampleDataWriter_dispose_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t instance_handle,
    const DDS_Time_t *source_timestamp
    );

 DDS_ReturnCode_t
queue_sampleDataWriter_writedispose (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t instance_handle
    );

 DDS_ReturnCode_t
queue_sampleDataWriter_writedispose_w_timestamp (
    queue_sampleDataWriter _this,
    const queue_sample *instance_data,
    const DDS_InstanceHandle_t instance_handle,
    const DDS_Time_t *source_timestamp
    );

 DDS_ReturnCode_t
queue_sampleDataWriter_get_key_value (
    queue_sampleDataWriter _this,
    queue_sample *key_holder,
    const DDS_InstanceHandle_t handle
    );

 DDS_InstanceHandle_t
queue_sampleDataWriter_lookup_instance (
    queue_sampleDataWriter _this,
    const queue_sample *key_holder
    );

#define queue_sampleDataReader DDS_DataReader

#define queue_sampleDataReader_enable DDS_Entity_enable

#define queue_sampleDataReader_get_status_changes DDS_Entity_get_status_changes

#define queue_sampleDataReader_get_statuscondition DDS_Entity_get_statuscondition

#define queue_sampleDataReader_get_instance_handle DDS_Entity_get_instance_handle

#define queue_sampleDataReader_create_querycondition DDS_DataReader_create_querycondition

#define queue_sampleDataReader_create_readcondition DDS_DataReader_create_readcondition

#define queue_sampleDataReader_delete_contained_entities DDS_DataReader_delete_contained_entities

#define queue_sampleDataReader_delete_readcondition DDS_DataReader_delete_readcondition

#define queue_sampleDataReader_get_listener DDS_DataReader_get_listener

#define queue_sampleDataReader_get_liveliness_changed_status DDS_DataReader_get_liveliness_changed_status

#define queue_sampleDataReader_get_matched_publication_data DDS_DataReader_get_matched_publication_data

#define queue_sampleDataReader_get_matched_publications DDS_DataReader_get_matched_publications

#define queue_sampleDataReader_get_qos DDS_DataReader_get_qos

#define queue_sampleDataReader_get_requested_deadline_missed_status DDS_DataReader_get_requested_deadline_missed_status

#define queue_sampleDataReader_get_requested_incompatible_qos_status DDS_DataReader_get_requested_incompatible_qos_status

#define queue_sampleDataReader_get_sample_lost_status DDS_DataReader_get_sample_lost_status

#define queue_sampleDataReader_get_sample_rejected_status DDS_DataReader_get_sample_rejected_status

#define queue_sampleDataReader_get_subscriber DDS_DataReader_get_subscriber

#define queue_sampleDataReader_get_subscription_match_status DDS_DataReader_get_subscription_match_status

#define queue_sampleDataReader_get_topicdescription DDS_DataReader_get_topicdescription

#define queue_sampleDataReader_set_listener DDS_DataReader_set_listener

#define queue_sampleDataReader_set_qos DDS_DataReader_set_qos

#define queue_sampleDataReader_wait_for_historical_data DDS_DataReader_wait_for_historical_data

 DDS_ReturnCode_t
queue_sampleDataReader_read (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    );

 DDS_ReturnCode_t
queue_sampleDataReader_take (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    );

 DDS_ReturnCode_t
queue_sampleDataReader_read_w_condition (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_ReadCondition a_condition
    );

 DDS_ReturnCode_t
queue_sampleDataReader_take_w_condition (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_ReadCondition a_condition
    );

 DDS_ReturnCode_t
queue_sampleDataReader_read_next_sample (
    queue_sampleDataReader _this,
    queue_sample *received_data,
    DDS_SampleInfo *sample_info
    );

 DDS_ReturnCode_t
queue_sampleDataReader_take_next_sample (
    queue_sampleDataReader _this,
    queue_sample *received_data,
    DDS_SampleInfo *sample_info
    );

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
    );

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
    );

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
    );

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
    );

 DDS_ReturnCode_t
queue_sampleDataReader_read_next_instance_w_condition (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_ReadCondition a_condition
    );

 DDS_ReturnCode_t
queue_sampleDataReader_take_next_instance_w_condition (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_ReadCondition a_condition
    );

 DDS_ReturnCode_t
queue_sampleDataReader_return_loan (
    queue_sampleDataReader _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq
    );

 DDS_ReturnCode_t
queue_sampleDataReader_get_key_value (
    queue_sampleDataReader _this,
    queue_sample *key_holder,
    const DDS_InstanceHandle_t handle
    );

 DDS_InstanceHandle_t
queue_sampleDataReader_lookup_instance (
    queue_sampleDataReader _this,
    const queue_sample *key_holder
    );

#define queue_sampleDataReaderView DDS_DataReaderView

#define queue_sampleDataReaderView_enable DDS_Entity_enable

#define queue_sampleDataReaderView_get_instance_handle DDS_Entity_get_instance_handle

#define queue_sampleDataReaderView_get_qos DDS_DataReaderView_get_qos

#define queue_sampleDataReaderView_get_datareader DDS_DataReaderView_get_datareader

#define queue_sampleDataReaderView_set_qos DDS_DataReaderView_set_qos

#define queue_sampleDataReaderView_create_readcondition DDS_DataReaderView_create_readcondition

#define queue_sampleDataReaderView_create_querycondition DDS_DataReaderView_create_querycondition

#define queue_sampleDataReaderView_delete_readcondition DDS_DataReaderView_delete_readcondition

#define queue_sampleDataReaderView_delete_contained_entities DDS_DataReaderView_delete_contained_entities

 DDS_ReturnCode_t
queue_sampleDataReaderView_read (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    );

 DDS_ReturnCode_t
queue_sampleDataReaderView_take (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_SampleStateMask sample_states,
    const DDS_ViewStateMask view_states,
    const DDS_InstanceStateMask instance_states
    );

 DDS_ReturnCode_t
queue_sampleDataReaderView_read_next_sample (
    queue_sampleDataReaderView _this,
    queue_sample *received_data,
    DDS_SampleInfo *sample_info
    );

 DDS_ReturnCode_t
queue_sampleDataReaderView_take_next_sample (
    queue_sampleDataReaderView _this,
    queue_sample *received_data,
    DDS_SampleInfo *sample_info
    );

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
    );

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
    );

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
    );

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
    );

 DDS_ReturnCode_t
queue_sampleDataReaderView_return_loan (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq
    );

 DDS_ReturnCode_t
queue_sampleDataReaderView_read_w_condition (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_ReadCondition a_condition
    );

 DDS_ReturnCode_t
queue_sampleDataReaderView_take_w_condition (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_ReadCondition a_condition
    );

 DDS_ReturnCode_t
queue_sampleDataReaderView_read_next_instance_w_condition (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_ReadCondition a_condition
    );

 DDS_ReturnCode_t
queue_sampleDataReaderView_take_next_instance_w_condition (
    queue_sampleDataReaderView _this,
    DDS_sequence_queue_sample *received_data,
    DDS_SampleInfoSeq *info_seq,
    const DDS_long max_samples,
    const DDS_InstanceHandle_t a_handle,
    const DDS_ReadCondition a_condition
    );

 DDS_ReturnCode_t
queue_sampleDataReaderView_get_key_value (
    queue_sampleDataReaderView _this,
    queue_sample *key_holder,
    const DDS_InstanceHandle_t handle
    );

 DDS_InstanceHandle_t
queue_sampleDataReaderView_lookup_instance (
    queue_sampleDataReaderView _this,
    queue_sample *key_holder
    );

#endif
