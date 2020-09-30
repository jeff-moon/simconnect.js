#include <napi.h>
#include "simconnectjs.hpp"

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
  return SimConnectJs::Init(env, exports);
}

NODE_API_MODULE(SimConnectJs, InitAll)