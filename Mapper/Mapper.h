//
// Created by Walker on 2022/6/4.
//

#ifndef WALKERNET_MAPPER_H
#define WALKERNET_MAPPER_H

#include "../PhysicalLayer/PhysicalLayerDefinition.h"

/**
 * Mapper: map file (json, xml, yaml, etc.) to C++ STL data structure.
 */
class Mapper {
public:
    virtual InterfacesInfo GetInterfacesInfo(const std::string& path) = 0;
    static Mapper& CreateMapper();
};



#endif //WALKERNET_MAPPER_H
