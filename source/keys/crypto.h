/*
 * Copyright (c) 2022 shchmue
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include "es_types.h"

#include "../hos/hos.h"
#include <sec/se_t210.h>
#include "../storage/nx_emmc.h"
#include <utils/types.h>

#include <string.h>

// Sha256 hash of the null string.
static const u8 null_hash[SE_SHA_256_SIZE] __attribute__((aligned(4))) = {
    0xE3, 0xB0, 0xC4, 0x42, 0x98, 0xFC, 0x1C, 0x14, 0x9A, 0xFB, 0xF4, 0xC8, 0x99, 0x6F, 0xB9, 0x24,
    0x27, 0xAE, 0x41, 0xE4, 0x64, 0x9B, 0x93, 0x4C, 0xA4, 0x95, 0x99, 0x1B, 0x78, 0x52, 0xB8, 0x55};

static const u8 aes_kek_generation_source[0x10] __attribute__((aligned(4))) = {
    0x4D, 0x87, 0x09, 0x86, 0xC4, 0x5D, 0x20, 0x72, 0x2F, 0xBA, 0x10, 0x53, 0xDA, 0x92, 0xE8, 0xA9};

static const u8 aes_key_generation_source[0x10] __attribute__((aligned(4))) = {
    0x89, 0x61, 0x5E, 0xE0, 0x5C, 0x31, 0xB6, 0x80, 0x5F, 0xE5, 0x8F, 0x3D, 0xA2, 0x4F, 0x7A, 0xA8};

static const u8 aes_key_decryption_source[0x10] __attribute__((aligned(4))) = {
    0x11, 0x70, 0x24, 0x2B, 0x48, 0x69, 0x11, 0xF1, 0x11, 0xB0, 0x0C, 0x47, 0x7C, 0xC3, 0xEF, 0x7E};

static const u8 device_master_kek_sources[KB_FIRMWARE_VERSION_MAX - KB_FIRMWARE_VERSION_400 + 1][0x10] __attribute__((aligned(4))) = {
    {0x88, 0x62, 0x34, 0x6E, 0xFA, 0xF7, 0xD8, 0x3F, 0xE1, 0x30, 0x39, 0x50, 0xF0, 0xB7, 0x5D, 0x5D}, /* 4.0.0  Device Master Kek Source. */
    {0x06, 0x1E, 0x7B, 0xE9, 0x6D, 0x47, 0x8C, 0x77, 0xC5, 0xC8, 0xE7, 0x94, 0x9A, 0xA8, 0x5F, 0x2E}, /* 5.0.0  Device Master Kek Source. */
    {0x99, 0xFA, 0x98, 0xBD, 0x15, 0x1C, 0x72, 0xFD, 0x7D, 0x9A, 0xD5, 0x41, 0x00, 0xFD, 0xB2, 0xEF}, /* 6.0.0  Device Master Kek Source. */
    {0x81, 0x3C, 0x6C, 0xBF, 0x5D, 0x21, 0xDE, 0x77, 0x20, 0xD9, 0x6C, 0xE3, 0x22, 0x06, 0xAE, 0xBB}, /* 6.2.0  Device Master Kek Source. */
    {0x86, 0x61, 0xB0, 0x16, 0xFA, 0x7A, 0x9A, 0xEA, 0xF6, 0xF5, 0xBE, 0x1A, 0x13, 0x5B, 0x6D, 0x9E}, /* 7.0.0  Device Master Kek Source. */
    {0xA6, 0x81, 0x71, 0xE7, 0xB5, 0x23, 0x74, 0xB0, 0x39, 0x8C, 0xB7, 0xFF, 0xA0, 0x62, 0x9F, 0x8D}, /* 8.1.0  Device Master Kek Source. */
    {0x03, 0xE7, 0xEB, 0x43, 0x1B, 0xCF, 0x5F, 0xB5, 0xED, 0xDC, 0x97, 0xAE, 0x21, 0x8D, 0x19, 0xED}, /* 9.0.0  Device Master Kek Source. */
    {0xCE, 0xFE, 0x41, 0x0F, 0x46, 0x9A, 0x30, 0xD6, 0xF2, 0xE9, 0x0C, 0x6B, 0xB7, 0x15, 0x91, 0x36}, /* 9.1.0  Device Master Kek Source. */
    {0xC2, 0x65, 0x34, 0x6E, 0xC7, 0xC6, 0x5D, 0x97, 0x3E, 0x34, 0x5C, 0x6B, 0xB3, 0x7E, 0xC6, 0xE3}, /* 12.1.0 Device Master Kek Source. */
    {0x77, 0x52, 0x92, 0xF0, 0xAA, 0xE3, 0xFB, 0xE0, 0x60, 0x16, 0xB3, 0x78, 0x68, 0x53, 0xF7, 0xA8}, /* 13.0.0 Device Master Kek Source. */
    {0x67, 0xD5, 0xD6, 0x0C, 0x08, 0xF5, 0xA3, 0x11, 0xBD, 0x6D, 0x5A, 0xEB, 0x96, 0x24, 0xB0, 0xD2}, /* 14.0.0 Device Master Kek Source. */
    {0x7C, 0x30, 0xED, 0x8B, 0x39, 0x25, 0x2C, 0x08, 0x8F, 0x48, 0xDC, 0x28, 0xE6, 0x1A, 0x6B, 0x49}, /* 15.0.0 Device Master Kek Source. */
    {0xF0, 0xF3, 0xFF, 0x52, 0x75, 0x2F, 0xBA, 0x4D, 0x09, 0x72, 0x30, 0x89, 0xA9, 0xDF, 0xFE, 0x1F}, /* 16.0.0 Device Master Kek Source. */
    {0x21, 0xD6, 0x35, 0xF1, 0x0F, 0x7A, 0xF0, 0x5D, 0xDF, 0x79, 0x1C, 0x7A, 0xE4, 0x32, 0x82, 0x9E}, /* 17.0.0 Device Master Kek Source. */
    {0xE7, 0x85, 0x8C, 0xA2, 0xF4, 0x49, 0xCB, 0x07, 0xD1, 0x8E, 0x48, 0x1B, 0xE8, 0x1E, 0x28, 0x3B}, /* 18.0.0 Device Master Kek Source. */
    {0x9B, 0xA5, 0xFD, 0x74, 0x7F, 0xCD, 0x23, 0xD1, 0xD9, 0xBD, 0x6C, 0x51, 0x72, 0x5F, 0x3D, 0x1F}, /* 19.0.0 Device Master Key Source. */
}; //!TODO: Update on mkey changes.

