// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// ZBI Processing Library
// This library is meant to be a generic processing library for the ZBI format
// defined in system/public/zircon/boot/image.h
//
// This library has several features:
// (1) Zero allocations / Exceptions
//     Safe to use at early boot time if necessary or in other situations where
//     allocation may not be desirable.
// (2) Trivially Portable
//     This library's only dependency on zircon is the image.h ZBI header.
//     Otherwise it should be trivially portable to other environments/
//     bootloaders as necessary.
// (3) Tested
//     Tests for this library can be found at zircon/system/utest/zbi/*

#pragma once

#include <stddef.h>
#include <zircon/boot/image.h>
#include <zircon/compiler.h>

__BEGIN_CDECLS

typedef uint32_t zbi_result_t;

#define ZBI_RESULT_OK             ((zbi_result_t)0)

#define ZBI_RESULT_ERROR          ((zbi_result_t)1)
#define ZBI_RESULT_BAD_TYPE       ((zbi_result_t)2)
#define ZBI_RESULT_BAD_MAGIC      ((zbi_result_t)3)
#define ZBI_RESULT_BAD_VERSION    ((zbi_result_t)4)
#define ZBI_RESULT_BAD_CRC        ((zbi_result_t)5)
#define ZBI_RESULT_ERR_TRUNCATED  ((zbi_result_t)6)

#define ZBI_RESULT_TOO_BIG        ((zbi_result_t)7)

typedef zbi_result_t (*zbi_foreach_cb_t)(zbi_header_t* hdr,
                                         void* payload,
                                         void* cookie);

// Checks the integrity of the underlying ZBI.
// If err is not null and an error is found, err will point to the ZBI entry
// in which a problem was found the return value will attempt to specify the
// nature of the error.
zbi_result_t zbi_check(void* base, zbi_header_t** err);

// Call `cb` with a pointer to the header and payload of each ZBI item
// excluding the root ZBI_TYPE_CONTAINER.
//
// If any of the callbacks return anything other than ZBI_RESULT_OK, iteration
// will cease and zbi_for_each will short circuit and return with the error
// result. If all items were successfully processed, ForEach will return
// ZBI_RESULT_OK.
//
// cookie will be passed transparently to each cb and is available for the
// client to pass data back from each of the callbacks.
zbi_result_t zbi_for_each(void* base, const zbi_foreach_cb_t cb, void* cookie);

// Creates a new ZBI section and appends it to the end of the ZBI pointed to by
// `base`. Assumes that the buffer at `base` has a length of `capacity` which
// is likely longer than the size of the ZBI at `base`.
//
// The new section will be aligned to the ZBI alignment boundary. Any padding
// added to achieve this alignment will be zero-filled.
//
// The caller need not set the ZBI_FLAG_VERSION field of the flags field as it
// will be set unconditionally for the new section.
//
// CRC computation is not currently supported and setting the ZBI_FLAG_CRC32
// flag will yield an error.
zbi_result_t zbi_append_section(void* base, const size_t capacity,
                                const uint32_t section_length,
                                const uint32_t type, const uint32_t extra,
                                const uint32_t flags, const void* payload);
__END_CDECLS