/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 François Pesce : francois.pesce (at) gmail (dot) com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ruby/ruby.h>
#include <ruby/encoding.h>

static const char z85_chars[] =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-:+=^!/*?&<>()[]{}@%$#";

static const unsigned char z85_bytes[128] = {
    /*    0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7  0x 8  0x 9  0x A  0x B  0x C  0x D  0x E  0x F */
/* 0x0 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2 */ 0x00, 0x44, 0x00, 0x54, 0x53, 0x52, 0x48, 0x00, 0x4B, 0x4C, 0x46, 0x41, 0x00, 0x3F, 0x3E, 0x45, 
/* 0x3 */ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x40, 0x00, 0x49, 0x42, 0x4A, 0x47, 
/* 0x4 */ 0x51, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 
/* 0x5 */ 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x4D, 0x00, 0x4E, 0x43, 0x00, 
/* 0x6 */ 0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 
/* 0x7 */ 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x4F, 0x00, 0x50, 0x00, 0x00
};

static VALUE z85_decode(VALUE module, VALUE data) {
    unsigned char *z85;
    char *buf;
    size_t out_idx, in_idx = 0, size;
    size_t z85_len;
    uint32_t value;

    StringValuePtr(data);
    z85 = (unsigned char *) RSTRING_PTR(data);
    z85_len = RSTRING_LEN(data);
    if (z85_len % 5 != 0) {
        rb_raise(rb_eRuntimeError, "z85 invalid string");
    }

    size = z85_len * 4 / 5;
    buf = alloca(size);
    in_idx = out_idx = 0;
    while (in_idx < z85_len) {
        value = 52200625 * z85_bytes[z85[in_idx++]];
        value += 614125 * z85_bytes[z85[in_idx++]];
        value += 7225 * z85_bytes[z85[in_idx++]];
        value += 85 * z85_bytes[z85[in_idx++]];
        value += z85_bytes[z85[in_idx++]];
        buf[out_idx++] = (value >> 24);
        buf[out_idx++] = (value >> 16) & 0x000000FF;
        buf[out_idx++] = (value >>  8) & 0x000000FF;
        buf[out_idx++] = value & 0x000000FF;
    }

    return rb_enc_str_new(buf, size, rb_utf8_encoding());
}

static VALUE z85_encode(VALUE module, VALUE data) {
    unsigned char *in;
    char *result;
    size_t z85_len, out_idx, in_idx, in_len;
    uint32_t value = 0;

    if (TYPE(data) == T_STRING) {
        StringValuePtr(data);
        in = (unsigned char *) RSTRING_PTR(data);
        in_len = RSTRING_LEN(data);
    } else {
        rb_raise(rb_eTypeError, "z85 invalid input");
    }

    if (in_len % 4) {
        rb_raise(rb_eRuntimeError, "z85 invalid input string should be padded to a multiple of 4");
    }

    z85_len = in_len * 5 / 4;
    result = alloca(z85_len + 1);
    out_idx = in_idx = 0;
    while (in_idx < in_len) {
        value = in[in_idx++] << 24;
        value += in[in_idx++] << 16;
        value += in[in_idx++] << 8;
        value +=  in[in_idx++];
        result[out_idx++] = z85_chars[(value / 52200625) % 85];
        result[out_idx++] = z85_chars[(value / 614125) % 85];
        result[out_idx++] = z85_chars[(value / 7225) % 85];
        result[out_idx++] = z85_chars[(value / 85) % 85];
        result[out_idx++] = z85_chars[value % 85];
    }
    result[out_idx] = '\0';

    return rb_str_new(result, out_idx);
}

void Init_z85()
{
    VALUE mZ85 = rb_define_module("Z85");
    rb_define_module_function(mZ85, "encode", z85_encode, 1);
    rb_define_module_function(mZ85, "decode", z85_decode, 1);
}
