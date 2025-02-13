/*
 * Copyright 2020 Intel Corporation
 * SPDX-License-Identifier: Apache 2.0
 */

#ifndef __FDOTYPES_H__
#define __FDOTYPES_H__

#include "fdo.h"
#include "fdoblockio.h"
#include "fdomodules.h"
#include <stddef.h>

/*
 * Wrapper around a URL
 * Just a string for now.
 */
typedef char *fdourl_t;

// Generic bit holder
typedef struct {
	size_t byte_sz;
	uint8_t *bytes;
} fdo_bits_t;

fdo_bits_t *fdo_bits_init(fdo_bits_t *b, int byte_sz);
fdo_bits_t *fdo_bits_alloc(int byte_sz);
fdo_bits_t *fdo_bits_alloc_with(int byte_sz, uint8_t *data);
void fdo_bits_free(fdo_bits_t *b);
void fdo_bits_empty(fdo_bits_t *b);
fdo_bits_t *fdo_bits_clone(fdo_bits_t *b);
bool fdo_bits_resize(fdo_bits_t *b, int byte_sz);
bool fdo_bits_fill(fdo_bits_t **b);
bool fdo_bits_fill_with(fdo_bits_t *b, uint8_t *data, uint32_t data_len);
bool fdo_bits_resize_with(fdo_bits_t *b, int new_byte_sz, uint8_t *data);
bool fdo_bits_equal(fdo_bits_t *b1, fdo_bits_t *b2);
int fdo_bits_randomize(fdo_bits_t *b);
char *fdo_bits_to_string_hex(fdo_bits_t *b, char *buf, int buf_sz);

#if 0
void fdo_bits_write(fdow_t *fdow, fdo_bits_t *b);
bool fdo_bits_read(fdor_t *fdor, fdo_bits_t *b);
#endif

// Byte Array
typedef fdo_bits_t fdo_byte_array_t;

#if 0
fdo_byte_array_t *fdo_byte_array_init(fdo_byte_array_t *bn, int byte_sz);
#endif
fdo_byte_array_t *fdo_byte_array_alloc(int byte_sz);
fdo_byte_array_t *fdo_byte_array_alloc_with_int(int val);
fdo_byte_array_t *fdo_byte_array_alloc_with_byte_array(uint8_t *ba, int ba_len);
void fdo_byte_array_free(fdo_byte_array_t *ba);
#if 0
void fdo_byte_array_empty(fdo_byte_array_t *ba);
#endif
bool fdo_byte_array_resize(fdo_byte_array_t *b, int byte_sz);
#if 0
bool fdo_byte_array_resize_with(fdo_byte_array_t *b, int new_byte_sz, uint8_t *data);
#endif
fdo_byte_array_t *fdo_byte_array_append(fdo_byte_array_t *baA,
					fdo_byte_array_t *baB);
fdo_byte_array_t *fdo_byte_array_clone(fdo_byte_array_t *ba);
bool fdo_byte_array_equal(fdo_byte_array_t *ba1, fdo_byte_array_t *ba2);

// Bignum

typedef struct {
	bool sign;
	fdo_bits_t *value;
} fdo_bignum_t;

#define BN_POSITIVE true
#define BN_NEGATIVE false

fdo_bignum_t *fdo_big_num_alloc(void);
void fdo_big_num_free(fdo_bignum_t *bn);
bool fdo_bignum_equal(fdo_bignum_t *bn1, fdo_bignum_t *bn2);
char *fdo_bignum_to_string(fdo_bignum_t *g, char *buf, int buf_sz);

// Generic string holder
typedef struct {
	int byte_sz;
	char *bytes;
} fdo_string_t;

void fdo_string_init(fdo_string_t *b);
fdo_string_t *fdo_string_alloc(void);
fdo_string_t *fdo_string_alloc_size(size_t byte_sz);
fdo_string_t *fdo_string_alloc_with(const char *data, int byte_sz);
fdo_string_t *fdo_string_alloc_with_str(const char *data);
void fdo_string_free(fdo_string_t *b);
bool fdo_string_resize(fdo_string_t *b, int byte_sz);
bool fdo_string_resize_with(fdo_string_t *b, int new_byte_sz, const char *data);

