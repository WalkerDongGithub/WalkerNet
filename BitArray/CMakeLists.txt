add_executable(TestBitArray
        BitArray.h
        BitArray.cpp
        ../Test/TestBitArray.cpp
        )

target_link_libraries(TestBitArray glog)
