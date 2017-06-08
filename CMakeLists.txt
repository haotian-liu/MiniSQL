cmake_minimum_required(VERSION 3.7)
project(MiniSQL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp src/RecordManager/RecordManager.cpp src/RecordManager/RecordManager.h src/CatelogManager/CatalogManager.cpp src/CatelogManager/CatalogManager.h src/IndexManager/IndexManager.cpp src/IndexManager/IndexManager.h lib/BPTree/BPTree.cpp lib/BPTree/BPTree.h)
add_executable(MiniSQL ${SOURCE_FILES})