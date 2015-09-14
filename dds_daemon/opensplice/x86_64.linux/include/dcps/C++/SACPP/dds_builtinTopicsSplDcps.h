#ifndef DDS_BUILTINTOPICSSPLTYPES_H
#define DDS_BUILTINTOPICSSPLTYPES_H

#include "ccpp_dds_builtinTopics.h"

#include <c_base.h>
#include <c_misc.h>
#include <c_sync.h>
#include <c_collection.h>
#include <c_field.h>

extern c_metaObject __dds_builtinTopics_DDS__load (c_base base);

extern c_metaObject __DDS_octSeq__load (c_base base);
typedef c_sequence _DDS_octSeq;

extern c_metaObject __DDS_BuiltinTopicKey_t__load (c_base base);
typedef c_long _DDS_BuiltinTopicKey_t[3];

extern c_metaObject __DDS_StringSeq__load (c_base base);
typedef c_sequence _DDS_StringSeq;

extern c_metaObject __DDS_Duration_t__load (c_base base);
extern const char * __DDS_Duration_t__keys (void);
extern const char * __DDS_Duration_t__name (void);
struct _DDS_Duration_t ;
extern  c_bool __DDS_Duration_t__copyIn(c_base base, struct DDS::Duration_t *from, struct _DDS_Duration_t *to);
extern  void __DDS_Duration_t__copyOut(void *_from, void *_to);
struct _DDS_Duration_t {
    c_long sec;
    c_ulong nanosec;
};

extern c_metaObject __DDS_UserDataQosPolicy__load (c_base base);
extern const char * __DDS_UserDataQosPolicy__keys (void);
extern const char * __DDS_UserDataQosPolicy__name (void);
struct _DDS_UserDataQosPolicy ;
extern  c_bool __DDS_UserDataQosPolicy__copyIn(c_base base, struct DDS::UserDataQosPolicy *from, struct _DDS_UserDataQosPolicy *to);
extern  void __DDS_UserDataQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_UserDataQosPolicy {
    _DDS_octSeq value;
};

extern c_metaObject __DDS_TopicDataQosPolicy__load (c_base base);
extern const char * __DDS_TopicDataQosPolicy__keys (void);
extern const char * __DDS_TopicDataQosPolicy__name (void);
struct _DDS_TopicDataQosPolicy ;
extern  c_bool __DDS_TopicDataQosPolicy__copyIn(c_base base, struct DDS::TopicDataQosPolicy *from, struct _DDS_TopicDataQosPolicy *to);
extern  void __DDS_TopicDataQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_TopicDataQosPolicy {
    _DDS_octSeq value;
};

extern c_metaObject __DDS_GroupDataQosPolicy__load (c_base base);
extern const char * __DDS_GroupDataQosPolicy__keys (void);
extern const char * __DDS_GroupDataQosPolicy__name (void);
struct _DDS_GroupDataQosPolicy ;
extern  c_bool __DDS_GroupDataQosPolicy__copyIn(c_base base, struct DDS::GroupDataQosPolicy *from, struct _DDS_GroupDataQosPolicy *to);
extern  void __DDS_GroupDataQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_GroupDataQosPolicy {
    _DDS_octSeq value;
};

extern c_metaObject __DDS_TransportPriorityQosPolicy__load (c_base base);
extern const char * __DDS_TransportPriorityQosPolicy__keys (void);
extern const char * __DDS_TransportPriorityQosPolicy__name (void);
struct _DDS_TransportPriorityQosPolicy ;
extern  c_bool __DDS_TransportPriorityQosPolicy__copyIn(c_base base, struct DDS::TransportPriorityQosPolicy *from, struct _DDS_TransportPriorityQosPolicy *to);
extern  void __DDS_TransportPriorityQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_TransportPriorityQosPolicy {
    c_long value;
};

extern c_metaObject __DDS_LifespanQosPolicy__load (c_base base);
extern const char * __DDS_LifespanQosPolicy__keys (void);
extern const char * __DDS_LifespanQosPolicy__name (void);
struct _DDS_LifespanQosPolicy ;
extern  c_bool __DDS_LifespanQosPolicy__copyIn(c_base base, struct DDS::LifespanQosPolicy *from, struct _DDS_LifespanQosPolicy *to);
extern  void __DDS_LifespanQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_LifespanQosPolicy {
    struct _DDS_Duration_t duration;
};

