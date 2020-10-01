#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H
#pragma once
#include <Windows.h>
#include <SimConnect.h>
#include <node_api.h>
#include <stdio.h>
#include <js_native_api.h>

/*******************************************************************************
 * FUNCTION
 * Executes a napi call and processes the return.
 * From:
 * https://nodejs.org/api/n-api.html
 *******************************************************************************/
#define NAPI_CALL(env, call)                                      \
  do {                                                            \
    napi_status status = (call);                                  \
    if (status != napi_ok) {                                      \
      const napi_extended_error_info* error_info = NULL;          \
      napi_get_last_error_info((env), &error_info);               \
      bool is_pending;                                            \
      napi_is_exception_pending((env), &is_pending);              \
      if (!is_pending) {                                          \
        const char* message = (error_info->error_message == NULL) \
                                  ? "empty error message"         \
                                  : error_info->error_message;    \
        napi_throw_error((env), NULL, message);                   \
        return NULL;                                              \
      }                                                           \
    }                                                             \
  } while (0)

/*******************************************************************************
 * FUNCTION
 * Executes a napi void call and processes the return.
 * From:
 * https://nodejs.org/api/n-api.html
 *******************************************************************************/
#define NAPI_VOID_CALL(env, call)                                 \
  do {                                                            \
    napi_status status = (call);                                  \
    if (status != napi_ok) {                                      \
      const napi_extended_error_info* error_info = NULL;          \
      napi_get_last_error_info((env), &error_info);               \
      bool is_pending;                                            \
      napi_is_exception_pending((env), &is_pending);              \
      if (!is_pending) {                                          \
        const char* message = (error_info->error_message == NULL) \
                                  ? "empty error message"         \
                                  : error_info->error_message;    \
        napi_throw_error((env), NULL, message);                   \
        return;                                                   \
      }                                                           \
    }                                                             \
  } while (0)

#define SIMCONNECT_CHECK_ERROR(env, hr, msg) \
  do {                                       \
    if (hr != 0) {                           \
      char errCode[10];                      \
      sprintf(errCode, "%d", hr);            \
      napi_throw_error(env, errCode, msg);   \
    }                                        \
  } while (0)
#endif