cmake_minimum_required(VERSION 3.14)

project(remover)

#set(BoostDir "C:/users/s-u-d/boost")

find_package(Boost REQUIRED COMPONENTS json filesystem)
message("message: "${Boost_INCLUDE_DIR})
#include_directories(${Boost_INCLUDE_DIR})
#include_directories(${BoostDir}/include)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Boost::json Boost::filesystem)

#target_link_directories(${PROJECT_NAME} PRIVATE ${BoostDir}/lib)
#target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
