
#ifndef MODULE_kernelModule_HEADER
#define MODULE_kernelModule_HEADER

#include "os_if.h"
#include "c_base.h"
#include "c_misc.h"
#include "c_sync.h"
#include "c_collection.h"
#include "c_field.h"


#ifdef MODEL_kernelModule_IMPLEMENTATION
#define OS_API OS_API_EXPORT
#else
#define OS_API OS_API_IMPORT
#endif

struct v_gid_s {
    c_ulong systemId;
    c_ulong localId;
    c_ulong serial;
};

typedef struct v_gid_s v_gid;

typedef struct c_time v_duration;

typedef c_ulong v_state;

typedef enum v_writeResult {
    V_WRITE_UNDEFINED,
    V_WRITE_SUCCESS,
    V_WRITE_SUCCESS_NOT_STORED,
    V_WRITE_REGISTERED,
    V_WRITE_UNREGISTERED,
    V_WRITE_PRE_NOT_MET,
    V_WRITE_ERROR,
    V_WRITE_TIMEOUT,
    V_WRITE_OUT_OF_RESOURCES,
    V_WRITE_REJECTED,
    V_WRITE_COUNT
} v_writeResult;

typedef enum v_historyResult {
    V_HISTORY_RESULT_UNDEFINED,
    V_HISTORY_RESULT_ERROR,
    V_HISTORY_RESULT_BAD_PARAM,
    V_HISTORY_RESULT_TIMEOUT,
    V_HISTORY_RESULT_PRE_NOT_MET,
    V_HISTORY_RESULT_OK
} v_historyResult;

struct v_timedValue_s {
    struct c_time lastUpdate;
    c_ulong value;
};

typedef struct v_timedValue_s v_timedValue;

typedef v_timedValue v_minValue;

typedef v_timedValue v_maxValue;

struct v_avgValue_s {
    c_ulong count;
    c_float value;
};

typedef struct v_avgValue_s v_avgValue;

struct v_fullCounter_s {
    c_ulong value;
    v_minValue min;
    v_maxValue max;
    v_avgValue avg;
};

typedef struct v_fullCounter_s v_fullCounter;

C_CLASS(v_statistics);

C_STRUCT(v_statistics) {
    struct c_time lastReset;
};

C_CLASS(v_writerStatistics);

C_STRUCT(v_writerStatistics) {
    C_EXTENDS(v_statistics);
    v_maxValue maxNumberOfSamplesPerInstance;
    c_ulong numberOfDisposes;
    c_ulong numberOfImplicitRegisters;
    c_ulong numberOfInstancesWithStatusAlive;
    c_ulong numberOfInstancesWithStatusDisposed;
    c_ulong numberOfInstancesWithStatusUnregistered;
    c_ulong numberOfRegisters;
    c_ulong numberOfRetries;
    c_ulong numberOfSamples;
    c_ulong numberOfTimedOutWrites;
    c_ulong numberOfUnregisters;
    c_ulong numberOfWrites;
    c_ulong numberOfWritesBlockedByInstanceLimit;
    c_ulong numberOfWritesBlockedBySamplesLimit;
    c_ulong numberOfWritesBlockedBySamplesPerInstanceLimit;
};

C_CLASS(v_readerStatistics);

C_STRUCT(v_readerStatistics) {
    C_EXTENDS(v_statistics);
    v_maxValue maxNumberOfInstances;
    v_maxValue maxNumberOfSamples;
    v_maxValue maxSampleSize;
    v_maxValue maxSamplesPerInstance;
    c_ulong numberOfInstanceLookups;
    c_ulong numberOfInstanceReads;
    c_ulong numberOfInstanceTakes;
    c_ulong numberOfInstances;
    c_ulong numberOfInstancesWithStatusAlive;
    c_ulong numberOfInstancesWithStatusDisposed;
    c_ulong numberOfInstancesWithStatusNew;
    c_ulong numberOfInstancesWithStatusNoWriters;
    c_ulong numberOfNextInstanceReads;
    c_ulong numberOfNextInstanceTakes;
    c_ulong numberOfReads;
    c_ulong numberOfSamples;
    c_ulong numberOfSamplesArrived;
    c_ulong numberOfSamplesDiscarded;
    c_ulong numberOfSamplesExpired;
    c_ulong numberOfSamplesInserted;
    c_ulong numberOfSamplesLost;
    c_ulong numberOfSamplesPurgedByDispose;
    c_ulong numberOfSamplesPurgedByNoWriters;
    c_ulong numberOfSamplesRead;
    c_ulong numberOfSamplesRejectedByInstancesLimit;
    c_ulong numberOfSamplesRejectedBySamplesLimit;
    c_ulong numberOfSamplesTaken;
    c_ulong numberOfSamplesWithStatusRead;
    c_ulong numberOfTakes;
    v_fullCounter readLatency;
    v_fullCounter transportLatency;
};

C_CLASS(v_queryStatistics);

C_STRUCT(v_queryStatistics) {
    C_EXTENDS(v_statistics);
    c_ulong numberOfInstanceReads;
    c_ulong numberOfInstanceTakes;
    c_ulong numberOfNextInstanceReads;
    c_ulong numberOfNextInstanceTakes;
    c_ulong numberOfReads;
    c_ulong numberOfTakes;
};

C_CLASS(v_networkQueueStatistics);

C_STRUCT(v_networkQueueStatistics) {
    c_ulong numberOfSamplesArrived;
    c_ulong numberOfSamplesInserted;
    c_ulong numberOfSamplesRejected;
    c_ulong numberOfSamplesTaken;
    v_fullCounter numberOfSamplesWaiting;
    c_string name;
};

C_CLASS(v_networkReaderStatistics);

C_STRUCT(v_networkReaderStatistics) {
    C_EXTENDS(v_statistics);
    c_ulong queuesCount;
    c_array queues /*C_ARRAY<kernelModule::v_networkQueueStatistics>*/;
};

C_CLASS(v_networkChannelStatistics);

C_STRUCT(v_networkChannelStatistics) {
    v_fullCounter adminQueueAcks;
    v_fullCounter adminQueueData;
    c_ulong maxNumberOfBytesResentToOneNode;
    c_ulong maxNumberOfPacketsResentToOneNode;
    c_ulong nofBytesAfterCompression;
    c_ulong nofBytesAfterDecompression;
    c_ulong nofBytesBeforeCompression;
    c_ulong nofBytesBeforeDecompression;
    c_ulong nofFreePacketBuffers;
    c_ulong nofUsedPacketBuffers;
    c_ulong numberOfAcksSent;
    c_ulong numberOfBytesDelivered;
    c_ulong numberOfBytesInResendBuffer;
    c_ulong numberOfBytesNotInterested;
    c_ulong numberOfBytesReceived;
    c_ulong numberOfBytesResent;
    c_ulong numberOfBytesSent;
    c_ulong numberOfKnownNodes;
    c_ulong numberOfMessagesDelivered;
    c_ulong numberOfMessagesFragmented;
    c_ulong numberOfMessagesNotInterested;
    c_ulong numberOfMessagesPacked;
    c_ulong numberOfMessagesReceived;
    c_ulong numberOfMessagesSent;
    c_ulong numberOfPacketsInResendBuffer;
    c_ulong numberOfPacketsInvalid;
    c_ulong numberOfPacketsLost;
    c_ulong numberOfPacketsNotConnectedPartition;
    c_ulong numberOfPacketsOutOfOrder;
    c_ulong numberOfPacketsOutOfOrderDropped;
    c_ulong numberOfPacketsReceived;
    c_ulong numberOfPacketsResent;
    c_ulong numberOfPacketsSent;
    c_ulong numberOfPacketsUnknownAddress;
    c_ulong reorderAdminBytesQueued;
    v_avgValue reorderAdminBytesQueuedAvg;
    v_maxValue reorderAdminBytesQueuedMax;
    v_minValue reorderAdminBytesQueuedMin;
    c_ulong reorderAdminPacketsQueued;
    v_avgValue reorderAdminPacketsQueuedAvg;
    v_maxValue reorderAdminPacketsQueuedMax;
    v_minValue reorderAdminPacketsQueuedMin;
    c_ulong resendAdminBytesQueued;
    v_avgValue resendAdminBytesQueuedAvg;
    v_maxValue resendAdminBytesQueuedMax;
    v_minValue resendAdminBytesQueuedMin;
    c_ulong resendAdminPacketsQueued;
    v_avgValue resendAdminPacketsQueuedAvg;
    v_maxValue resendAdminPacketsQueuedMax;
    v_minValue resendAdminPacketsQueuedMin;
    c_ulong ringBufferMessagesQueued;
    v_avgValue ringBufferMessagesQueuedAvg;
    v_maxValue ringBufferMessagesQueuedMax;
    v_minValue ringBufferMessagesQueuedMin;
    c_string name;
};

C_CLASS(v_networkingStatistics);

C_STRUCT(v_networkingStatistics) {
    C_EXTENDS(v_statistics);
    c_ulong channelsCount;
    c_ulong numberOfErrors;
    c_array channels /*C_ARRAY<kernelModule::v_networkChannelStatistics>*/;
};

