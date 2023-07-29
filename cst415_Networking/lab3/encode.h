#pragma once

// *****************************************************
// Declaration of functions for marshalling request_t structures
//
// Author: Philip Howard
// Email:  phil.howard@oit.edu

#include "nameserver.h"

#define INVALID_MSG_TYPE        1
#define INVALID_STATUS          2
#define INVALID_SERVICE_NAME    3
#define INVALID_OTHER           4

// *****************************************************
// encode a request_t into network order
//
// request: the request_t to be encoded
// buff: the buffer in which to place the encoded request
//      buff and request can point to the same location. If they don't
//      they must not overlap
//
// Note: encode does NOT perform validity checks. The assumption is that
//       the code that created the request created a valid one. The same is
//       not true for decode because the creator is across the network.
//
// return value:
//     on success returns buff
//     on failure returns NULL
void *encode(request_t *request, void *buff);

// *****************************************************
// check's the validity of a request
//
// Checks include:
//      msg_type is a valid value
//      status is a valid value
//      service_name is a text string. The string does not need to be null
//                  terminated, but if it contains any null characters, all 
//                  characters after the null must also be null.
//      
//      NOTE: is_invalid can be called on a request in either network byte
//      order or host byte order.
//
// return value:
//      if request is valid, return 0
//      else return an error code indicating what was invalid.
//          The error codes must be one of the values #define'd above
int is_invalid(request_t *request);

// *****************************************************
// decode a request_t from network order
//
// buff: the request_t to be decoded. It is assumed that the buff
//       is in network order
// decoded: the buffer in which to place the decoded request.
//      buff and decoded can point to the same location. If they don't
//      they must not overlap
//
// return value:
//     on success returns decoded
//     on failure returns NULL
//
// Note: decode calls is_invalid(buff) prior to decoding
request_t *decode(void *buff, request_t *decoded);