static const u8 device_master_kek_sources_dev[KB_FIRMWARE_VERSION_MAX - KB_FIRMWARE_VERSION_400 + 1][0x10] __attribute__((aligned(4))) = {
    {0xD6, 0xBD, 0x9F, 0xC6, 0x18, 0x09, 0xE1, 0x96, 0x20, 0x39, 0x60, 0xD2, 0x89, 0x83, 0x31, 0x34}, /* 4.0.0  Device Master Kek Source. */
    {0x59, 0x2D, 0x20, 0x69, 0x33, 0xB5, 0x17, 0xBA, 0xCF, 0xB1, 0x4E, 0xFD, 0xE4, 0xC2, 0x7B, 0xA8}, /* 5.0.0  Device Master Kek Source. */
    {0xF6, 0xD8, 0x59, 0x63, 0x8F, 0x47, 0xCB, 0x4A, 0xD8, 0x74, 0x05, 0x7F, 0x88, 0x92, 0x33, 0xA5}, /* 6.0.0  Device Master Kek Source. */
    {0x20, 0xAB, 0xF2, 0x0F, 0x05, 0xE3, 0xDE, 0x2E, 0xA1, 0xFB, 0x37, 0x5E, 0x8B, 0x22, 0x1A, 0x38}, /* 6.2.0  Device Master Kek Source. */
    {0x60, 0xAE, 0x56, 0x68, 0x11, 0xE2, 0x0C, 0x99, 0xDE, 0x05, 0xAE, 0x68, 0x78, 0x85, 0x04, 0xAE}, /* 7.0.0  Device Master Kek Source. */
    {0x94, 0xD6, 0xA8, 0xC0, 0x95, 0xAF, 0xD0, 0xA6, 0x27, 0x53, 0x5E, 0xE5, 0x8E, 0x70, 0x1F, 0x87}, /* 8.1.0  Device Master Kek Source. */
    {0x61, 0x6A, 0x88, 0x21, 0xA3, 0x52, 0xB0, 0x19, 0x16, 0x25, 0xA4, 0xE3, 0x4C, 0x54, 0x02, 0x0F}, /* 9.0.0  Device Master Kek Source. */
    {0x9D, 0xB1, 0xAE, 0xCB, 0xF6, 0xF6, 0xE3, 0xFE, 0xAB, 0x6F, 0xCB, 0xAF, 0x38, 0x03, 0xFC, 0x7B}, /* 9.1.0  Device Master Kek Source. */
    {0xC4, 0xBB, 0xF3, 0x9F, 0xA3, 0xAA, 0x00, 0x99, 0x7C, 0x97, 0xAD, 0x91, 0x8F, 0xE8, 0x45, 0xCB}, /* 12.1.0 Device Master Kek Source. */
    {0x20, 0x20, 0xAA, 0xFB, 0x89, 0xC2, 0xF0, 0x70, 0xB5, 0xE0, 0xA3, 0x11, 0x8A, 0x29, 0x8D, 0x0F}, /* 13.0.0 Device Master Kek Source. */
    {0xCE, 0x14, 0x74, 0x66, 0x98, 0xA8, 0x6D, 0x7D, 0xBD, 0x54, 0x91, 0x68, 0x5F, 0x1D, 0x0E, 0xEA}, /* 14.0.0 Device Master Kek Source. */
    {0xAE, 0x05, 0x48, 0x65, 0xAB, 0x17, 0x9D, 0x3D, 0x51, 0xB7, 0x56, 0xBD, 0x9B, 0x0B, 0x5B, 0x6E}, /* 15.0.0 Device Master Kek Source. */
    {0xFF, 0xF6, 0x4B, 0x0F, 0xFF, 0x0D, 0xC0, 0x4F, 0x56, 0x8A, 0x40, 0x74, 0x67, 0xC5, 0xFE, 0x9F}, /* 16.0.0 Device Master Kek Source. */
    {0x4E, 0xCE, 0x7B, 0x2A, 0xEA, 0x2E, 0x3D, 0x16, 0xD5, 0x2A, 0xDE, 0xF6, 0xF8, 0x6A, 0x7D, 0x43}, /* 17.0.0 Device Master Kek Source. */
    {0x3B, 0x00, 0x89, 0xD7, 0xA9, 0x9E, 0xB7, 0x70, 0x86, 0x00, 0xC3, 0x49, 0x52, 0x8C, 0xA4, 0xAF}, /* 18.0.0 Device Master Kek Source. */
    {0xAE, 0x78, 0x36, 0xB6, 0x91, 0xEB, 0xAF, 0x9C, 0x18, 0xF1, 0xC0, 0xD5, 0x8A, 0x0C, 0x7C, 0xA1}, /* 19.0.0 Device Master Key Source. */
}; //!TODO: Update on mkey changes.