C_CLASS(v_cmsoapStatistics);

C_STRUCT(v_cmsoapStatistics) {
    C_EXTENDS(v_statistics);
    c_ulong clientThreads;
    c_ulong connectedClients;
    v_maxValue maxClientThreads;
    v_maxValue maxConnectedClients;
    c_ulong requestsHandled;
};

C_CLASS(v_groupQueueStatistics);

C_STRUCT(v_groupQueueStatistics) {
    C_EXTENDS(v_statistics);
    c_ulong numberOfReads;
    v_fullCounter numberOfSamples;
    c_ulong numberOfTakes;
    c_ulong numberOfWrites;
};

C_CLASS(v_durabilityStatistics);

C_STRUCT(v_durabilityStatistics) {
    C_EXTENDS(v_statistics);
    c_ulong aligneeRequestsOpen;
    v_maxValue aligneeRequestsOpenMax;
    c_ulong aligneeRequestsSent;
    c_ulong aligneeRequestsWaiting;
    v_maxValue aligneeRequestsWaitingMax;
    c_ulong aligneeSamplesDispose;
    c_ulong aligneeSamplesRegister;
    c_ulong aligneeSamplesTotal;
    c_ulong aligneeSamplesUnregister;
    c_ulong aligneeSamplesWrite;
    c_ulong aligneeSamplesWriteDispose;
    c_ulong aligneeTotalSize;
    c_ulong alignerRequestsAnswered;
    c_ulong alignerRequestsCombined;
    c_ulong alignerRequestsCombinedAnswered;
    c_ulong alignerRequestsCombinedOpen;
    v_maxValue alignerRequestsCombinedOpenMax;
    c_ulong alignerRequestsIgnored;
    c_ulong alignerRequestsOpen;
    v_maxValue alignerRequestsOpenMax;
    c_ulong alignerRequestsReceived;
    c_ulong alignerSamplesDispose;
    c_ulong alignerSamplesRegister;
    c_ulong alignerSamplesTotal;
    c_ulong alignerSamplesUnregister;
    c_ulong alignerSamplesWrite;
    c_ulong alignerSamplesWriteDispose;
    c_ulong alignerTotalSize;
    c_ulong fellowsApproved;
    c_ulong fellowsIncompatibleDataModel;
    c_ulong fellowsIncompatibleState;
    c_ulong fellowsKnown;
    v_maxValue fellowsKnownMax;
    c_ulong groupsCompletePersistent;
    c_ulong groupsCompleteTotal;
    c_ulong groupsCompleteTransient;
    c_ulong groupsCompleteVolatile;
    c_ulong groupsIgnoredPersistent;
    c_ulong groupsIgnoredTotal;
    c_ulong groupsIgnoredTransient;
    c_ulong groupsIgnoredVolatile;
    c_ulong groupsIncompletePersistent;
    c_ulong groupsIncompleteTotal;
    c_ulong groupsIncompleteTransient;
    c_ulong groupsIncompleteVolatile;
    c_ulong groupsKnownPersistent;
    c_ulong groupsKnownTotal;
    c_ulong groupsKnownTransient;
    c_ulong groupsKnownVolatile;
    c_ulong groupsToCreatePersistent;
    c_ulong groupsToCreateTotal;
    c_ulong groupsToCreateTransient;
    c_ulong groupsToCreateVolatile;
    c_ulong nameSpacesKnown;
    c_ulong nameSpacesMaster;
    c_ulong nameSpacesSlave;
    c_ulong persistentSamplesWritten;
};

C_CLASS(v_kernelStatistics);

C_STRUCT(v_kernelStatistics) {
    C_EXTENDS(v_statistics);
    v_maxValue maxShmClaims;
    v_maxValue maxShmGarbage;
    v_maxValue maxShmUsed;
    c_ulong shmClaimFails;
    c_ulong shmClaims;
    c_ulong shmGarbage;
    c_ulong shmUsed;
};

C_CLASS(v_rnrGroupStatistics);

C_STRUCT(v_rnrGroupStatistics) {
    C_EXTENDS(v_statistics);
    v_fullCounter dataRateRecorded;
    v_fullCounter dataRateReplayed;
    c_ulong numberOfBytesRecorded;
    c_ulong numberOfBytesReplayed;
    c_ulong numberOfSamplesRecorded;
    c_ulong numberOfSamplesReplayed;
    c_string name;
};

C_CLASS(v_rnrStorageStatistics);

C_STRUCT(v_rnrStorageStatistics) {
    C_EXTENDS(v_statistics);
    c_string name;
    c_set topics;
};

C_CLASS(v_rnrStatistics);

C_STRUCT(v_rnrStatistics) {
    C_EXTENDS(v_statistics);
    c_string name;
    c_set storages;
};

struct v_handleInfo_s {
    c_address object_nextFree;
    c_ulong serial;
    c_ulong status_count;
};

typedef struct v_handleInfo_s v_handleInfo;

typedef c_array v_handleInfoList /*C_ARRAY<kernelModule::v_handleInfo>*/;

C_CLASS(v_handleServer);

C_STRUCT(v_handleServer) {
    c_long firstFree;
    c_long lastIndex;
    c_mutex mutex;
    c_bool suspended;
    c_array handleInfos /*C_ARRAY<kernelModule::v_handleInfoList>*/;
};

struct v_handle_s {
    c_address server;
    c_ulong index;
    c_ulong serial;
};

typedef struct v_handle_s v_handle;

typedef enum v_kind {
    K_KERNEL,
    K_OBJECT,
    K_ENTITY,
    K_GROUPSET,
    K_WAITSET,
    K_CONDITION,
    K_QUERY,
    K_DATAREADERQUERY,
    K_DATAVIEW,
    K_PROJECTION,
    K_MAPPING,
    K_FILTER,
    K_DEADLINEINSTANCE,
    K_MESSAGE,
    K_TRANSACTION,
    K_WRITERINSTANCE,
    K_WRITERSAMPLE,
    K_WRITERCACHEITEM,
    K_GROUPINSTANCE,
    K_GROUPSAMPLE,
    K_GROUPCACHEITEM,
    K_DATAREADERINSTANCE,
    K_READERSAMPLE,
    K_DATAVIEWINSTANCE,
    K_DATAVIEWQUERY,
    K_DATAVIEWSAMPLE,
    K_WRITERINSTANCETEMPLATE,
    K_TOPIC,
    K_PUBLISHER,
    K_SUBSCRIBER,
    K_DOMAIN,
    K_DOMAININTEREST,
    K_DOMAINADMIN,
    K_READER,
    K_WRITER,
    K_ENTRY,
    K_DATAREADERENTRY,
    K_DELIVERYSERVICEENTRY,
    K_GROUP,
    K_WRITERGROUP,
    K_CACHE,
    K_GROUPACTION,
    K_GROUPSTREAM,
    K_GROUPQUEUE,
    K_GROUPQUEUESAMPLE,
    K_DATAREADER,
    K_DELIVERYSERVICE,
    K_PARTICIPANT,
    K_PURGELISTITEM,
    K_GROUPPURGEITEM,
    K_INDEX,
    K_READERSTATUS,
    K_WRITERSTATUS,
    K_PUBLISHERSTATUS,
    K_SUBSCRIBERSTATUS,
    K_DOMAINSTATUS,
    K_TOPICSTATUS,
    K_PARTICIPANTSTATUS,
    K_KERNELSTATUS,
    K_READERSTATISTICS,
    K_WRITERSTATISTICS,
    K_QUERYSTATISTICS,
    K_PROXY,
    K_WAITSETEVENT,
    K_WAITSETEVENTHISTORYDELETE,
    K_WAITSETEVENTHISTORYREQUEST,
    K_WAITSETEVENTPERSISTENTSNAPSHOT,
    K_WAITSETEVENTCONNECTWRITER,
    K_SERVICEMANAGER,
    K_SERVICE,
    K_SERVICESTATE,
    K_NETWORKING,
    K_DURABILITY,
    K_CMSOAP,
    K_RNR,
    K_LEASEMANAGER,
    K_LEASE,
    K_LEASEACTION,
    K_SPLICED,
    K_CONFIGURATION,
    K_REGISTRATION,
    K_NETWORKREADER,
    K_NETWORKREADERENTRY,
    K_NETWORKMESSAGE,
    K_NETWORKMAPENTRY,
    K_HISTORICALDATAREQUEST,
    K_PERSISTENTSNAPSHOTREQUEST,
    K_PENDINGDISPOSEELEMENT,
    K_TYPECOUNT
} v_kind;

C_CLASS(v_object);

C_STRUCT(v_object) {
    c_voidp kernel;
    enum v_kind kind;
};

C_CLASS(v_public);

C_STRUCT(v_public) {
    C_EXTENDS(v_object);
    v_handle handle;
    c_voidp userDataPublic;
};

C_CLASS(v_status);

C_STRUCT(v_status) {
    C_EXTENDS(v_object);
    c_ulong state;
};

