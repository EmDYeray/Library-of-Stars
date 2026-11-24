cmake_minimum_required(VERSION 3.24)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    message(FATAL_ERROR "This target needs an Apple iOS SDK. Use Makefile on Linux.")
endif()
project(StarryLibrary VERSION 1.1.0 LANGUAGES C CXX OBJC OBJCXX)

include(FetchContent)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_OPENGLES ON CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG f00dc919ca35b9756316e86c4777392de442f9b2)
FetchContent_MakeAvailable(SDL3)

set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH FALSE)
set(PKG_CONFIG_ARGN --static)
find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED IMPORTED_TARGET cairo freetype2)

file(GLOB BOOK_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER BOOK_SOURCES EXCLUDE REGEX "/(main|desktop|checks)\\.cpp$")
add_executable(StarryLibrary MACOSX_BUNDLE ios/main.cpp ${BOOK_SOURCES})
target_include_directories(StarryLibrary PRIVATE src)
target_compile_features(StarryLibrary PRIVATE cxx_std_20)
target_compile_definitions(StarryLibrary PRIVATE BIBLIOTECA_IPAD=1)
target_link_directories(StarryLibrary PRIVATE ${CAIRO_LIBRARY_DIRS})
target_link_libraries(StarryLibrary PRIVATE SDL3::SDL3-static PkgConfig::CAIRO)

set(IPAD_BUNDLE_ID "org.cuentosentreestrellas.biblioteca" CACHE STRING "Bundle identifier")
set_target_properties(StarryLibrary PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist.in"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${IPAD_BUNDLE_ID}"
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
    XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
