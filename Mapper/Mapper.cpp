//
// Created by Walker on 2022/6/5.
//
#include "JsonMapper.h"

JsonMapper jsonMapper;
Mapper& Mapper::CreateMapper() {
    return jsonMapper;
}