static const u8 device_master_key_source_sources[KB_FIRMWARE_VERSION_MAX - KB_FIRMWARE_VERSION_400 + 1][0x10] __attribute__((aligned(4))) = {
    {0x8B, 0x4E, 0x1C, 0x22, 0x42, 0x07, 0xC8, 0x73, 0x56, 0x94, 0x08, 0x8B, 0xCC, 0x47, 0x0F, 0x5D}, /* 4.0.0  Device Master Key Source Source. */
    {0x6C, 0xEF, 0xC6, 0x27, 0x8B, 0xEC, 0x8A, 0x91, 0x99, 0xAB, 0x24, 0xAC, 0x4F, 0x1C, 0x8F, 0x1C}, /* 5.0.0  Device Master Key Source Source. */
    {0x70, 0x08, 0x1B, 0x97, 0x44, 0x64, 0xF8, 0x91, 0x54, 0x9D, 0xC6, 0x84, 0x8F, 0x1A, 0xB2, 0xE4}, /* 6.0.0  Device Master Key Source Source. */
    {0x8E, 0x09, 0x1F, 0x7A, 0xBB, 0xCA, 0x6A, 0xFB, 0xB8, 0x9B, 0xD5, 0xC1, 0x25, 0x9C, 0xA9, 0x17}, /* 6.2.0  Device Master Key Source Source. */
    {0x8F, 0x77, 0x5A, 0x96, 0xB0, 0x94, 0xFD, 0x8D, 0x28, 0xE4, 0x19, 0xC8, 0x16, 0x1C, 0xDB, 0x3D}, /* 7.0.0  Device Master Key Source Source. */
    {0x67, 0x62, 0xD4, 0x8E, 0x55, 0xCF, 0xFF, 0x41, 0x31, 0x15, 0x3B, 0x24, 0x0C, 0x7C, 0x07, 0xAE}, /* 8.1.0  Device Master Key Source Source. */
    {0x4A, 0xC3, 0x4E, 0x14, 0x8B, 0x96, 0x4A, 0xD5, 0xD4, 0x99, 0x73, 0xC4, 0x45, 0xAB, 0x8B, 0x49}, /* 9.0.0  Device Master Key Source Source. */
    {0x14, 0xB8, 0x74, 0x12, 0xCB, 0xBD, 0x0B, 0x8F, 0x20, 0xFB, 0x30, 0xDA, 0x27, 0xE4, 0x58, 0x94}, /* 9.1.0  Device Master Key Source Source. */
    {0xAA, 0xFD, 0xBC, 0xBB, 0x25, 0xC3, 0xA4, 0xEF, 0xE3, 0xEE, 0x58, 0x53, 0xB7, 0xF8, 0xDD, 0xD6}, /* 12.1.0 Device Master Key Source Source. */
    {0xE4, 0xF3, 0x45, 0x6F, 0x18, 0xA1, 0x89, 0xF8, 0xDA, 0x4C, 0x64, 0x75, 0x68, 0xE6, 0xBD, 0x4F}, /* 13.0.0 Device Master Key Source Source. */
    {0x5B, 0x94, 0x63, 0xF7, 0xAD, 0x96, 0x1B, 0xA6, 0x23, 0x30, 0x06, 0x4D, 0x01, 0xE4, 0xCE, 0x1D}, /* 14.0.0 Device Master Key Source Source. */ 
    {0x5E, 0xC9, 0xC5, 0x0A, 0xD0, 0x5F, 0x8B, 0x7B, 0xA7, 0x39, 0xEA, 0xBC, 0x60, 0x0F, 0x74, 0xE6}, /* 15.0.0 Device Master Key Source Source. */
    {0xEA, 0x90, 0x6E, 0xA8, 0xAE, 0x92, 0x99, 0x64, 0x36, 0xC1, 0xF3, 0x1C, 0xC6, 0x32, 0x83, 0x8C}, /* 16.0.0 Device Master Key Source Source. */
    {0xDA, 0xB9, 0xD6, 0x77, 0x52, 0x2D, 0x1F, 0x78, 0x73, 0xC9, 0x98, 0x5B, 0x06, 0xFE, 0xA0, 0x52}, /* 17.0.0 Device Master Key Source Source. */
    {0x14, 0xF5, 0xA5, 0xD0, 0x73, 0x6D, 0x44, 0x80, 0x5F, 0x31, 0x5A, 0x8F, 0x1E, 0xD4, 0x0D, 0x63}, /* 18.0.0 Device Master Key Source Source. */
    {0x07, 0x38, 0x9A, 0xEC, 0x9C, 0xBD, 0x50, 0x4A, 0x4C, 0x1F, 0x04, 0xDA, 0x40, 0x68, 0x29, 0xE3}, /* 19.0.0 Device Master Key Source Source. */
}; //!TODO: Update on mkey changes.