typedef enum v_entityState {
    V_ENTITYSTATE_DISABLED,
    V_ENTITYSTATE_SUSPENDED,
    V_ENTITYSTATE_ENABLED
} v_entityState;

C_CLASS(v_entity);

C_STRUCT(v_entity) {
    C_EXTENDS(v_public);
    c_bool enabled;
    c_string name;
    v_statistics statistics;
    v_status status;
};

C_CLASS(v_proxy);

C_STRUCT(v_proxy) {
    C_EXTENDS(v_object);
    v_handle source;
    c_voidp source2;
    c_voidp userData;
};

C_CLASS(v_observable);

C_STRUCT(v_observable) {
    C_EXTENDS(v_entity);
    c_mutex mutex;
    c_set observers;
};

C_CLASS(v_observer);

C_STRUCT(v_observer) {
    C_EXTENDS(v_observable);
    c_cond cv;
    c_voidp eventData;
    c_ulong eventFlags;
    c_ulong eventMask;
    c_mutex mutex;
    c_long waitCount;
};

C_CLASS(v_query);

C_CLASS(v_collection);

C_STRUCT(v_collection) {
    C_EXTENDS(v_observer);
    c_set queries;
};

C_STRUCT(v_query) {
    C_EXTENDS(v_collection);
    c_voidp source;
};

C_CLASS(v_lifespanSample);

C_STRUCT(v_lifespanSample) {
    struct c_time expiryTime;
    c_voidp prev;
    v_lifespanSample next;
};

C_CLASS(v_readerSample);

C_STRUCT(v_readerSample) {
    C_EXTENDS(v_lifespanSample);
    c_voidp instance;
    struct c_time readTime;
    v_state sampleState;
    c_voidp viewSamples;
};

C_CLASS(v_dataReaderSample);

C_STRUCT(v_dataReaderSample) {
    C_EXTENDS(v_readerSample);
    c_long disposeCount;
    struct c_time insertTime;
    c_voidp newer;
    c_long noWritersCount;
    v_gid publicationHandle;
    c_ulong readId;
    v_dataReaderSample older;
};

C_CLASS(v_dataReaderQuery);

C_STRUCT(v_dataReaderQuery) {
    C_EXTENDS(v_query);
    v_state instanceMask;
    v_state sampleMask;
    c_ulong state;
    c_ulong updateCnt;
    v_state viewMask;
    c_bool walkRequired;
    c_string expression;
    c_array instanceQ /*C_ARRAY<c_query>*/;
    c_string params;
    c_array sampleQ /*C_ARRAY<c_query>*/;
    v_dataReaderSample triggerValue;
};

C_CLASS(v_dataViewSampleList);

C_STRUCT(v_dataViewSampleList) {
    C_EXTENDS(v_readerSample);
    c_voidp next;
    c_voidp prev;
};

C_CLASS(v_dataViewSample);

C_STRUCT(v_dataViewSample) {
    C_EXTENDS(v_dataViewSampleList);
    c_voidp next;
    v_dataViewSample prev;
};

C_CLASS(v_dataViewQuery);

C_STRUCT(v_dataViewQuery) {
    C_EXTENDS(v_query);
    v_state instanceMask;
    v_state sampleMask;
    c_ulong state;
    c_ulong updateCnt;
    v_state viewMask;
    c_bool walkRequired;
    c_string expression;
    c_array instanceQ /*C_ARRAY<c_query>*/;
    c_string params;
    c_array sampleQ /*C_ARRAY<c_query>*/;
    v_dataViewSample triggerValue;
};

struct v_resourcePolicy {
    c_long max_samples;
    c_long max_instances;
    c_long max_samples_per_instance;
};

C_CLASS(v_historicalDataRequest);

C_STRUCT(v_historicalDataRequest) {
    struct c_time maxSourceTimestamp;
    struct c_time minSourceTimestamp;
    struct v_resourcePolicy resourceLimits;
    c_string filter;
    c_array filterParams /*C_ARRAY<c_string>*/;
};

C_CLASS(v_persistentSnapshotRequest);

C_STRUCT(v_persistentSnapshotRequest) {
    c_string partitionExpr;
    c_string topicExpr;
    c_string uri;
};

C_CLASS(v_waitsetEvent);

C_STRUCT(v_waitsetEvent) {
    c_ulong kind;
    v_handle source;
    c_voidp userData;
    v_waitsetEvent next;
};

C_CLASS(v_waitsetEventHistoryDelete);

C_STRUCT(v_waitsetEventHistoryDelete) {
    C_EXTENDS(v_waitsetEvent);
    struct c_time deleteTime;
    c_string partitionExpr;
    c_string topicExpr;
};

C_CLASS(v_waitsetEventHistoryRequest);

C_STRUCT(v_waitsetEventHistoryRequest) {
    C_EXTENDS(v_waitsetEvent);
    v_historicalDataRequest request;
};

C_CLASS(v_waitsetEventPersistentSnapshot);

C_STRUCT(v_waitsetEventPersistentSnapshot) {
    C_EXTENDS(v_waitsetEvent);
    v_persistentSnapshotRequest request;
};

typedef enum v_qosKind {
    V_PARTITION_QOS,
    V_PARTICIPANT_QOS,
    V_TOPIC_QOS,
    V_WRITER_QOS,
    V_READER_QOS,
    V_PUBLISHER_QOS,
    V_SUBSCRIBER_QOS,
    V_INDEX_QOS,
    V_WRITERHISTORY_QOS,
    V_GROUPHISTORY_QOS,
    V_VIEW_QOS,
    V_DATAVIEW_QOS,
    V_KERNEL_QOS,
    V_COUNT_QOS
} v_qosKind;

C_CLASS(v_qos);

C_STRUCT(v_qos) {
    enum v_qosKind kind;
};

struct v_topicDataPolicy {
    c_array value /*C_ARRAY<c_octet>*/;
    c_long size;
};

typedef enum v_durabilityKind {
    V_DURABILITY_VOLATILE,
    V_DURABILITY_TRANSIENT_LOCAL,
    V_DURABILITY_TRANSIENT,
    V_DURABILITY_PERSISTENT
} v_durabilityKind;

struct v_durabilityPolicy {
    enum v_durabilityKind kind;
};

typedef enum v_historyQosKind {
    V_HISTORY_KEEPLAST,
    V_HISTORY_KEEPALL
} v_historyQosKind;

struct v_durabilityServicePolicy {
    v_duration service_cleanup_delay;
    enum v_historyQosKind history_kind;
    c_long history_depth;
    c_long max_samples;
    c_long max_instances;
    c_long max_samples_per_instance;
};

struct v_deadlinePolicy {
    v_duration period;
};

struct v_latencyPolicy {
    v_duration duration;
};

typedef enum v_livelinessKind {
    V_LIVELINESS_AUTOMATIC,
    V_LIVELINESS_PARTICIPANT,
    V_LIVELINESS_TOPIC
} v_livelinessKind;

struct v_livelinessPolicy {
    enum v_livelinessKind kind;
    v_duration lease_duration;
};

typedef enum v_reliabilityKind {
    V_RELIABILITY_BESTEFFORT,
    V_RELIABILITY_RELIABLE
} v_reliabilityKind;

struct v_reliabilityPolicy {
    enum v_reliabilityKind kind;
    v_duration max_blocking_time;
    c_bool synchronous;
};

typedef enum v_orderbyKind {
    V_ORDERBY_RECEPTIONTIME,
    V_ORDERBY_SOURCETIME
} v_orderbyKind;

struct v_orderbyPolicy {
    enum v_orderbyKind kind;
};

struct v_historyPolicy {
    enum v_historyQosKind kind;
    c_long depth;
};

struct v_transportPolicy {
    c_long value;
};

struct v_lifespanPolicy {
    v_duration duration;
};

typedef enum v_ownershipKind {
    V_OWNERSHIP_SHARED,
    V_OWNERSHIP_EXCLUSIVE
} v_ownershipKind;

struct v_ownershipPolicy {
    enum v_ownershipKind kind;
};

C_CLASS(v_topicQos);

C_STRUCT(v_topicQos) {
    C_EXTENDS(v_qos);
    struct v_deadlinePolicy deadline;
    struct v_durabilityPolicy durability;
    struct v_durabilityServicePolicy durabilityService;
    struct v_historyPolicy history;
    struct v_latencyPolicy latency;
    struct v_lifespanPolicy lifespan;
    struct v_livelinessPolicy liveliness;
    struct v_orderbyPolicy orderby;
    struct v_ownershipPolicy ownership;
    struct v_reliabilityPolicy reliability;
    struct v_resourcePolicy resource;
    struct v_topicDataPolicy topicData;
    struct v_transportPolicy transport;
};

typedef enum v_accessMode {
    V_ACCESS_MODE_UNDEFINED,
    V_ACCESS_MODE_READ,
    V_ACCESS_MODE_WRITE,
    V_ACCESS_MODE_READ_WRITE,
    V_ACCESS_MODE_NONE,
    V_ACCESS_MODE_COUNT
} v_accessMode;

C_CLASS(v_topic);

