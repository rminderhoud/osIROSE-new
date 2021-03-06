
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT CharServer)
project(${PROJECT})

if (GCC)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
endif()

set( SOURCES_MAIN
  src/main
)

set( SOURCES_HEADER
  include/ccharserver.h
  include/ccharclient.h
  include/ccharisc.h )

set( SOURCES_BASE
  src/ccharserver.cpp
  src/ccharclient.cpp
  src/ccharisc.cpp
)

add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

add_dependencies(${PROJECT} RosePackets_generated)

target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT} CURL::libcurl)

if(WITH_MYSQL)
  target_link_libraries(${PROJECT} mysql::mysql)
endif()

if (MINGW)
  target_link_libraries(${PROJECT} wsock32 ws2_32)
elseif (MSVC)
  target_link_libraries(${PROJECT} wsock32.lib)
endif()

if(WITH_CRASH_REPORTS)
  target_link_libraries(${PROJECT} Breakpad::Breakpad)
endif()

add_dependencies(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} RoseCommon)

if(WITH_CRASH_REPORTS)
  generate_symbol_data(${PROJECT})
endif()

if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
