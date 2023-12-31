set(Boolector_MIN_VERSION "3.2.0")

if(DEFINED Boolector_DIR)
    set(ENABLE_BOOLECTOR ON)
endif()

if(EXISTS $ENV{HOME}/boolector)
    set(ENABLE_BOOLECTOR ON)
endif()

if(ENABLE_BOOLECTOR)
    find_package(Boolector REQUIRED
            HINTS ${Boolector_DIR}/lib/cmake $ENV{HOME}/boolector)

    message(STATUS "Found Boolector at: ${Boolector_DIR}")
    message(STATUS "Boolector version: ${Boolector_VERSION}")
    if(Boolector_VERSION VERSION_LESS Boolector_MIN_VERSION)
        message(FATAL_ERROR "Expected version ${Boolector_MIN_VERSION} or greater")
    endif()

    add_library(solverbtor boolector_conv.cpp)
    target_include_directories(solverbtor
            PRIVATE ${Boost_INCLUDE_DIRS}
            PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(solverbtor Boolector::boolector fmt::fmt)

    target_link_libraries(solvers INTERFACE solverbtor)

    set(ESBMC_ENABLE_boolector 1 PARENT_SCOPE)
    set(ESBMC_AVAILABLE_SOLVERS "${ESBMC_AVAILABLE_SOLVERS} boolector" PARENT_SCOPE)
endif()