extern c_metaObject __DDS_DurabilityQosPolicyKind__load (c_base base);
enum _DDS_DurabilityQosPolicyKind {
    _DDS_VOLATILE_DURABILITY_QOS,
    _DDS_TRANSIENT_LOCAL_DURABILITY_QOS,
    _DDS_TRANSIENT_DURABILITY_QOS,
    _DDS_PERSISTENT_DURABILITY_QOS
};

extern c_metaObject __DDS_DurabilityQosPolicy__load (c_base base);
extern const char * __DDS_DurabilityQosPolicy__keys (void);
extern const char * __DDS_DurabilityQosPolicy__name (void);
struct _DDS_DurabilityQosPolicy ;
extern  c_bool __DDS_DurabilityQosPolicy__copyIn(c_base base, struct DDS::DurabilityQosPolicy *from, struct _DDS_DurabilityQosPolicy *to);
extern  void __DDS_DurabilityQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_DurabilityQosPolicy {
    enum _DDS_DurabilityQosPolicyKind kind;
};

extern c_metaObject __DDS_PresentationQosPolicyAccessScopeKind__load (c_base base);
enum _DDS_PresentationQosPolicyAccessScopeKind {
    _DDS_INSTANCE_PRESENTATION_QOS,
    _DDS_TOPIC_PRESENTATION_QOS,
    _DDS_GROUP_PRESENTATION_QOS
};

extern c_metaObject __DDS_PresentationQosPolicy__load (c_base base);
extern const char * __DDS_PresentationQosPolicy__keys (void);
extern const char * __DDS_PresentationQosPolicy__name (void);
struct _DDS_PresentationQosPolicy ;
extern  c_bool __DDS_PresentationQosPolicy__copyIn(c_base base, struct DDS::PresentationQosPolicy *from, struct _DDS_PresentationQosPolicy *to);
extern  void __DDS_PresentationQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_PresentationQosPolicy {
    enum _DDS_PresentationQosPolicyAccessScopeKind access_scope;
    c_bool coherent_access;
    c_bool ordered_access;
};

extern c_metaObject __DDS_DeadlineQosPolicy__load (c_base base);
extern const char * __DDS_DeadlineQosPolicy__keys (void);
extern const char * __DDS_DeadlineQosPolicy__name (void);
struct _DDS_DeadlineQosPolicy ;
extern  c_bool __DDS_DeadlineQosPolicy__copyIn(c_base base, struct DDS::DeadlineQosPolicy *from, struct _DDS_DeadlineQosPolicy *to);
extern  void __DDS_DeadlineQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_DeadlineQosPolicy {
    struct _DDS_Duration_t period;
};

extern c_metaObject __DDS_LatencyBudgetQosPolicy__load (c_base base);
extern const char * __DDS_LatencyBudgetQosPolicy__keys (void);
extern const char * __DDS_LatencyBudgetQosPolicy__name (void);
struct _DDS_LatencyBudgetQosPolicy ;
extern  c_bool __DDS_LatencyBudgetQosPolicy__copyIn(c_base base, struct DDS::LatencyBudgetQosPolicy *from, struct _DDS_LatencyBudgetQosPolicy *to);
extern  void __DDS_LatencyBudgetQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_LatencyBudgetQosPolicy {
    struct _DDS_Duration_t duration;
};

extern c_metaObject __DDS_OwnershipQosPolicyKind__load (c_base base);
enum _DDS_OwnershipQosPolicyKind {
    _DDS_SHARED_OWNERSHIP_QOS,
    _DDS_EXCLUSIVE_OWNERSHIP_QOS
};

extern c_metaObject __DDS_OwnershipQosPolicy__load (c_base base);
extern const char * __DDS_OwnershipQosPolicy__keys (void);
extern const char * __DDS_OwnershipQosPolicy__name (void);
struct _DDS_OwnershipQosPolicy ;
extern  c_bool __DDS_OwnershipQosPolicy__copyIn(c_base base, struct DDS::OwnershipQosPolicy *from, struct _DDS_OwnershipQosPolicy *to);
extern  void __DDS_OwnershipQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_OwnershipQosPolicy {
    enum _DDS_OwnershipQosPolicyKind kind;
};

