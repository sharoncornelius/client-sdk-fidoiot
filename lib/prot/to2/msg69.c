/*
 * Copyright 2020 Intel Corporation
 * SPDX-License-Identifier: Apache 2.0
 */

/*!
 * \file
 * \brief This file implements msg49 of TO2 state machine.
 */

#include "fdoprot.h"
#include "fdokeyexchange.h"
#include "util.h"

/**
 * msg49() - TO2.OwnerServiceInfo
 * Device receives the Owner ServiceInfo.
 *
 * TO2.OwnerServiceInfo = [
 *   IsMoreServiceInfo,		// bool
 *   IsDone,				// bool
 *   ServiceInfo
 * ]
 * where,
 * ServiceInfo = [
 *   *ServiceInfoKeyVal
 * ]
 * ServiceInfoKeyVal = [
 *   *ServiceInfoKV
 * ]
 * ServiceInfoKV = [
 *   ServiceInfoKey: tstr,
 *   ServiceInfoVal: cborSimpleType
 * ]
 */
int32_t msg69(fdo_prot_t *ps)
{
	int ret = -1;
	char prot[] = "FDOProtTO2";
	fdo_encrypted_packet_t *pkt = NULL;
	bool IsMoreServiceInfo;
	bool isDone;
	int module_ret_val = -1;
	fdo_sdk_service_info_module_list_t *module_list_itr = NULL;

	if (!fdo_check_to2_round_trips(ps)) {
		goto err;
	}

	if (!fdo_prot_rcv_msg(&ps->fdor, &ps->fdow, prot, &ps->state)) {
		ret = 0; /* Get the data, and come back */
		goto err;
	}

	LOG(LOG_DEBUG, "TO2.OwnerServiceInfo started\n");

	/* If the packet is encrypted, decrypt it */
	pkt = fdo_encrypted_packet_read(&ps->fdor);
	if (pkt == NULL) {
		LOG(LOG_ERROR, "TO2.OwnerServiceInfo: Failed to parse encrypted packet\n");
		goto err;
	}
	if (!fdo_encrypted_packet_unwind(&ps->fdor, pkt, ps->iv)) {
		LOG(LOG_ERROR, "TO2.OwnerServiceInfo: Failed to decrypt packet!\n");
		goto err;
	}

	if (!fdor_start_array(&ps->fdor)) {
		LOG(LOG_ERROR, "TO2.OwnerServiceInfo: Failed to start array\n");
		goto err;
	}

	if (!fdor_boolean(&ps->fdor, &IsMoreServiceInfo)) {
		LOG(LOG_ERROR, "TO2.OwnerServiceInfo: Failed to read IsMoreServiceInfo\n");
		goto err;
	}

	if (!fdor_boolean(&ps->fdor, &isDone)) {
		LOG(LOG_ERROR, "TO2.OwnerServiceInfo: Failed to read IsDone\n");
		goto err;
	}

	if (ps->device_serviceinfo_ismore && !IsMoreServiceInfo && !isDone) {
		// Expecting ServiceInfo to be an empty array [].
		if (!fdor_start_array(&ps->fdor)) {
			LOG(LOG_ERROR, "TO2.OwnerServiceInfo: Failed to start empty ServiceInfo array\n");
			goto err;
		}

		if (!fdor_end_array(&ps->fdor)) {
			LOG(LOG_ERROR, "TO2.OwnerServiceInfo: Failed to end empty ServiceInfo array\n");
			goto err;
		}
	} else {
		// the message [bool,bool, [],[]..], when CBOR encoded, will always take up 3 bytes:
		// 1 byte for main array, 1 byte each for bool values.
		// the remaining data is the ServiceInfo, and hence we can deduce the size of received
		// ServiceInfo by subtracting 3 from the total message length.
		if ((int)(ps->fdor.b.block_size - 3) <= ps->maxOwnerServiceInfoSz) {
			// process the received ServiceInfo
			module_list_itr = ps->sv_info_mod_list_head;
			if (!fdo_serviceinfo_read(&ps->fdor, module_list_itr, &module_ret_val)) {
				LOG(LOG_ERROR, "TO2.OwnerServiceInfo: Failed to read ServiceInfo\n");
				goto err;
			}
		} else {
			// do not process ServiceInfo since the ServiceInfo size received is more than the
			// agreed maxOwnerServiceInfoSz from TO2.OwnerServiceInfoReady, Type 67
			LOG(LOG_ERROR,
				"TO2.OwnerServiceInfo: Received ServiceInfo size is greater than maxOwnerServiceInfoSz\n");
			goto err;
		}
	}

	if (!fdor_end_array(&ps->fdor)) {
		LOG(LOG_ERROR, "TO2.OwnerServiceInfo: Failed to end array\n");
		goto err;
	}

	if (isDone) {
		ps->state = FDO_STATE_TO2_SND_DONE;
	} else {
		ps->state = FDO_STATE_T02_SND_GET_NEXT_OWNER_SERVICE_INFO;
	}

	LOG(LOG_DEBUG, "TO2.OwnerServiceInfo completed successfully\n");
	ret = 0; /*Mark as success */

err:
	fdo_block_reset(&ps->fdor.b);
	ps->fdor.have_block = false;
	return ret;
}
