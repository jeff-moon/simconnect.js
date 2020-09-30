#include <Windows.h>
#include <stdio.h>
#include "call_dispatch_worker.hpp"

CallDispatchWorker::CallDispatchWorker(Napi::Env &env, HANDLE *fsHandle, std::function<void(SIMCONNECT_RECV *pData, DWORD cData)> cb) : Napi::AsyncWorker(env)
{
  this->cb = cb;
  this->fsHandle = fsHandle;
}

void CallDispatchWorker::Execute()
{
  while (true)
  {
    if (this->fsHandle)
    {
      SIMCONNECT_RECV *pData;
      DWORD cbData;

      HRESULT hr = SimConnect_GetNextDispatch(*this->fsHandle, &pData, &cbData);

      // If successful
      if (hr == 0)
      {
        this->cb(pData, cbData);
      }
    }
    Sleep(500);
  }
}

void CallDispatchWorker::OnOK()
{
  printf("ON OK");
}

void CallDispatchWorker::OnError(const Napi::Error &e)
{
  printf("ON ERROR");
}