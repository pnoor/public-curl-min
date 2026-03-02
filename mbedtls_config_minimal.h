/**
 * Ultra-minimal mbedTLS config for HTTPS-only curl (no cert verification).
 * SSL_VERIFYPEER=0, SSL_VERIFYHOST=0 — stripped PEM/Base64/SNI/CBC/SHA-384+.
 * TLS 1.2 only, ECDHE key exchange only, AES-GCM only.
 *
 * Usage in GitHub Actions workflow (CMake):
 *   Copy this file into mbedtls/include/mbedtls/mbedtls_config.h before building.
 */

#ifndef MBEDTLS_CONFIG_MINIMAL_H
#define MBEDTLS_CONFIG_MINIMAL_H

/* ── Platform ──────────────────────────────────────────────────────────────── */
#define MBEDTLS_HAVE_ASM
#define MBEDTLS_HAVE_TIME
#define MBEDTLS_PLATFORM_C

/* ── Cipher suites — AES-GCM only ─────────────────────────────────────────── */
#define MBEDTLS_AES_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_GCM_C                    /* AES-GCM (modern, preferred) */
/* Removed: MBEDTLS_CHACHAPOLY_C — AES-GCM is sufficient, saves ~15 KB */
/* Removed: MBEDTLS_CHACHA20_C */
/* Removed: MBEDTLS_POLY1305_C */
/* Removed: MBEDTLS_CIPHER_MODE_CBC — GCM only, no legacy CBC */
/* Removed: MBEDTLS_CCM_C — not needed, GCM is sufficient */
#define MBEDTLS_AESCE_C                  /* ARM Crypto Extensions for AES acceleration */

/* ── Hashing — SHA-256 only ───────────────────────────────────────────────── */
#define MBEDTLS_MD_C
#define MBEDTLS_SHA256_C                 /* SHA-256 — only hash needed */
/* Removed: MBEDTLS_SHA1_C — legacy; modern servers use SHA-256 */
/* Removed: MBEDTLS_SHA384_C — force SHA-256 only */
/* Removed: MBEDTLS_SHA512_C — force SHA-256 only */
/* Removed: MBEDTLS_MD5_C — ancient, not needed */

/* ── Public key crypto ─────────────────────────────────────────────────────── */
#define MBEDTLS_BIGNUM_C                 /* Big number math (RSA, ECC) */
#define MBEDTLS_RSA_C                    /* RSA — still needed for ECDHE-RSA */
#define MBEDTLS_ECDH_C                   /* ECDH key exchange */
#define MBEDTLS_ECDSA_C                  /* ECDSA signatures */
#define MBEDTLS_ECP_C                    /* Elliptic curve core */
#define MBEDTLS_PK_C                     /* Public key abstraction */
#define MBEDTLS_PK_PARSE_C              /* Public key parsing */
#define MBEDTLS_PKCS1_V15               /* RSA PKCS#1 v1.5 */
/* Removed: MBEDTLS_PKCS1_V21 — OAEP/PSS not needed */
/* Removed: MBEDTLS_PK_WRITE_C — not needed for TLS client */

/* ── Elliptic curves — P-256 only ──────────────────────────────────────────── */
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED /* P-256 — most common */
#define MBEDTLS_ECP_NIST_OPTIM          /* Optimized NIST curve math */
/* Removed: MBEDTLS_ECP_DP_SECP384R1_ENABLED — P-256 is sufficient */

/* ── TLS key exchange — ECDHE only ─────────────────────────────────────────── */
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED       /* ECDHE-RSA (most common) */
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED     /* ECDHE-ECDSA */
/* Removed: MBEDTLS_KEY_EXCHANGE_RSA_ENABLED — keep only ECDHE variants */

/* ── X.509 certificates (minimal — needed by TLS handshake) ───────────────── */
#define MBEDTLS_X509_USE_C
#define MBEDTLS_X509_CRT_PARSE_C        /* Certificate parsing for TLS */
#define MBEDTLS_ASN1_PARSE_C            /* ASN.1 parsing */
#define MBEDTLS_ASN1_WRITE_C            /* ASN.1 writing (needed by crypto) */
#define MBEDTLS_OID_C                   /* OID database */
/* Removed: MBEDTLS_PEM_PARSE_C — not loading PEM certs (VERIFYPEER=0) */
/* Removed: MBEDTLS_BASE64_C — only needed by PEM parsing */
/* Removed: MBEDTLS_X509_RSASSA_PSS_SUPPORT — RSA-PSS rare */

