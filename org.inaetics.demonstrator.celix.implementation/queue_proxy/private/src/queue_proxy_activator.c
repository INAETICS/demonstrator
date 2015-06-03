/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */

#include <stdlib.h>

#include "bundle_activator.h"
#include "service_registration.h"

#include "queue_proxy_impl.h"
#include "inaetics_demonstrator_api/sample_queue.h"

#include "remote_proxy.h"

struct activator {
    bundle_context_pt context;
    remote_proxy_factory_pt factory_ptr;
};

static celix_status_t queueProxyFactory_create(void *handle, endpoint_description_pt endpointDescription, remote_service_admin_pt rsa, sendToHandle sendToCallback, properties_pt properties,
        void **service);
static celix_status_t queueProxyFactory_destroy(void *handle, void *service);


celix_status_t bundleActivator_create(bundle_context_pt context, void **userData) {
    celix_status_t status = CELIX_SUCCESS;
    struct activator *activator;

    activator = calloc(1, sizeof(*activator));
    if (!activator) {
        status = CELIX_ENOMEM;
    } else {
        activator->factory_ptr = NULL;
        activator->context = context;

        *userData = activator;
    }

    return status;
}

celix_status_t bundleActivator_start(void * userData, bundle_context_pt context) {
    celix_status_t status = CELIX_SUCCESS;
    struct activator *activator = userData;

    remoteProxyFactory_create(context, (char *) INAETICS_DEMONSTRATOR_API__SAMPLE_QUEUE_SERVICE_NAME, activator, queueProxyFactory_create, queueProxyFactory_destroy, &activator->factory_ptr);
    remoteProxyFactory_register(activator->factory_ptr);

    return status;
}

celix_status_t bundleActivator_stop(void * userData, bundle_context_pt context) {
    celix_status_t status = CELIX_SUCCESS;
    struct activator *activator = userData;

    remoteProxyFactory_unregister(activator->factory_ptr);
    remoteProxyFactory_destroy(&activator->factory_ptr);

    return status;
}

celix_status_t bundleActivator_destroy(void * userData, bundle_context_pt context) {
    celix_status_t status = CELIX_SUCCESS;
    struct activator *activator = userData;

    free(activator);

    return status;
}

static celix_status_t queueProxyFactory_create(void *handle, endpoint_description_pt endpointDescription, remote_service_admin_pt rsa, sendToHandle sendToCallback, properties_pt properties,
        void **service) {
    celix_status_t status = CELIX_SUCCESS;
    struct activator *activator = handle;
    struct sample_queue_service* queueService = NULL;

    queueService = calloc(1, sizeof(*queueService));

    if (queueService) {

        sample_queue_type* sampleQueue;
        queueProxy_create(activator->context, &sampleQueue);

        queueService->put = queueProxy_put;
        queueService->putAll = queueProxy_putAll;
        queueService->take = queueProxy_take;
        queueService->takeAll = queueProxy_takeAll;

        sampleQueue->endpoint = endpointDescription;
        sampleQueue->sendToHandler = rsa;
        sampleQueue->sendToCallback = sendToCallback;

        queueService->sampleQueue = (void*) sampleQueue;

        *service = queueService;
    } else {
        status = CELIX_ENOMEM;
    }
    return status;
}

static celix_status_t queueProxyFactory_destroy(void *handle, void *service) {
    celix_status_t status = CELIX_SUCCESS;
    struct sample_queue_service* queueService = service;

    if (!queueService) {
        status = CELIX_ILLEGAL_ARGUMENT;
    }
    else {
        queueProxy_destroy((sample_queue_type**)&queueService->sampleQueue);
        free(queueService);
    }

    return status;
}
