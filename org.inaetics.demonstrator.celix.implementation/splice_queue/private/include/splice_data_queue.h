/* ************************** Copyright disclosure ****************************
* This document carrier contains proprietary information which shall not be
* used for other purposes than those for which it has been released, nor be
* reproduced or disclosed to third parties, without the prior written
* consent of Thales Nederland B.V.
* ****************************************************************************/

/*** Header ***
* \file            splice_data_queue.h
*
* \brief           Contains the functions to store sample data in open splice.
*
*/

#ifndef __SPLICE_DATA_QUEUE_H__
#define __SPLICE_DATA_QUEUE_H__

/****************************************************************
 *
 * Includes
 *
 ****************************************************************/
#include "sample.h"

/****************************************************************
 *
 * Defines
 *
 ****************************************************************/

/****************************************************************
 *
 * Macros
 *
 ****************************************************************/

/****************************************************************
 *
 * Type definitions
 *
 ****************************************************************/
typedef struct splice_data_queue_type* splice_data_queue_handle;

/****************************************************************
 *
 * External data declarations
 *
 ****************************************************************/

/****************************************************************
 *
 * External function prototypes
 *
 ****************************************************************/

extern splice_data_queue_handle
splice_data_queue_create(DDS_DomainId_t domain_id);

extern void
splice_data_queue_destroy(splice_data_queue_handle const p_handle);

extern void
splice_data_queue_put(splice_data_queue_handle const p_handle, const struct queue_sample *sample);

extern void
splice_data_queue_get_one(splice_data_queue_handle const p_handle, struct queue_sample *sample);

extern int
splice_data_queue_get_multiple(splice_data_queue_handle const p_handle, int num_samples, struct queue_sample *sample);

extern void
splice_data_queue_number_of_samples(splice_data_queue_handle const p_handle);

#endif /* __SPLICE_DATA_QUEUE_H__ */
