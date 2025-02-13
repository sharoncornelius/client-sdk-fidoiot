/*
 * Copyright 2020 Intel Corporation
 * SPDX-License-Identifier: Apache 2.0
 */

#ifndef __FDOKEYEXCHANGE_H__
#define __FDOKEYEXCHANGE_H__

#include "fdotypes.h"
#include <stdbool.h>

typedef enum { KEXSUITE_NONE, DHKEXid14, ASYMKEX, ECDH } key_ex_suite_t;
typedef enum { HMAC_NONE, HMAC_SHA256, HMAC_SHA384 } hmac_name_t;
typedef enum { HASH_SHA1, HASH_SHA256, HASH_SHA384 } hash_type_t;
typedef enum { NIST_P_256, NIST_P_384 } ecccurve_type_t;
typedef enum { RFC3526_P2048, RFC3526_P3072 } modp_group_ke_t;

int32_t fdo_set_kex_paramA(fdo_byte_array_t *xA, fdo_public_key_t *encrypt_key);
int32_t fdo_get_kex_paramB(fdo_byte_array_t **xB);
fdo_string_t *get_kex_name(void);
fdo_string_t *get_cipher_suite_name(void);

int set_sek_svk(uint8_t *sek, uint32_t sek_len, uint8_t *svk, uint8_t svk_len);

#endif /* __FDOKEYEXCHANGE_H__ */
