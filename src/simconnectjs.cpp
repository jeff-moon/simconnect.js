#include "simconnectjs.hpp"

Napi::Object SimConnectJs::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "SimCononectJs",
        { InstanceMethod("Open", &SimConnectJs::Open),
            InstanceMethod("AddToDataDefinition", &SimConnectJs::AddToDataDefinition),
            InstanceMethod("RequestDataOnSimObjectType", &Addon::RequestDataOnSimObjectType) });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);
    exports.Set("SimConnectJs", func);
    return exports;
}

SimConnectJs::SimConnectJs(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<Addon>(info)
{
}

Napi::Boolean SimConnectJs::Open(Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string name = info[0].As<Napi::String>().Utf8Value();
    HRESULT result = SimConnect_Open(&this->fsHandle, name, NULL, 0, NULL, 0);

    if (result == 0) {
        return Napi::Boolean::New(env, true);
    } else {
        return Napi::Boolean::New(env, false);
    }
}