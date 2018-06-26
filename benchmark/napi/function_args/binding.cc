#include <v8.h>
#include <node.h>
#include <assert.h>

using v8::Local;
using v8::Value;
using v8::String;
using v8::Object;
using v8::Array;
using v8::ArrayBufferView;
using v8::ArrayBuffer;

void CallWithString(const v8::FunctionCallbackInfo<v8::Value>& args) {
  assert(args.Length() == 1 && args[0]->IsString());
  if (args.Length() == 1 && args[0]->IsString()) {
    Local<String> str = args[0].As<v8::String>();
    const int32_t length = str->Utf8Length() + 1;
    char* buf = new char[length];
    str->WriteUtf8(buf, length);
    delete [] buf;
  }
}

void CallWithArray(const v8::FunctionCallbackInfo<v8::Value>& args) {
  assert(args.Length() == 1 && args[0]->IsArray());
  if (args.Length() == 1 && args[0]->IsArray()) {
    const Local<Array> array = args[0].As<v8::Array>();
    uint32_t length = array->Length();
    for (uint32_t i = 0; i < length; ++ i) {
      Local<Value> v = array->Get(i);
    }
  }
}

void CallWithNumber(const v8::FunctionCallbackInfo<v8::Value>& args) {
  assert(args.Length() == 1 && args[0]->IsNumber());
  if (args.Length() == 1 && args[0]->IsNumber()) {
    double value = args[0].As<v8::Number>()->Value();
  }
}

void CallWithObject(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  assert(args.Length() == 1 && args[0]->IsObject());
  if (args.Length() == 1 && args[0]->IsObject()) {
    Local<Object> obj = args[0].As<v8::Object>();
    Local<Value> map = obj->Get(v8::String::NewFromUtf8(isolate, "map"));
    Local<Value> oper = obj->Get(v8::String::NewFromUtf8(isolate, "operand"));
    Local<Value> data = obj->Get(v8::String::NewFromUtf8(isolate, "data"));
    Local<Value> reduce = obj->Get(v8::String::NewFromUtf8(isolate, "reduce"));
  }
}

void CallWithTypedarray(const v8::FunctionCallbackInfo<v8::Value>& args) {
  assert(args.Length() == 1 && args[0]->IsArrayBufferView());
  if (args.Length() == 1 && args[0]->IsArrayBufferView()) {
    assert(args[0]->IsArrayBufferView());
    Local<ArrayBufferView> view = args[0].As<v8::ArrayBufferView>();
    size_t byte_offset = view->ByteOffset();
    size_t byte_length = view->ByteLength();
    assert(view->HasBuffer());
    Local<ArrayBuffer> buffer = view->Buffer();
    ArrayBuffer::Contents contents = buffer->GetContents();
    uint32_t* data = static_cast<uint32_t*>(contents.Data()) + byte_offset;
  }
}

void CallWithArguments(const v8::FunctionCallbackInfo<v8::Value>& args) {
  assert(args.Length() > 1 && args[0]->IsNumber());
  if (args.Length() > 1 && args[0]->IsNumber()) {
    uint32_t loop = args[0].As<v8::Uint32>()->Value();
    for (uint32_t i = 1; i < loop; ++i) {
      assert(i < args.Length());
      assert(args[i]->IsUint32());
      uint32_t value = args[i].As<v8::Uint32>()->Value();
    }
  }
}

void Initialize(v8::Local<v8::Object> target) {
  NODE_SET_METHOD(target, "callWithString", CallWithString);
  NODE_SET_METHOD(target, "callWithLongString", CallWithString);

  NODE_SET_METHOD(target, "callWithArray", CallWithArray);
  NODE_SET_METHOD(target, "callWithLargeArray", CallWithArray);
  NODE_SET_METHOD(target, "callWithHugeArray", CallWithArray);

  NODE_SET_METHOD(target, "callWithNumber", CallWithNumber);
  NODE_SET_METHOD(target, "callWithObject", CallWithObject);
  NODE_SET_METHOD(target, "callWithTypedarray", CallWithTypedarray);

  NODE_SET_METHOD(target, "callWith10Numbers", CallWithArguments);
  NODE_SET_METHOD(target, "callWith100Numbers", CallWithArguments);
  NODE_SET_METHOD(target, "callWith1000Numbers", CallWithArguments);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
