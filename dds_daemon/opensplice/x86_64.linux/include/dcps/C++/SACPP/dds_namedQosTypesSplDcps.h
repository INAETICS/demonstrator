#ifndef DDS_NAMEDQOSTYPESSPLTYPES_H
#define DDS_NAMEDQOSTYPESSPLTYPES_H

#include "ccpp_dds_namedQosTypes.h"

#include <c_base.h>
#include <c_misc.h>
#include <c_sync.h>
#include <c_collection.h>
#include <c_field.h>

#include "dds_dcps_builtintopicsSplDcps.h"

extern c_metaObject __dds_namedQosTypes_DDS__load (c_base base);

extern c_metaObject __DDS_NamedDomainParticipantQos__load (c_base base);
extern const char * __DDS_NamedDomainParticipantQos__keys (void);
extern const char * __DDS_NamedDomainParticipantQos__name (void);
struct _DDS_NamedDomainParticipantQos ;
extern  c_bool __DDS_NamedDomainParticipantQos__copyIn(c_base base, struct DDS::NamedDomainParticipantQos *from, struct _DDS_NamedDomainParticipantQos *to);
extern  void __DDS_NamedDomainParticipantQos__copyOut(void *_from, void *_to);
struct _DDS_NamedDomainParticipantQos {
    c_string name;
    struct _DDS_DomainParticipantQos domainparticipant_qos;
};

extern c_metaObject __DDS_NamedPublisherQos__load (c_base base);
extern const char * __DDS_NamedPublisherQos__keys (void);
extern const char * __DDS_NamedPublisherQos__name (void);
struct _DDS_NamedPublisherQos ;
extern  c_bool __DDS_NamedPublisherQos__copyIn(c_base base, struct DDS::NamedPublisherQos *from, struct _DDS_NamedPublisherQos *to);
extern  void __DDS_NamedPublisherQos__copyOut(void *_from, void *_to);
struct _DDS_NamedPublisherQos {
    c_string name;
    struct _DDS_PublisherQos publisher_qos;
};

extern c_metaObject __DDS_NamedSubscriberQos__load (c_base base);
extern const char * __DDS_NamedSubscriberQos__keys (void);
extern const char * __DDS_NamedSubscriberQos__name (void);
struct _DDS_NamedSubscriberQos ;
extern  c_bool __DDS_NamedSubscriberQos__copyIn(c_base base, struct DDS::NamedSubscriberQos *from, struct _DDS_NamedSubscriberQos *to);
extern  void __DDS_NamedSubscriberQos__copyOut(void *_from, void *_to);
struct _DDS_NamedSubscriberQos {
    c_string name;
    struct _DDS_SubscriberQos subscriber_qos;
};

extern c_metaObject __DDS_NamedTopicQos__load (c_base base);
extern const char * __DDS_NamedTopicQos__keys (void);
extern const char * __DDS_NamedTopicQos__name (void);
struct _DDS_NamedTopicQos ;
extern  c_bool __DDS_NamedTopicQos__copyIn(c_base base, struct DDS::NamedTopicQos *from, struct _DDS_NamedTopicQos *to);
extern  void __DDS_NamedTopicQos__copyOut(void *_from, void *_to);
struct _DDS_NamedTopicQos {
    c_string name;
    struct _DDS_TopicQos topic_qos;
};

extern c_metaObject __DDS_NamedDataWriterQos__load (c_base base);
extern const char * __DDS_NamedDataWriterQos__keys (void);
extern const char * __DDS_NamedDataWriterQos__name (void);
struct _DDS_NamedDataWriterQos ;
extern  c_bool __DDS_NamedDataWriterQos__copyIn(c_base base, struct DDS::NamedDataWriterQos *from, struct _DDS_NamedDataWriterQos *to);
extern  void __DDS_NamedDataWriterQos__copyOut(void *_from, void *_to);
struct _DDS_NamedDataWriterQos {
    c_string name;
    struct _DDS_DataWriterQos datawriter_qos;
};

extern c_metaObject __DDS_NamedDataReaderQos__load (c_base base);
extern const char * __DDS_NamedDataReaderQos__keys (void);
extern const char * __DDS_NamedDataReaderQos__name (void);
struct _DDS_NamedDataReaderQos ;
extern  c_bool __DDS_NamedDataReaderQos__copyIn(c_base base, struct DDS::NamedDataReaderQos *from, struct _DDS_NamedDataReaderQos *to);
extern  void __DDS_NamedDataReaderQos__copyOut(void *_from, void *_to);
struct _DDS_NamedDataReaderQos {
    c_string name;
    struct _DDS_DataReaderQos datareader_qos;
};

#endif
