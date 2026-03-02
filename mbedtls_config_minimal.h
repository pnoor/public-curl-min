
/**
 * Minimal mbedTLS configuration for eSign Mobile API AAR.
 *
 * This config enables ONLY what is needed for:
 *   1. libcurl HTTPS client (TLS 1.2)
 *   2. SHA-256 and HMAC-SHA256 (Utils.cpp)
 *
 * Expected savings: ~200-400 KB in final .so compared to default config.
 *
 * Usage in GitHub Actions workflow (CMake):
 *   -DMBEDTLS_CONFIG_FILE=\"mbedtls_config_minimal.h\"
 *   Copy this file into mbedtls/include/mbedtls/ before building.
 *
 * If a server connection fails after applying this config, the most likely
 * cause is a missing cipher suite or curve. Re-enable as needed.
 */

#ifndef MBEDTLS_CONFIG_MINIMAL_H
#define MBEDTLS_CONFIG_MINIMAL_H

/* ── Platform ──────────────────────────────────────────────────────────────── */
#define MBEDTLS_HAVE_ASM
#define MBEDTLS_HAVE_TIME
#define MBEDTLS_HAVE_TIME_DATE
#define MBEDTLS_PLATFORM_C

/* ── AES cipher + GCM/CBC modes ────────────────────────────────────────────── */
#define MBEDTLS_AES_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_GCM_C                    /* AES-GCM (modern, preferred) */
#define MBEDTLS_CCM_C                    /* AES-CCM (some servers use it) */
#define MBEDTLS_CIPHER_MODE_CBC          /* AES-CBC (legacy compatibility) */

/* ARM Crypto Extensions for AES acceleration (ARM64 devices) */
#define MBEDTLS_AESCE_C

/* ── Hashing ───────────────────────────────────────────────────────────────── */
#define MBEDTLS_MD_C                     /* HMAC abstraction — used by Utils.cpp */
#define MBEDTLS_SHA1_C                   /* SHA-1 — required by TLS 1.2 handshake */
#define MBEDTLS_SHA224_C                 /* SHA-224 — compiled with SHA-256 */
#define MBEDTLS_SHA256_C                 /* SHA-256 — used by Utils.cpp + TLS */
#define MBEDTLS_SHA384_C                 /* SHA-384 — some servers require it */
#define MBEDTLS_SHA512_C                 /* SHA-512 — dependency of SHA-384 */
#define MBEDTLS_MD5_C                    /* MD5 — required by TLS 1.2 PRF */

/* ── Public key crypto ─────────────────────────────────────────────────────── */
#define MBEDTLS_BIGNUM_C                 /* Big number math (RSA, ECC) */
#define MBEDTLS_RSA_C                    /* RSA signatures + key exchange */
#define MBEDTLS_ECDH_C                   /* ECDH key exchange */
#define MBEDTLS_ECDSA_C                  /* ECDSA signatures */
#define MBEDTLS_ECDSA_DETERMINISTIC      /* Deterministic ECDSA (RFC 6979) */
#define MBEDTLS_ECP_C                    /* Elliptic curve core */
#define MBEDTLS_PK_C                     /* Public key abstraction */
#define MBEDTLS_PK_PARSE_C              /* Public key parsing */
#define MBEDTLS_PKCS1_V15               /* RSA PKCS#1 v1.5 */
#define MBEDTLS_PKCS1_V21               /* RSA PKCS#1 v2.1 (OAEP/PSS) */
#define MBEDTLS_GENPRIME                 /* Prime generation (RSA) */

/* ── Elliptic curves — only common ones ────────────────────────────────────── */
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED /* P-256 — most common */
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED /* P-384 — second most common */
#define MBEDTLS_ECP_NIST_OPTIM          /* Optimized NIST curve math */

/* ── TLS key exchange — only what modern servers use ───────────────────────── */
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED             /* RSA key exchange */
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED       /* ECDHE-RSA (most common) */
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED     /* ECDHE-ECDSA */

/* ── X.509 certificates ────────────────────────────────────────────────────── */
#define MBEDTLS_X509_USE_C
#define MBEDTLS_X509_CRT_PARSE_C        /* Certificate parsing for TLS */
#define MBEDTLS_ASN1_PARSE_C            /* ASN.1 parsing */
#define MBEDTLS_ASN1_WRITE_C            /* ASN.1 writing (needed by crypto) */
#define MBEDTLS_BASE64_C                /* Base64 (PEM parsing) */
#define MBEDTLS_OID_C                   /* OID database */
#define MBEDTLS_PEM_PARSE_C             /* PEM certificate parsing */
#define MBEDTLS_X509_RSASSA_PSS_SUPPORT /* RSA-PSS signatures in certs */

/* ── TLS 1.2 client only ──────────────────────────────────────────────────── */
#define MBEDTLS_SSL_CLI_C               /* TLS client */
#define MBEDTLS_SSL_TLS_C               /* TLS core */
#define MBEDTLS_SSL_PROTO_TLS1_2        /* TLS 1.2 only (no 1.3) */
#define MBEDTLS_SSL_ENCRYPT_THEN_MAC    /* TLS extension */
#define MBEDTLS_SSL_EXTENDED_MASTER_SECRET /* TLS extension */
#define MBEDTLS_SSL_SERVER_NAME_INDICATION /* SNI — needed for shared hosts */
#define MBEDTLS_SSL_KEEP_PEER_CERTIFICATE  /* Needed by curl */

/* ── Random number generation ──────────────────────────────────────────────── */
#define MBEDTLS_CTR_DRBG_C              /* CTR-DRBG random generator */
#define MBEDTLS_ENTROPY_C               /* Entropy source */

