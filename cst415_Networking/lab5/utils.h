#pragma once
//******************************************************
// utils.h
//
// Author: Philip Howard
//
// Various utility functions
//

#ifdef __cplusplus
extern "C" {
#endif

// Write an error message to stderr and then exit().
// The function allows formatting consistent with printf
void Fatal_Error(const char *msg, ...);

#ifdef __cplusplus
}
#endif

