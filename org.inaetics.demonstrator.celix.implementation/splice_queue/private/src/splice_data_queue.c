/* ************************** Copyright disclosure ****************************
 * This document carrier contains proprietary information which shall not be
 * used for other purposes than those for which it has been released, nor be
 * reproduced or disclosed to third parties, without the prior written
 * consent of Thales Nederland B.V.
 * ****************************************************************************/

/*** Header ***
 * \file            splice_data_queue.c
 *
 * \brief           Contains the functions to store sample data in open splice.
 *
 */


/****************************************************************
 *
 * Includes
 *
 ****************************************************************/
#include "splice_data_queue.h"

/****************************************************************
 *
 * Local defines
 *
 ****************************************************************/
typedef struct splice_data_queue_type {
    DDS_DomainParticipant domain_participant;
    DDS_Publisher publisher;
    DDS_Subscriber subscriber;
    queue_sampleDataWriter writer;
    queue_sampleDataReader reader;
    DDS_WaitSet waitset;
    queue_sampleTypeSupport sample_data_type;
    DDS_Topic topic;
}splice_data_queue_type;

/****************************************************************
 *
 * Local macros
 *
 ****************************************************************/

/****************************************************************
 *
 * Local type definitions
 *
 ****************************************************************/
#define PARTITION_LENGTH         ((DDS_unsigned_long)(1))
#define DEFAULT_PARTITION        ((char*)("sample_data_partition"))
#define TOPIC_NAME               ((DDS_string)("SAMPLE"))
#define TOPIC_TYPE               ((DDS_string)("QUEUE:SAMPLE"))

#define HISTORY                  DDS_KEEP_ALL_HISTORY_QOS
/****************************************************************
 *
 * Global data declarations
 *
 ****************************************************************/

/****************************************************************
 *
 * Static data declarations
 *
 ****************************************************************/

/****************************************************************
 *
 * Static function prototypes
 *
 ****************************************************************/
static DDS_DomainParticipant
splice_data_queue_create_domain_participant(const DDS_DomainId_t domain_id);

static DDS_Publisher
splice_data_queue_create_publisher(DDS_DomainParticipant dp,
                                   char* write_partition);

static DDS_Subscriber
splice_data_queue_create_subscriber(DDS_DomainParticipant dp,
                                    char* read_partition);

static DDS_Topic
splice_data_queue_create_topic(DDS_DomainParticipant domain_participant,
                               queue_sampleTypeSupport sample_type);

static void
splice_data_queue_set_wait_state(splice_data_queue_handle p_handle);

static DDS_DataWriter
splice_data_queue_create_writer(DDS_Publisher publisher,
                                DDS_Topic topic);

static DDS_DataReader
splice_data_queue_create_reader(DDS_Subscriber subscriber,
                                DDS_Topic topic);

static void
splice_data_queue_process_return_code(char *functionname,
                                      DDS_ReturnCode_t result);


/****************************************************************
 *
 * Global functions
 *
 ****************************************************************/
splice_data_queue_handle
splice_data_queue_create(DDS_DomainId_t domain_id)
{
    splice_data_queue_handle p_handle = NULL;

    p_handle = (splice_data_queue_handle) calloc(1, sizeof(*p_handle));
    p_handle->domain_participant = splice_data_queue_create_domain_participant(domain_id);

    p_handle->sample_data_type = queue_sampleTypeSupport__alloc();
    p_handle->topic = splice_data_queue_create_topic(p_handle->domain_participant, p_handle->sample_data_type);

    p_handle->publisher = splice_data_queue_create_publisher(p_handle->domain_participant, DEFAULT_PARTITION);
    p_handle->writer = splice_data_queue_create_writer(p_handle->publisher, p_handle->topic);

    p_handle->subscriber = splice_data_queue_create_subscriber(p_handle->domain_participant, DEFAULT_PARTITION);
    p_handle->reader = splice_data_queue_create_reader(p_handle->subscriber, p_handle->topic);
    splice_data_queue_set_wait_state(p_handle);

    return p_handle;
}