extern c_metaObject __DDS_OwnershipStrengthQosPolicy__load (c_base base);
extern const char * __DDS_OwnershipStrengthQosPolicy__keys (void);
extern const char * __DDS_OwnershipStrengthQosPolicy__name (void);
struct _DDS_OwnershipStrengthQosPolicy ;
extern  c_bool __DDS_OwnershipStrengthQosPolicy__copyIn(c_base base, struct DDS::OwnershipStrengthQosPolicy *from, struct _DDS_OwnershipStrengthQosPolicy *to);
extern  void __DDS_OwnershipStrengthQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_OwnershipStrengthQosPolicy {
    c_long value;
};

extern c_metaObject __DDS_LivelinessQosPolicyKind__load (c_base base);
enum _DDS_LivelinessQosPolicyKind {
    _DDS_AUTOMATIC_LIVELINESS_QOS,
    _DDS_MANUAL_BY_PARTICIPANT_LIVELINESS_QOS,
    _DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS
};

extern c_metaObject __DDS_LivelinessQosPolicy__load (c_base base);
extern const char * __DDS_LivelinessQosPolicy__keys (void);
extern const char * __DDS_LivelinessQosPolicy__name (void);
struct _DDS_LivelinessQosPolicy ;
extern  c_bool __DDS_LivelinessQosPolicy__copyIn(c_base base, struct DDS::LivelinessQosPolicy *from, struct _DDS_LivelinessQosPolicy *to);
extern  void __DDS_LivelinessQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_LivelinessQosPolicy {
    enum _DDS_LivelinessQosPolicyKind kind;
    struct _DDS_Duration_t lease_duration;
};

extern c_metaObject __DDS_TimeBasedFilterQosPolicy__load (c_base base);
extern const char * __DDS_TimeBasedFilterQosPolicy__keys (void);
extern const char * __DDS_TimeBasedFilterQosPolicy__name (void);
struct _DDS_TimeBasedFilterQosPolicy ;
extern  c_bool __DDS_TimeBasedFilterQosPolicy__copyIn(c_base base, struct DDS::TimeBasedFilterQosPolicy *from, struct _DDS_TimeBasedFilterQosPolicy *to);
extern  void __DDS_TimeBasedFilterQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_TimeBasedFilterQosPolicy {
    struct _DDS_Duration_t minimum_separation;
};

extern c_metaObject __DDS_PartitionQosPolicy__load (c_base base);
extern const char * __DDS_PartitionQosPolicy__keys (void);
extern const char * __DDS_PartitionQosPolicy__name (void);
struct _DDS_PartitionQosPolicy ;
extern  c_bool __DDS_PartitionQosPolicy__copyIn(c_base base, struct DDS::PartitionQosPolicy *from, struct _DDS_PartitionQosPolicy *to);
extern  void __DDS_PartitionQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_PartitionQosPolicy {
    _DDS_StringSeq name;
};

extern c_metaObject __DDS_ReliabilityQosPolicyKind__load (c_base base);
enum _DDS_ReliabilityQosPolicyKind {
    _DDS_BEST_EFFORT_RELIABILITY_QOS,
    _DDS_RELIABLE_RELIABILITY_QOS
};

extern c_metaObject __DDS_ReliabilityQosPolicy__load (c_base base);
extern const char * __DDS_ReliabilityQosPolicy__keys (void);
extern const char * __DDS_ReliabilityQosPolicy__name (void);
struct _DDS_ReliabilityQosPolicy ;
extern  c_bool __DDS_ReliabilityQosPolicy__copyIn(c_base base, struct DDS::ReliabilityQosPolicy *from, struct _DDS_ReliabilityQosPolicy *to);
extern  void __DDS_ReliabilityQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_ReliabilityQosPolicy {
    enum _DDS_ReliabilityQosPolicyKind kind;
    struct _DDS_Duration_t max_blocking_time;
    c_bool synchronous;
};

extern c_metaObject __DDS_DestinationOrderQosPolicyKind__load (c_base base);
enum _DDS_DestinationOrderQosPolicyKind {
    _DDS_BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS,
    _DDS_BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS
};

