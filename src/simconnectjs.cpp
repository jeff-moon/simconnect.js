#include <napi.h>
#include "simconnectjs.hpp"
#include "types.h"
#include "worker/call_dispatch_worker.hpp"

/**
 * NAPI Init lifecycle hook
 */
Napi::Object SimConnectJs::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func =
        DefineClass(env,
                    "SimConnectJs",
                    {InstanceMethod("open", &SimConnectJs::Open),
                     InstanceMethod("requestDataOnSimObjectType", &SimConnectJs::RequestDataOnSimObjectType),
                     InstanceMethod("createDataDefinition", &SimConnectJs::CreateDataDefinition)});

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);
    exports.Set("SimConnectJs", func);
    return exports;
}

/**
 * Constructor ?
 */
SimConnectJs::SimConnectJs(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<SimConnectJs>(info)
{
}

/**
 * WRAPPER for SimConnect_Open
 */
Napi::Value SimConnectJs::Open(const Napi::CallbackInfo &info)
{
    //todo verify params
    Napi::Env env = info.Env();
    std::string name = NAPI_TO_STRING(info[0]);

    HRESULT result = SimConnect_Open(&this->fsHandle, name.c_str(), NULL, 0, NULL, 0);
    bool retVal = result == 0 ? true : false;

    if (retVal)
    {
        auto fn = std::bind(&SimConnectJs::DataHandler, this, std::placeholders::_1, std::placeholders::_2);
        this->cdWk = new CallDispatchWorker(env, &this->fsHandle, fn);
        this->cdWk->Queue();
    }
    return Napi::Boolean::New(env, retVal);
}

Napi::Value SimConnectJs::CreateDataDefinition(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    bool success = true;
    int32_t result = -1;

    // Make sure we're operating on an array
    if (info[0].IsArray()) {
        // Get the array values
        Napi::Array infoValues = info[0].As<Napi::Array>();
        std::vector<DataDefinition> definitions;

        // Iterate over array values, parsing the data definition from each
        for (uint32_t i = 0; i < infoValues.Length(); i++) {
            Napi::Object obj = infoValues.Get(i).As<Napi::Object>();
            DataDefinition definition;
            definition.datumName = NAPI_TO_STRING(obj.Get("datumName"));
            definition.units = NAPI_TO_STRING(obj.Get("units"));
            definition.dataType = NAPI_TO_SC_DATATYPE(obj.Get("dataType"));
            definitions.push_back(definition);
        }

        // Create a data definition from the parsed result
        result = GenerateDataDefinition(definitions);

        // If valid result
        if (result >= 0) {
            this->definitions[result] = definitions;
        }
    }

    return Napi::Number::New(env, result);
}

Napi::Value SimConnectJs::RequestDataOnSimObjectType(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    //todo verify params
    int32_t requestId = GenerateRequestId();
    int32_t definition = NAPI_TO_I32(info[0]);
    SIMCONNECT_SIMOBJECT_TYPE simObjectType = SIMCONNECT_SIMOBJECT_TYPE_USER;

    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
    this->requests[requestId] = deferred;
    HRESULT result = SimConnect_RequestDataOnSimObjectType(this->fsHandle, requestId, definition, 0, simObjectType);

    bool retVal = result == 0 ? true : false;
    return deferred.Promise();
}

/*----------------------------------------------------------------------------------------------------------------------
 * DATA CALLBACK AND HANDLERS
 *--------------------------------------------------------------------------------------------------------------------*/
int32_t SimConnectJs::GenerateDataDefinition(std::vector<DataDefinition> values)
{
    // Get next definition
    this->definitionMtx.lock();
    int32_t def = this->definiton;
    this->definiton++;
    this->definitionMtx.unlock();

    int32_t result;
    bool success = true;
    for (DataDefinition &val : values)
    {
        result = SimConnect_AddToDataDefinition(this->fsHandle, 
                                                def, 
                                                val.datumName.c_str(), 
                                                val.units.c_str(), 
                                                val.dataType);
        success &= result == 0;
    }

    return def;
}

int32_t SimConnectJs::GenerateRequestId()
{
    this->requestIdMtx.lock();
    int32_t req = this->requestId;
    this->requestId++; 
    this->requestIdMtx.unlock();

    return req;
}

void SimConnectJs::DataHandler(SIMCONNECT_RECV *pData, DWORD cData)
{
    switch (pData->dwID)
    {
    case SIMCONNECT_RECV_ID_OPEN:
        printf("OPENED\n");
        break;
    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
        printf("JUST OBJECT\n");
        break;
    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
        HandleDataObjectByType(pData, cData);
        break;
    default:
        printf("UNKNOWN TYPE: %d\n", pData->dwID);
        break;
    }
}

void SimConnectJs::HandleDataObjectByType(SIMCONNECT_RECV *pData, DWORD cData)
{
    // Should probably do some null checking
    SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *) pData;
    int32_t reqId = pObjData->dwRequestID;
    std::vector<DataDefinition> definitions = this->definitions[reqId];
    Napi::Promise::Deferred deferred = this->requests[reqId];
    Napi::Env env = deferred.Env();
    Napi::Object retObj = Napi::Object::New(env);

    // for prototype assuming all doubles. use datatype checks
    uint32_t offset = 0;
    for(DataDefinition def : definitions) {
        printf("1\n");
        double *val = (double*) (&pObjData->dwData + offset);
        printf("2\n");
        offset += 8;
        printf("3\n");
        retObj.Set(def.datumName, Napi::Number::New(env, *val));
        printf("4\n");
    }

    deferred.Resolve(retObj);
}