static const u8 seal_key_masks[][0x10] __attribute__((aligned(4))) = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // SealKey_LoadAesKey
    {0xA2, 0xAB, 0xBF, 0x9C, 0x92, 0x2F, 0xBB, 0xE3, 0x78, 0x79, 0x9B, 0xC0, 0xCC, 0xEA, 0xA5, 0x74}, // SealKey_DecryptDeviceUniqueData
    {0x57, 0xE2, 0xD9, 0x45, 0xE4, 0x92, 0xF4, 0xFD, 0xC3, 0xF9, 0x86, 0x38, 0x89, 0x78, 0x9F, 0x3C}, // SealKey_ImportLotusKey
    {0xE5, 0x4D, 0x9A, 0x02, 0xF0, 0x4F, 0x5F, 0xA8, 0xAD, 0x76, 0x0A, 0xF6, 0x32, 0x95, 0x59, 0xBB}, // SealKey_ImportEsDeviceKey
    {0x59, 0xD9, 0x31, 0xF4, 0xA7, 0x97, 0xB8, 0x14, 0x40, 0xD6, 0xA2, 0x60, 0x2B, 0xED, 0x15, 0x31}, // SealKey_ReencryptDeviceUniqueData
    {0xFD, 0x6A, 0x25, 0xE5, 0xD8, 0x38, 0x7F, 0x91, 0x49, 0xDA, 0xF8, 0x59, 0xA8, 0x28, 0xE6, 0x75}, // SealKey_ImportSslKey
    {0x89, 0x96, 0x43, 0x9A, 0x7C, 0xD5, 0x59, 0x55, 0x24, 0xD5, 0x24, 0x18, 0xAB, 0x6C, 0x04, 0x61}, // SealKey_ImportEsClientCertKey
};

