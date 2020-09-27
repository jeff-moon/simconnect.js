#include <windows.h>
#include <stdio.h>

#include "simconnect.h"

int quit = 0;

struct PositionStruct
{
  char title[256];
  double latitude;
  double longitude;
  double altitude;
};

enum Definitions
{
  POSITION
};

enum Request
{
  REQUEST_1
};

void CALLBACK MyCallback(SIMCONNECT_RECV *pData, DWORD cbData, void *pContext)
{
  printf("DWID: %d\n", pData->dwID);
  SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *)pData;
  struct PositionStruct *ps = (struct PositionStruct *)&pObjData->dwData;
  printf("DATA:\n");
  printf("Altitude: %f\n", ps->altitude);
  printf("Latitude: %f\n", ps->latitude);
  printf("Longitude: %f\n", ps->longitude);
}

int main()
{
  HANDLE handle;
  HRESULT result = SimConnect_Open(&handle, "simconnect.js", NULL, 0, NULL, 0);
  result = SimConnect_AddToDataDefinition(handle, POSITION, "TITLE", NULL, SIMCONNECT_DATATYPE_STRING256);
  result = SimConnect_AddToDataDefinition(handle, POSITION, "PLANE LATITUDE", "degrees", SIMCONNECT_DATATYPE_FLOAT64, 0.0f, SIMCONNECT_UNUSED);
  result = SimConnect_AddToDataDefinition(handle, POSITION, "PLANE LONGITUDE", "degrees", SIMCONNECT_DATATYPE_FLOAT64, 0.0f, SIMCONNECT_UNUSED);
  result = SimConnect_AddToDataDefinition(handle, POSITION, "PLANE ALTITUDE", "feet", SIMCONNECT_DATATYPE_FLOAT64, 0.0f, SIMCONNECT_UNUSED);

  Sleep(10);

  result = SimConnect_RequestDataOnSimObjectType(handle, REQUEST_1, POSITION, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);

  while (0 == quit)
  {
    SimConnect_CallDispatch(handle, MyCallback, NULL);
    Sleep(1);
  }

  result = SimConnect_Close(handle);
}