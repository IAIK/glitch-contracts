find_package(Backward)
if(NOT TARGET cxxopts::cxxopts)
  find_package(cxxopts REQUIRED)
endif()
if(NOT TARGET fmt::fmt)
  find_package(fmt REQUIRED)
endif()
if(NOT TARGET nlohmann_json)
  find_package(nlohmann_json REQUIRED)
endif()

add_executable(simulator-concrete
  ../contract/contract.cpp
  ../contract/contract_memory.cpp
  ../simulator-common/contract_runtime_impl.cpp
  ../simulator-common/main.cpp
  ../simulator-common/Options.cpp
  ../simulator-common/common.cpp
  bitvector.cpp
)
target_link_libraries("simulator-concrete" PUBLIC Backward::Backward cxxopts::cxxopts fmt::fmt nlohmann_json::nlohmann_json)
#add_dependencies(simulator-concrete libkissat)
#target_link_libraries(simulator-concrete libkissat)

target_compile_definitions(simulator-concrete PUBLIC CONTRACT_MODE_SIMULATOR)
target_compile_definitions(simulator-concrete PUBLIC RV_I)
target_include_directories(simulator-concrete PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(simulator-concrete PRIVATE ../simulator-common)