static const u8 retail_specific_aes_key_source[0x10] __attribute__((aligned(4))) = {
    0xE2, 0xD6, 0xB8, 0x7A, 0x11, 0x9C, 0xB8, 0x80, 0xE8, 0x22, 0x88, 0x8A, 0x46, 0xFB, 0xA1, 0x95};

static const u8 secure_data_source[0x10] __attribute__((aligned(4))) = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const u8 secure_data_counters[1][0x10] __attribute__((aligned(4))) = {
    {0x3C, 0xD5, 0x92, 0xEC, 0x68, 0x31, 0x4A, 0x06, 0xD4, 0x1B, 0x0C, 0xD9, 0xF6, 0x2E, 0xD9, 0xE9}
};

static const u8 secure_data_tweaks[1][0x10] __attribute__((aligned(4))) = {
    {0xAC, 0xCA, 0x9A, 0xCA, 0xFF, 0x2E, 0xB9, 0x22, 0xCC, 0x1F, 0x4F, 0xAD, 0xDD, 0x77, 0x21, 0x1E}
};

 //!TODO: Update on keygen changes.
#define TSEC_ROOT_KEY_VERSION 2

// Lockpick_RCM keyslots
#define KS_BIS_00_CRYPT 0
#define KS_BIS_00_TWEAK 1
#define KS_BIS_01_CRYPT 2
#define KS_BIS_01_TWEAK 3
#define KS_BIS_02_CRYPT 4
#define KS_BIS_02_TWEAK 5
#define KS_AES_CTR  6
#define KS_AES_ECB  8
#define KS_AES_CMAC 10

// Mariko keyslots
#define KS_MARIKO_KEK 12
#define KS_MARIKO_BEK 13

// Other Switch keyslots
#define KS_TSEC        12
#define KS_SECURE_BOOT 14

// Atmosphere keygen keyslots
#define KS_TSEC_ROOT_DEV 11
#define KS_TSEC_ROOT     13

#define RSA_PUBLIC_EXPONENT 65537

#define KEYBLOB_UNK_DATA_SIZE 0x70
#define KEYBLOB_UNUSED_SIZE (NX_EMMC_BLOCKSIZE - SE_AES_CMAC_DIGEST_SIZE - SE_AES_IV_SIZE - sizeof(keyblob_t))

typedef struct {
    u8 master_kek[SE_KEY_128_SIZE];
    u8 data[KEYBLOB_UNK_DATA_SIZE];
    u8 package1_key[SE_KEY_128_SIZE];
} keyblob_t;

typedef struct {
    u8 cmac[SE_AES_CMAC_DIGEST_SIZE];
    u8 iv[SE_AES_IV_SIZE];
    keyblob_t key_data;
    u8 unused[KEYBLOB_UNUSED_SIZE];
} encrypted_keyblob_t;

