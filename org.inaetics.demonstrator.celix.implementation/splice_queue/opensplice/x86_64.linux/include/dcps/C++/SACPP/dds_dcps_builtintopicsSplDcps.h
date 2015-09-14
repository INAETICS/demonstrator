#ifndef DDS_DCPS_BUILTINTOPICSSPLTYPES_H
#define DDS_DCPS_BUILTINTOPICSSPLTYPES_H

#include "ccpp_dds_dcps_builtintopics.h"

#include <c_base.h>
#include <c_misc.h>
#include <c_sync.h>
#include <c_collection.h>
#include <c_field.h>

#include "dds_builtinTopicsSplDcps.h"

extern c_metaObject __dds_dcps_builtintopics_DDS__load (c_base base);

extern c_metaObject __DDS_Time_t__load (c_base base);
extern const char * __DDS_Time_t__keys (void);
extern const char * __DDS_Time_t__name (void);
struct _DDS_Time_t ;
extern  c_bool __DDS_Time_t__copyIn(c_base base, struct DDS::Time_t *from, struct _DDS_Time_t *to);
extern  void __DDS_Time_t__copyOut(void *_from, void *_to);
struct _DDS_Time_t {
    c_long sec;
    c_ulong nanosec;
};

extern c_metaObject __DDS_EntityFactoryQosPolicy__load (c_base base);
extern const char * __DDS_EntityFactoryQosPolicy__keys (void);
extern const char * __DDS_EntityFactoryQosPolicy__name (void);
struct _DDS_EntityFactoryQosPolicy ;
extern  c_bool __DDS_EntityFactoryQosPolicy__copyIn(c_base base, struct DDS::EntityFactoryQosPolicy *from, struct _DDS_EntityFactoryQosPolicy *to);
extern  void __DDS_EntityFactoryQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_EntityFactoryQosPolicy {
    c_bool autoenable_created_entities;
};

extern c_metaObject __DDS_WriterDataLifecycleQosPolicy__load (c_base base);
extern const char * __DDS_WriterDataLifecycleQosPolicy__keys (void);
extern const char * __DDS_WriterDataLifecycleQosPolicy__name (void);
struct _DDS_WriterDataLifecycleQosPolicy ;
extern  c_bool __DDS_WriterDataLifecycleQosPolicy__copyIn(c_base base, struct DDS::WriterDataLifecycleQosPolicy *from, struct _DDS_WriterDataLifecycleQosPolicy *to);
extern  void __DDS_WriterDataLifecycleQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_WriterDataLifecycleQosPolicy {
    c_bool autodispose_unregistered_instances;
    struct _DDS_Duration_t autopurge_suspended_samples_delay;
    struct _DDS_Duration_t autounregister_instance_delay;
};

extern c_metaObject __DDS_InvalidSampleVisibilityQosPolicyKind__load (c_base base);
enum _DDS_InvalidSampleVisibilityQosPolicyKind {
    _DDS_NO_INVALID_SAMPLES,
    _DDS_MINIMUM_INVALID_SAMPLES,
    _DDS_ALL_INVALID_SAMPLES
};

extern c_metaObject __DDS_InvalidSampleVisibilityQosPolicy__load (c_base base);
extern const char * __DDS_InvalidSampleVisibilityQosPolicy__keys (void);
extern const char * __DDS_InvalidSampleVisibilityQosPolicy__name (void);
struct _DDS_InvalidSampleVisibilityQosPolicy ;
extern  c_bool __DDS_InvalidSampleVisibilityQosPolicy__copyIn(c_base base, struct DDS::InvalidSampleVisibilityQosPolicy *from, struct _DDS_InvalidSampleVisibilityQosPolicy *to);
extern  void __DDS_InvalidSampleVisibilityQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_InvalidSampleVisibilityQosPolicy {
    enum _DDS_InvalidSampleVisibilityQosPolicyKind kind;
};

extern c_metaObject __DDS_ReaderDataLifecycleQosPolicy__load (c_base base);
extern const char * __DDS_ReaderDataLifecycleQosPolicy__keys (void);
extern const char * __DDS_ReaderDataLifecycleQosPolicy__name (void);
struct _DDS_ReaderDataLifecycleQosPolicy ;
extern  c_bool __DDS_ReaderDataLifecycleQosPolicy__copyIn(c_base base, struct DDS::ReaderDataLifecycleQosPolicy *from, struct _DDS_ReaderDataLifecycleQosPolicy *to);
extern  void __DDS_ReaderDataLifecycleQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_ReaderDataLifecycleQosPolicy {
    struct _DDS_Duration_t autopurge_nowriter_samples_delay;
    struct _DDS_Duration_t autopurge_disposed_samples_delay;
    c_bool enable_invalid_samples;
    struct _DDS_InvalidSampleVisibilityQosPolicy invalid_sample_visibility;
};

