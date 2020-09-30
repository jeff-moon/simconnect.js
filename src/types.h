#pragma once
#include "napi.h"
#include "SimConnect.h"
#include "debug.h"

#define NAPI_TO_I32(v) (v.As<Napi::Number>().Int32Value())
#define NAPI_TO_U32(v) (v.As<Napi::Number>().Uint32Value())
#define NAPI_TO_FLOAT(v) (v.As<Napi::Number>().FloatValue())
#define NAPI_TO_DOUBLE(v) (v.As<Napi::Number>().DoubleValue())
#define NAPI_TO_STRING(v) (v.As<Napi::String>().Utf8Value())
#define NAPI_CALLBACK(v) (Napi::Persistent(v.As<Napi::Function>()))

#define NAPI_TO_SC_DATATYPE(v) ((SIMCONNECT_DATATYPE) NAPI_TO_I32(v));