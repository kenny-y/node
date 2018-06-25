#include <v8.h>
#include <node.h>
#include <assert.h>

void CallWithString(const v8::FunctionCallbackInfo<v8::Value>& args) {
  assert(args.Length() == 1 && args[0]->IsString());
  if (args.Length() == 1 && args[0]->IsString()) {
    auto str = args[0].As<v8::String>();
    const auto length = str->Utf8Length();
    auto buf = new char[length];
    str->WriteUtf8(buf, length);
    delete [] buf;
  }
}

void CallWithArray(const v8::FunctionCallbackInfo<v8::Value>& args) {
  assert(args.Length() == 1 && args[0]->IsArray());
  if (args.Length() == 1 && args[0]->IsArray()) {
    auto array = args[0].As<v8::Array>();
    uint32_t length = array->Length();
    for (uint32_t i = 0; i < length; ++ i) {
      auto v = array->Get(i);
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
    auto obj = args[0].As<v8::Object>();
    auto map = obj->Get(v8::String::NewFromUtf8(isolate, "map"));
    auto operand = obj->Get(v8::String::NewFromUtf8(isolate, "operand"));
    auto data = obj->Get(v8::String::NewFromUtf8(isolate, "data"));
    auto reduce = obj->Get(v8::String::NewFromUtf8(isolate, "reduce"));
  }
}

void CallWithTypedarray(const v8::FunctionCallbackInfo<v8::Value>& args) {
  assert(args.Length() == 1 && args[0]->IsArrayBufferView());
  if (args.Length() == 1 && args[0]->IsArrayBufferView()) {
    assert(args[0]->IsArrayBufferView());
    auto view = args[0].As<v8::ArrayBufferView>();
    auto byte_offset = view->ByteOffset();
    auto byte_length = view->ByteLength();
    assert(view->HasBuffer());
    auto buffer = view->Buffer();
    auto contents = buffer->GetContents();
    auto data = static_cast<uint32_t*>(contents.Data()) + byte_offset;
  }
}

void CallWithArguments(const v8::FunctionCallbackInfo<v8::Value>& args) {
  assert(args.Length() > 1 && args[0]->IsNumber());
  if (args.Length() > 1 && args[0]->IsNumber()) {
    int32_t loop = args[0].As<v8::Uint32>()->Value();
    for (int32_t i = 1; i < loop; ++i) {
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

  NODE_SET_METHOD(target, "callWith10Arguments", CallWithArguments);
  NODE_SET_METHOD(target, "callWith100Arguments", CallWithArguments);
  NODE_SET_METHOD(target, "callWith1000Arguments", CallWithArguments);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
