#pragma once

#include <Windows.h>
#include <map>
#include <cstdint>
#include <mutex>
#include <napi.h>
#include <vector>
#include <SimConnect.h>
#include "worker/call_dispatch_worker.hpp"

struct DataDefinition {
    std::string datumName;
    std::string units;
    SIMCONNECT_DATATYPE dataType;
};

class SimConnectJs : public Napi::ObjectWrap<SimConnectJs>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    SimConnectJs(const Napi::CallbackInfo &info);

private:
    // member variables
    std::mutex definitionMtx;
    std::mutex requestIdMtx;
    std::map<int32_t, Napi::Promise::Deferred> requests;
    std::map<int32_t, std::vector<DataDefinition>> definitions; 
    HANDLE fsHandle;
    CallDispatchWorker *cdWk;

    int32_t definiton = 0;
    int32_t requestId = 0;

    // member functions
    Napi::Value Open(const Napi::CallbackInfo &info);
    Napi::Value CreateDataDefinition(const Napi::CallbackInfo &info);
    Napi::Value RequestDataOnSimObjectType(const Napi::CallbackInfo &info);
    int32_t GenerateDataDefinition(std::vector<DataDefinition> values);
    int32_t GenerateRequestId();
    void DataHandler(SIMCONNECT_RECV* pData, DWORD cData);
    void HandleDataObjectByType(SIMCONNECT_RECV *pData, DWORD cData);

};