extern c_metaObject __DDS_SubscriptionKeyQosPolicy__load (c_base base);
extern const char * __DDS_SubscriptionKeyQosPolicy__keys (void);
extern const char * __DDS_SubscriptionKeyQosPolicy__name (void);
struct _DDS_SubscriptionKeyQosPolicy ;
extern  c_bool __DDS_SubscriptionKeyQosPolicy__copyIn(c_base base, struct DDS::SubscriptionKeyQosPolicy *from, struct _DDS_SubscriptionKeyQosPolicy *to);
extern  void __DDS_SubscriptionKeyQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_SubscriptionKeyQosPolicy {
    c_bool use_key_list;
    _DDS_StringSeq key_list;
};

extern c_metaObject __DDS_ReaderLifespanQosPolicy__load (c_base base);
extern const char * __DDS_ReaderLifespanQosPolicy__keys (void);
extern const char * __DDS_ReaderLifespanQosPolicy__name (void);
struct _DDS_ReaderLifespanQosPolicy ;
extern  c_bool __DDS_ReaderLifespanQosPolicy__copyIn(c_base base, struct DDS::ReaderLifespanQosPolicy *from, struct _DDS_ReaderLifespanQosPolicy *to);
extern  void __DDS_ReaderLifespanQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_ReaderLifespanQosPolicy {
    c_bool use_lifespan;
    struct _DDS_Duration_t duration;
};

extern c_metaObject __DDS_ShareQosPolicy__load (c_base base);
extern const char * __DDS_ShareQosPolicy__keys (void);
extern const char * __DDS_ShareQosPolicy__name (void);
struct _DDS_ShareQosPolicy ;
extern  c_bool __DDS_ShareQosPolicy__copyIn(c_base base, struct DDS::ShareQosPolicy *from, struct _DDS_ShareQosPolicy *to);
extern  void __DDS_ShareQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_ShareQosPolicy {
    c_string name;
    c_bool enable;
};

extern c_metaObject __DDS_SchedulingClassQosPolicyKind__load (c_base base);
enum _DDS_SchedulingClassQosPolicyKind {
    _DDS_SCHEDULE_DEFAULT,
    _DDS_SCHEDULE_TIMESHARING,
    _DDS_SCHEDULE_REALTIME
};

extern c_metaObject __DDS_SchedulingClassQosPolicy__load (c_base base);
extern const char * __DDS_SchedulingClassQosPolicy__keys (void);
extern const char * __DDS_SchedulingClassQosPolicy__name (void);
struct _DDS_SchedulingClassQosPolicy ;
extern  c_bool __DDS_SchedulingClassQosPolicy__copyIn(c_base base, struct DDS::SchedulingClassQosPolicy *from, struct _DDS_SchedulingClassQosPolicy *to);
extern  void __DDS_SchedulingClassQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_SchedulingClassQosPolicy {
    enum _DDS_SchedulingClassQosPolicyKind kind;
};

extern c_metaObject __DDS_SchedulingPriorityQosPolicyKind__load (c_base base);
enum _DDS_SchedulingPriorityQosPolicyKind {
    _DDS_PRIORITY_RELATIVE,
    _DDS_PRIORITY_ABSOLUTE
};

extern c_metaObject __DDS_SchedulingPriorityQosPolicy__load (c_base base);
extern const char * __DDS_SchedulingPriorityQosPolicy__keys (void);
extern const char * __DDS_SchedulingPriorityQosPolicy__name (void);
struct _DDS_SchedulingPriorityQosPolicy ;
extern  c_bool __DDS_SchedulingPriorityQosPolicy__copyIn(c_base base, struct DDS::SchedulingPriorityQosPolicy *from, struct _DDS_SchedulingPriorityQosPolicy *to);
extern  void __DDS_SchedulingPriorityQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_SchedulingPriorityQosPolicy {
    enum _DDS_SchedulingPriorityQosPolicyKind kind;
};