void
splice_data_queue_destroy(splice_data_queue_handle const p_handle)
{
    DDS_ReturnCode_t result = DDS_RETCODE_OK;
    DDS_DomainParticipantFactory domain_participant_factory = DDS_OBJECT_NIL;

    if(p_handle != NULL)
    {
        DDS_free(p_handle->waitset);
        DDS_Subscriber_delete_datareader(p_handle->subscriber, p_handle->reader);
        DDS_Publisher_delete_datawriter(p_handle->publisher, p_handle->writer);

        result = DDS_DomainParticipant_delete_topic(p_handle->domain_participant, p_handle->topic);
        splice_data_queue_process_return_code("DDS_DomainParticipant_delete_topic", result);

        DDS_free(p_handle->sample_data_type);
        result = DDS_DomainParticipant_delete_subscriber(p_handle->domain_participant, p_handle->subscriber);
        splice_data_queue_process_return_code("DDS_DomainParticipant_delete_subscriber", result);

        result = DDS_DomainParticipant_delete_publisher(p_handle->domain_participant, p_handle->publisher);
        splice_data_queue_process_return_code("DDS_DomainParticipant_delete_publisher", result);

        domain_participant_factory = DDS_DomainParticipantFactory_get_instance();
        result = DDS_DomainParticipantFactory_delete_participant(domain_participant_factory, p_handle->domain_participant);
        splice_data_queue_process_return_code("DDS_DomainParticipantFactory_delete_participant", result);

        free(p_handle);
    }
}

void
splice_data_queue_put(splice_data_queue_handle const p_handle, const struct queue_sample *sample)
{
    DDS_ReturnCode_t result = DDS_RETCODE_OK;

    result = queue_sampleDataWriter_write(p_handle->writer, sample, DDS_HANDLE_NIL);
    splice_data_queue_process_return_code("queue_sampleDataWriter_write", result);
    if(result != DDS_RETCODE_OK)
    {
        printf("Write returned an ERROR\n");
        exit(1);
    }
    result = queue_sampleDataWriter_unregister_instance (p_handle->writer, sample, DDS_HANDLE_NIL);
    splice_data_queue_process_return_code("queue_sampleDataWriter_unregister_instance (put)", result);
}

void
splice_data_queue_get_one(splice_data_queue_handle const p_handle, struct queue_sample *sample)
{
    DDS_ReturnCode_t result = DDS_RETCODE_OK;
    DDS_ConditionSeq *conditionList = NULL;
    DDS_Duration_t wait_timeout = {0, 1}; /*DDS_DURATION_INFINITE;*/

    DDS_sequence_queue_sample sample_sequence = {0, 0, DDS_OBJECT_NIL, FALSE};
    DDS_SampleInfoSeq infoList = {0, 0, DDS_OBJECT_NIL, FALSE};
/*
    conditionList = DDS_ConditionSeq__alloc();
    result = DDS_WaitSet_wait(p_handle->waitset, conditionList, &wait_timeout);

    splice_data_queue_process_return_code("DDS_WaitSet_wait (one)", result);
    if(result != DDS_RETCODE_OK)
    {
        printf("WaitSet_wait returned an ERROR (one)\n");
        exit(2);
    }

    DDS_free(conditionList);
*/
    do
    {
        sample_sequence._maximum = 0;
        sample_sequence._length = 0;
        sample_sequence._buffer = DDS_OBJECT_NIL;
        sample_sequence._release = FALSE;
        infoList._maximum = 0;
        infoList._length = 0;
        infoList._buffer = DDS_OBJECT_NIL;
        infoList._release = FALSE;
        
        result = queue_sampleDataReader_take(p_handle->reader, &sample_sequence, &infoList, 1, DDS_NOT_READ_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE);
        if(result != DDS_RETCODE_OK)
        {
            conditionList = DDS_ConditionSeq__alloc();
            result = DDS_WaitSet_wait(p_handle->waitset, conditionList, &wait_timeout);
            DDS_free(conditionList);
        }
    }
    while(result != DDS_RETCODE_OK);
/*
    splice_data_queue_process_return_code("queue_sampleDataReader_take (one)", result);
    if(result != DDS_RETCODE_OK)
    {
        printf("Take returned an ERROR\n");
        exit(3);
    }
*/
    if(sample_sequence._length > 0)
    {
        *sample =  sample_sequence._buffer[0];
        result = queue_sampleDataWriter_dispose(p_handle->writer, &sample_sequence._buffer[0], DDS_HANDLE_NIL);
        splice_data_queue_process_return_code("queue_sampleDataWriter_dispose (one)", result);
        result = queue_sampleDataWriter_unregister_instance (p_handle->writer, &sample_sequence._buffer[0], DDS_HANDLE_NIL);
        splice_data_queue_process_return_code("queue_sampleDataWriter_unregister_instance (one)", result);
    }
    result = queue_sampleDataReader_return_loan (p_handle->reader, &sample_sequence, &infoList);
    splice_data_queue_process_return_code("queue_sampleDataReader_return_loan (one)", result);
}