C_STRUCT(v_topic) {
    C_EXTENDS(v_observer);
    enum v_accessMode accessMode;
    c_ulong crcOfName;
    c_ulong crcOfTypeName;
    c_property dataField;
    c_string keyExpr;
    c_type keyType;
    c_array messageKeyList /*C_ARRAY<c_field>*/;
    c_type messageType;
    v_topicQos qos;
};

C_CLASS(v_partition);

C_STRUCT(v_partition) {
    C_EXTENDS(v_entity);
    v_qos qos;
};

C_CLASS(v_entry);

C_STRUCT(v_entry) {
    C_EXTENDS(v_object);
    c_voidp reader;
    c_set groups;
};

struct v_cacheLink {
    c_voidp next;
    c_voidp prev;
};

C_CLASS(v_cacheNode);

C_STRUCT(v_cacheNode) {
    struct v_cacheLink connections;
    struct v_cacheLink targets;
};

typedef enum v_cacheKind {
    V_CACHE_CONNECTION,
    V_CACHE_TARGETS,
    V_CACHE_ANY
} v_cacheKind;

C_CLASS(v_cache);

C_STRUCT(v_cache) {
    C_EXTENDS(v_cacheNode);
    enum v_cacheKind kind;
    c_type itemType;
};

C_CLASS(v_groupEntry);

C_STRUCT(v_groupEntry) {
    c_long sequenceNumber;
    v_cache connectionCache;
    v_entry entry;
    v_groupEntry next;
};

struct v_groupEntrySet {
    v_groupEntry firstEntry;
    c_long lastSequenceNumber;
};

struct v_entrySet {
    c_mutex mutex;
    c_set entries;
};

typedef enum v_presentationKind {
    V_PRESENTATION_INSTANCE,
    V_PRESENTATION_TOPIC,
    V_PRESENTATION_GROUP
} v_presentationKind;

struct v_presentationPolicy {
    enum v_presentationKind access_scope;
    c_bool coherent_access;
    c_bool ordered_access;
};

typedef c_string v_partitionPolicy;

struct v_groupDataPolicy {
    c_array value /*C_ARRAY<c_octet>*/;
    c_long size;
};

struct v_entityFactoryPolicy {
    c_bool autoenable_created_entities;
};

struct v_sharePolicy {
    c_string name;
    c_bool enable;
};

C_CLASS(v_subscriberQos);

C_STRUCT(v_subscriberQos) {
    C_EXTENDS(v_qos);
    struct v_groupDataPolicy groupData;
    struct v_presentationPolicy presentation;
    struct v_sharePolicy share;
    struct v_entityFactoryPolicy entityFactory;
    v_partitionPolicy partition;
};

struct v_userDataPolicy {
    c_array value /*C_ARRAY<c_octet>*/;
    c_long size;
};

struct v_pacingPolicy {
    v_duration minSeperation;
};

struct v_readerLifecyclePolicy {
    v_duration autopurge_nowriter_samples_delay;
    v_duration autopurge_disposed_samples_delay;
    c_bool enable_invalid_samples;
};

struct v_readerLifespanPolicy {
    c_bool used;
    v_duration duration;
};

struct v_userKeyPolicy {
    c_bool enable;
    c_string expression;
};

C_CLASS(v_readerQos);

C_STRUCT(v_readerQos) {
    C_EXTENDS(v_qos);
    struct v_deadlinePolicy deadline;
    struct v_durabilityPolicy durability;
    struct v_historyPolicy history;
    struct v_latencyPolicy latency;
    struct v_readerLifecyclePolicy lifecycle;
    struct v_readerLifespanPolicy lifespan;
    struct v_livelinessPolicy liveliness;
    struct v_orderbyPolicy orderby;
    struct v_ownershipPolicy ownership;
    struct v_pacingPolicy pacing;
    struct v_reliabilityPolicy reliability;
    struct v_resourcePolicy resource;
    struct v_sharePolicy share;
    struct v_userDataPolicy userData;
    struct v_userKeyPolicy userKey;
};

C_CLASS(v_reader);

C_STRUCT(v_reader) {
    C_EXTENDS(v_collection);
    struct v_entrySet entrySet;
    c_cond historicalDataCondition;
    c_voidp subscriber;
    c_bool historicalDataComplete;
    v_historicalDataRequest historicalDataRequest;
    v_readerQos qos;
    v_subscriberQos subQos;
};

C_CLASS(v_group);

C_CLASS(v_groupStream);

C_STRUCT(v_groupStream) {
    C_EXTENDS(v_reader);
    c_list expr;
    c_set groups;
};

typedef c_array v_messageQos /*C_ARRAY<c_octet>*/;

C_CLASS(v_registration);

C_STRUCT(v_registration) {
    struct c_time writeTime;
    v_gid writerGID;
    v_messageQos qos;
    v_registration next;
};

struct v_owner {
    v_gid gid;
    c_long strength;
    c_bool exclusive;
};

C_CLASS(v_groupInstance);

C_STRUCT(v_groupInstance) {
    C_EXTENDS(v_object);
    c_long count;
    struct c_time epoch;
    c_voidp group;
    c_long messageCount;
    c_voidp oldest;
    struct v_owner owner;
    c_ulong state;
    v_registration registrations;
    v_cache targetCache;
    v_registration unregistrations;
};

C_CLASS(v_groupPurgeItem);

C_STRUCT(v_groupPurgeItem) {
    struct c_time insertionTime;
    v_groupInstance instance;
    v_groupPurgeItem next;
};

C_CLASS(v_lifespanAdmin);

C_STRUCT(v_lifespanAdmin) {
    c_long sampleCount;
    v_lifespanSample head;
    v_lifespanSample tail;
};

C_CLASS(v_node);

C_STRUCT(v_node) {
    v_state nodeState;
};

C_CLASS(v_message);

C_STRUCT(v_message) {
    C_EXTENDS(v_node);
    struct c_time allocTime;
    c_ulong sequenceNumber;
    c_ulong transactionId;
    struct c_time writeTime;
    v_gid writerGID;
    v_gid writerInstanceGID;
    v_messageQos qos;
};

C_CLASS(v_groupwriterAdministration);

C_STRUCT(v_groupwriterAdministration) {
    v_gid gid;
    c_ulong seqNumber;
};

C_STRUCT(v_group) {
    C_EXTENDS(v_entity);
    c_long count;
    struct c_time creationTime;
    c_cond cv;
    c_long depth;
    c_voidp disposedInstancesLast;
    struct c_time lastDisposeAllTime;
    c_mutex mutex;
    struct v_groupEntrySet networkEntrySet;
    enum v_accessMode partitionAccessMode;
    c_voidp purgeListEmptyLast;
    struct v_groupEntrySet routedEntrySet;
    c_long sequenceNumber;
    struct v_groupEntrySet topicEntrySet;
    struct v_groupEntrySet variantEntrySet;
    c_bool complete;
    c_bool infWait;
    c_set attachedServices;
    v_groupInstance cachedInstance;
    v_message cachedRegMsg;
    v_groupPurgeItem disposedInstances;
    c_type instanceType;
    c_set instances;
    v_lifespanAdmin lifespanAdmin;
    c_set notInterestedServices;
    v_partition partition;
    v_groupPurgeItem purgeListEmpty;
    c_type sampleType;
    c_set streams;
    v_topic topic;
    c_set writerAdministration;
};

C_CLASS(v_waitsetEventConnectWriter);

C_STRUCT(v_waitsetEventConnectWriter) {
    C_EXTENDS(v_waitsetEvent);
    v_group group;
};

C_CLASS(v_waitset);

C_STRUCT(v_waitset) {
    C_EXTENDS(v_observer);
    c_voidp participant;
    v_waitsetEvent eventCache;
    c_set observables;
};

typedef enum v_leaseActionId {
    V_LEASEACTION_UNDEFINED,
    V_LEASEACTION_SERVICESTATE_EXPIRED,
    V_LEASEACTION_READER_DEADLINE_MISSED,
    V_LEASEACTION_WRITER_DEADLINE_MISSED,
    V_LEASEACTION_LIVELINESS_CHECK,
    V_LEASEACTION_HEARTBEAT_SEND,
    V_LEASEACTION_HEARTBEAT_CHECK,
    V_LEASEACTION_SPLICED_DEATH_DETECTED,
    V_LEASEACTION_COUNT
} v_leaseActionId;

C_CLASS(v_leaseManager);

C_CLASS(v_lease);

C_STRUCT(v_lease) {
    C_EXTENDS(v_object);
    v_duration duration;
    struct c_time expiryTime;
    c_mutex mutex;
    c_bool lastRenewInternal;
    c_set observers;
};

C_CLASS(v_leaseAction);

C_STRUCT(v_leaseAction) {
    C_EXTENDS(v_object);
    enum v_leaseActionId actionId;
    v_handle actionObject;
    c_bool repeat;
    v_lease lease;
};

C_STRUCT(v_leaseManager) {
    C_EXTENDS(v_object);
    c_cond cond;
    c_mutex mutex;
    struct c_time nextExpiryTime;
    c_bool quit;
    c_set leases;
};