extern c_metaObject __DDS_SchedulingQosPolicy__load (c_base base);
extern const char * __DDS_SchedulingQosPolicy__keys (void);
extern const char * __DDS_SchedulingQosPolicy__name (void);
struct _DDS_SchedulingQosPolicy ;
extern  c_bool __DDS_SchedulingQosPolicy__copyIn(c_base base, struct DDS::SchedulingQosPolicy *from, struct _DDS_SchedulingQosPolicy *to);
extern  void __DDS_SchedulingQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_SchedulingQosPolicy {
    struct _DDS_SchedulingClassQosPolicy scheduling_class;
    struct _DDS_SchedulingPriorityQosPolicy scheduling_priority_kind;
    c_long scheduling_priority;
};

extern c_metaObject __DDS_ViewKeyQosPolicy__load (c_base base);
extern const char * __DDS_ViewKeyQosPolicy__keys (void);
extern const char * __DDS_ViewKeyQosPolicy__name (void);
struct _DDS_ViewKeyQosPolicy ;
extern  c_bool __DDS_ViewKeyQosPolicy__copyIn(c_base base, struct DDS::ViewKeyQosPolicy *from, struct _DDS_ViewKeyQosPolicy *to);
extern  void __DDS_ViewKeyQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_ViewKeyQosPolicy {
    c_bool use_key_list;
    _DDS_StringSeq key_list;
};

extern c_metaObject __DDS_DataReaderViewQos__load (c_base base);
extern const char * __DDS_DataReaderViewQos__keys (void);
extern const char * __DDS_DataReaderViewQos__name (void);
struct _DDS_DataReaderViewQos ;
extern  c_bool __DDS_DataReaderViewQos__copyIn(c_base base, struct DDS::DataReaderViewQos *from, struct _DDS_DataReaderViewQos *to);
extern  void __DDS_DataReaderViewQos__copyOut(void *_from, void *_to);
struct _DDS_DataReaderViewQos {
    struct _DDS_ViewKeyQosPolicy view_keys;
};

extern c_metaObject __DDS_DomainParticipantFactoryQos__load (c_base base);
extern const char * __DDS_DomainParticipantFactoryQos__keys (void);
extern const char * __DDS_DomainParticipantFactoryQos__name (void);
struct _DDS_DomainParticipantFactoryQos ;
extern  c_bool __DDS_DomainParticipantFactoryQos__copyIn(c_base base, struct DDS::DomainParticipantFactoryQos *from, struct _DDS_DomainParticipantFactoryQos *to);
extern  void __DDS_DomainParticipantFactoryQos__copyOut(void *_from, void *_to);
struct _DDS_DomainParticipantFactoryQos {
    struct _DDS_EntityFactoryQosPolicy entity_factory;
};

extern c_metaObject __DDS_DomainParticipantQos__load (c_base base);
extern const char * __DDS_DomainParticipantQos__keys (void);
extern const char * __DDS_DomainParticipantQos__name (void);
struct _DDS_DomainParticipantQos ;
extern  c_bool __DDS_DomainParticipantQos__copyIn(c_base base, struct DDS::DomainParticipantQos *from, struct _DDS_DomainParticipantQos *to);
extern  void __DDS_DomainParticipantQos__copyOut(void *_from, void *_to);
struct _DDS_DomainParticipantQos {
    struct _DDS_UserDataQosPolicy user_data;
    struct _DDS_EntityFactoryQosPolicy entity_factory;
    struct _DDS_SchedulingQosPolicy watchdog_scheduling;
    struct _DDS_SchedulingQosPolicy listener_scheduling;
};

extern c_metaObject __DDS_TopicQos__load (c_base base);
extern const char * __DDS_TopicQos__keys (void);
extern const char * __DDS_TopicQos__name (void);
struct _DDS_TopicQos ;
extern  c_bool __DDS_TopicQos__copyIn(c_base base, struct DDS::TopicQos *from, struct _DDS_TopicQos *to);
extern  void __DDS_TopicQos__copyOut(void *_from, void *_to);
struct _DDS_TopicQos {
    struct _DDS_TopicDataQosPolicy topic_data;
    struct _DDS_DurabilityQosPolicy durability;
    struct _DDS_DurabilityServiceQosPolicy durability_service;
    struct _DDS_DeadlineQosPolicy deadline;
    struct _DDS_LatencyBudgetQosPolicy latency_budget;
    struct _DDS_LivelinessQosPolicy liveliness;
    struct _DDS_ReliabilityQosPolicy reliability;
    struct _DDS_DestinationOrderQosPolicy destination_order;
    struct _DDS_HistoryQosPolicy history;
    struct _DDS_ResourceLimitsQosPolicy resource_limits;
    struct _DDS_TransportPriorityQosPolicy transport_priority;
    struct _DDS_LifespanQosPolicy lifespan;
    struct _DDS_OwnershipQosPolicy ownership;
};

