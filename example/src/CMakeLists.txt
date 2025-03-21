#
# Main CMakelists for the next generation embedded controler.

cmake_minimum_required(VERSION 3.14...3.22)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

# Put the project early since modules might need to detect the compiler.
# More information git@cmake.org/cmake/help/latest/command/project.html
project(
    "CANopenNodeWin32" # This will exposed as the variable PROJECT_NAME.
    VERSION 0.1.0 # Used for installation and defines variables PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH, and PROJECT_VERSION_TWEAK.
    LANGUAGES C CXX  # Used to determine the languages to use based on file extensions
)


set(DRV_SRC "${CMAKE_CURRENT_SOURCE_DIR}")
set(CANOPEN_SRC "C:/Users/p96/Documents/3_Entwicklung/CANopen/1_CANopenNode/CANopenNode")
set(APPL_SRC "${CMAKE_CURRENT_SOURCE_DIR}")

set(INCLUDE_DIRS
	${DRV_SRC}
	${CANOPEN_SRC}
	${APPL_SRC}
)

message(DRV_SRC="${DRV_SRC}")
message(CANOPEN_SRC="${CANOPEN_SRC}")
message(APPL_SRC="${APPL_SRC}")


set(SOURCES 
	${DRV_SRC}/CO_driver_blank.c
	${DRV_SRC}/CO_storageBlank.c
	${CANOPEN_SRC}/301/CO_ODinterface.c 
	${CANOPEN_SRC}/301/CO_NMT_Heartbeat.c 
	${CANOPEN_SRC}/301/CO_HBconsumer.c 
	${CANOPEN_SRC}/301/CO_Emergency.c 
	${CANOPEN_SRC}/301/CO_SDOserver.c 
	${CANOPEN_SRC}/301/CO_TIME.c 
	${CANOPEN_SRC}/301/CO_SYNC.c 
	${CANOPEN_SRC}/301/CO_PDO.c 
	${CANOPEN_SRC}/303/CO_LEDs.c
	${CANOPEN_SRC}/305/CO_LSSslave.c
	${CANOPEN_SRC}/storage/CO_storage.c
	${CANOPEN_SRC}/CANopen.c
	${APPL_SRC}/OD.c 
	${DRV_SRC}/main_blank.c
	${DRV_SRC}/PCANBasic.h
	${DRV_SRC}/targetver.h
	${DRV_SRC}/CAN_Write.h
	${DRV_SRC}/CAN_Write.cpp
	${DRV_SRC}/Interface.cpp
	${DRV_SRC}/Struct.h
)


add_executable(${PROJECT_NAME} ${DRV_SRC}/main_blank.c ${SOURCES} ${INCLUDE_DIRS})
target_compile_features(${PROJECT_NAME} PRIVATE ${CPP_STANDARD})
include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
# group_folders("${SOURCES}" "${HEADERS}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME}) 