typedef struct {
    u8  temp_key[SE_KEY_128_SIZE],
        bis_key[4][SE_KEY_128_SIZE * 2],
        device_key[SE_KEY_128_SIZE],
        device_key_4x[SE_KEY_128_SIZE],
        sd_seed[SE_KEY_128_SIZE],
        // FS-related keys
        header_key[SE_KEY_128_SIZE * 2],
        save_mac_key[SE_KEY_128_SIZE],
        // other sysmodule keys
        eticket_rsa_kek[SE_KEY_128_SIZE],
        eticket_rsa_kek_personalized[SE_KEY_128_SIZE],
        ssl_rsa_kek[SE_KEY_128_SIZE],
        ssl_rsa_kek_legacy[SE_KEY_128_SIZE],
        ssl_rsa_kek_personalized[SE_KEY_128_SIZE],
        ssl_rsa_key[SE_RSA2048_DIGEST_SIZE + 0x20],
        // keyblob-derived families
        keyblob_key[KB_FIRMWARE_VERSION_600 + 1][SE_KEY_128_SIZE],
        keyblob_mac_key[KB_FIRMWARE_VERSION_600 + 1][SE_KEY_128_SIZE],
        package1_key[KB_FIRMWARE_VERSION_600 + 1][SE_KEY_128_SIZE],
        // master key-derived families
        key_area_key[3][KB_FIRMWARE_VERSION_MAX + 1][SE_KEY_128_SIZE],
        master_kek[KB_FIRMWARE_VERSION_MAX + 1][SE_KEY_128_SIZE],
        master_key[KB_FIRMWARE_VERSION_MAX + 1][SE_KEY_128_SIZE],
        package2_key[KB_FIRMWARE_VERSION_MAX + 1][SE_KEY_128_SIZE],
        titlekek[KB_FIRMWARE_VERSION_MAX + 1][SE_KEY_128_SIZE],
        tsec_key[SE_KEY_128_SIZE],
        tsec_root_key[SE_KEY_128_SIZE];
    u32 secure_boot_key[4];
    keyblob_t keyblob[KB_FIRMWARE_VERSION_600 + 1];
    eticket_rsa_keypair_t eticket_rsa_keypair;
} key_storage_t;

typedef enum {
    SEAL_KEY_LOAD_AES_KEY = 0,
    SEAL_KEY_DECRYPT_DEVICE_UNIQUE_DATA = 1,
    SEAL_KEY_IMPORT_LOTUS_KEY = 2,
    SEAL_KEY_IMPORT_ES_DEVICE_KEY = 3,
    SEAL_KEY_REENCRYPT_DEVICE_UNIQUE_DATA = 4,
    SEAL_KEY_IMPORT_SSL_KEY = 5,
    SEAL_KEY_IMPORT_ES_CLIENT_CERT_KEY = 6,
} seal_key_t;

typedef enum {
    NOT_DEVICE_UNIQUE = 0,
    IS_DEVICE_UNIQUE = 1,
} device_unique_t;

#define SET_SEAL_KEY_INDEX(x) (((x) & 7) << 5)
#define GET_SEAL_KEY_INDEX(x) (((x) >> 5) & 7)
#define GET_IS_DEVICE_UNIQUE(x) ((x) & 1)

int key_exists(const void *data);

int run_ams_keygen();

bool check_keyslot_access();

bool test_rsa_keypair(const void *public_exponent, const void *private_exponent, const void *modulus);
u32 rsa_oaep_decode(void *dst, u32 dst_size, const void *label_digest, u32 label_digest_size, u8 *buf, u32 buf_size);

void derive_rsa_kek(u32 ks, key_storage_t *keys, void *out_rsa_kek, const void *kekek_source, const void *kek_source, u32 generation, u32 option);

// Equivalent to spl::GenerateAesKek
void generate_aes_kek(u32 ks, key_storage_t *keys, void *out_kek, const void *kek_source, u32 generation, u32 option);
// Equivalent to spl::GenerateAesKey
void generate_aes_key(u32 ks, key_storage_t *keys, void *out_key, u32 key_size, const void *access_key, const void *key_source);
// Equivalent to spl::GenerateSpecificAesKey
void generate_specific_aes_key(u32 ks, key_storage_t *keys, void *out_key, const void *key_source, u32 generation);
// Equivalent to spl::DecryptAesKey.
void decrypt_aes_key(u32 ks, key_storage_t *keys, void *out_key, const void *key_source, u32 generation, u32 option);
// Based on spl::LoadAesKey but instead of prepping keyslot, returns calculated key
void load_aes_key(u32 ks, void *out_key, const void *access_key, const void *key_source);

// Equivalent to smc::PrepareDeviceUniqueDataKey but with no sealing
void get_device_unique_data_key(u32 ks, void *out_key, const void *access_key, const void *key_source);
// Equivalent to smc::GetSecureData
void get_secure_data(key_storage_t *keys, void *out_data);
// Equivalent to smc::PrepareDeviceMasterKey
void get_device_key(u32 ks, key_storage_t *keys, void *out_device_key, u32 generation);

#endif