int
splice_data_queue_get_multiple(splice_data_queue_handle const p_handle, int num_samples, struct queue_sample *sample)
{
    DDS_ReturnCode_t result = DDS_RETCODE_OK;
    DDS_ConditionSeq *conditionList = NULL;
    DDS_Duration_t wait_timeout = DDS_DURATION_INFINITE;

    int read_samples = 0;

    conditionList = DDS_ConditionSeq__alloc();
    result = DDS_WaitSet_wait(p_handle->waitset, conditionList, &wait_timeout);
    splice_data_queue_process_return_code("DDS_WaitSet_wait (multiple)", result);
    if(result != DDS_RETCODE_OK)
    {
        printf("WaitSet_wait returned an ERROR (multiple)\n");
        exit(4);
    }
    DDS_free(conditionList);

    DDS_sequence_queue_sample sample_sequence = {0, 0, DDS_OBJECT_NIL, FALSE};
    DDS_SampleInfoSeq infoList = {0, 0, DDS_OBJECT_NIL, FALSE};

    result = queue_sampleDataReader_take(p_handle->reader, &sample_sequence, &infoList, num_samples, DDS_NOT_READ_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE);
    splice_data_queue_process_return_code("queue_sampleDataReader_take (multiple)", result);
    if(result == DDS_RETCODE_OK)
    {
        for(read_samples = 0; read_samples < sample_sequence._length; read_samples++)
        {
            sample[read_samples] = sample_sequence._buffer[read_samples];
            result = queue_sampleDataWriter_dispose(p_handle->writer, &sample_sequence._buffer[read_samples], DDS_HANDLE_NIL);
            splice_data_queue_process_return_code("queue_sampleDataWriter_dispose (multiple)", result);
            result = queue_sampleDataWriter_unregister_instance (p_handle->writer, &sample_sequence._buffer[read_samples], DDS_HANDLE_NIL);
            splice_data_queue_process_return_code("queue_sampleDataWriter_unregister_instance (multiple)", result);
        }
    }
    result = queue_sampleDataReader_return_loan (p_handle->reader, &sample_sequence, &infoList);
    splice_data_queue_process_return_code("queue_sampleDataReader_return_loan (multiple)", result);

    return read_samples;
}

void
splice_data_queue_number_of_samples(splice_data_queue_handle const p_handle)
{
    DDS_ReturnCode_t result = DDS_RETCODE_OK;
    DDS_sequence_queue_sample sample_sequence = {0, 0, DDS_OBJECT_NIL, FALSE};
    DDS_SampleInfoSeq infoList = {0, 0, DDS_OBJECT_NIL, FALSE};
    int length = 0;

    result = queue_sampleDataReader_read(p_handle->reader, &sample_sequence, &infoList, DDS_LENGTH_UNLIMITED, DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);
    splice_data_queue_process_return_code("queue_sampleDataReader_read", result);

    length =  sample_sequence._length;
    printf("QUEUE: Number of Samples: %d\n", length);
    result = queue_sampleDataReader_return_loan (p_handle->reader, &sample_sequence, &infoList);
    splice_data_queue_process_return_code("queue_sampleDataReader_return_loan (one)", result);
}

/****************************************************************
 *
 * Local functions
 *
 ****************************************************************/
static DDS_DomainParticipant
splice_data_queue_create_domain_participant(const DDS_DomainId_t domain_id)
{
    DDS_DomainParticipantFactory domain_participant_factory = DDS_OBJECT_NIL;
    DDS_DomainParticipant dp = DDS_OBJECT_NIL;

    domain_participant_factory = DDS_DomainParticipantFactory_get_instance();
    printf("QUEUE: Connect to domain %d\n", domain_id);

    dp = DDS_DomainParticipantFactory_create_participant(domain_participant_factory, domain_id, DDS_PARTICIPANT_QOS_DEFAULT, NULL, DDS_STATUS_MASK_NONE);
    if (dp == DDS_HANDLE_NIL)
    {
        printf("QUEUE: ERROR - Allocation Domain Participant failed\n");
        exit(5);
    }

    return dp;
}

