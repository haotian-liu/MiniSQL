cmake_minimum_required(VERSION 3.7)
project(MiniSQL)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -lreadline)

set(SOURCE_FILES main.cpp
        src/RecordManager/RecordManager.cpp
        src/RecordManager/RecordManager.h
        src/CatalogManager/CatalogManager.cpp
        src/CatalogManager/CatalogManager.h
        src/IndexManager/IndexManager.cpp
        src/IndexManager/IndexManager.h
        lib/BPTree/BPTree.h
        src/API/Api.cpp
        src/API/Api.h
        src/Interpreter/Interpreter.cpp
        src/Interpreter/Interpreter.h
        src/Interpreter/QueryRequest.cpp
        src/Interpreter/QueryRequest.h
        src/Interpreter/scan.cpp
        src/Interpreter/parse.cpp
        src/Interpreter/parser_public.h
        src/BufferManager/BufferManager.cpp
        include/DataStructure.h
        )

add_executable(MiniSQL ${SOURCE_FILES})
