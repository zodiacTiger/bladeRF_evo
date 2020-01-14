cmake_minimum_required(VERSION 3.5)
project(bladeRF_evo CXX)
set(LINK_DIRS /opt/local/lib)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)


#if(NOT PKG_CONFIG_FOUND)
#    message(FATAL_ERROR "QT Widgets not found, but are needed.")
#endif()
set(LIBS bladeRF)

if (MSVC)
    find_package(LibPThreadsWin32 REQUIRED)
    set(INCLUDES ${INCLUDES} ${MSVC_C99_INCLUDES})
    set(TEST_CONFIG_FILE_INCLUDES
            ${TEST_CONFIG_FILE_INCLUDES}
            ${MSVC_C99_INCLUDES}
            ${LIBPTHREADSWIN32_INCLUDE_DIRS}
            )
    set(SRC
            ${SRC}
            ${BLADERF_HOST_COMMON_SOURCE_DIR}/windows/clock_gettime.c
            ${BLADERF_HOST_COMMON_SOURCE_DIR}/windows/mkdtemp.c
            )
    set(TEST_CONFIG_FILE_LIBS
            ${TEST_CONFIG_FILE_LIBS}
            ${LIBPTHREADSWIN32_LIBRARIES}
            )
endif ()

########################################################################
# Find boost
########################################################################
if (UNIX AND EXISTS "/usr/lib64")
    list(APPEND BOOST_LIBRARYDIR "/usr/lib64") #fedora 64-bit fix
endif (UNIX AND EXISTS "/usr/lib64")
set(Boost_ADDITIONAL_VERSIONS
        "1.35.0" "1.35" "1.36.0" "1.36" "1.37.0" "1.37" "1.38.0" "1.38" "1.39.0" "1.39"
        "1.40.0" "1.40" "1.41.0" "1.41" "1.42.0" "1.42" "1.43.0" "1.43" "1.44.0" "1.44"
        "1.45.0" "1.45" "1.46.0" "1.46" "1.47.0" "1.47" "1.48.0" "1.48" "1.49.0" "1.49"
        "1.50.0" "1.50" "1.51.0" "1.51" "1.52.0" "1.52" "1.53.0" "1.53" "1.54.0" "1.54"
        "1.55.0" "1.55" "1.56.0" "1.56" "1.57.0" "1.57" "1.58.0" "1.58" "1.59.0" "1.59"
        "1.60.0" "1.60" "1.61.0" "1.61" "1.62.0" "1.62" "1.63.0" "1.63" "1.64.0" "1.64"
        "1.65.0" "1.65" "1.66.0" "1.66" "1.67.0" "1.67" "1.68.0" "1.68" "1.69.0" "1.69"
        )
find_package(Boost "1.35" COMPONENTS filesystem system thread)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost required")
endif ()

find_package(SWIG)
SET(GCC_COVERAGE_COMPILE_FLAGS "-Wno-c++11-extensions")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

if (SWIG_FOUND)
    # Minimum SWIG version required is 1.3.31
    set(SWIG_VERSION_CHECK FALSE)
    if ("${SWIG_VERSION}" VERSION_GREATER "1.3.30")
        set(SWIG_VERSION_CHECK TRUE)
    endif ()
else ()
    message(FATAL_ERROR "SWIG required to compile")
endif (SWIG_FOUND)

########################################################################
# Find gnuradio build dependencies
########################################################################
set(GR_REQUIRED_COMPONENTS RUNTIME FILTER PMT)
#find_package(Gnuradio)
#find_package(Volk)
#find_package(CppUnit)
#find_package(fftw)
#find_package(Doxygen)

option(LOCAL_OSMOCOM "Build with local osmocom libraries" OFF)
#find_package(Libosmocore)
#find_package(Libosmocodec)
#find_package(Libosmocoding)
#find_package(Libosmogsm)

#if(NOT PKG_CONFIG_FOUND)
#    message(FATAL_ERROR "pkg-config is required")
#endif()
#if(NOT GNURADIO_RUNTIME_FOUND)
#    message(FATAL_ERROR "GnuRadio Runtime required")
#endif()
#if(NOT VOLK_FOUND)
#    message(FATAL_ERROR "Volk Not Found")
#endif()
#if(NOT CPPUNIT_FOUND)
#    message(FATAL_ERROR "CppUnit required to compile stuff")
#endif()

#if(NOT LIBOSMOCORE_FOUND OR NOT LIBOSMOCODEC_FOUND OR NOT LIBOSMOGSM_FOUND)
#    set(LOCAL_OSMOCOM ON)
#endif()
#
#if(LOCAL_OSMOCOM)
#    message(STATUS "Compiling with local osmocom libraries")
#elseif(NOT LIBOSMOCODING_FOUND)
#    message(STATUS "Compiling local version of libosmocoding")
#endif()

find_package(Qt5Core REQUIRED)

find_package(Qt5Widgets CONFIG REQUIRED)


set(BLANK_BOX_LINE "|                                             |")
link_directories(${LINK_DIRS})
add_definitions(-DLIBBLADERF_SEARCH_PREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(-DLOGGING_ENABLED=1)
include_directories(${TEST_CONFIG_FILE_INCLUDES} ${INCLUDES}
        "../include"
        "/opt/local/libexec/qt5/include"
        "/opt/local/include"
        "/opt/local/include/boost"


        )
#qt5_wrap_cpp(moc_sources ../src/mainwindow.cpp)
#qt5_wrap_ui(uic_sources ../src/mainwindow.cpp)
qt5_generate_moc(../src/main.cpp main.moc)

list(APPEND library_sources ${moc_sources} ${uic_sources})
add_executable(bladeRF_evo
        ../src/main.cpp
        ../include/conversions.c
        ../src/signal_gen.cpp
        ../src/device.cpp
        ../src/logging.cpp
        ../src/processSamples.cpp
        #        ../src/plusMinus.cpp
        #        ../src/gui.cpp
        ../include/stream.hpp
        ../src/mainwindow.cpp
        ../include/mainwindow.h
        ##### FIR FILTER #####
        # ../src/firFilters.cpp
        ######################
        ../src/test_tones.cpp
        ../include/rxThread.hpp
        ../src/rxThread.cpp
        ../include/txThread.hpp
        ../src/txThread.cpp
        session.cpp session.hpp)
target_link_libraries(bladeRF_evo LINK_PUBLIC ${LIBS} Qt5::Core Qt5::Widgets ${Boost_LIBRARIES})
message(STATUS "FFTW3 LIBRARIES: " ${FFTW3_LIBRARIES})
message(STATUS "FFTW3 INCLUDE DIRS: " ${FFTW3_INCLUDE_DIR})
message(STATUS "-----------------------------------------------")
message(STATUS ${BLANK_BOX_LINE})
message(STATUS "|          Install Prefix:" ${CMAKE_INSTALL_PREFIX} "          |")
message(STATUS "|          Build Type: " ${CMAKE_BUILD_TYPE} "                  |")
message(STATUS "|          C : " ${CMAKE_C_COMPILER_VERSION} "                |")
message(STATUS "|          C++ : " ${CMAKE_CXX_COMPILER_VERSION} "              |")
message(STATUS "|          Boost Ver. : ${Boost_VERSION}                |")
message(STATUS ${BLANK_BOX_LINE})
message(STATUS ${BLANK_BOX_LINE})
message(STATUS "|                 ALL GOOD!                   |")
message(STATUS "-----------------------------------------------")

