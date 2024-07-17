cmake_minimum_required(VERSION 3.10)
project(smtgen-cbmc)

set(cbmc_path "/home/jharing/git/cbmc/build/bin/cbmc")

add_custom_target(
    contract.smt ALL
    # --verbosity 10
    COMMAND ${cbmc_path}
            "--cpp11"
            "--verbosity" "1"
            "--stop-on-fail"
            "--no-built-in-assertions"
            "--show-goto-symex-steps"
            #"--smt2"
            "--z3"
            #"--drop-unused-functions"
            "--outfile" "contract.smt"
            "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/../contract/contract.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/../contract/contract_memory.cpp"
            #"-D" "RV_I"
            "-I${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
)

add_custom_target(
    contract.log ALL
    # --verbosity 10
    COMMAND ${cbmc_path}
            "--program-only"
            "--cpp11"
            "--verbosity" "1"
            "--stop-on-fail"
            "--no-built-in-assertions"
            "--show-goto-symex-steps"
            #"--smt2"
            "--z3"
            #"--drop-unused-functions"
            "--outfile" "contract.smt"
            "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/../contract/contract.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/../contract/contract_memory.cpp"
            #"-D" "RV_I"
            "-I${CMAKE_CURRENT_SOURCE_DIR}" > contract.log
    VERBATIM
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
