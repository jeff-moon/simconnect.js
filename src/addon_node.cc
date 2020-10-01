#include "addon_node.h"
#include <map>
#include <vector>

#include "common_types.h"
#include "object/napi_object_helper.h"

// Simple wrapper for dispatch data
struct Worker_Data {
  SIMCONNECT_RECV* p_data;
  DWORD pcb_data;
  napi_deferred deferred;
};

// Standard string length
#define STRLEN 256U

// Single definition entry
struct DefinitionEntry {
  char id[STRLEN];
  char units[STRLEN];
  SIMCONNECT_DATATYPE data_type;
};

/** The static SimConnect handle */
static HANDLE handle;

/** The list of definitions */
std::map<SIMCONNECT_DATA_DEFINITION_ID, std::vector<DefinitionEntry>>
    definitionMap;

/** The static definition ID */
static SIMCONNECT_DATA_DEFINITION_ID defId = 0;

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
static void perform_async_task(napi_env env, napi_deferred deferred,
                               const char* taskname);

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
static void worker_execute(napi_env env, void* data);

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
static void worker_complete(napi_env env, napi_status status, void* data);

/*******************************************************************************
 * FUNCTION
 *******************************************************************************/
static void handle_open(napi_env env, Worker_Data* data);

/*******************************************************************************
 * NAPI FUNCTION
 *******************************************************************************/
napi_value print(napi_env env, napi_callback_info info) {
  napi_value ret;
  napi_create_int32(env, 0, &ret);

  printf("HERE\n");

  return ret;
}

/*******************************************************************************
 * NAPI FUNCTION
 *******************************************************************************/
napi_value open(napi_env env, napi_callback_info info) {
  napi_value result;
  size_t argc = 1;
  napi_value args[1];

  char name[STRLEN];
  size_t resultLen;

  napi_value promise;
  napi_deferred deferred;

  // Parse args
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));
  NAPI_CALL(env, napi_get_value_string_utf8(env, args[0], &name[0], STRLEN,
                                            &resultLen));

  // Perform SimConnect call
  // TODO throw error if this fails?
  HRESULT hr = SimConnect_Open(&handle, &name[0], NULL, 0, NULL, 0);
  NAPI_CALL(env, napi_create_int32(env, hr, &result));
  NAPI_CALL(env, napi_create_promise(env, &deferred, &promise));

  perform_async_task(env, deferred, "open_worker");
  return promise;
}

/*******************************************************************************
 * NAPI FUNCTION
 *******************************************************************************/
napi_value create_data_definition(napi_env env, napi_callback_info info) {
  napi_value result;
  napi_value entry;
  int32_t data_type;

  size_t argc = 1U;
  napi_value args[1];

  uint32_t arrayLen = 0U;

  // Parse single argument
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));
  NAPI_CALL(env, napi_get_array_length(env, args[0], &arrayLen));

  std::vector<DefinitionEntry> defVector;

  // Parse all elements in array
  for (uint32_t i = 0; i < arrayLen; i++) {
    DefinitionEntry d;
    NAPI_CALL(env, napi_get_element(env, args[0], i, &entry));
    noh_get_property_string(env, entry, "id", &(d.id[0]), STRLEN);
    noh_get_property_string(env, entry, "units", &(d.units[0]), STRLEN);
    noh_get_property_i32(env, entry, "dataType", &data_type);
    d.data_type = (SIMCONNECT_DATATYPE)data_type;
    defVector.push_back(d);

    HRESULT hr = SimConnect_AddToDataDefinition(handle, defId, d.id, d.units,
                                                d.data_type);
    SIMCONNECT_CHECK_ERROR(env, hr, "SimConnect_AddToDataDefinition failed");
  }

  NAPI_CALL(env, napi_create_int32(env, defId, &result));
  defId++;
  return result;
}

/*******************************************************************************
 * FUNCTION
 * Performs an asynchronous SimConnect_GetNextDispatch
 *******************************************************************************/
static void perform_async_task(napi_env env, napi_deferred deferred,
                               const char* taskname) {
  // create the background worker
  napi_async_work worker;
  napi_value worker_name;

  // Create this, we'll free it after we're done with it!
  struct Worker_Data* data = new Worker_Data();
  data->deferred = deferred;

  // Create the name string
  NAPI_VOID_CALL(env, napi_create_string_utf8(env, taskname, NAPI_AUTO_LENGTH,
                                              &worker_name));

  // Create the actual worker
  NAPI_VOID_CALL(env,
                 napi_create_async_work(env, NULL, worker_name, worker_execute,
                                        worker_complete, data, &worker));

  // Perform the work!
  NAPI_VOID_CALL(env, napi_queue_async_work(env, worker));
}

/*******************************************************************************
 * FUNCTION
 * Execute function for async worker
 *******************************************************************************/
static void worker_execute(napi_env env, void* data) {
  printf("Worker Queued\n");
  // do some stuff
  HRESULT hr = -1;
  SIMCONNECT_RECV* p_data;
  DWORD pcb_data;
  Worker_Data* dp_data = (Worker_Data*)data;

  while (hr < 0) {
    hr = SimConnect_GetNextDispatch(handle, &p_data, &pcb_data);
    if (0 <= hr) {
      dp_data->p_data = p_data;
      dp_data->pcb_data = pcb_data;
    }
  }
}

/*******************************************************************************
 * FUNCTION
 * Complete function for async worker
 *******************************************************************************/
static void worker_complete(napi_env env, napi_status status, void* data) {
  printf("Worker complete\n");
  Worker_Data* worker_data = (Worker_Data*)data;

  switch (worker_data->p_data->dwID) {
    case SIMCONNECT_RECV_ID_OPEN:
      handle_open(env, worker_data);
      break;
  }
}

/*******************************************************************************
 * FUNCTION
 * Handles the OPEN event
 *******************************************************************************/
static void handle_open(napi_env env, Worker_Data* data) {
  napi_value object;
  // Cast to open data
  SIMCONNECT_RECV_OPEN* pOpenData = (SIMCONNECT_RECV_OPEN*)data->p_data;

  // Create the resolve object
  object = noh_create(env);
  noh_set_property_i32(env, object, "appVersionMajor",
                       pOpenData->dwApplicationVersionMajor);
  noh_set_property_i32(env, object, "appVersionMinor",
                       pOpenData->dwApplicationVersionMinor);
  noh_set_property_i32(env, object, "appBuildMajor",
                       pOpenData->dwApplicationBuildMajor);
  noh_set_property_i32(env, object, "appBuildMinor",
                       pOpenData->dwApplicationBuildMinor);
  noh_set_property_i32(env, object, "scVersionMajor",
                       pOpenData->dwSimConnectVersionMajor);
  noh_set_property_i32(env, object, "scVersionMinor",
                       pOpenData->dwSimConnectVersionMinor);
  noh_set_property_i32(env, object, "scBuildMajor",
                       pOpenData->dwSimConnectBuildMajor);
  noh_set_property_i32(env, object, "scBuildMinor",
                       pOpenData->dwSimConnectBuildMinor);

  // Resolve the promise
  NAPI_VOID_CALL(env, napi_resolve_deferred(env, data->deferred, object));

  // We are NOW done with this data
  free(data);
}