typedef c_long v_policyId;

struct v_QosPolicyCount {
    v_policyId policyId;
    c_long count;
};

typedef enum v_sampleRejectedKind {
    S_NOT_REJECTED,
    S_REJECTED_BY_INSTANCES_LIMIT,
    S_REJECTED_BY_SAMPLES_LIMIT,
    S_REJECTED_BY_SAMPLES_PER_INSTANCE_LIMIT
} v_sampleRejectedKind;

struct v_inconsistentTopicInfo {
    c_long totalCount;
    c_long totalChanged;
};

struct v_allDataDisposedInfo {
    c_long totalCount;
    c_long totalChanged;
};

struct v_sampleLostInfo {
    c_long totalCount;
    c_long totalChanged;
};

struct v_livelinessLostInfo {
    c_long totalCount;
    c_long totalChanged;
};

struct v_deadlineMissedInfo {
    c_long totalCount;
    c_long totalChanged;
    v_handle instanceHandle;
};

struct v_sampleRejectedInfo {
    c_long totalCount;
    c_long totalChanged;
    enum v_sampleRejectedKind lastReason;
    v_gid instanceHandle;
};

struct v_incompatibleQosInfo {
    c_long totalCount;
    c_long totalChanged;
    v_policyId lastPolicyId;
    c_array policyCount /*C_ARRAY<c_long>*/;
};

struct v_livelinessChangedInfo {
    c_long activeCount;
    c_long activeChanged;
    c_long inactiveCount;
    c_long inactiveChanged;
    v_gid instanceHandle;
};

struct v_topicMatchInfo {
    c_long totalCount;
    c_long totalChanged;
    c_long currentCount;
    c_long currentChanged;
    v_gid instanceHandle;
};

struct v_servicesChangedInfo {
    c_long totalCount;
    c_long totalChanged;
};

struct v_groupsChangedInfo {
    c_long totalCount;
    c_long totalChanged;
};

C_CLASS(v_kernelStatus);

C_STRUCT(v_kernelStatus) {
    C_EXTENDS(v_status);
    struct v_servicesChangedInfo servicesInfo;
};

C_CLASS(v_partitionStatus);

C_STRUCT(v_partitionStatus) {
    C_EXTENDS(v_status);
    struct v_groupsChangedInfo groupsChanged;
};

C_CLASS(v_topicStatus);

C_STRUCT(v_topicStatus) {
    C_EXTENDS(v_status);
    struct v_allDataDisposedInfo allDataDisposed;
    struct v_inconsistentTopicInfo inconsistentTopic;
};

C_CLASS(v_subscriberStatus);

C_STRUCT(v_subscriberStatus) {
    C_EXTENDS(v_status);
    c_long dummy;
};

C_CLASS(v_writerStatus);

C_STRUCT(v_writerStatus) {
    C_EXTENDS(v_status);
    struct v_deadlineMissedInfo deadlineMissed;
    struct v_incompatibleQosInfo incompatibleQos;
    struct v_livelinessLostInfo livelinessLost;
    struct v_topicMatchInfo publicationMatch;
};

C_CLASS(v_readerStatus);

C_STRUCT(v_readerStatus) {
    C_EXTENDS(v_status);
    struct v_deadlineMissedInfo deadlineMissed;
    struct v_incompatibleQosInfo incompatibleQos;
    struct v_livelinessChangedInfo livelinessChanged;
    struct v_sampleLostInfo sampleLost;
    struct v_sampleRejectedInfo sampleRejected;
    struct v_topicMatchInfo subscriptionMatch;
};

#define V_LENGTH_UNLIMITED (-1)
#define V_UNKNOWN_POLICY_ID (0)
#define V_USERDATAPOLICY_ID (1)
#define V_DURABILITYPOLICY_ID (2)
#define V_PRESENTATIONPOLICY_ID (3)
#define V_DEADLINEPOLICY_ID (4)
#define V_LATENCYPOLICY_ID (5)
#define V_OWNERSHIPPOLICY_ID (6)
#define V_STRENGTHPOLICY_ID (7)
#define V_LIVELINESSPOLICY_ID (8)
#define V_PACINGPOLICY_ID (9)
#define V_PARTITIONPOLICY_ID (10)
#define V_RELIABILITYPOLICY_ID (11)
#define V_ORDERBYPOLICY_ID (12)
#define V_HISTORYPOLICY_ID (13)
#define V_RESOURCEPOLICY_ID (14)
#define V_ENTITYFACTORYPOLICY_ID (15)
#define V_WRITERLIFECYCLEPOLICY_ID (16)
#define V_READERLIFECYCLEPOLICY_ID (17)
#define V_TOPICDATAPOLICY_ID (18)
#define V_GROUPDATAPOLICY_ID (19)
#define V_TRANSPORTPOLICY_ID (20)
#define V_LIFESPANPOLICY_ID (21)
#define V_DURABILITYSERVICEPOLICY_ID (22)
#define V_USERKEYPOLICY_ID (23)
#define V_VIEWKEYPOLICY_ID (24)
#define V_READERLIFESPANPOLICY_ID (25)
#define V_SHAREPOLICY_ID (26)
#define V_SCHEDULINGPOLICY_ID (27)
#define V_POLICY_ID_COUNT (28)
#define V_USERDATAPOLICY_NAME ("UserData")
#define V_DURABILITYPOLICY_NAME ("Durability")
#define V_PRESENTATIONPOLICY_NAME ("Presentation")
#define V_DEADLINEPOLICY_NAME ("Deadline")
#define V_LATENCYPOLICY_NAME ("LatencyBudget")
#define V_OWNERSHIPPOLICY_NAME ("Ownership")
#define V_STRENGTHPOLICY_NAME ("OwnershipStrength")
#define V_LIVELINESSPOLICY_NAME ("Liveliness")
#define V_PACINGPOLICY_NAME ("TimeBasedFilter")
#define V_PARTITIONPOLICY_NAME ("Partition")
#define V_RELIABILITYPOLICY_NAME ("Reliability")
#define V_ORDERBYPOLICY_NAME ("DestinationOrder")
#define V_HISTORYPOLICY_NAME ("History")
#define V_RESOURCEPOLICY_NAME ("ResourceLimits")
#define V_ENTITYFACTORYPOLICY_NAME ("EntityFactory")
#define V_WRITERLIFECYCLEPOLICY_NAME ("WriterDataLifecycle")
#define V_READERLIFECYCLEPOLICY_NAME ("ReaderDataLifecycle")
#define V_TOPICDATAPOLICY_NAME ("TopicData")
#define V_GROUPDATAPOLICY_NAME ("GroupData")
#define V_TRANSPORTPOLICY_NAME ("TransportPriority")
#define V_LIFESPANPOLICY_NAME ("Lifespan")
#define V_DURABILITYSERVICEPOLICY_NAME ("DurabilityService")
#define V_USERKEYPOLICY_NAME ("SubscriptionKey")
#define V_READERLIFESPANPOLICY_NAME ("ReaderLifespan")
#define V_SHAREPOLICY_NAME ("Share")
#define V_SCHEDULINGPOLICY_NAME ("Scheduling")
typedef enum v_scheduleKind {
    V_SCHED_DEFAULT,
    V_SCHED_TIMESHARING,
    V_SCHED_REALTIME
} v_scheduleKind;

typedef enum v_schedulePriorityKind {
    V_SCHED_PRIO_RELATIVE,
    V_SCHED_PRIO_ABSOLUTE
} v_schedulePriorityKind;

struct v_strengthPolicy {
    c_long value;
};

struct v_writerLifecyclePolicy {
    c_bool autodispose_unregistered_instances;
    v_duration autopurge_suspended_samples_delay;
    v_duration autounregister_instance_delay;
};

struct v_simulationPolicy {
    struct c_time oldTime;
    struct c_time newTime;
    c_double relativeSpeed;
};

struct v_schedulePolicy {
    enum v_scheduleKind kind;
    enum v_schedulePriorityKind priorityKind;
    c_long priority;
};

struct v_builtinTopicPolicy {
    c_bool enabled;
};

C_CLASS(v_kernelQos);

C_STRUCT(v_kernelQos) {
    C_EXTENDS(v_qos);
    struct v_builtinTopicPolicy builtin;
};

C_CLASS(v_partitionQos);

C_STRUCT(v_partitionQos) {
    C_EXTENDS(v_qos);
    struct v_simulationPolicy simulation;
    struct v_userDataPolicy userData;
    struct v_entityFactoryPolicy entityFactory;
};

C_CLASS(v_participantQos);

C_STRUCT(v_participantQos) {
    C_EXTENDS(v_qos);
    struct v_userDataPolicy userData;
    struct v_schedulePolicy watchdogScheduling;
    struct v_entityFactoryPolicy entityFactory;
};

C_CLASS(v_writerQos);

