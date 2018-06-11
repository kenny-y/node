#include <v8.h>
#include <node.h>

void CallWithString(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() == 1 && args[0]->IsString()) {
    auto str = args[0].As<v8::String>();
    const auto length = str->Utf8Length();
    auto buf = new char[length];
    str->WriteUtf8(buf, length);
    delete [] buf;
  }
}

void Initialize(v8::Local<v8::Object> target) {
  NODE_SET_METHOD(target, "callWithString", CallWithString);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