#define FDO_GUID_BYTES (128 / 8)
#define FDO_GID_BYTES (128 / 8)
#define FDO_NONCE_BYTES (128 / 8)
#define FDO_NONCE_FIELD_BYTES 32
// EAT-UEID is of length 17 (EAT-RAND(1) + EAT-GUID(16))
#define FDO_UEID_BYTES (1 + FDO_GUID_BYTES)
#define FDO_MSG_PRIFIX_LEN 48
#define FDO_MSG_UUID_LEN 16
#define FDO_APP_ID_BYTES 16

/*
 * GUID - 128-bit Random number used for identification.
 */
typedef uint8_t fdo_guid_t[FDO_GUID_BYTES];
/*
 * nonce - 128-bit Random number, intended to be used naught but once.
 */
typedef uint8_t fdo_nonce_t[FDO_NONCE_BYTES];
typedef uint8_t fdo_ueid_t[FDO_UEID_BYTES];

/* Nonce  */
void fdo_nonce_init_rand(fdo_byte_array_t *n);
char *fdo_nonce_to_string(uint8_t *n, char *buf, int buf_sz);
bool fdo_nonce_equal(fdo_byte_array_t *n1, fdo_byte_array_t *n2);

typedef struct _fdo_hash_t {
	int hash_type;
	fdo_byte_array_t *hash;
} fdo_hash_t;

/*GID*/
bool fdo_siginfo_write(fdow_t *fdow);

// 3.3.2, hashtype as defined in FDO spec
#define FDO_CRYPTO_HASH_TYPE_SHA_256 -16
#define FDO_CRYPTO_HASH_TYPE_SHA_384 -43
#define FDO_CRYPTO_HMAC_TYPE_SHA_256 5
#define FDO_CRYPTO_HMAC_TYPE_SHA_384 6

// Legacy value, Currently used to represent an empty hash type for now
#define FDO_CRYPTO_HASH_TYPE_NONE 0

#if !defined(KEX_ECDH384_ENABLED) /* TODO: do more generic */
#define FDO_CRYPTO_HASH_TYPE_USED FDO_CRYPTO_HASH_TYPE_SHA_256
#define FDO_CRYPTO_HMAC_TYPE_USED FDO_CRYPTO_HMAC_TYPE_SHA_256
#else
#define FDO_CRYPTO_HASH_TYPE_USED FDO_CRYPTO_HASH_TYPE_SHA_384
#define FDO_CRYPTO_HMAC_TYPE_USED FDO_CRYPTO_HMAC_TYPE_SHA_384
#endif
fdo_hash_t *fdo_hash_alloc_empty(void);
fdo_hash_t *fdo_hash_alloc(int hash_type, int size);
void fdo_hash_free(fdo_hash_t *hp);
int fdo_hash_read(fdor_t *fdor, fdo_hash_t *hp);
bool fdo_hash_write(fdow_t *fdow, fdo_hash_t *hp);

typedef fdo_byte_array_t fdo_key_exchange_t;

typedef struct {
	uint8_t length;
	uint8_t addr[16];
} fdo_ip_address_t;

fdo_ip_address_t *fdo_ipaddress_alloc(void);
bool fdo_null_ipaddress(fdo_ip_address_t *fdoip);
void fdo_init_ipv4_address(fdo_ip_address_t *fdoip, uint8_t *ipv4);
#if 0
void fdo_init_ipv6_address(fdo_ip_address_t *fdoip, uint8_t *ipv6);
#endif
bool fdo_read_ipaddress(fdor_t *fdor, fdo_ip_address_t *fdoip);
bool fdo_convert_to_ipaddress(fdo_byte_array_t * ip_bytes, fdo_ip_address_t *fdoip);
char *fdo_ipaddress_to_string(fdo_ip_address_t *fdoip, char *buf, int buf_sz);
#if 0
int fdo_ipaddress_to_mem(fdo_ip_address_t *fdoip, uint8_t *copyto);
#endif

typedef struct {
	uint16_t length;
	char *name;
} fdo_dns_name_t;

