#pragma once

//*******************************************************
// nameservice.h
//
// Type definitions for CST 415 Name Service Server interface
//
// Author: Philip Howard phil.howard@oit.edu
//
#include <stdint.h>

#define MAX_SERVICE_NAME_LEN 50

// Note on service names:
// The service name is a text string. The name can be MAX_SERVICE_NAME_LEN
// bytes long, meaning there may not be a null terminator. 
// If the name is shorter, it must be null terminated, and every byte after the
// null terminator must also be null.
//
// Note on byte orders. When sent over the network, but port and timeout must 
// be in network byte order. 
typedef struct
{
    int8_t msg_type;                                // see #defines below
    int8_t status;                                  // see #defines below
    uint16_t port;                                  // order port num
    uint32_t timeout;                               // requested timeout in 
                                                    // microseconds 
    char service_name[MAX_SERVICE_NAME_LEN];        // name of service
} request_t;

// Valid msg_type values
#define MSG_TYPE_MIN            1
#define MSG_TYPE_REQUEST_PORT   1
#define MSG_TYPE_LOOKUP_PORT    2
#define MSG_TYPE_KEEP_ALIVE     3
#define MSG_TYPE_CLOSE_PORT     4
#define MSG_TYPE_PORT_DEAD      5
#define MSG_TYPE_STOP           6
#define MSG_TYPE_RESPONSE       7
#define MSG_TYPE_MAX            7

// Valid status values
#define STATUS_MIN               0
#define STATUS_SUCCESS           0
#define STATUS_SERVICE_IN_USE    1
#define STATUS_SERVICE_NOT_FOUND 2
#define STATUS_ALL_PORTS_BUSY    3
#define STATUS_INVALID_ARG       4
#define STATUS_UNDEFINED_ERROR   5
#define STATUS_MAX               5