static DDS_Publisher
splice_data_queue_create_publisher(DDS_DomainParticipant dp,
                                   char* write_partition)
{
    DDS_Publisher publisher = DDS_OBJECT_NIL;
    DDS_PublisherQos *pQos = NULL;
    DDS_ReturnCode_t result = DDS_RETCODE_OK;

    pQos = DDS_PublisherQos__alloc();
    result = DDS_DomainParticipant_get_default_publisher_qos(dp, pQos);
    splice_data_queue_process_return_code("DDS_DomainParticipant_get_default_publisher_qos", result);
    pQos->partition.name._length = PARTITION_LENGTH;
    pQos->partition.name._maximum = PARTITION_LENGTH;
    pQos->partition.name._buffer = DDS_StringSeq_allocbuf(PARTITION_LENGTH);
    pQos->partition.name._buffer[0] = DDS_string_dup(write_partition);
    publisher = DDS_DomainParticipant_create_publisher(dp, pQos, NULL, DDS_STATUS_MASK_NONE);
    if(publisher == DDS_HANDLE_NIL)
    {
        printf("QUEUE: ERROR - Allocation publisher failed\n");
        exit(6);
    }
    DDS_free(pQos);

    return publisher;
}

static DDS_Subscriber
splice_data_queue_create_subscriber(DDS_DomainParticipant dp,
                                    char* read_partition)
{
    DDS_Subscriber subscriber = DDS_OBJECT_NIL;
    DDS_SubscriberQos *sQos = NULL;
    DDS_ReturnCode_t result = DDS_RETCODE_OK;

    sQos = DDS_SubscriberQos__alloc();
    result = DDS_DomainParticipant_get_default_subscriber_qos(dp, sQos);
    splice_data_queue_process_return_code("DDS_DomainParticipant_get_default_subscriber_qos", result);
    sQos->partition.name._length = PARTITION_LENGTH;
    sQos->partition.name._maximum = PARTITION_LENGTH;
    sQos->partition.name._buffer = DDS_StringSeq_allocbuf(PARTITION_LENGTH);
    sQos->partition.name._buffer[0] = DDS_string_dup(read_partition);
    subscriber = DDS_DomainParticipant_create_subscriber(dp, sQos, NULL, DDS_STATUS_MASK_NONE);

    if(subscriber == DDS_HANDLE_NIL)
    {
        printf("QUEUE: ERROR - Allocation subscriber failed\n");
        exit(7);
    }
    DDS_free(sQos);

    return subscriber;
}

static DDS_Topic
splice_data_queue_create_topic(DDS_DomainParticipant domain_participant,
                               queue_sampleTypeSupport sample_type)
{
    DDS_Topic topic = DDS_OBJECT_NIL;
    DDS_ReturnCode_t result = DDS_RETCODE_OK;
    DDS_TopicQos *topicQos = DDS_OBJECT_NIL;    

    result = queue_sampleTypeSupport_register_type(sample_type, domain_participant, TOPIC_TYPE);
    splice_data_queue_process_return_code("queue_sampleTypeSupport_register_type", result);
    
    topicQos = DDS_TopicQos__alloc();
    DDS_DomainParticipant_get_default_topic_qos(domain_participant, topicQos);
    topicQos->durability.kind = DDS_TRANSIENT_DURABILITY_QOS;
    topicQos->history.kind = HISTORY;
    topic = DDS_DomainParticipant_create_topic(domain_participant, TOPIC_NAME, TOPIC_TYPE, topicQos, NULL, DDS_STATUS_MASK_NONE);
    if(topic == DDS_HANDLE_NIL)
    {
        printf("QUEUE: ERROR - Allocation topic failed\n");
        exit(8);
    }
    DDS_free(topicQos);

    return topic;
}

static void
splice_data_queue_set_wait_state(splice_data_queue_handle p_handle)
{
    DDS_ReturnCode_t result = DDS_RETCODE_OK;

    DDS_StatusCondition status_condition = DDS_DataReader_get_statuscondition(p_handle->reader);
    DDS_StatusCondition_set_enabled_statuses(status_condition, DDS_DATA_AVAILABLE_STATUS);

    p_handle->waitset = DDS_WaitSet__alloc();
    if(p_handle->waitset == DDS_HANDLE_NIL)
    {
        printf("QUEUE: ERROR - Allocation waitset failed\n");
        exit(9);
    }
 
    result = DDS_WaitSet_attach_condition(p_handle->waitset, status_condition);
    splice_data_queue_process_return_code("DDS_WaitSet_attach_condition", result);
}