extern c_metaObject __DDS_DestinationOrderQosPolicy__load (c_base base);
extern const char * __DDS_DestinationOrderQosPolicy__keys (void);
extern const char * __DDS_DestinationOrderQosPolicy__name (void);
struct _DDS_DestinationOrderQosPolicy ;
extern  c_bool __DDS_DestinationOrderQosPolicy__copyIn(c_base base, struct DDS::DestinationOrderQosPolicy *from, struct _DDS_DestinationOrderQosPolicy *to);
extern  void __DDS_DestinationOrderQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_DestinationOrderQosPolicy {
    enum _DDS_DestinationOrderQosPolicyKind kind;
};

extern c_metaObject __DDS_HistoryQosPolicyKind__load (c_base base);
enum _DDS_HistoryQosPolicyKind {
    _DDS_KEEP_LAST_HISTORY_QOS,
    _DDS_KEEP_ALL_HISTORY_QOS
};

extern c_metaObject __DDS_HistoryQosPolicy__load (c_base base);
extern const char * __DDS_HistoryQosPolicy__keys (void);
extern const char * __DDS_HistoryQosPolicy__name (void);
struct _DDS_HistoryQosPolicy ;
extern  c_bool __DDS_HistoryQosPolicy__copyIn(c_base base, struct DDS::HistoryQosPolicy *from, struct _DDS_HistoryQosPolicy *to);
extern  void __DDS_HistoryQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_HistoryQosPolicy {
    enum _DDS_HistoryQosPolicyKind kind;
    c_long depth;
};

extern c_metaObject __DDS_ResourceLimitsQosPolicy__load (c_base base);
extern const char * __DDS_ResourceLimitsQosPolicy__keys (void);
extern const char * __DDS_ResourceLimitsQosPolicy__name (void);
struct _DDS_ResourceLimitsQosPolicy ;
extern  c_bool __DDS_ResourceLimitsQosPolicy__copyIn(c_base base, struct DDS::ResourceLimitsQosPolicy *from, struct _DDS_ResourceLimitsQosPolicy *to);
extern  void __DDS_ResourceLimitsQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_ResourceLimitsQosPolicy {
    c_long max_samples;
    c_long max_instances;
    c_long max_samples_per_instance;
};

extern c_metaObject __DDS_DurabilityServiceQosPolicy__load (c_base base);
extern const char * __DDS_DurabilityServiceQosPolicy__keys (void);
extern const char * __DDS_DurabilityServiceQosPolicy__name (void);
struct _DDS_DurabilityServiceQosPolicy ;
extern  c_bool __DDS_DurabilityServiceQosPolicy__copyIn(c_base base, struct DDS::DurabilityServiceQosPolicy *from, struct _DDS_DurabilityServiceQosPolicy *to);
extern  void __DDS_DurabilityServiceQosPolicy__copyOut(void *_from, void *_to);
struct _DDS_DurabilityServiceQosPolicy {
    struct _DDS_Duration_t service_cleanup_delay;
    enum _DDS_HistoryQosPolicyKind history_kind;
    c_long history_depth;
    c_long max_samples;
    c_long max_instances;
    c_long max_samples_per_instance;
};

extern c_metaObject __DDS_ParticipantBuiltinTopicData__load (c_base base);
extern const char * __DDS_ParticipantBuiltinTopicData__keys (void);
extern const char * __DDS_ParticipantBuiltinTopicData__name (void);
struct _DDS_ParticipantBuiltinTopicData ;
extern  c_bool __DDS_ParticipantBuiltinTopicData__copyIn(c_base base, struct DDS::ParticipantBuiltinTopicData *from, struct _DDS_ParticipantBuiltinTopicData *to);
extern  void __DDS_ParticipantBuiltinTopicData__copyOut(void *_from, void *_to);
struct _DDS_ParticipantBuiltinTopicData {
    _DDS_BuiltinTopicKey_t key;
    struct _DDS_UserDataQosPolicy user_data;
};

