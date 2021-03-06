set(SQLITE3_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

message(STATUS "Building sqlite3 since sqlite3 was not found")

set(_byproducts
  ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.lib
  ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.so
  ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.a
)

if(MINGW)
  ExternalProject_Add(
    sqlite3
    URL https://sqlite.org/2018/sqlite-autoconf-3260000.tar.gz
    URL_HASH SHA1=9af2df1a6da5db6e2ecf3f463625f16740e036e9
    CONFIGURE_COMMAND ""
    BUILD_COMMAND gcc -shared sqlite3.c -o ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.a
    BUILD_IN_SOURCE true
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_COMMAND ""
    INSTALL_DIR ${SQLITE3_INSTALL_DIR}
  )
  
  ExternalProject_Add_Step(
    sqlite3
    copy-sqlite3
    DEPENDEES download
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src" "<INSTALL_DIR>/include/" "*.h"
  )
else()
  ExternalProject_Add(
    sqlite3
    URL https://sqlite.org/2018/sqlite-autoconf-3260000.tar.gz
    URL_HASH SHA1=9af2df1a6da5db6e2ecf3f463625f16740e036e9
    CONFIGURE_COMMAND ./configure --prefix ${SQLITE3_INSTALL_DIR}
    BUILD_COMMAND make
    BUILD_IN_SOURCE true
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_COMMAND make install
    INSTALL_DIR ${SQLITE3_INSTALL_DIR}
  )
endif()


ExternalProject_Get_Property(
  sqlite3
  install_dir
)

set(SQLITE3_INCLUDE_DIR "${install_dir}/include")
if(NOT MINGW AND WIN32)
  set(SQLITE3_LIBRARY_DIR "${install_dir}/lib")
  set(SQLITE3_LIBRARY "${install_dir}/lib/libsqlite3.lib")
  set(SQLITE3_LIBRARIES "${install_dir}/lib/libsqlite3.lib" "${SQLITE3_LIBRARIES}")
else()
  set(SQLITE3_LIBRARY_DIR "${install_dir}/lib")
  set(SQLITE3_LIBRARY "${install_dir}/lib/libsqlite3.a")
  set(SQLITE3_LIBRARIES "${install_dir}/lib/libsqlite3.a" "${SQLITE3_LIBRARIES}")
endif()
if(NOT TARGET sqlite3::sqlite3)
  add_library(sqlite3::sqlite3 INTERFACE IMPORTED)
  add_dependencies(sqlite3::sqlite3 sqlite3)
  set_target_properties(sqlite3::sqlite3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SQLITE3_INCLUDE_DIR}")
  set_target_properties(sqlite3::sqlite3 PROPERTIES INTERFACE_LINK_LIBRARIES "${SQLITE3_LIBRARIES}")
endif()

mark_as_advanced( SQLITE3_INCLUDE_DIR SQLITE3_LIBRARIES SQLITE3_LIBRARY SQLITE3_LIBRARY_PATH )