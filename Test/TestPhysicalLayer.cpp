//
// Created by Walker on 2022/6/4.
//
#include "../PhysicalLayer/PhysicalLayer.h"
#include "../Mapper/Mapper.h"

/**
 * @Date: 2022/06/04
 * @Author: Walker
 *
 *          Test physical layer, physical layer is a simulation about the rx/tx behaviour
 *          of switches, input an vector<pair<string, string>> to the physical layer class,
 *          so that we can build a plane that can send and recv packets.
 */

int main() {
    Mapper& mapper = Mapper::CreateMapper();
    PhysicalLayer(mapper.GetInterfacesInfo("../../Test/test.json")).Test();
}
