#ifndef _ADDON_NODE_H
#define _ADDON_NODE_H

#pragma once
#include "common_types.h"

/*******************************************************************************
 * NAPI FUNCTION
 * Prints a simple test string
 *
 * JAVASCRIPT PARAMS
 * N/A
 *
 * RETURNS
 * N/A
 *******************************************************************************/
napi_value print(napi_env env, napi_callback_info info);

/*******************************************************************************
 * NAPI FUNCTION
 * Opens a SimConnect connection
 *
 * JAVASCRIPT PARAMS
 * name [string] The name of the connection
 *
 * RETURNS
 * Promise<SimConnectInfo>
 *******************************************************************************/
napi_value open(napi_env env, napi_callback_info info);

/*******************************************************************************
 * NAPI FUNCTION
 * Creates a data definition
 *
 * JAVASCRIPT PARAMS
 * definitions [array<{id, units, dataType}>] The list of definitions
 *
 * RETURNS
 * The ID of the created definition (32-bit int)
 *******************************************************************************/
napi_value create_data_definition(napi_env env, napi_callback_info info);

/*******************************************************************************
 * NAPI FUNCTION
 * Requests sim object data
 *
 * JAVASCRIPT PARAMS
 * definitionId [int] The definition's data to retrieve.
 *                    Create with create_data_definition()
 *
 * RETURNS
 * Promise<{...definitions}>
 *******************************************************************************/
napi_value get_sim_object_data(napi_env env, napi_callback_info info);

#endif /* _ADDON_NODE_H */