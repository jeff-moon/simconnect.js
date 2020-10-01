#include "addon_node.h"

/*******************************************************************************
 * FUNCTION
 * Creates a single addon reference
 *******************************************************************************/
static void create_addon(napi_env env, napi_callback fn, char* name,
                         napi_value* exports) {
  napi_value exported_func;
  napi_create_function(env, name, NAPI_AUTO_LENGTH, fn, NULL, &exported_func);
  napi_set_named_property(env, *exports, name, exported_func);
}

/*******************************************************************************
 * FUNCTION
 * Creates all addons
 *******************************************************************************/
napi_value create_addons(napi_env env) {
  napi_value result;
  napi_create_object(env, &result);

  create_addon(env, print, "print", &result);
  create_addon(env, open, "open", &result);
  create_addon(env, create_data_definition, "createDataDefinition", &result);

  return result;
}

NAPI_MODULE_INIT() { return create_addons(env); }