/* ── TLS protocol — TLS 1.2 only ───────────────────────────────────────────── */
#define MBEDTLS_SSL_CLI_C               /* TLS client only */
#define MBEDTLS_SSL_TLS_C               /* TLS core */
#define MBEDTLS_SSL_PROTO_TLS1_2        /* TLS 1.2 only */
/* Removed: MBEDTLS_SSL_PROTO_TLS1_3 — requires HKDF+KEEP_PEER_CERT, large code size */
/* Removed: MBEDTLS_SSL_SESSION_TICKETS — not needed for simple API calls */
/* Removed: MBEDTLS_SSL_ENCRYPT_THEN_MAC — CBC extension, no CBC */
/* Removed: MBEDTLS_SSL_EXTENDED_MASTER_SECRET — MITM mitigation, verification off */
/* Removed: MBEDTLS_SSL_SERVER_NAME_INDICATION — not needed (VERIFYPEER=0) */
/* Removed: MBEDTLS_SSL_KEEP_PEER_CERTIFICATE — not verifying certs */
/* Removed: MBEDTLS_SSL_MAX_FRAGMENT_LENGTH — rarely used */

/* ── Random number generation ──────────────────────────────────────────────── */
#define MBEDTLS_CTR_DRBG_C              /* CTR-DRBG random generator */
#define MBEDTLS_ENTROPY_C               /* Entropy source */

/* ── PSA crypto (required by mbedTLS 3.x internally) ──────────────────────── */
#define MBEDTLS_PSA_CRYPTO_C

/* ── Error handling ────────────────────────────────────────────────────────── */
#define MBEDTLS_ERROR_STRERROR_DUMMY    /* Stub error strings (saves space) */

/*
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ INTENTIONALLY DISABLED                                                  │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ MBEDTLS_SSL_SRV_C          — TLS server (client-only)                 │
 * │ MBEDTLS_CIPHER_MODE_CBC    — CBC mode (GCM only)                      │
 * │ MBEDTLS_CCM_C              — AES-CCM (GCM sufficient)                 │
 * │ MBEDTLS_MD5_C              — MD5 (ancient)                            │
 * │ MBEDTLS_SHA1_C             — SHA-1 (legacy)                           │
 * │ MBEDTLS_SHA384_C           — SHA-384 (SHA-256 only)                   │
 * │ MBEDTLS_SHA512_C           — SHA-512 (SHA-256 only)                   │
 * │ MBEDTLS_PEM_PARSE_C        — PEM parsing (VERIFYPEER=0)              │
 * │ MBEDTLS_BASE64_C           — Base64 (no PEM)                          │
 * │ MBEDTLS_KEY_EXCHANGE_RSA   — Static RSA (ECDHE only)                  │
 * │ MBEDTLS_PKCS1_V21          — OAEP/PSS (not needed)                    │
 * │ MBEDTLS_ECP_DP_SECP384R1   — P-384 (P-256 only)                      │
 * │ MBEDTLS_CHACHAPOLY_C       — ChaCha20-Poly1305 (AES-GCM sufficient)   │
 * │ MBEDTLS_SSL_PROTO_TLS1_3  — TLS 1.3 (requires HKDF, large)          │
 * │ MBEDTLS_SSL_SNI            — SNI (VERIFYPEER=0)                       │
 * │ MBEDTLS_SSL_KEEP_PEER_CERT — Keep peer cert (not verifying)           │
 * │ MBEDTLS_SSL_SESSION_TICKETS — Not needed for simple API calls         │
 * │ MBEDTLS_NET_C              — Net I/O (curl handles sockets)           │
 * │ MBEDTLS_TIMING_C           — Timing (DTLS only)                       │
 * │ All DTLS, DHM, PSK, DES, CAMELLIA, ARIA, PKCS#5/7/12                 │
 * │ Debug, self-test, version, full error strings, file I/O               │
 * └─────────────────────────────────────────────────────────────────────────┘
 */

/* NOTE: Do NOT include config_adjust_*.h here.
 * mbedTLS 3.x build_info.h includes them automatically after this config.
 * Including them manually causes "Do not include manually" build errors. */

#endif /* MBEDTLS_CONFIG_MINIMAL_H */
