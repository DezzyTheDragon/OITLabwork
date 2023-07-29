// *****************************************************
// Declaration of functions for marshalling request_t structures
//
// Starter code supplied by: Philip Howard

#include "nameserver.h"
#include "encode.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
// *****************************************************
// encode a request_t into network order
// see encode.h for documentation
void *encode(request_t *request, void *buff)
{
    //NOTE: buff and request can point to the same place
    
    //avoid copying data when I dont need to
    //need to just change order on port(uint16) and timeout(uint32)
    //newPort = htons(request->port)
    //newTO = htonl(request->timeout)
    //
    //if buff != request
    //  copy and adjust
    //else
    //  adjust

    //Alter the data to be in network order
    uint16_t htonPort = htons(request->port);
    uint32_t htonTimeout = htonl(request->timeout);

    //If the reqest and buff do NOT point to the same location
    //copy the data from request to buff
    if(request != buff)
    {
        memcpy(buff, request, sizeof(request_t));
    }
    
    //Alter the data in the struct
    //If request and buff point to the same location
    //we can just alter it without any other memory
    //manipulation
    //Cast for eas of access
    request_t *castBuff = (request_t*)buff;
    castBuff->port = htonPort;
    castBuff->timeout = htonTimeout;

    //Encode assumes that the data is good and thus
    //no way for it to realisticly fail
    //However I am sure someone somewere can prove
    //me wrong
    return buff;
}

// *****************************************************
// check's the validity of a request
// see encode.h for documentation
int is_invalid(request_t *request)
{
    //Check for valid data!
    //msg_type and status need to be within valid ranges
    //MSG_TYPE_MIN <= msg_type <= MSG_TYPE_MAX
    //STATUS_MIN <= status <= STATUS_MAX
    //
    //And the service name needs to also be valid
    //If there is a null terminator '\0' then the rest
    //need to be null terminated but if it takes up the
    //entire length no null terminator is required

    //return value
    //0 = success
    //1 = type error
    //2 = status error
    //4 = service_name error
    //+ = combination of errors
    int returnVal = 0;
    //check to make sure that msg_type is in range
    if(request->msg_type < MSG_TYPE_MIN || request->msg_type > MSG_TYPE_MAX)
    {
        //Bad msg_type :(
        returnVal += 1;
    }
    //check to make sure that statis is in range
    if(request->status < STATUS_MIN || request->status > STATUS_MAX)
    {
        //Bad status :(
        returnVal += 2;
    }

    //check that the service_name is good
    //Value to track if a null terminator is found
    int foundTerminator = 0;
    //Value to track if a character is found after a null terminator
    int foundInterlacedChar = 0;
    for(int i = 0; i < MAX_SERVICE_NAME_LEN; i++)
    {
        //check if there is a null terminator
        if(request->service_name[i] == '\0')
        {
            //We found a null terminator
            foundTerminator = 1;
        }
        else
        {
            //target char is not a null terminator
            //check to see if one was previously found
            if(foundTerminator)
            {
                //null terminator was perviously found
                //string is invalid :(
                foundInterlacedChar = 1;
            }
        }
    }
    //Results from loop
    if(foundInterlacedChar)
    {
        //Bad string
        returnVal += 4;
    }

    return returnVal;
}

// *****************************************************
// decode a request_t from network order
// see encode.h for documentation
request_t *decode(void *buff, request_t *decoded)
{
    //NOTE: buff and decoded can point to the same place
    //      decode can only fail if the incoming data is invalid
    //
    //      also buff is the source data not decoded so it needs to
    //      be cast into request_t when using

    //first validate the data
    //if the data is bad
    //  return NULL
    //else
    //  continue
    //
    //Similar to encode only need to change port and timeout
    //newPort = ntohs((request_t)buff->port)
    //newTO = ntohl((request_t)buff->timeout)
    //
    //again similar to encode check if buff and decoded point
    //to different locations and copy the data
    //if buff != decoded
    //  copy and alter
    //else
    //  alter
    //
    //done :)

    //First cast buffer once to avoid wasting time
    //casting over and over
    request_t *castBuff = (request_t*)buff;

    //Validate the data
    //printf("msg_type: %i\n", castBuff->msg_type);
    int check = is_invalid(castBuff);
    if(check)
    {
        //If the data is bad return NULL
        //printf("Bad data with code: %i\n", check);
        return NULL;
    }
    //If the data is good proceed with the decodeing
    //the rest of the decode works the exact same as
    //encode

    uint16_t ntohPort = ntohs(castBuff->port);
    uint32_t ntohTimeout = ntohl(castBuff->timeout);

    if(buff != decoded)
    {
        memcpy(decoded, buff, sizeof(request_t));
    }

    decoded->port = ntohPort;
    decoded->timeout = ntohTimeout;

    //Return decoded because we already checked for
    //the failiure condition
    return decoded;
}
