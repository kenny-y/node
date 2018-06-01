#include <node_api.h>
#include <assert.h>

static int32_t counter = 0;

napi_value Hello(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value value;
  napi_create_int32(env, counter++, &value);
  assert(status == napi_ok);
  return value;
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc = DECLARE_NAPI_METHOD("hello", Hello);
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
