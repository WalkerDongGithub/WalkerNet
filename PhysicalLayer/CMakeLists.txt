project(PhysicalLayer)
add_executable(PhysicalLayer

        PhysicalLayerDefinition.h

        PacketQueue.h
        PacketQueue.cpp

        PacketQueue/UtopiaPacketQueue.h
        PacketQueue/UtopiaPacketQueue.cpp
        PacketQueue/ErrorFreePacketQueue.h
        PacketQueue/ErrorFreePacketQueue.cpp
        PacketQueue/ErrorHighPacketQueue.h
        PacketQueue/ErrorHighPacketQueue.cpp
        
        ../Mapper/Mapper.h
        ../Mapper/Mapper.cpp

        PhysicalLayer.h
        PhysicalLayer.cpp

        ../Test/TestPhysicalLayer.cpp

        )

target_link_libraries(PhysicalLayer pthread pcap)