static DDS_DataWriter
splice_data_queue_create_writer(DDS_Publisher publisher,
                                DDS_Topic topic)
{
    DDS_DataWriter writer = DDS_OBJECT_NIL;
    DDS_DataWriterQos *dwQos = NULL;
    DDS_ReturnCode_t result = DDS_RETCODE_OK;

    dwQos = DDS_DataWriterQos__alloc();

    result = DDS_Publisher_get_default_datawriter_qos(publisher, dwQos);
    splice_data_queue_process_return_code("DDS_Publisher_get_default_datawriter_qos", result);
    dwQos->reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
    dwQos->history.kind = HISTORY;
    dwQos->durability.kind = DDS_TRANSIENT_DURABILITY_QOS;
    dwQos->writer_data_lifecycle.autodispose_unregistered_instances = FALSE;

    writer = DDS_Publisher_create_datawriter(publisher, topic, dwQos, NULL, DDS_STATUS_MASK_NONE);
    if(writer == DDS_HANDLE_NIL)
    {
        printf("QUEUE: ERROR - Allocation writer failed\n");
        exit(10);
    }

    DDS_free(dwQos);

    return writer;
}

static DDS_DataReader
splice_data_queue_create_reader(DDS_Subscriber subscriber,
                                DDS_Topic topic)
{
    DDS_DataReader reader = DDS_OBJECT_NIL;
    DDS_DataReaderQos *drQos = NULL;
    DDS_ReturnCode_t result = DDS_RETCODE_OK;
    
    drQos = DDS_DataReaderQos__alloc();

    result = DDS_Subscriber_get_default_datareader_qos (subscriber, drQos);
    splice_data_queue_process_return_code("DDS_Subscriber_get_default_datareader_qos", result);
    drQos->history.kind = HISTORY;
    drQos->durability.kind = DDS_TRANSIENT_DURABILITY_QOS;
    drQos->reader_data_lifecycle.invalid_sample_visibility.kind = _DDS_NO_INVALID_SAMPLES;

    reader = DDS_Subscriber_create_datareader(subscriber, topic, drQos, NULL, DDS_STATUS_MASK_NONE);
    if(reader == DDS_HANDLE_NIL)
    {
        printf("QUEUE: ERROR - Allocation reader failed\n");
        exit(11);
    }

    DDS_free(drQos); 

    return reader;
}

static void
splice_data_queue_process_return_code(char *functionname,
                                      DDS_ReturnCode_t result)
{
    if(result != DDS_RETCODE_OK)
    {
        printf("QUEUE: %s, ", functionname);
    }  
    switch(result)
    {
    case DDS_RETCODE_OK:
        /* No error */
        break;
    case DDS_RETCODE_ERROR:
        printf("DDS_RETCODE_ERROR\n");
        break;
    case DDS_RETCODE_UNSUPPORTED:
        printf("DDS_RETCODE_UNSUPPORTED\n");
        break;
    case DDS_RETCODE_BAD_PARAMETER:
        printf("DDS_RETCODE_BAD_PARAMETER\n");
        break;
    case DDS_RETCODE_PRECONDITION_NOT_MET:
        printf("DDS_RETCODE_PRECONDITION_NOT_MET\n");
        break;
    case DDS_RETCODE_OUT_OF_RESOURCES:
        printf("DDS_RETCODE_OUT_OF_RESOURCES\n");
        break;
    case DDS_RETCODE_NOT_ENABLED:
        printf("DDS_RETCODE_NOT_ENABLED\n");
        break;
    case DDS_RETCODE_IMMUTABLE_POLICY:
        printf("DDS_RETCODE_IMMUTABLE_POLICY\n");
        break;
    case DDS_RETCODE_INCONSISTENT_POLICY:
        printf("DDS_RETCODE_INCONSISTENT_POLICY\n");
        break;
    case DDS_RETCODE_ALREADY_DELETED:
        printf("DDS_RETCODE_ALREADY_DELETED\n");
        break;
    case DDS_RETCODE_TIMEOUT:
        printf("DDS_RETCODE_TIMEOUT\n");
        break;
    case DDS_RETCODE_NO_DATA:
        printf("DDS_RETCODE_ERROR\n");
        break;
    case DDS_RETCODE_ILLEGAL_OPERATION:
        printf("DDS_RETCODE_ERROR\n");
        break;
    default:
        printf("Unknown return value\n");
        break;
    }
}


