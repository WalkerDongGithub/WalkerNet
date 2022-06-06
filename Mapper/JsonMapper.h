//
// Created by Walker on 2022/6/4.
//

#ifndef WALKERNET_JSONMAPMAPPER_H
#define WALKERNET_JSONMAPMAPPER_H

#include "Mapper.h"
#include "json.hpp"
#include <fstream>
using std::ifstream;
using nlohmann::json;
class JsonMapper: public Mapper {
public:
    virtual InterfacesInfo GetInterfacesInfo(const std::string& path) {
        json j;
        ifstream in(path);
        in >> j;
        InterfacesInfo res;
        for (const auto &config: j.at("interfaces")) {
            std::string if_name = config.at("name").get<std::string>();
            std::string if_ip = config.at("ip").get<std::string>();
            res.emplace_back(if_name, if_ip);
        } return res;
    }
};




#endif //WALKERNET_JSONMAPMAPPER_H
