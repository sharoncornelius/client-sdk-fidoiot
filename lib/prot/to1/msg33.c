/*
 * Copyright 2020 Intel Corporation
 * SPDX-License-Identifier: Apache 2.0
 */

/*!
 * \file
 * \brief Implementation of TO1 protocol message 33.
 */

#include "safe_lib.h"
#include "util.h"
#include "fdoprot.h"

/**
 * msg33() - TO1.RVRedirect, Type 33
 * This is the last message of TO1. The device receives the owner info from RV.
 *
 * TO1.RVRedirect = to1d
 * to1d = CoseSignature
 * [
 * {keyalg:<value>},	// protected header
 * {},					// unprotected header
 * to1dBlobPayload,		// COSEPayloads
 * signature			// signature
 * ]
 * where,
 *	to1dBlobPayload = [
 *	[+[RVIP, RVDNS, RVPort, RVProtocol]], // one or more array of inner entries (inner array)
 *	[hashtype, hash]
 *	]
 *
 */
int32_t msg33(fdo_prot_t *ps)
{
	int ret = -1;
	fdo_hash_t *ob_hash = NULL;
	char prot[] = "FDOProtTO1";

	/* Try to read from internal buffer */
	if (!fdo_prot_rcv_msg(&ps->fdor, &ps->fdow, prot, &ps->state)) {
		ret = 0; /*Mark for retry */
		goto err;
	}

	LOG(LOG_DEBUG, "TO1.RVRedirect started\n");

	// allocate memory for to1d here, free when TO2 is done
	if (ps->to1d_cose) {
		fdo_cose_free(ps->to1d_cose);
	}
	ps->to1d_cose = fdo_alloc(sizeof(fdo_cose_t));
	if (!ps->to1d_cose) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to alloc COSE\n");
		goto err;
	}

	if (!fdo_cose_read(&ps->fdor, ps->to1d_cose, true)) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to read COSE\n");
		goto err;
	}

	// clear the FDOR buffer and push COSE payload into it, essentially reusing the FDOR object.
	fdo_block_reset(&ps->fdor.b);
	ps->fdor.b.block_size = ps->to1d_cose->cose_payload->byte_sz;
	if (0 != memcpy_s(ps->fdor.b.block, ps->fdor.b.block_size,
		ps->to1d_cose->cose_payload->bytes, ps->to1d_cose->cose_payload->byte_sz)) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to copy Nonce4\n");
		goto err;
	}

	// initialize the parser once the buffer contains COSE payload to be decoded.
	if (!fdor_parser_init(&ps->fdor)) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to initialize FDOR parser\n");
		goto err;
	}

	size_t num_payloadbasemap_items = 0;
	if (!fdor_array_length(&ps->fdor, &num_payloadbasemap_items) ||
		num_payloadbasemap_items != 2) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to read array length\n");
		goto err;
	}

	if (!fdor_start_array(&ps->fdor)) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to start array\n");
		goto err;
	}
	// allocate here, free when TO2 is done
	if (ps->rvto2addr) {
		fdo_rvto2addr_free(ps->rvto2addr);
	}
	ps->rvto2addr = fdo_alloc(sizeof(fdo_rvto2addr_t));
	if (!ps->rvto2addr) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to alloc to1dRV\n");
		goto err;
	}
	if (!fdo_rvto2addr_read(&ps->fdor, ps->rvto2addr)) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to read to1dRV\n");
		goto err;
	}

	// read hash now.
	ob_hash = fdo_hash_alloc_empty();
	if (!ob_hash || !fdo_hash_read(&ps->fdor, ob_hash)) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to read to1dTo0dHash\n");
		goto err;
	}

	if (!fdor_end_array(&ps->fdor)) {
		LOG(LOG_ERROR, "TO1.RVRedirect: Failed to end array\n");
		goto err;
	}

	/* Mark as success and ready for TO2 */
	ps->state = FDO_STATE_DONE;
	fdo_block_reset(&ps->fdor.b);
	ps->fdor.have_block = false;
	ret = 0;
	LOG(LOG_DEBUG, "TO1.RVRedirect completed successfully\n");

err:
	if (ob_hash) {
		fdo_hash_free(ob_hash);
		ob_hash = NULL;
	}
	return ret;
}