// Legacy Hash and HMAC types
// TO-DO : Used in RSA-based crypto operations. Remove when the classes themselves are removed.
#define FDO_PK_HASH_NONE 0
#define FDO_PK_HASH_SHA1 3
#define FDO_PK_HASH_SHA256 8
#define FDO_PK_HASH_SHA512 10
#define FDO_PK_HASH_SHA384 14
#define FDO_PK_HASH_HMAC_SHA256 108
#define FDO_PK_HASH_HMAC_SHA512 110
#define FDO_PK_HASH_HMAC_SHA_384 114

// 3.3.4, PublicKey types (pkType)
#define FDO_CRYPTO_PUB_KEY_ALGO_ECDSAp256 -7
#define FDO_CRYPTO_PUB_KEY_ALGO_ECDSAp384 -35

// TO-DO: Legacy, Used in RSA-based crypto operations.
// Remove when the classes themselves are removed.
#define FDO_CRYPTO_PUB_KEY_ALGO_RSA 1

// 3.3.5 COSECompatibleSignatureTypes
#define FDO_CRYPTO_SIG_TYPE_ECSDAp256 -7
#define FDO_CRYPTO_SIG_TYPE_ECSDAp384 -35
#define FDO_CRYPTO_SIG_TYPE_ECSDAp512 -36

#define FDO_COSE_ALG_KEY 1

#define FDO_COSE_ENCRYPT0_AESPLAINTYPE_KEY 1
#define FDO_COSE_ENCRYPT0_AESIV_KEY 5

// Appendix E
#define FDO_EATFDO -17760707
#define FDO_EAT_MAROE_PREFIX_KEY -17760708
#define FDO_EAT_EUPHNONCE_KEY -17760709
#define FDO_EATNONCE_KEY 9
#define FDO_EATUEID_KEY 10

#define FDO_COSE_SIGN1_CUPHNONCE_KEY -17760701
#define FDO_COSE_SIGN1_CUPHOWNERPUBKEY_KEY -17760702

// Appendix E. AESPlainType values.
#define FDO_CRYPTO_COSEAES128CBC -17760703
#define FDO_CRYPTO_COSEAES128CTR -17760704
#define FDO_CRYPTO_COSEAES256CBC -17760705
#define FDO_CRYPTO_COSEAES256CTR -17760706

// 3.3.4 PublicKey encodings (pkEnc)
#define FDO_CRYPTO_PUB_KEY_ENCODING_NONE 0
#define FDO_CRYPTO_PUB_KEY_ENCODING_X509 1
#define FDO_CRYPTO_PUB_KEY_ENCODING_COSEX509 2
#define FDO_CRYPTO_PUB_KEY_ENCODING_COSEKEY 3

// TO-DO: Legacy, Used in RSA-based crypto operations.
// Remove when the classes themselves are removed.
#define FDO_CRYPTO_PUB_KEY_ENCODING_RSA_MOD_EXP 3

#define FDOEPID20_GID_LEN (16)

//#define FDO_PK_ENC_DEFAULT FDO_CRYPTO_PUB_KEY_ENCODING_X509
#define FDO_PK_ENC_DEFAULT FDO_CRYPTO_PUB_KEY_ENCODING_RSA_MOD_EXP
// Define the encryption values
//#define FDOEAlgAES_ECB_No_padding 1

typedef struct {
	int pkalg;
	int pkenc;
	fdo_byte_array_t *key1; // in RSA, the Modulus/ binary for DSA
	fdo_byte_array_t *key2; // In RSA, the Exponent
} fdo_public_key_t;

typedef struct {
	int sig_type; /* Used to differentiate between epid and ecdsa in
			 EBread*/
	fdo_public_key_t *pubkey;
} fdo_sig_info_t;

bool fdo_siginfo_read(fdor_t *fdor);

fdo_public_key_t *fdo_public_key_alloc_empty(void);
fdo_public_key_t *fdo_public_key_alloc(int pkalg, int pkenc, int pklen,
				       uint8_t *pkey);
