#include <node_api.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static napi_value CallWithString(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  napi_valuetype types[1];
  status = napi_typeof(env, args[0], types);
  assert(status == napi_ok);

  assert(types[0] == napi_string);
  if (types[0] == napi_string) {
    size_t len = 0;
    // Get the length, C-style
    status = napi_get_value_string_utf8(env, args[0], NULL, 0, &len);
    assert(status == napi_ok);
    char* buf = (char*)malloc(len + 1);
    status = napi_get_value_string_utf8(env, args[0], buf, len + 1, &len);
    assert(status == napi_ok);
    free(buf);
  }

  return NULL;
}

static napi_value CallWithArray(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  napi_value array = args[0];
  bool is_array = false;
  status = napi_is_array(env, array, &is_array);
  assert(status == napi_ok);

  assert(is_array);
  if (is_array) {
    uint32_t length;
    status = napi_get_array_length(env, array, &length);
    assert(status == napi_ok);

    for (uint32_t i = 0; i < length; ++i) {
      napi_value v;
      status = napi_get_element(env, array, i, &v);
      assert(status == napi_ok);
    }
  }

  return NULL;
}

static napi_value CallWithNumber(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  napi_valuetype types[1];
  status = napi_typeof(env, args[0], types);
  assert(status == napi_ok);

  assert(types[0] == napi_number);
  if (types[0] == napi_number) {
    double value = 0.0;
    status = napi_get_value_double(env, args[0], &value);
    assert(status == napi_ok);
  }

  return NULL;
}

static napi_value CallWithObject(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  napi_valuetype types[1];
  status = napi_typeof(env, args[0], types);
  assert(status == napi_ok);

  assert(types[0] == napi_object);
  if (types[0] == napi_object) {
    napi_value value;
    napi_value key;

    status = napi_create_string_utf8(env, "map", strlen("map"), &key);
    assert(status == napi_ok);
    status = napi_get_property(env, args[0], key, &value);
    assert(status == napi_ok);

    status = napi_create_string_utf8(env, "operand", strlen("operand"), &key);
    assert(status == napi_ok);
    status = napi_get_property(env, args[0], key, &value);
    assert(status == napi_ok);

    status = napi_create_string_utf8(env, "data", strlen("data"), &key);
    assert(status == napi_ok);
    status = napi_get_property(env, args[0], key, &value);
    assert(status == napi_ok);

    status = napi_create_string_utf8(env, "reduce", strlen("reduce"), &key);
    assert(status == napi_ok);
    status = napi_get_property(env, args[0], key, &value);
    assert(status == napi_ok);
  }

  return NULL;
}

static napi_value CallWithTypedarray(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  bool is_typedarray = false;
  status = napi_is_typedarray(env, args[0], &is_typedarray);
  assert(status == napi_ok);

  assert(is_typedarray);
  if (is_typedarray) {
    napi_typedarray_type type;
    napi_value input_buffer;
    size_t byte_offset = 0;
    size_t length = 0;
    status = napi_get_typedarray_info(env, args[0], &type, &length,
        NULL, &input_buffer, &byte_offset);
    assert(status == napi_ok);

    void* data = NULL;
    size_t byte_length = 0;
    status = napi_get_arraybuffer_info(env,
        input_buffer, &data, &byte_length);
    assert(status == napi_ok);

    uint32_t* input_integers = (uint32_t*)((data) + byte_offset);
  }

  return NULL;
}

static napi_value CallWithArguments(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[10000];
  status = napi_get_cb_info(env, info, &argc, NULL, NULL, NULL);
  assert(status == napi_ok);

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  napi_valuetype types[1];
  status = napi_typeof(env, args[0], types);
  assert(status == napi_ok);

  assert(argc > 1 && types[0] == napi_number);
  if (argc > 1 && types[0] == napi_number) {
    uint32_t loop = 0;
    status = napi_get_value_uint32(env, args[0], &loop);
    assert(status == napi_ok);

    for (uint32_t i = 1; i < loop; ++i) {
      status = napi_typeof(env, args[i], types);
      assert(status == napi_ok);
      assert(types[0] == napi_number);

      uint32_t value = 0;
      status = napi_get_value_uint32(env, args[i], &value);
      assert(status == napi_ok);
    }
  }

  return NULL;
}

#define EXPORT_FUNC(name, func) \
  do { \
    napi_value func ## _v; \
    status = napi_create_function(env, \
                                  name, \
                                  NAPI_AUTO_LENGTH, \
                                  func, \
                                  NULL, \
                                  &func ## _v); \
    assert(status == napi_ok); \
    status = napi_set_named_property(env, exports, name, func ## _v); \
    assert(status == napi_ok); \
  } while (0);

NAPI_MODULE_INIT() {
  napi_status status;

  EXPORT_FUNC("callWithString", CallWithString);
  EXPORT_FUNC("callWithLongString", CallWithString);

  EXPORT_FUNC("callWithArray", CallWithArray);
  EXPORT_FUNC("callWithLargeArray", CallWithArray);
  EXPORT_FUNC("callWithHugeArray", CallWithArray);

  EXPORT_FUNC("callWithNumber", CallWithNumber);

  EXPORT_FUNC("callWithObject", CallWithObject);
  EXPORT_FUNC("callWithTypedarray", CallWithTypedarray);

  EXPORT_FUNC("callWith10Arguments", CallWithArguments);
  EXPORT_FUNC("callWith100Arguments", CallWithArguments);
  EXPORT_FUNC("callWith1000Arguments", CallWithArguments);

  return exports;
}
