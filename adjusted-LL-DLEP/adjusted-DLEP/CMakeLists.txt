cmake_minimum_required (VERSION 2.8)
project (dlep)

SET(DLEP_VERSION 2.1) # Release version of the software
SET(SOVERSION 2.1) # The library version (e.g. libdlep.so.1.0 would be SOVERSION 1.0)

SET(YUVAL_DLEPLIB_SRC_ADDED OutLoggerEnviromentVariables.cpp OutLogger.cpp Parameter.cpp ConfigOutLoggerMsg.cpp ProgressionOutLoggerMsg.cpp  OutLoggerMsg.cpp OutWriter.cpp OutDB.cpp)
SET(YUVAL_PUBLIC_HDRS_ADDED OutLoggerEnviromentVariables.h OutLogger.h Parameter.h ConfigOutLoggerMsg.h ProgressionOutLoggerMsg.h  OutLoggerMsg.h OutWriter.h OutDB.h json.hpp)
#SET(YUVAL_LIBS_ADDED -lboost_system -pthread)
#SET(YUVAL_LIBS_FOR_MONGOCXX_DRIVER -lcrypto -lssl -lmongocxx -lbsoncxx)

# for mongodb
#SET(YUVAL_INCLUDE_DIRECTORIES /usr/local/include/mongocxx/v_noabi /usr/local/include/bsoncxx/v_noabi /usr/local/lib)

SET(DLEPLIB_SRCS ${YUVAL_DLEPLIB_SRC_ADDED} DestAdvert.cpp DlepInit.cpp DlepLogger.cpp Dlep.cpp Peer.cpp InfoBaseMgr.cpp PeerDiscovery.cpp DlepServiceImpl.cpp DlepMac.cpp PeriodicMcastSendRcv.cpp NetUtils.cpp ProtocolMessage.cpp DataItem.cpp ProtocolConfigImpl.cpp)
SET(DLEP_PUBLIC_HDRS ${YUVAL_PUBLIC_HDRS_ADDED} DlepInit.h DlepClient.h DlepService.h DlepCommon.h DataItem.h ProtocolConfig.h IdTypes.h DlepMac.h Serialize.h LatencyRange.h MultiHop.h)

# Enable "all" c++ warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ggdb")

# By default, treat all warnings as errors
# Can be disalbed with -DWARNINGS_AS_ERRORS:OFF
#set(WARNINGS_AS_ERRORS ON CACHE BOOL "Treat all warnings as errors")
#if(WARNINGS_AS_ERRORS)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
#endif()

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
        message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
SET(BUILD_SHARED_LIBS ON)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

if(APPLE)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
      SET(CMAKE_INSTALL_PREFIX "/opt/local" CACHE PATH "Dlep Install Prefix" FORCE)
    endif()
    EXEC_PROGRAM(sw_vers ARGS -productVersion OUTPUT_VARIABLE OSX_VERSION)
    message(STATUS "Detected version of OS X is ${OSX_VERSION}")
    if(OSX_VERSION VERSION_GREATER 10.12)
      SET(PROTOBUF_MINVERSION 3.2)
    endif()
endif()

find_package (LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

INCLUDE(FindProtobuf)
find_package(Protobuf ${PROTOBUF_MINVERSION} REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS destadvert.proto)

find_package(Boost 1.48 REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})
SET(BOOST_INCREASE_VARIANT_TYPES -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS -DBOOST_MPL_LIMIT_LIST_SIZE=30 -DBOOST_MPL_LIMIT_VECTOR_SIZE=30)
add_definitions(${BOOST_INCREASE_VARIANT_TYPES})

