#include "napi_object_helper.h"

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
napi_value noh_create(napi_env env) {
  napi_value object;
  NAPI_CALL(env, napi_create_object(env, &object));

  return object;
}

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
void noh_set_property_string(napi_env env, napi_value object, const char* key,
                             const char* val) {
  napi_value napi_val;
  NAPI_VOID_CALL(
      env, napi_create_string_utf8(env, val, NAPI_AUTO_LENGTH, &napi_val));
  NAPI_VOID_CALL(env, napi_set_named_property(env, object, key, napi_val));
}

void noh_get_property_string(napi_env env, napi_value object, const char* key,
                             char* val, size_t valLen) {
  napi_value napi_val;
  size_t len = 0U;

  NAPI_VOID_CALL(env, napi_get_named_property(env, object, key, &napi_val));
  NAPI_VOID_CALL(env,
                 napi_get_value_string_utf8(env, napi_val, val, valLen, &len));
}

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
void noh_set_property_i32(napi_env env, napi_value object, const char* key,
                          int32_t val) {
  napi_value napi_val;
  NAPI_VOID_CALL(env, napi_create_int32(env, val, &napi_val));
  NAPI_VOID_CALL(env, napi_set_named_property(env, object, key, napi_val));
}

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
void noh_get_property_i32(napi_env env, napi_value object, const char* key,
                          int32_t* val) {
  napi_value napi_val;
  NAPI_VOID_CALL(env, napi_get_named_property(env, object, key, &napi_val));
  NAPI_VOID_CALL(env, napi_get_value_int32(env, napi_val, val));
}

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
void noh_set_property_double(napi_env env, napi_value object, const char* key,
                             double val) {
  napi_value napi_val;
  NAPI_VOID_CALL(env, napi_create_double(env, val, &napi_val));
  NAPI_VOID_CALL(env, napi_set_named_property(env, object, key, napi_val));
}

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
void noh_set_property_float(napi_env env, napi_value object, const char* key,
                            float val) {
  napi_value napi_val;
  NAPI_VOID_CALL(env, napi_create_double(env, val, &napi_val));
  NAPI_VOID_CALL(env, napi_set_named_property(env, object, key, napi_val));
}