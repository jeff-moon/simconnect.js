#ifndef _NAPI_OBJECT_HELPER_H
#define _NAPI_OBJECT_HELPER_H
#pragma once

#include "../common_types.h"

/*******************************************************************************
 * FUNCTION
 * Creates an empty object
 *******************************************************************************/
napi_value noh_create(napi_env env);

/*******************************************************************************
 * FUNCTION
 * Sets a property, with a string value
 *******************************************************************************/
void noh_set_property_string(napi_env env, napi_value object, const char* key,
                             const char* val);

/*******************************************************************************
 * FUNCTION
 * Gets a property, with a string value
 *******************************************************************************/
void noh_get_property_string(napi_env env, napi_value object, const char* key,
                             char* val, size_t valLen);

/*******************************************************************************
 * FUNCTION
 * Sets a property, with an 32-bit int value
 *******************************************************************************/
void noh_set_property_i32(napi_env env, napi_value object, const char* key,
                          int32_t val);

/*******************************************************************************
 * FUNCTION
 * Gets a property, with a 32-bit int value
 *******************************************************************************/
void noh_get_property_i32(napi_env env, napi_value object, const char* key,
                          int32_t* val);

/*******************************************************************************
 * FUNCTION
 * Sets a property, with a double value
 *******************************************************************************/
void noh_set_property_double(napi_env env, napi_value object, const char* key,
                             double val);

/*******************************************************************************
 * FUNCTION
 * Sets a property, with a float value
 *******************************************************************************/
void noh_set_property_float(napi_env env, napi_value object, const char* key,
                            float val);
#endif