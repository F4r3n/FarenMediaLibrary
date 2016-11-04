#pragma once
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
class Serializer {
    public:
  Serializer() {}
 virtual ~Serializer() {}
 virtual void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) = 0;
 virtual void parse(rapidjson::Value &value) = 0;
};