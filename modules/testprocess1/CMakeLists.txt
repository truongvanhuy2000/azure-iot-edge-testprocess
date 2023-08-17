cmake_minimum_required(VERSION 2.8.11)

macro(compileAsC99)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_C_FLAGS "--std=c99 ${CMAKE_C_FLAGS}")
      set (CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
    endif()
  else()
    set (CMAKE_C_STANDARD 99)
    set (CMAKE_CXX_STANDARD 11)
  endif()
endmacro(compileAsC99)

compileAsC99()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_EDGE_MODULES")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_EDGE_MODULES")

set(AZUREIOT_INC_FOLDER "/usr/include/azureiot" "/usr/include/azureiot/inc")

include_directories(${AZUREIOT_INC_FOLDER})

set(process1_file
    ./process1.c
)

set(process2_file
    ./process2.c
)


add_executable(process1 ${process1_file})
add_executable(process2 ${process2_file})
add_executable(2process1program ./2process1program.c)


target_link_libraries(process1
    iothub_client_mqtt_transport
    iothub_client
    parson
    umqtt
    prov_auth_client
    hsm_security_client
    uhttp
    utpm
    aziotsharedutil
    pthread
    curl
    ssl
    crypto
    m
    uuid
    
)

target_link_libraries(process2
    iothub_client_mqtt_transport
    iothub_client
    parson
    umqtt
    prov_auth_client
    hsm_security_client
    uhttp
    aziotsharedutil
    pthread
    curl
    utpm
    ssl
    crypto
    m
    uuid
)

target_link_libraries(2process1program
    iothub_client_mqtt_transport
    iothub_client
    parson
    umqtt
    prov_auth_client
    hsm_security_client
    uhttp
    aziotsharedutil
    pthread
    curl
    utpm
    ssl
    crypto
    m
    uuid
)