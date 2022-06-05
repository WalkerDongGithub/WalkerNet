project(PhysicalLayer)
add_executable(PhysicalLayer

        PhysicalLayerDefinition.h

        PacketQueue.h
        PacketQueue.cpp

        ../Mapper/Mapper.h
        ../Mapper/Mapper.cpp

        PhysicalLayer.h
        PhysicalLayer.cpp

        ../Test/TestPhysicalLayer.cpp

        )

target_link_libraries(PhysicalLayer pthread pcap)