# This approach to only build the sources once for the shared/statis objects from:
# https://stackoverflow.com/a/29824424
add_library(dlepObj OBJECT ${DLEPLIB_SRCS} ${PROTO_SRCS} ${PROTO_HDRS})
# shared libraries need PIC
set_target_properties(dlepObj PROPERTIES POSITION_INDEPENDENT_CODE 1)
add_library(dlepShared SHARED  $<TARGET_OBJECTS:dlepObj>)
target_link_libraries(dlepShared ${LIBXML2_LIBRARIES} ${PROTOBUF_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties(dlepShared PROPERTIES VERSION ${DLEP_VERSION} SOVERSION ${SOVERSION} OUTPUT_NAME dlep MACOSX_RPATH ON)
add_library(dlepStatic STATIC  $<TARGET_OBJECTS:dlepObj>)
set_target_properties(dlepStatic PROPERTIES OUTPUT_NAME dlep)

SET(ETCDIR "${CMAKE_INSTALL_PREFIX}/etc")
# Default docs location
SET(DOCDIR "share/dlep")

### CMake configuration that needs to be different when building for packages
### Hide them behind one variable for convenience
### Generate a package with: cmake .. -DPACKAGE=ON && make package
if(PACKAGE)
  # Install to /usr
  SET(CMAKE_INSTALL_PREFIX "/usr")
  # Take install prefix out of /etc
  SET(ETCDIR "/etc")
  # Clear RPATH for installed objects
  SET(CMAKE_INSTALL_RPATH "")
  # Strip libraries and binaries
  SET(CMAKE_CXX_FLAGS "-s ${CMAKE_CXX_FLAGS}")
  # Perform checks for the 'dpkg' and 'rpm' commands
  find_file(DPKG dpkg)
  find_file(RPM rpm)
  # Set docdir to appropriate /usr/share/doc location
  if(DPKG)
    SET(DOCDIR "share/doc/${PROJECT_NAME}")
  endif()
  if(RPM)
    SET(DOCDIR "share/doc/${PROJECT_NAME}-${DLEP_VERSION}")
  endif()
endif()

# Generate pkg-config file for programs that want to consume the shared lib
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig)
configure_file(libdlep.pc.in ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/libdlep.pc @ONLY)

configure_file(build-defs.h.in build-defs.h @ONLY)
add_executable(Dlep ExampleMain.cpp ExampleDlepClientImpl.cpp Table.cpp)
target_link_libraries(Dlep dlepShared readline ${LIBXML2_LIBRARIES})

install(TARGETS dlepShared DESTINATION lib)
install(TARGETS dlepStatic DESTINATION lib)
install(TARGETS Dlep DESTINATION bin)
install(FILES ${DLEP_PUBLIC_HDRS} DESTINATION include)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig DESTINATION share 
	DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/package/copyright DESTINATION ${DOCDIR})

ENABLE_TESTING()
add_subdirectory(config)
add_subdirectory(doc)
add_subdirectory(tests)

# Linux Packaging
SET(CPACK_PACKAGE_CONTACT "David P. Wiggins <David.Wiggins@ll.mit.edu>")
SET(CPACK_PACKAGE_VERSION ${DLEP_VERSION})
SET(CPACK_PACKAGE_VENDOR "MIT Lincoln Laboratory")
SET(PACKAGE_HOMEPAGE "https://github.com/mit-ll/LL-DLEP")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MIT Lincoln Laboratory Dynamic Link Exchange Protocol")
FILE(READ "${CMAKE_CURRENT_SOURCE_DIR}/package/packagedesc.txt" PACKAGEDESC)
if(DPKG)
  SET(CPACK_GENERATOR "DEB")
  SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n${PACKAGEDESC}")
  SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${PACKAGE_HOMEPAGE}")
  if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    SET(DEB_ARCH "amd64")
  else()
    SET(DEB_ARCH ${CMAKE_SYSTEM_PROCESSOR})
  endif()
  SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}.${DEB_ARCH}")
  SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/package/postinst;${CMAKE_CURRENT_SOURCE_DIR}/package/postrm")
endif()
if(RPM)
  SET(CPACK_GENERATOR "RPM")
  SET(CPACK_RPM_PACKAGE_RELEASE 1)
  SET(CPACK_RPM_PACKAGE_DESCRIPTION "${PACKAGEDESC}")
  SET(CPACK_RPM_PACKAGE_URL "${PACKAGE_HOMEPAGE}")
  SET(CPACK_RPM_PACKAGE_LICENSE "MIT")
  SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
  SET(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/package/postinst")
  SET(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/package/postinst")  # Currently we do the same action for post/postun (run ldconfig)
endif()
include(CPack)
