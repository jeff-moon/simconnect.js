#pragma once

#include "napi.h"
#include <SimConnect.h>

class SimConnectJs {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    SimConnectJs(const Napi::CallbackInfo& info);

private:
    HANDLE fsHandle;
    Napi::Boolean Open(Napi::CallbackInfo& info);
    Napi::Boolean AddToDataDefinition(Napi::CallbackInfo& info);
    Napi::Boolean RequestDataOnSimObjectType(Napi::CallbackInfo& info);
}