void fdo_public_key_free(fdo_public_key_t *pk);
bool fdo_public_key_write(fdow_t *fdow, fdo_public_key_t *pk);
fdo_public_key_t *fdo_public_key_read(fdor_t *fdor);
fdo_public_key_t *fdo_public_key_clone(fdo_public_key_t *pk);

#define AES_IV 16
#define AES_CTR_IV 12
#define AES_CTR_IV_COUNTER 4

typedef struct {
	uint8_t nulls_added;
	fdo_byte_array_t *ct_string;
	fdo_byte_array_t *em_body; // Ciphertext of Encrypted Message Body
	fdo_hash_t *hmac;	  // HMAC of ct body
	uint8_t iv[AES_IV];	// iv of ctr/cbc.
	uint32_t offset;
	int aes_plain_type;
} fdo_encrypted_packet_t;

typedef struct {
	uint8_t ctr_iv[AES_CTR_IV];
	uint32_t ctr_enc;
	uint32_t ctr_dec;
	uint8_t cbc_iv_enc[AES_IV];
	uint8_t cbc_iv_dec[AES_IV];
	uint32_t pkt_count;
} fdo_iv_t; // IV store

fdo_encrypted_packet_t *fdo_encrypted_packet_alloc(void);
void fdo_encrypted_packet_free(fdo_encrypted_packet_t *pkt);
fdo_encrypted_packet_t *fdo_encrypted_packet_read(fdor_t *fdor);
bool fdo_etminnerblock_write(fdow_t *fdow, fdo_encrypted_packet_t *pkt);
bool fdo_etmouterblock_write(fdow_t *fdow, fdo_encrypted_packet_t *pkt);
bool fdo_encrypted_packet_unwind(fdor_t *fdor, fdo_encrypted_packet_t *pkt,
				 fdo_iv_t *iv);
bool fdo_encrypted_packet_windup(fdow_t *fdow, int type, fdo_iv_t *iv);

#define FDO_AES_128_BLOCK_SIZE 16

typedef struct {
	int sig_block_start;
	fdo_public_key_t *pk;
	fdo_byte_array_t *sg;
} fdo_sig_t;
#define FDO_EPDI_GROUPID_SZ 4

typedef struct {
	fdo_byte_array_t *plain_text;
	fdo_byte_array_t *obsig;
} fdo_redirect_t;

typedef struct {
	int aes_plain_type;
} fdo_cose_encrypt0_protected_header_t;

typedef struct {
	uint8_t aes_iv[AES_IV];
} fdo_cose_encrypt0_unprotected_header_t;

typedef struct {
	fdo_cose_encrypt0_protected_header_t *protected_header;
	fdo_cose_encrypt0_unprotected_header_t *unprotected_header;
	fdo_byte_array_t *payload;
} fdo_cose_encrypt0_t;

bool fdo_cose_encrypt0_free(fdo_cose_encrypt0_t *cose_encrypt0);
fdo_cose_encrypt0_t* fdo_cose_encrypt0_alloc(void);
bool fdo_cose_encrypt0_read_protected_header(fdor_t *fdor,
	fdo_cose_encrypt0_protected_header_t *protected_header);
bool fdo_cose_encrypt0_read_unprotected_header(fdor_t *fdor,
	fdo_cose_encrypt0_unprotected_header_t *unprotected_header);
bool fdo_cose_encrypt0_read(fdor_t *fdor, fdo_cose_encrypt0_t *cose_encrypt0);
bool fdo_cose_encrypt0_write_protected_header(fdow_t *fdow,
	fdo_cose_encrypt0_protected_header_t *protected_header);
bool fdo_cose_encrypt0_write_unprotected_header(fdow_t *fdow,
	fdo_cose_encrypt0_unprotected_header_t *unprotected_header);
bool fdo_cose_encrypt0_write(fdow_t *fdow, fdo_cose_encrypt0_t *cose_encrypt0);

typedef struct {
	int mac_type;
} fdo_cose_mac0_protected_header_t;

typedef struct {
	fdo_cose_mac0_protected_header_t *protected_header;
	fdo_byte_array_t *payload;
	fdo_byte_array_t *hmac;
} fdo_cose_mac0_t;

