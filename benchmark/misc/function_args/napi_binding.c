#include <node_api.h>
#include <assert.h>
#include <stdlib.h>

static napi_value CallWithString(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok); \

  napi_valuetype types[1];
  status = napi_typeof(env, args[0], types);
  assert(status == napi_ok); \

  if (types[0] == napi_string) {
    size_t len = 0;
    // Get the length, C-style
    status = napi_get_value_string_utf8(env, args[0], NULL, 0, &len);
    assert(status == napi_ok); \
    char* buf = (char*)malloc(len + 1);
    status = napi_get_value_string_utf8(env, args[0], buf, len, &len);
    assert(status == napi_ok); \
    free(buf);
  }

  return NULL;
}


#define EXPORT_FUNC(name, func) \
  napi_value func ## _v; \
  status = napi_create_function(env, \
                                name, \
                                NAPI_AUTO_LENGTH, \
                                func, \
                                NULL, \
                                &func ## _v); \
  assert(status == napi_ok); \
  status = napi_set_named_property(env, exports, name, func ## _v); \
  assert(status == napi_ok);


NAPI_MODULE_INIT() {
  napi_status status;

  EXPORT_FUNC("callWithString", CallWithString);

  return exports;
}