C_STRUCT(v_writerQos) {
    C_EXTENDS(v_qos);
    struct v_deadlinePolicy deadline;
    struct v_durabilityPolicy durability;
    struct v_historyPolicy history;
    struct v_latencyPolicy latency;
    struct v_writerLifecyclePolicy lifecycle;
    struct v_lifespanPolicy lifespan;
    struct v_livelinessPolicy liveliness;
    struct v_orderbyPolicy orderby;
    struct v_ownershipPolicy ownership;
    struct v_reliabilityPolicy reliability;
    struct v_resourcePolicy resource;
    struct v_strengthPolicy strength;
    struct v_transportPolicy transport;
    struct v_userDataPolicy userData;
};

C_CLASS(v_publisherQos);

C_STRUCT(v_publisherQos) {
    C_EXTENDS(v_qos);
    struct v_groupDataPolicy groupData;
    struct v_presentationPolicy presentation;
    struct v_entityFactoryPolicy entityFactory;
    v_partitionPolicy partition;
};

C_CLASS(v_dataViewQos);

C_STRUCT(v_dataViewQos) {
    C_EXTENDS(v_qos);
    struct v_userKeyPolicy userKey;
};

C_CLASS(v_mapping);

C_STRUCT(v_mapping) {
    c_field destination;
    c_field source;
};

C_CLASS(v_projection);

C_STRUCT(v_projection) {
    c_type resultType;
    c_array rules /*C_ARRAY<kernelModule::v_mapping>*/;
};

C_CLASS(v_filter);

C_STRUCT(v_filter) {
    v_topic topic;
    c_object predicate;
};

C_CLASS(v_instance);

C_STRUCT(v_instance) {
    C_EXTENDS(v_public);
    struct c_time lastDeadlineResetTime;
    c_voidp next;
    c_voidp prev;
};

C_CLASS(v_deadLineInstanceList);

C_STRUCT(v_deadLineInstanceList) {
    C_EXTENDS(v_instance);
    enum v_leaseActionId actionId;
    c_voidp actionObject;
    v_duration leaseDuration;
    v_lease deadlineLease;
    v_leaseManager leaseManager;
};

C_CLASS(v_partitionInterest);

C_STRUCT(v_partitionInterest) {
    c_string expression;
};

C_CLASS(v_partitionAdmin);

C_STRUCT(v_partitionAdmin) {
    C_EXTENDS(v_object);
    c_mutex mutex;
    c_set partitionInterests;
    c_set partitions;
};

C_CLASS(v_cacheItem);

C_STRUCT(v_cacheItem) {
    C_EXTENDS(v_cacheNode);
    c_voidp instance;
};

typedef v_cacheItem v_writerCacheItem;

C_CLASS(v_groupCacheItem);

C_STRUCT(v_groupCacheItem) {
    C_EXTENDS(v_cacheItem);
    c_voidp groupInstance;
    c_long pendingResends;
    c_long registrationCount;
};

C_CLASS(v_dataViewSampleTemplate);

C_STRUCT(v_dataViewSampleTemplate) {
    C_EXTENDS(v_dataViewSample);
    v_readerSample sample;
};

C_CLASS(v_dataViewInstance);

C_STRUCT(v_dataViewInstance) {
    C_EXTENDS(v_public);
    c_voidp dataView;
    v_state instanceState;
    c_long sampleCount;
};

C_CLASS(v_dataViewInstanceTemplate);

C_STRUCT(v_dataViewInstanceTemplate) {
    C_EXTENDS(v_dataViewInstance);
    v_dataViewSample sample;
};

C_CLASS(v_dataReaderSampleTemplate);

C_STRUCT(v_dataReaderSampleTemplate) {
    C_EXTENDS(v_dataReaderSample);
    v_message message;
};

struct v_historyBookmark {
    struct c_time sourceTimestamp;
    v_gid gid;
    c_ulong sequenceNumber;
};

C_CLASS(v_dataReaderInstance);

C_STRUCT(v_dataReaderInstance) {
    C_EXTENDS(v_instance);
    c_long accessibleCount;
    c_long disposeCount;
    struct c_time epoch;
    c_voidp index;
    v_state instanceState;
    struct v_historyBookmark lastConsumed;
    struct c_time lastInsertionTime;
    c_long liveliness;
    c_long noWritersCount;
    struct v_owner owner;
    struct c_time purgeInsertionTime;
    c_long sampleCount;
    c_voidp userDataDataReaderInstance;
    c_bool hasBeenAlive;
    c_bool inNotEmptyList;
};

C_CLASS(v_dataReaderInstanceTemplate);

C_STRUCT(v_dataReaderInstanceTemplate) {
    C_EXTENDS(v_dataReaderInstance);
    c_voidp oldest;
    v_dataReaderSampleTemplate sample;
};

C_CLASS(v_groupSample);

C_STRUCT(v_groupSample) {
    C_EXTENDS(v_lifespanSample);
    c_voidp instance;
    c_voidp newer;
    v_groupSample older;
};

C_CLASS(v_groupSampleTemplate);

C_STRUCT(v_groupSampleTemplate) {
    C_EXTENDS(v_groupSample);
    v_message message;
};

C_CLASS(v_groupInstanceTemplate);

C_STRUCT(v_groupInstanceTemplate) {
    C_EXTENDS(v_groupInstance);
    v_groupSampleTemplate newest;
};

typedef enum v_groupAttachState {
    V_GROUP_ATTACH_STATE_UNKNOWN,
    V_GROUP_ATTACH_STATE_ATTACHED,
    V_GROUP_ATTACH_STATE_NO_INTEREST
} v_groupAttachState;

typedef c_octet v_resendScope;

C_CLASS(v_writerSample);

C_STRUCT(v_writerSample) {
    c_long decayCount;
    c_voidp prev;
    c_ulong sequenceNumber;
    c_bool resend;
    v_resendScope resendScope;
    c_bool sentBefore;
    v_writerSample next;
};

C_CLASS(v_writerSampleTemplate);

C_STRUCT(v_writerSampleTemplate) {
    C_EXTENDS(v_writerSample);
    v_message message;
};

C_CLASS(v_writerInstance);

C_STRUCT(v_writerInstance) {
    C_EXTENDS(v_instance);
    c_long deadlineCount;
    c_voidp last;
    c_long messageCount;
    c_long sampleSequenceNumber;
    c_ulong state;
    c_voidp writer;
    c_bool resend;
    v_cache targetCache;
};

C_CLASS(v_writerInstanceTemplate);

C_STRUCT(v_writerInstanceTemplate) {
    C_EXTENDS(v_writerInstance);
    v_writerSample sample;
};

C_CLASS(v_writerGroup);

C_STRUCT(v_writerGroup) {
    v_group group;
    v_cache targetCache;
    v_writerGroup next;
};

struct v_writerGroupSet {
    v_writerGroup firstGroup;
};

C_CLASS(v_deliveryWaitList);

C_STRUCT(v_deliveryWaitList) {
    c_cond cv;
    c_voidp guard;
    c_mutex mutex;
    c_ulong sequenceNumber;
    c_array readerGID /*C_ARRAY<kernelModule::v_gid>*/;
};

C_CLASS(v_deliveryPublisher);

C_STRUCT(v_deliveryPublisher) {
    c_ulong count;
    v_gid readerGID;
};

C_CLASS(v_deliveryGuard);

C_STRUCT(v_deliveryGuard) {
    c_mutex mutex;
    c_voidp owner;
    v_gid writerGID;
    c_type gidType;
    c_set publications;
    c_set waitlists;
};

C_CLASS(v_writer);

C_STRUCT(v_writer) {
    C_EXTENDS(v_observer);
    c_long count;
    c_long deadlineCountLimit;
    c_long depth;
    struct v_writerGroupSet groupSet;
    c_voidp publisher;
    c_ulong sequenceNumber;
    c_ulong transactionCount;
    c_ulong transactionId;
    c_bool alive;
    c_bool infWait;
    v_writerInstance cachedInstance;
    v_deadLineInstanceList deadlineList;
    v_deliveryGuard deliveryGuard;
    c_type instanceType;
    c_set instances;
    v_lease livelinessLease;
    c_property messageField;
    v_messageQos msgQos;
    c_type msgQosType;
    v_publisherQos pubQos;
    v_writerQos qos;
    v_messageQos relQos;
    c_set resendInstances;
    c_type sampleType;
    v_topic topic;
    v_message transactionsLastMessage;
};

C_CLASS(v_subscriber);

C_STRUCT(v_subscriber) {
    C_EXTENDS(v_observer);
    c_lock lock;
    c_voidp participant;
    c_ulong shareCount;
    c_mutex sharesMutex;
    v_partitionAdmin partitions;
    v_subscriberQos qos;
    c_set readers;
    c_set shares;
};

C_CLASS(v_publisher);

C_STRUCT(v_publisher) {
    C_EXTENDS(v_observer);
    c_lock lock;
    c_voidp participant;
    struct c_time suspendTime;
    c_ulong transactionId;
    v_partitionAdmin partitions;
    v_publisherQos qos;
    c_set writers;
};

#define V_BUILT_IN_PARTICIPANT_NAME ("Built-in participant")
C_CLASS(v_participant);