extern c_metaObject __DDS_DataWriterQos__load (c_base base);
extern const char * __DDS_DataWriterQos__keys (void);
extern const char * __DDS_DataWriterQos__name (void);
struct _DDS_DataWriterQos ;
extern  c_bool __DDS_DataWriterQos__copyIn(c_base base, struct DDS::DataWriterQos *from, struct _DDS_DataWriterQos *to);
extern  void __DDS_DataWriterQos__copyOut(void *_from, void *_to);
struct _DDS_DataWriterQos {
    struct _DDS_DurabilityQosPolicy durability;
    struct _DDS_DeadlineQosPolicy deadline;
    struct _DDS_LatencyBudgetQosPolicy latency_budget;
    struct _DDS_LivelinessQosPolicy liveliness;
    struct _DDS_ReliabilityQosPolicy reliability;
    struct _DDS_DestinationOrderQosPolicy destination_order;
    struct _DDS_HistoryQosPolicy history;
    struct _DDS_ResourceLimitsQosPolicy resource_limits;
    struct _DDS_TransportPriorityQosPolicy transport_priority;
    struct _DDS_LifespanQosPolicy lifespan;
    struct _DDS_UserDataQosPolicy user_data;
    struct _DDS_OwnershipQosPolicy ownership;
    struct _DDS_OwnershipStrengthQosPolicy ownership_strength;
    struct _DDS_WriterDataLifecycleQosPolicy writer_data_lifecycle;
};

extern c_metaObject __DDS_PublisherQos__load (c_base base);
extern const char * __DDS_PublisherQos__keys (void);
extern const char * __DDS_PublisherQos__name (void);
struct _DDS_PublisherQos ;
extern  c_bool __DDS_PublisherQos__copyIn(c_base base, struct DDS::PublisherQos *from, struct _DDS_PublisherQos *to);
extern  void __DDS_PublisherQos__copyOut(void *_from, void *_to);
struct _DDS_PublisherQos {
    struct _DDS_PresentationQosPolicy presentation;
    struct _DDS_PartitionQosPolicy partition;
    struct _DDS_GroupDataQosPolicy group_data;
    struct _DDS_EntityFactoryQosPolicy entity_factory;
};

extern c_metaObject __DDS_DataReaderQos__load (c_base base);
extern const char * __DDS_DataReaderQos__keys (void);
extern const char * __DDS_DataReaderQos__name (void);
struct _DDS_DataReaderQos ;
extern  c_bool __DDS_DataReaderQos__copyIn(c_base base, struct DDS::DataReaderQos *from, struct _DDS_DataReaderQos *to);
extern  void __DDS_DataReaderQos__copyOut(void *_from, void *_to);
struct _DDS_DataReaderQos {
    struct _DDS_DurabilityQosPolicy durability;
    struct _DDS_DeadlineQosPolicy deadline;
    struct _DDS_LatencyBudgetQosPolicy latency_budget;
    struct _DDS_LivelinessQosPolicy liveliness;
    struct _DDS_ReliabilityQosPolicy reliability;
    struct _DDS_DestinationOrderQosPolicy destination_order;
    struct _DDS_HistoryQosPolicy history;
    struct _DDS_ResourceLimitsQosPolicy resource_limits;
    struct _DDS_UserDataQosPolicy user_data;
    struct _DDS_OwnershipQosPolicy ownership;
    struct _DDS_TimeBasedFilterQosPolicy time_based_filter;
    struct _DDS_ReaderDataLifecycleQosPolicy reader_data_lifecycle;
    struct _DDS_SubscriptionKeyQosPolicy subscription_keys;
    struct _DDS_ReaderLifespanQosPolicy reader_lifespan;
    struct _DDS_ShareQosPolicy share;
};

extern c_metaObject __DDS_SubscriberQos__load (c_base base);
extern const char * __DDS_SubscriberQos__keys (void);
extern const char * __DDS_SubscriberQos__name (void);
struct _DDS_SubscriberQos ;
extern  c_bool __DDS_SubscriberQos__copyIn(c_base base, struct DDS::SubscriberQos *from, struct _DDS_SubscriberQos *to);
extern  void __DDS_SubscriberQos__copyOut(void *_from, void *_to);
struct _DDS_SubscriberQos {
    struct _DDS_PresentationQosPolicy presentation;
    struct _DDS_PartitionQosPolicy partition;
    struct _DDS_GroupDataQosPolicy group_data;
    struct _DDS_EntityFactoryQosPolicy entity_factory;
    struct _DDS_ShareQosPolicy share;
};

#endif
