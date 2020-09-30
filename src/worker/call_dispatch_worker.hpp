#pragma once

#include <Windows.h>
#include <napi.h>
#include "SimConnect.h"

class CallDispatchWorker : public Napi::AsyncWorker
{
public:
  CallDispatchWorker(Napi::Env &env, HANDLE *fsHandle, std::function<void(SIMCONNECT_RECV *pData, DWORD cData)> cb);
  void Execute() override;
  void OnOK() override;
  void OnError(const Napi::Error &e) override;

private:
  std::function<void(SIMCONNECT_RECV *pData, DWORD cData)> cb;
  HANDLE *fsHandle;
  Napi::Env *env;
};