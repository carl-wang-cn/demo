PROJECT(T1)
SET(SRC_LIST main.cpp)

MESSAGE(STATUS "This is the binary dir: " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is the source dir: " ${PROJECT_SOURCE_DIR})

# 此处的t1跟PROJECT定义的T1没什么关系
ADD_EXECUTABLE(t1 ${SRC_LIST})