extern c_metaObject __DDS_TopicBuiltinTopicData__load (c_base base);
extern const char * __DDS_TopicBuiltinTopicData__keys (void);
extern const char * __DDS_TopicBuiltinTopicData__name (void);
struct _DDS_TopicBuiltinTopicData ;
extern  c_bool __DDS_TopicBuiltinTopicData__copyIn(c_base base, struct DDS::TopicBuiltinTopicData *from, struct _DDS_TopicBuiltinTopicData *to);
extern  void __DDS_TopicBuiltinTopicData__copyOut(void *_from, void *_to);
struct _DDS_TopicBuiltinTopicData {
    _DDS_BuiltinTopicKey_t key;
    c_string name;
    c_string type_name;
    struct _DDS_DurabilityQosPolicy durability;
    struct _DDS_DurabilityServiceQosPolicy durability_service;
    struct _DDS_DeadlineQosPolicy deadline;
    struct _DDS_LatencyBudgetQosPolicy latency_budget;
    struct _DDS_LivelinessQosPolicy liveliness;
    struct _DDS_ReliabilityQosPolicy reliability;
    struct _DDS_TransportPriorityQosPolicy transport_priority;
    struct _DDS_LifespanQosPolicy lifespan;
    struct _DDS_DestinationOrderQosPolicy destination_order;
    struct _DDS_HistoryQosPolicy history;
    struct _DDS_ResourceLimitsQosPolicy resource_limits;
    struct _DDS_OwnershipQosPolicy ownership;
    struct _DDS_TopicDataQosPolicy topic_data;
};

extern c_metaObject __DDS_PublicationBuiltinTopicData__load (c_base base);
extern const char * __DDS_PublicationBuiltinTopicData__keys (void);
extern const char * __DDS_PublicationBuiltinTopicData__name (void);
struct _DDS_PublicationBuiltinTopicData ;
extern  c_bool __DDS_PublicationBuiltinTopicData__copyIn(c_base base, struct DDS::PublicationBuiltinTopicData *from, struct _DDS_PublicationBuiltinTopicData *to);
extern  void __DDS_PublicationBuiltinTopicData__copyOut(void *_from, void *_to);
struct _DDS_PublicationBuiltinTopicData {
    _DDS_BuiltinTopicKey_t key;
    _DDS_BuiltinTopicKey_t participant_key;
    c_string topic_name;
    c_string type_name;
    struct _DDS_DurabilityQosPolicy durability;
    struct _DDS_DeadlineQosPolicy deadline;
    struct _DDS_LatencyBudgetQosPolicy latency_budget;
    struct _DDS_LivelinessQosPolicy liveliness;
    struct _DDS_ReliabilityQosPolicy reliability;
    struct _DDS_LifespanQosPolicy lifespan;
    struct _DDS_DestinationOrderQosPolicy destination_order;
    struct _DDS_UserDataQosPolicy user_data;
    struct _DDS_OwnershipQosPolicy ownership;
    struct _DDS_OwnershipStrengthQosPolicy ownership_strength;
    struct _DDS_PresentationQosPolicy presentation;
    struct _DDS_PartitionQosPolicy partition;
    struct _DDS_TopicDataQosPolicy topic_data;
    struct _DDS_GroupDataQosPolicy group_data;
};

extern c_metaObject __DDS_SubscriptionBuiltinTopicData__load (c_base base);
extern const char * __DDS_SubscriptionBuiltinTopicData__keys (void);
extern const char * __DDS_SubscriptionBuiltinTopicData__name (void);
struct _DDS_SubscriptionBuiltinTopicData ;
extern  c_bool __DDS_SubscriptionBuiltinTopicData__copyIn(c_base base, struct DDS::SubscriptionBuiltinTopicData *from, struct _DDS_SubscriptionBuiltinTopicData *to);
extern  void __DDS_SubscriptionBuiltinTopicData__copyOut(void *_from, void *_to);
struct _DDS_SubscriptionBuiltinTopicData {
    _DDS_BuiltinTopicKey_t key;
    _DDS_BuiltinTopicKey_t participant_key;
    c_string topic_name;
    c_string type_name;
    struct _DDS_DurabilityQosPolicy durability;
    struct _DDS_DeadlineQosPolicy deadline;
    struct _DDS_LatencyBudgetQosPolicy latency_budget;
    struct _DDS_LivelinessQosPolicy liveliness;
    struct _DDS_ReliabilityQosPolicy reliability;
    struct _DDS_OwnershipQosPolicy ownership;
    struct _DDS_DestinationOrderQosPolicy destination_order;
    struct _DDS_UserDataQosPolicy user_data;
    struct _DDS_TimeBasedFilterQosPolicy time_based_filter;
    struct _DDS_PresentationQosPolicy presentation;
    struct _DDS_PartitionQosPolicy partition;
    struct _DDS_TopicDataQosPolicy topic_data;
    struct _DDS_GroupDataQosPolicy group_data;
};

#endif
