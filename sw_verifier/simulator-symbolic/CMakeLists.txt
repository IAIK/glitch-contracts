find_package(Backward)
if(NOT TARGET cxxopts::cxxopts)
  find_package(cxxopts REQUIRED)
endif()
if(NOT TARGET fmt::fmt)
  find_package(fmt REQUIRED)
endif()
if(NOT TARGET nlohmann_json::nlohmann_json)
  find_package(nlohmann_json REQUIRED)
endif()

include(ExternalProject)

set(BACKEND CADICAL)
#set(BACKEND CRYPTOMINISAT)
#set(BACKEND KISSAT)

add_executable(simulator-symbolic
  ../contract/contract.cpp
  ../contract/contract_memory.cpp
  ../simulator-common/contract_runtime_impl.cpp
  ../simulator-common/main.cpp
  ../simulator-common/Options.cpp
  ../simulator-common/common.cpp
  ../simulator-common/contract_runtime.h
  bitvector.cpp
  fragments.cpp
  PropVarSet.cpp
  bitvector.h
  SatSolver.cpp
  SymbolicExpression.cpp
  SymbolicExpression.h
  SymbolicManager.cpp
)

if("${BACKEND}" STREQUAL "CRYPTOMINISAT")
    message("Adding external Cryptominisat target")
    set(SOLVER_NAME cryptominisat)
    set(SOLVER_DIR ${PROJECT_SOURCE_DIR}/external/${SOLVER_NAME})
    set(SOLVER_BUILD_DIR ${SOLVER_DIR}/build)
    set(SOLVER_OPTIONS -DIPASIR=ON -DSTATICCOMPILE=OFF -DNOZLIB=ON)
    set(SOLVER_CONFIG_CMD cmake -B ${SOLVER_BUILD_DIR} ${SOLVER_OPTIONS})
    set(SOLVER_LIB_NAME lib${SOLVER_NAME})
    set(SOLVER_LIB_PATH ${SOLVER_BUILD_DIR}/lib/libipasircryptominisat5.so)
    set(SOLVER_LIB_LINKAGE SHARED)
elseif("${BACKEND}" STREQUAL "CADICAL")
    message("Adding external Cadical target")
    set(SOLVER_NAME cadical)
    set(SOLVER_DIR ${PROJECT_SOURCE_DIR}/external/${SOLVER_NAME})
    set(SOLVER_BUILD_DIR ${SOLVER_DIR}/build)
    set(SOLVER_OPTIONS )
    set(SOLVER_CONFIG_CMD ./configure)
    set(SOLVER_LIB_NAME lib${SOLVER_NAME})
    set(SOLVER_LIB_PATH ${SOLVER_BUILD_DIR}/libcadical.a)
    set(SOLVER_LIB_LINKAGE STATIC)
elseif("${BACKEND}" STREQUAL "KISSAT")
    message("Adding external kissat target")
    set(SOLVER_NAME kissat)
    set(SOLVER_DIR ${PROJECT_SOURCE_DIR}/external/${SOLVER_NAME})
    set(SOLVER_BUILD_DIR ${SOLVER_DIR}/build)
    set(SOLVER_OPTIONS )
    set(SOLVER_CONFIG_CMD ./configure)
    set(SOLVER_LIB_NAME lib${SOLVER_NAME})
    set(SOLVER_LIB_PATH ${SOLVER_BUILD_DIR}/libkissat.a)
    set(SOLVER_LIB_LINKAGE STATIC)
    set(SOLVER_INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/external/kissat/src")
    target_compile_definitions(simulator-symbolic PUBLIC BACKEND_API_KISSAT)
else()
    message(FATAL_ERROR "No valid backend provided, aborting.")
    die()
endif()

ExternalProject_Add(${SOLVER_NAME}
  SOURCE_DIR        "${SOLVER_DIR}"
  DOWNLOAD_COMMAND  git submodule init && git submodule update
  CONFIGURE_COMMAND cd ${SOLVER_DIR} && ${SOLVER_CONFIG_CMD}
  BUILD_COMMAND     cd ${SOLVER_BUILD_DIR} && make -j4
  BUILD_BYPRODUCTS  ${SOLVER_LIB_PATH}
  INSTALL_COMMAND   ""
  )
add_library(${SOLVER_LIB_NAME} ${SOLVER_LIB_LINKAGE} IMPORTED)
add_library(${SOLVER_LIB_NAME}::${SOLVER_LIB_NAME} ALIAS ${SOLVER_LIB_NAME})
set_target_properties(${SOLVER_LIB_NAME} PROPERTIES
  IMPORTED_LOCATION             "${SOLVER_LIB_PATH}"
  INTERFACE_INCLUDE_DIRECTORIES "${SOLVER_INTERFACE_INCLUDE_DIRECTORIES}"
  LINKER_LANGUAGE               CXX)

add_dependencies(simulator-symbolic ${SOLVER_NAME})
target_link_libraries(simulator-symbolic PRIVATE ${SOLVER_LIB_NAME}::${SOLVER_LIB_NAME})
target_link_libraries(simulator-symbolic PUBLIC
  Backward::Backward
  dw
  cxxopts::cxxopts
  fmt::fmt
  nlohmann_json::nlohmann_json
)

target_compile_definitions(simulator-symbolic PUBLIC CONTRACT_MODE_SIMULATOR RV_I)
target_compile_definitions(simulator-symbolic PUBLIC SIMULATOR_BIT_LEVEL)
#target_compile_definitions(simulator-symbolic PUBLIC DEBUG_SOLVER)
target_include_directories(simulator-symbolic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(simulator-symbolic PRIVATE ../simulator-common)
