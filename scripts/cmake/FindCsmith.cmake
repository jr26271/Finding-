# Module to find and configure Csmith

if(ENABLE_CSMITH)
    find_program(CSMITH csmith REQUIRED)
    find_path(CSMITH_INC csmith.h PATH_SUFFIXES csmith csmith-2.3.0)    

    message(STATUS "Found Csmith: ${CSMITH} ${CSMITH_INC}")

    # DEFAULT CSMITH ARGS
    # --no-argc: see https://github.com/esbmc/esbmc/issues/170
    # --inline-function: inline attributes on generated functions
    # --paranoid: pointer-related assertions
    # --float: enable floats
    # --no-math64: disable 64-bit math ops
    set(CSMITH_DEFAULT_ARGS "--inline-function --paranoid --float --no-math64")
    set(CSMITH_TIMEOUT 30)

    # DEFAULT ESBMC ARGS
    set(CSMITH_ESBMC_DEFAULT_ARGS "-I${CSMITH_INC}")
    set(CSMITH_ESBMC_TIMEOUT 10)

    assert_variable_is_defined(ESBMC_BIN)
    # Creates a csmith target
    # - csmith_args: args to be passed to csmith
    # - esbmc_args: args to be passed to esbmc
    # - c_file_name: unique name for the C generated by csmith
    function(create_csmith_test test_name esbmc_args)   
        set(CMD ${CMAKE_SOURCE_DIR}/scripts/run_csmith.py ${CSMITH} ${CSMITH_INC} ${CSMITH_DEFAULT_ARGS} ${ESBMC_BIN} ${esbmc_args} ${CSMITH_ESBMC_TIMEOUT}  ${CSMITH_TIMEOUT})
        add_test(NAME "csmith-${test_name}"
            COMMAND python3 ${CMD})

    endfunction()
    
    # incremental: default sv-comp arguments
    create_csmith_test(incremental "${CSMITH_ESBMC_DEFAULT_ARGS} --incremental-bmc --no-div-by-zero-check --force-malloc-success --state-hashing --no-align-check --k-step 2 --floatbv --unlimited-k-steps --context-bound 3 --32 --no-pointer-check --no-bounds-check --interval-analysis ")
    # k-induction: default sv-comp arguments
    create_csmith_test(kind "${CSMITH_ESBMC_DEFAULT_ARGS} --k-induction --no-div-by-zero-check --force-malloc-success --state-hashing --no-align-check --k-step 2 --floatbv --unlimited-k-steps --context-bound 3 --32 --no-pointer-check --no-bounds-check --interval-analysis ")
    # k-induction-parallel: default sv-comp arguments
    create_csmith_test(kind-parallel "${CSMITH_ESBMC_DEFAULT_ARGS} --k-induction-parallel --no-div-by-zero-check --force-malloc-success --state-hashing --no-align-check --k-step 2 --floatbv --unlimited-k-steps --context-bound 3 --32 --no-pointer-check --no-bounds-check --interval-analysis ")
endif()