C_STRUCT(v_participant) {
    C_EXTENDS(v_observer);
    c_mutex builtinLock;
    c_lock lock;
    c_mutex newGroupListMutex;
    c_cond resendCond;
    c_ulong resendIteration;
    c_mutex resendMutex;
    c_bool resendQuit;
    v_subscriber builtinSubscriber;
    c_set entities;
    v_leaseManager leaseManager;
    c_list newGroupList;
    v_participantQos qos;
    c_set resendWriters;
};

typedef enum v_serviceStateKind {
    STATE_NONE,
    STATE_INITIALISING,
    STATE_OPERATIONAL,
    STATE_INCOMPATIBLE_CONFIGURATION,
    STATE_TERMINATING,
    STATE_TERMINATED,
    STATE_DIED
} v_serviceStateKind;

C_CLASS(v_serviceState);

C_STRUCT(v_serviceState) {
    C_EXTENDS(v_observable);
    c_lock lock;
    enum v_serviceStateKind stateKind;
};

C_CLASS(v_service);

C_STRUCT(v_service) {
    C_EXTENDS(v_participant);
    v_lease lease;
    v_serviceState state;
};

C_CLASS(v_networking);

C_STRUCT(v_networking) {
    C_EXTENDS(v_service);
    c_long dummy;
};

C_CLASS(v_durability);

C_STRUCT(v_durability) {
    C_EXTENDS(v_service);
    c_long dummy;
};

C_CLASS(v_cmsoap);

C_STRUCT(v_cmsoap) {
    C_EXTENDS(v_service);
    c_long dummy;
};

C_CLASS(v_rnr);

C_STRUCT(v_rnr) {
    C_EXTENDS(v_service);
    c_long dummy;
};

C_CLASS(v_serviceManager);

C_STRUCT(v_serviceManager) {
    C_EXTENDS(v_observer);
    c_mutex mutex;
    c_set serviceStates;
};

typedef enum v_cfKind {
    V_CFNODE,
    V_CFATTRIBUTE,
    V_CFELEMENT,
    V_CFDATA,
    V_CFNODECOLLECTION,
    V_CFCOUNT
} v_cfKind;

C_CLASS(v_cfElement);

C_CLASS(v_configuration);

C_STRUCT(v_configuration) {
    C_EXTENDS(v_public);
    c_ulong idCounter;
    c_string uri;
    v_cfElement root;
};

C_CLASS(v_cfNode);

C_STRUCT(v_cfNode) {
    c_ulong id;
    enum v_cfKind kind;
    v_configuration configuration;
    c_string name;
};

C_CLASS(v_cfAttribute);

C_STRUCT(v_cfAttribute) {
    C_EXTENDS(v_cfNode);
    struct c_value value;
};

C_STRUCT(v_cfElement) {
    C_EXTENDS(v_cfNode);
    c_set attributes;
    c_set children;
};

C_CLASS(v_cfData);

C_STRUCT(v_cfData) {
    C_EXTENDS(v_cfNode);
    struct c_value value;
};

typedef v_gid v_builtinTopicKey;

typedef enum v_infoId {
    V_PARTICIPANTINFO_ID,
    V_TOPICINFO_ID,
    V_PUBLICATIONINFO_ID,
    V_SUBSCRIPTIONINFO_ID,
    V_HEARTBEATINFO_ID,
    V_DELIVERYINFO_ID,
    V_C_AND_M_COMMAND_ID,
    V_CMPARTICIPANTINFO_ID,
    V_INFO_ID_COUNT
} v_infoId;

struct v_builtinPartitionPolicy {
    c_sequence name /*C_SEQUENCE<c_string>*/;
};

struct v_builtinTopicDataPolicy {
    c_array value /*C_ARRAY<c_octet>*/;
};

struct v_builtinGroupDataPolicy {
    c_array value /*C_ARRAY<c_octet>*/;
};

struct v_builtinUserDataPolicy {
    c_array value /*C_ARRAY<c_octet>*/;
};

struct v_productDataPolicy {
    c_string value;
};

struct v_participantCMInfo {
    v_builtinTopicKey key;
    struct v_productDataPolicy product;
};

struct v_participantInfo {
    v_builtinTopicKey key;
    struct v_userDataPolicy user_data;
};

struct v_topicInfo {
    v_builtinTopicKey key;
    c_string name;
    c_string type_name;
    struct v_durabilityPolicy durability;
    struct v_durabilityServicePolicy durabilityService;
    struct v_deadlinePolicy deadline;
    struct v_latencyPolicy latency_budget;
    struct v_livelinessPolicy liveliness;
    struct v_reliabilityPolicy reliability;
    struct v_transportPolicy transport_priority;
    struct v_lifespanPolicy lifespan;
    struct v_orderbyPolicy destination_order;
    struct v_historyPolicy history;
    struct v_resourcePolicy resource_limits;
    struct v_ownershipPolicy ownership;
    struct v_builtinTopicDataPolicy topic_data;
    c_string meta_data;
    c_string key_list;
};

struct v_publicationInfo {
    v_builtinTopicKey key;
    v_builtinTopicKey participant_key;
    c_string topic_name;
    c_string type_name;
    struct v_durabilityPolicy durability;
    struct v_deadlinePolicy deadline;
    struct v_latencyPolicy latency_budget;
    struct v_livelinessPolicy liveliness;
    struct v_reliabilityPolicy reliability;
    struct v_lifespanPolicy lifespan;
    struct v_orderbyPolicy destination_order;
    struct v_builtinUserDataPolicy user_data;
    struct v_ownershipPolicy ownership;
    struct v_strengthPolicy ownership_strength;
    struct v_presentationPolicy presentation;
    struct v_builtinPartitionPolicy partition;
    struct v_builtinTopicDataPolicy topic_data;
    struct v_builtinGroupDataPolicy group_data;
    struct v_writerLifecyclePolicy lifecycle;
    c_bool alive;
};

C_CLASS(v_publicationInfoTemplate);

C_STRUCT(v_publicationInfoTemplate) {
    C_EXTENDS(v_message);
    struct v_publicationInfo userData;
};

struct v_subscriptionInfo {
    v_builtinTopicKey key;
    v_builtinTopicKey participant_key;
    c_string topic_name;
    c_string type_name;
    struct v_durabilityPolicy durability;
    struct v_deadlinePolicy deadline;
    struct v_latencyPolicy latency_budget;
    struct v_livelinessPolicy liveliness;
    struct v_reliabilityPolicy reliability;
    struct v_ownershipPolicy ownership;
    struct v_orderbyPolicy destination_order;
    struct v_builtinUserDataPolicy user_data;
    struct v_pacingPolicy time_based_filter;
    struct v_presentationPolicy presentation;
    struct v_builtinPartitionPolicy partition;
    struct v_builtinTopicDataPolicy topic_data;
    struct v_builtinGroupDataPolicy group_data;
    struct v_readerLifespanPolicy lifespan;
};

C_CLASS(v_subscriptionInfoTemplate);

C_STRUCT(v_subscriptionInfoTemplate) {
    C_EXTENDS(v_message);
    struct v_subscriptionInfo userData;
};

struct v_deliveryInfo {
    v_gid writerGID;
    v_gid readerGID;
    c_ulong sequenceNumber;
};

C_CLASS(v_deliveryInfoTemplate);

C_STRUCT(v_deliveryInfoTemplate) {
    C_EXTENDS(v_message);
    struct v_deliveryInfo userData;
};

struct v_heartbeatInfo {
    v_gid id;
    v_duration period;
};

typedef enum v_controlAndMonitoringCommandKind {
    V_COMMAND_DISPOSE_ALL_DATA
} v_controlAndMonitoringCommandKind;

struct v_commandDisposeAllData {
    c_string topicExpr;
    c_string partitionExpr;
};

struct command_kind {
    enum v_controlAndMonitoringCommandKind _d;
    union {
        struct v_commandDisposeAllData dispose_all_data_info;
    } _u;
};

struct v_controlAndMonitoringCommand_s {
    v_builtinTopicKey key;
    struct command_kind u;
};

typedef struct v_controlAndMonitoringCommand_s v_controlAndMonitoringCommand;

#define V_OWNERSTRENGTH_MINIMUM (2147483647)
C_CLASS(v_index);

C_STRUCT(v_index) {
    C_EXTENDS(v_object);
    c_voidp entry;
    c_voidp reader;
    c_array messageKeyList /*C_ARRAY<c_field>*/;
    c_set notEmptyList;
    c_type objectType;
    c_set objects;
    c_array sourceKeyList /*C_ARRAY<c_field>*/;
};

C_CLASS(v_purgeListItem);

C_STRUCT(v_purgeListItem) {
    c_long genCount;
    struct c_time insertionTime;
    v_dataReaderInstance instance;
};

C_CLASS(v_transaction);

C_STRUCT(v_transaction) {
    c_long count;
    c_ulong transactionId;
    v_gid writerGID;
};

C_CLASS(v_dataReaderEntry);

