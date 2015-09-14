#include <stdio.h>
#include <unistd.h>
#include "sample.h"

int main(void)
{
    DDS_DomainParticipantFactory domain_participant_factory = DDS_OBJECT_NIL;

    domain_participant_factory = DDS_DomainParticipantFactory_get_instance();
    DDS_DomainParticipantFactory_create_participant(domain_participant_factory, DDS_DOMAIN_ID_DEFAULT, DDS_PARTICIPANT_QOS_DEFAULT, NULL, DDS_STATUS_MASK_NONE);
    printf("Connected to domain: %d\n", DDS_DOMAIN_ID_DEFAULT);
 
    while(1)
    {

        sleep(5);
    }
}