bool fdo_cose_mac0_free(fdo_cose_mac0_t *cose);
bool fdo_cose_mac0_read_protected_header(fdor_t *fdor,
	fdo_cose_mac0_protected_header_t *protected_header);
bool fdo_cose_mac0_read_unprotected_header(fdor_t *fdor);
bool fdo_cose_mac0_read(fdor_t *fdor, fdo_cose_mac0_t *cose);
bool fdo_cose_mac0_write_protected_header(fdow_t *fdow,
	fdo_cose_mac0_protected_header_t *protected_header);
bool fdo_cose_mac0_write_unprotected_header(fdow_t *fdow);
bool fdo_cose_mac0_write(fdow_t *fdow, fdo_cose_mac0_t *cose);

typedef struct {
	int ph_sig_alg;
} fdo_eat_protected_header_t;

typedef struct {
	fdo_byte_array_t *eatmaroeprefix;
	fdo_byte_array_t *euphnonce;
} fdo_eat_unprotected_header_t;

typedef struct {
	fdo_eat_protected_header_t *eat_ph;
	fdo_eat_unprotected_header_t *eat_uph;
	fdo_byte_array_t *eat_payload;
	fdo_byte_array_t *eat_signature;
} fdo_eat_t;

// methods to handle Entity Attestation Token (EAT).
fdo_eat_t* fdo_eat_alloc(void);
void fdo_eat_free(fdo_eat_t *eat);
bool fdo_eat_write_protected_header(fdow_t *fdow, fdo_eat_protected_header_t *eat_ph);
bool fdo_eat_write_unprotected_header(fdow_t *fdow, fdo_eat_unprotected_header_t *eat_uph);
bool fdo_eat_write(fdow_t *fdow, fdo_eat_t *eat);

typedef struct {
	fdo_byte_array_t *eatpayloads;
	fdo_nonce_t eatnonce;
	fdo_ueid_t eatueid;
	// EATOtherClaims: Unused in  implementation. Should be added depending on the requirement.
} fdo_eat_payload_base_map_t;

bool fdo_eat_write_payloadbasemap(fdow_t *fdow, fdo_eat_payload_base_map_t *eat_payload);

typedef struct {
	int ph_sig_alg;
} fdo_cose_protected_header_t;

typedef struct {
	fdo_nonce_t cuphnonce;
	fdo_public_key_t *cuphowner_public_key;
} fdo_cose_unprotected_header_t;

typedef struct {
	fdo_cose_protected_header_t *cose_ph;
	fdo_cose_unprotected_header_t *cose_uph;
	fdo_byte_array_t *cose_payload;
	fdo_byte_array_t *cose_signature;
} fdo_cose_t;

bool fdo_cose_free(fdo_cose_t *cose);
bool fdo_cose_read_protected_header(fdor_t *fdor, fdo_cose_protected_header_t *cose_ph);
bool fdo_cose_read_unprotected_header(fdor_t *fdor, fdo_cose_unprotected_header_t *cose_uph);
bool fdo_cose_read(fdor_t *fdor, fdo_cose_t *cose, bool empty_uph);
bool fdo_cose_write_protected_header(fdow_t *fdow, fdo_cose_protected_header_t *cose_ph);
bool fdo_cose_write_unprotected_header(fdow_t *fdow);
bool fdo_cose_write(fdow_t *fdow, fdo_cose_t *cose);

/*
 * This is a lookup on all possible TransportProtocol values (Section 3.3.12)
 */
#define PROTTCP 1
#define PROTTLS 2
#define PROTHTTP 3
#define PROTCOAP 4
#define PROTHTTPS 5
#define PROTCOAPS 6

typedef struct fdo_rvto2addr_entry_s {
	fdo_byte_array_t *rvip;
	fdo_string_t *rvdns;
	int rvport;
	int rvprotocol;
	struct fdo_rvto2addr_entry_s *next;
} fdo_rvto2addr_entry_t;

typedef struct {
	int num_rvto2addr;
	fdo_rvto2addr_entry_t *rv_to2addr_entry;
} fdo_rvto2addr_t;