C_STRUCT(v_dataReaderEntry) {
    C_EXTENDS(v_entry);
    c_array filterData /*C_ARRAY<kernelModule::v_filter>*/;
    c_array filterInstance /*C_ARRAY<kernelModule::v_filter>*/;
    v_index index;
    v_lifespanAdmin lifespanAdmin;
    c_list purgeListDisposed;
    c_list purgeListNotEmpty;
    v_topic topic;
    c_list transactionList;
};

C_CLASS(v_deadLineReaderInstanceList);

C_STRUCT(v_deadLineReaderInstanceList) {
    c_voidp lastInstance;
    v_dataReaderInstance firstInstance;
};

C_CLASS(v_dataView);

C_STRUCT(v_dataView) {
    C_EXTENDS(v_collection);
    c_voidp reader;
    c_type instanceType;
    c_set instances;
    v_dataViewQos qos;
    c_type sampleType;
    v_dataViewInstance takenInstance;
};

C_CLASS(v_dataReader);

C_STRUCT(v_dataReader) {
    C_EXTENDS(v_reader);
    c_long cachedSampleCount;
    c_long depth;
    c_long notReadCount;
    c_long notReadTriggerCount;
    c_long notReadTriggerThreshold;
    c_ulong readCnt;
    c_long sampleCount;
    c_ulong shareCount;
    c_ulong updateCnt;
    c_bool maxInstances;
    c_bool walkRequired;
    v_dataReaderSample cachedSample;
    v_deadLineInstanceList deadLineList;
    v_index index;
    v_projection projection;
    c_type sampleType;
    v_dataReaderSample triggerValue;
    c_set views;
};

C_CLASS(v_deliveryService);

C_STRUCT(v_deliveryService) {
    C_EXTENDS(v_reader);
    c_mutex mutex;
    c_set guards;
    c_set subscriptions;
};

C_CLASS(v_deliveryServiceEntry);

C_STRUCT(v_deliveryServiceEntry) {
    C_EXTENDS(v_entry);
    v_topic topic;
};

typedef enum v_groupActionKind {
    V_GROUP_ACTION_WRITE,
    V_GROUP_ACTION_DISPOSE,
    V_GROUP_ACTION_REGISTER,
    V_GROUP_ACTION_UNREGISTER,
    V_GROUP_ACTION_LIFESPAN_EXPIRE,
    V_GROUP_ACTION_CLEANUP_DELAY_EXPIRE,
    V_GROUP_ACTION_DELETE_DATA
} v_groupActionKind;

C_CLASS(v_groupAction);

C_STRUCT(v_groupAction) {
    C_EXTENDS(v_object);
    struct c_time actionTime;
    enum v_groupActionKind kind;
    v_group group;
    v_message message;
};

C_CLASS(v_groupQueueSample);

C_STRUCT(v_groupQueueSample) {
    v_groupAction action;
    v_groupQueueSample next;
};

C_CLASS(v_groupQueue);

C_STRUCT(v_groupQueue) {
    C_EXTENDS(v_groupStream);
    c_ulong maxSize;
    c_ulong size;
    c_bool markerReached;
    v_groupQueueSample head;
    v_groupQueueSample marker;
    v_groupQueueSample tail;
};

typedef c_ulong v_networkId;

#define V_NETWORKID_LOCAL (0U)
#define V_NETWORKID_ANY (4294967295U)
#define V_NETWORKID_DDSI (2880289470U)
typedef c_ulong v_networkPartitionId;

#define V_NETWORKPARTITIONID_LOCALHOST (4294967295U)
struct v_networkHashValue_s {
    c_octet h1;
    c_octet h2;
    c_octet h3;
    c_octet h4;
};

typedef struct v_networkHashValue_s v_networkHashValue;

C_CLASS(v_networkReaderEntry);

C_STRUCT(v_networkReaderEntry) {
    C_EXTENDS(v_entry);
    c_ulong channelCountdown;
    c_mutex channelCountdownMutex;
    c_voidp group;
    v_networkId networkId;
    v_networkPartitionId networkPartitionId;
    v_networkHashValue hashValue;
    c_bool isRouting;
};

C_CLASS(v_networkQueueSample);

C_STRUCT(v_networkQueueSample) {
    v_gid receiver;
    v_gid sender;
    c_ulong sequenceNumber;
    c_bool sendTo;
    v_networkReaderEntry entry;
    v_message message;
    v_networkQueueSample next;
};

C_CLASS(v_networkStatusMarker);

C_STRUCT(v_networkStatusMarker) {
    c_voidp lastSample;
    c_ulong priority;
    struct c_time sendBefore;
    v_networkQueueSample firstSample;
    v_networkStatusMarker next;
};

C_CLASS(v_networkQueue);

C_STRUCT(v_networkQueue) {
    c_ulong currentMsgCount;
    c_cond cv;
    c_voidp lastStatusMarker;
    c_ulong maxMsgCount;
    c_ulong msecsResolution;
    c_mutex mutex;
    struct c_time nextWakeup;
    c_ulong phaseMilliSeconds;
    c_ulong priority;
    struct c_time resolution;
    c_bool P2P;
    c_bool periodic;
    c_bool reliable;
    c_bool threadWaiting;
    c_bool triggered;
    v_networkStatusMarker firstStatusMarker;
    v_networkQueueSample freeSamples;
    v_networkStatusMarker freeStatusMarkers;
    c_type sampleType;
    v_networkQueueStatistics statistics;
    c_type statusMarkerType;
};

C_CLASS(v_networkReader);

C_STRUCT(v_networkReader) {
    C_EXTENDS(v_reader);
    c_ulong nofQueues;
    c_bool ignoreReliabilityQoS;
    c_bool remoteActivity;
    v_networkQueue defaultQueue;
    c_array queueCache /*C_ARRAY<kernelModule::v_networkQueue>*/;
    c_array queues /*C_ARRAY<kernelModule::v_networkQueue>*/;
};

C_CLASS(v_groupSet);

C_STRUCT(v_groupSet) {
    C_EXTENDS(v_observable);
    c_lock lock;
    c_long sequenceNumber;
    c_set groups;
};

typedef c_string v_statisticsCategory;

C_CLASS(v_builtin);

C_STRUCT(v_builtin) {
    v_topic topics[8] /*C_ARRAY<kernelModule::v_topic,8>*/;
    v_writer writers[8] /*C_ARRAY<kernelModule::v_writer,8>*/;
    v_kernelQos kernelQos;
    v_participant participant;
    v_publisher publisher;
};

#define V_CRC_TABLE_SIZE (256U)
#define V_CRC_KEY (79764919U)
C_CLASS(v_crc);

C_STRUCT(v_crc) {
    c_ulong key;
    c_ulong table[256] /*C_ARRAY<c_ulong,256>*/;
};

C_CLASS(v_pendingDisposeElement);

C_STRUCT(v_pendingDisposeElement) {
    struct v_commandDisposeAllData disposeCmd;
    struct c_time disposeTimestamp;
};

C_CLASS(v_kernel);

C_STRUCT(v_kernel) {
    C_EXTENDS(v_observable);
    v_gid GID;
    c_lock lock;
    c_ulong maxInstancesWarnLevel;
    c_ulong maxSamplesPerInstanceWarnLevel;
    c_ulong maxSamplesWarnLevel;
    c_ulong networkServiceCount;
    c_mutex pendingDisposeListMutex;
    c_mutex sharesMutex;
    c_ulong transactionCount;
    c_type type[88] /*C_ARRAY<c_type,88>*/;
    c_ulong userCount;
    c_bool maxInstancesWarnShown;
    c_bool maxSamplesPerInstanceWarnShown;
    c_bool maxSamplesWarnShown;
    c_bool splicedRunning;
    v_builtin builtin;
    v_configuration configuration;
    v_crc crc;
    v_deliveryService deliveryService;
    c_set enabledStatisticsCategories;
    v_groupSet groupSet;
    v_handleServer handleServer;
    v_leaseManager livelinessLM;
    c_set participants;
    c_set partitions;
    c_list pendingDisposeList;
    v_kernelQos qos;
    v_serviceManager serviceManager;
    c_set shares;
    c_set topics;
};

#define V_SPLICED_NAME ("splicedaemon")
C_CLASS(v_spliced);

C_STRUCT(v_spliced) {
    C_EXTENDS(v_service);
    c_set builtinData[8] /*C_ARRAY<kernelModule::C_SET<kernelModule::v_message>,8>*/;
    c_mutex builtinDataMutex;
    c_mutex cAndMCommandMutex;
    struct v_heartbeatInfo hb;
    c_mutex mtx;
    v_dataReader readers[8] /*C_ARRAY<kernelModule::v_dataReader,8>*/;
    c_bool cAndMCommandDispatcherQuit;
    c_bool quit;
    v_subscriber builtinSubscriber;
    v_waitset cAndMCommandWaitSet;
    v_lease hbCheck;
    v_leaseManager hbManager;
    v_lease hbUpdate;
    c_set missedHB;
    v_waitset ws;
};

OS_API c_bool
loadkernelModule(
    c_base base);

#undef OS_API


#endif /* MODULE_kernelModule_HEADER */