/* ── PSA crypto (required by mbedTLS 3.x internally) ──────────────────────── */
#define MBEDTLS_PSA_CRYPTO_C

/* ── Error handling ────────────────────────────────────────────────────────── */
#define MBEDTLS_ERROR_STRERROR_DUMMY    /* Stub error strings (saves space) */

/* ── NET I/O (curl handles its own, but mbedTLS may need it) ───────────────── */
#define MBEDTLS_NET_C

/*
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ INTENTIONALLY DISABLED (saves ~200-400 KB)                             │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ MBEDTLS_SSL_SRV_C          — TLS server (client-only)                 │
 * │ MBEDTLS_SSL_PROTO_TLS1_3   — TLS 1.3 (large, TLS 1.2 sufficient)     │
 * │ MBEDTLS_SSL_PROTO_DTLS     — DTLS + all DTLS_* features              │
 * │ MBEDTLS_CAMELLIA_C         — Camellia cipher                          │
 * │ MBEDTLS_ARIA_C             — ARIA cipher                              │
 * │ MBEDTLS_DES_C              — DES/3DES cipher                          │
 * │ MBEDTLS_CHACHA20_C         — ChaCha20                                 │
 * │ MBEDTLS_CHACHAPOLY_C       — ChaCha20-Poly1305                        │
 * │ MBEDTLS_POLY1305_C         — Poly1305 MAC                             │
 * │ MBEDTLS_DHM_C              — Diffie-Hellman (ECDHE is enough)         │
 * │ MBEDTLS_KEY_EXCHANGE_*PSK* — PSK key exchanges                        │
 * │ MBEDTLS_KEY_EXCHANGE_DHE_* — DHE key exchanges                        │
 * │ MBEDTLS_ECJPAKE_C          — EC J-PAKE                                │
 * │ MBEDTLS_SHA3_C             — SHA-3                                    │
 * │ MBEDTLS_RIPEMD160_C        — RIPEMD-160                               │
 * │ MBEDTLS_LMS_C              — Leighton-Micali Signatures               │
 * │ MBEDTLS_CMAC_C             — CMAC                                     │
 * │ MBEDTLS_NIST_KW_C          — NIST Key Wrapping                        │
 * │ MBEDTLS_HKDF_C             — HKDF (TLS 1.3 only)                     │
 * │ MBEDTLS_HMAC_DRBG_C        — HMAC-DRBG (CTR-DRBG is enough)          │
 * │ MBEDTLS_PKCS5_C            — PKCS#5                                   │
 * │ MBEDTLS_PKCS7_C            — PKCS#7                                   │
 * │ MBEDTLS_PKCS12_C           — PKCS#12                                  │
 * │ MBEDTLS_PEM_WRITE_C        — PEM writing                              │
 * │ MBEDTLS_PK_WRITE_C         — Public key writing                       │
 * │ MBEDTLS_X509_CRL_PARSE_C   — CRL parsing                              │
 * │ MBEDTLS_X509_CSR_PARSE_C   — CSR parsing                              │
 * │ MBEDTLS_X509_CREATE_C      — X.509 creation                           │
 * │ MBEDTLS_X509_CRT_WRITE_C   — Certificate writing                      │
 * │ MBEDTLS_X509_CSR_WRITE_C   — CSR writing                              │
 * │ MBEDTLS_SSL_CACHE_C        — SSL session cache (server-side)          │
 * │ MBEDTLS_SSL_COOKIE_C       — DTLS cookies (server-side)               │
 * │ MBEDTLS_SSL_TICKET_C       — Session tickets (server-side)            │
 * │ MBEDTLS_DEBUG_C            — Debug logging                             │
 * │ MBEDTLS_SELF_TEST          — Self-test functions                       │
 * │ MBEDTLS_VERSION_C          — Version info                              │
 * │ MBEDTLS_TIMING_C           — Timing (DTLS)                             │
 * │ MBEDTLS_ERROR_C            — Full error strings                        │
 * │ MBEDTLS_FS_IO              — File system I/O                           │
 * │ MBEDTLS_SSL_RENEGOTIATION  — TLS renegotiation                         │
 * │ MBEDTLS_SSL_SESSION_TICKETS — Session tickets                          │
 * │ MBEDTLS_SSL_ALPN           — ALPN extension                            │
 * │ MBEDTLS_SSL_ALL_ALERT_MESSAGES — All TLS alerts                        │
 * │ MBEDTLS_SSL_CONTEXT_SERIALIZATION — Context serialization              │
 * │ MBEDTLS_SSL_MAX_FRAGMENT_LENGTH — Max fragment length extension        │
 * │ MBEDTLS_PSA_CRYPTO_STORAGE_C — PSA key storage                        │
 * │ MBEDTLS_PSA_ITS_FILE_C    — PSA ITS file backend                      │
 * │ MBEDTLS_AESNI_C           — AES-NI (x86 only, not ARM)               │
 * │ MBEDTLS_PADLOCK_C         — VIA PadLock (x86 only)                    │
 * │ 11 unused ECP curves      — Only secp256r1 + secp384r1 kept           │
 * └─────────────────────────────────────────────────────────────────────────┘
 */

/* ── Include internal config adjustment headers ────────────────────────────── */
#include "mbedtls/config_adjust_legacy_crypto.h"
#include "mbedtls/config_adjust_x509.h"
#include "mbedtls/config_adjust_ssl.h"
#include "mbedtls/config_adjust_legacy_from_psa.h"
#include "mbedtls/config_adjust_psa_from_legacy.h"
#include "mbedtls/config_adjust_psa_superset_legacy.h"

#endif /* MBEDTLS_CONFIG_MINIMAL_H */