void fdo_rvto2addr_entry_free(fdo_rvto2addr_entry_t *rvto2addr_entry);
void fdo_rvto2addr_free(fdo_rvto2addr_t *rvto2addr);
bool fdo_rvto2addr_entry_read(fdor_t *fdor, fdo_rvto2addr_entry_t *rvto2addr_entry);
bool fdo_rvto2addr_read(fdor_t *fdor, fdo_rvto2addr_t *rvto2addr);

typedef struct fdo_key_value_s {
	struct fdo_key_value_s *next;
	fdo_string_t *key;
	fdo_string_t *str_val;
	fdo_byte_array_t *bin_val;
	int *int_val;
	bool *bool_val;
} fdo_key_value_t;

fdo_key_value_t *fdo_kv_alloc(void);
fdo_key_value_t *fdo_kv_alloc_with_array(const char *key,
					 fdo_byte_array_t *val);
fdo_key_value_t *fdo_kv_alloc_with_str(const char *key, const char *val);
fdo_key_value_t *fdo_kv_alloc_key_only(const char *key);
void fdo_kv_free(fdo_key_value_t *kv);

/*
 * This is a lookup on all possible RVVariable
 */
#define BADKEY -1
#define RVDEVONLY 0
#define RVOWNERONLY 1
#define RVIPADDRESS 2
#define RVDEVPORT 3
#define RVOWNERPORT 4
#define RVDNS 5
#define RVSVCERTHASH 6
#define RVCLCERTHASH 7
#define RVUSERINPUT 8
#define RVWIFISSID 9
#define RVWIFIPW 10
#define RVMEDIUM 11
#define RVPROTOCOL 12
#define RVDELAYSEC 13
#define RVBYPASS 14
#define RVEXTRV 15

/*
 * This is a lookup on all possible RVProtocolValue (RVVariable 12)
 */
#define RVPROTREST 0
#define RVPROTHTTP 1
#define RVPROTHTTPS 2
#define RVPROTTCP 3
#define RVPROTTLS 4
#define RVPROTCOAPTCP 5
#define RVPROTCOAPUDP 6

typedef struct fdo_rendezvous_s {
	int num_params;
	struct fdo_rendezvous_s *next;
	bool *dev_only;
	bool *owner_only;
	fdo_ip_address_t *ip;
	int *po;
	int *pow;
	fdo_string_t *dn;
	fdo_hash_t *sch;
	fdo_hash_t *cch;
	bool *ui;
	fdo_string_t *ss;
	fdo_string_t *pw;
	fdo_string_t *wsp;
	uint64_t *me;
	uint64_t *pr;
	uint64_t *delaysec;
	bool *bypass;
} fdo_rendezvous_t;

fdo_rendezvous_t *fdo_rendezvous_alloc(void);
void fdo_rendezvous_free(fdo_rendezvous_t *rv);
bool fdo_rendezvous_read(fdor_t *fdor, fdo_rendezvous_t *rv);
bool fdo_rendezvous_write(fdow_t *fdow, fdo_rendezvous_t *rv);
#define FDO_RENDEZVOUS_GET_IP_ADDRESS_P(rv) ((rv)->ip)
#define FDO_RENDEZVOUS_GET_PORT(rv) (*(rv)->po)
//#define FDORendezvous_set_port(rv,p) ((rv)->po = (p))

typedef struct fdo_rendezvous_directive_s {
	uint16_t num_entries;
	struct fdo_rendezvous_directive_s *next;
	fdo_rendezvous_t *rv_entries;
} fdo_rendezvous_directive_t;

typedef struct fdo_rendezvous_list_s {
	uint16_t num_rv_directives;
	fdo_rendezvous_directive_t *rv_directives;
} fdo_rendezvous_list_t;

int fdo_rendezvous_directive_add(fdo_rendezvous_list_t *list,
	fdo_rendezvous_directive_t *directive);
fdo_rendezvous_directive_t *fdo_rendezvous_directive_get(
	fdo_rendezvous_list_t *list, int num);
