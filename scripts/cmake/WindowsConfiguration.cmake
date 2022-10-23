# Module to setup Windows specific flags

if (WIN32)
  # Prebuilt LLVM for Windows doesn't come with CMake files
  message(STATUS "Detected MS Windows")
  # Note: This should check for the CMake version instead of OS
  cmake_policy(SET CMP0079 NEW)

  # std::min fails on windows after including windows.h
  # this is a workaround
  add_compile_definitions(NOMINMAX=1)

  find_package (Python COMPONENTS Interpreter)
  if(NOT Python_FOUND)
	message(WARNING "Python not found, cmake will assume that it is on path")
	set(Python_EXECUTABLE python)
  endif()
  message(STATUS "Found Python: ${Python_EXECUTABLE}")
  set(LIBGOMP_LIB "-lgomp -ldl")
  set(OS_FLEX_SMTLIB_FLAGS "--wincompat")



  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	  # There are a LOT of warnings from clang headers
	  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything")
	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything")
      set(OS_Z3_LIBS "stdc++")
      set(OS_C2GOTO_FLAGS "-D_MSVC")
      set(OS_X86_INCLUDE_FOLDER "C:/")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	  add_compile_options(/bigobj)
      add_compile_options(/W1)
      set(OS_C2GOTO_FLAGS "-D_MSVC")
      set(OS_Z3_LIBS "")
      set(OS_X86_INCLUDE_FOLDER "C:/")
  else()
	  message(AUTHOR_WARNING "${CMAKE_CXX_COMPILER_ID} is not tested in Windows. You may run into issues.")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything -lbcrypt")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything -lbcrypt")
      #set(OS_C2GOTO_FLAGS -D_MINGW -D__GNUC__=12)
      set(OS_C2GOTO_FLAGS  -IC:/msys64/mingw64/include -D_MINGW -D__GNUC__=12 -D__GNUC_MINOR__=7 -D_X86_)
      set(OS_Z3_LIBS "")
      set(OS_X86_INCLUDE_FOLDER "C:/msys64/mingw64/include")
	endif()

  if(DOWNLOAD_WINDOWS_DEPENDENCIES)
    download_zip_and_extract(LLVM https://sourceforge.net/projects/esbmc-deps/files/clang.zip/download)
    download_zip_and_extract(Z3 https://sourceforge.net/projects/esbmc-deps/files/z3.zip/download)
    set(LLVM_DIR ${CMAKE_BINARY_DIR}/LLVM/clang)
    set(Clang_DIR ${CMAKE_BINARY_DIR}/LLVM/clang)
    set(Z3_DIR ${CMAKE_BINARY_DIR}/Z3)
  endif()
endif()