fdo_rendezvous_list_t *fdo_rendezvous_list_alloc(void);
void fdo_rendezvous_list_free(fdo_rendezvous_list_t *list);
int fdo_rendezvous_list_add(fdo_rendezvous_directive_t *list, fdo_rendezvous_t *rv);
fdo_rendezvous_t *fdo_rendezvous_list_get(fdo_rendezvous_directive_t *list, int num);
int fdo_rendezvous_list_read(fdor_t *fdor, fdo_rendezvous_list_t *list);
bool fdo_rendezvous_list_write(fdow_t *fdow, fdo_rendezvous_list_t *list);

typedef struct fdo_service_info_s {
	int numKV;
	fdo_key_value_t *kv;
} fdo_service_info_t;

fdo_service_info_t *fdo_service_info_alloc(void);
void fdo_service_info_free(fdo_service_info_t *si);
fdo_key_value_t **fdo_service_info_fetch(fdo_service_info_t *si,
					 const char *key);
fdo_key_value_t **fdo_service_info_get(fdo_service_info_t *si, int key_num);
bool fdo_service_info_add_kv_str(fdo_service_info_t *si, const char *key,
				 const char *val);
bool fdo_service_info_add_kv_bin(fdo_service_info_t *si, const char *key,
				 const fdo_byte_array_t *val);
bool fdo_service_info_add_kv_bool(fdo_service_info_t *si, const char *key,
				 bool val);
bool fdo_service_info_add_kv_int(fdo_service_info_t *si, const char *key,
				 int val);
bool fdo_service_info_add_kv(fdo_service_info_t *si, fdo_key_value_t *kv);
bool fdo_signature_verification(fdo_byte_array_t *plain_text,
				fdo_byte_array_t *sg, fdo_public_key_t *pk);

bool fdo_compare_public_keys(fdo_public_key_t *pk1, fdo_public_key_t *pk2);
bool fdo_serviceinfo_write(fdow_t *fdow, fdo_service_info_t *si);
bool fdo_serviceinfo_modules_list_write(fdow_t *fdow);

/*==================================================================*/
/* Service Info functionality */

#define EMPTY_STRING_LEN 1

/* Module list */
typedef struct fdo_sdk_service_info_module_list_s {
	fdo_sdk_service_info_module module;
	int module_psi_index;
	int module_dsi_count;
	int module_osi_index;
	struct fdo_sdk_service_info_module_list_s
	    *next; // ptr to next module node
} fdo_sdk_service_info_module_list_t;

typedef struct fdo_sv_info_dsi_info_s {
	fdo_sdk_service_info_module_list_t *list_dsi;
	int module_dsi_index;
} fdo_sv_info_dsi_info_t;

/* exposed API for modules to register */
void fdo_sdk_service_info_register_module(fdo_sdk_service_info_module *module);
void fdo_sdk_service_info_deregister_module(void);
void print_service_info_module_list(void);

bool fdo_mod_exec_sv_infotype(fdo_sdk_service_info_module_list_t *module_list,
			      fdo_sdk_si_type type);

void fdo_sv_info_clear_module_psi_osi_index(
    fdo_sdk_service_info_module_list_t *module_list);

bool fdo_serviceinfo_read(fdor_t *fdor, fdo_sdk_service_info_module_list_t *module_list,
	int *cb_return_val);
bool fdo_supply_serviceinfoval(fdor_t *fdor, char *module_name, char *module_message,
	fdo_sdk_service_info_module_list_t *module_list, int *cb_return_val);

bool fdo_compare_hashes(fdo_hash_t *hash1, fdo_hash_t *hash2);
bool fdo_compare_byte_arrays(fdo_byte_array_t *ba1, fdo_byte_array_t *ba2);
bool fdo_compare_rv_lists(fdo_rendezvous_list_t *rv_list1,
			  fdo_rendezvous_list_t *rv_list2);
bool fdo_rendezvous_instr_compare(fdo_rendezvous_t *entry1,
	fdo_rendezvous_t *entry2);

void fdo_log_block(fdo_block_t *fdob);

#define FDO_DSI_ACTIVE_LEN 6
/*==================================================================*/

#endif /* __FDOTYPES_H__ */
