# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.22.1)

# Declares and names the project.

project("mushaf")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source Files
set(HARFBUZZ_FILES
        ${HARFBUZZ_DIR}/src/hb-unicode.cc
        ${HARFBUZZ_DIR}/src/hb-aat-layout.cc
        ${HARFBUZZ_DIR}/src/hb-aat-map.cc
        ${HARFBUZZ_DIR}/src/hb-blob.cc
        ${HARFBUZZ_DIR}/src/hb-buffer.cc
        ${HARFBUZZ_DIR}/src/hb-common.cc
        ${HARFBUZZ_DIR}/src/hb-face.cc
        ${HARFBUZZ_DIR}/src/hb-fallback-shape.cc
        ${HARFBUZZ_DIR}/src/hb-font.cc
        ${HARFBUZZ_DIR}/src/hb-ft.cc
        ${HARFBUZZ_DIR}/src/hb-ot-cff1-table.cc
        ${HARFBUZZ_DIR}/src/hb-ot-cff2-table.cc
        ${HARFBUZZ_DIR}/src/hb-ot-face.cc
        ${HARFBUZZ_DIR}/src/hb-ot-font.cc
        ${HARFBUZZ_DIR}/src/hb-ot-layout.cc
        ${HARFBUZZ_DIR}/src/hb-ot-map.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-arabic.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-default.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-hangul.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-hebrew.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-indic-table.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-indic.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-khmer.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-myanmar.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-thai.cc
        #${HARFBUZZ_DIR}/src/hb-ot-shaper-use-table.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-use.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-syllabic.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shaper-vowel-constraints.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shape-fallback.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shape-normalize.cc
        ${HARFBUZZ_DIR}/src/hb-ot-shape.cc
        ${HARFBUZZ_DIR}/src/hb-ot-tag.cc
        ${HARFBUZZ_DIR}/src/hb-ot-var.cc
        ${HARFBUZZ_DIR}/src/hb-set.cc
        ${HARFBUZZ_DIR}/src/hb-shape-plan.cc
        ${HARFBUZZ_DIR}/src/hb-shape.cc
        ${HARFBUZZ_DIR}/src/hb-shaper.cc
        ${HARFBUZZ_DIR}/src/hb-unicode.cc
        ${HARFBUZZ_DIR}/src/hb-static.cc
        ${HARFBUZZ_DIR}/src/hb-ucd.cc
        ${HARFBUZZ_DIR}/src/hb-ot-metrics.cc
        ${HARFBUZZ_DIR}/src/hb-number.cc
        ${HARFBUZZ_DIR}/src/hb-ot-name.cc
        ${HARFBUZZ_DIR}/src/hb-ot-layout-jtst-context.cc
        ${HARFBUZZ_DIR}/src/hb-ot-color.cc
        ${HARFBUZZ_DIR}/src/hb-paint.cc
        ${HARFBUZZ_DIR}/src/hb-paint-extents.cc
        ${HARFBUZZ_DIR}/src/hb-draw.cc
        ${HARFBUZZ_DIR}/src/hb-outline.cc
        ${HARFBUZZ_DIR}/src/hb-buffer-verify.cc
        ${HARFBUZZ_DIR}/src/hb-buffer-serialize.cc
        )

set(VMF_FILES ${VMF_DIR}/src/qurantext/quran.cpp ${VMF_DIR}/src/qurantext/quran.h)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

#message(HARFBUZZ_FILES="${HARFBUZZ_FILES}")

add_library( # Sets the name of the library.
        mushaf

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${HARFBUZZ_FILES} ${VMF_FILES} text-rendering.cpp hb_skia_canvas.h hb_skia_canvas.cpp)


add_library(skia SHARED IMPORTED)

#set(skiaDir "${SKIA_DIR}/out/cmake/${ANDROID_ABI}")

set_target_properties(skia PROPERTIES IMPORTED_LOCATION "${SKIA_DIR}/out/${ANDROID_ABI}/libskia.so")

target_include_directories(skia INTERFACE ${SKIA_DIR} ${SKIA_DIR}/include/config ${SKIA_DIR}/include/core)

#add_subdirectory("${skiaDir}" "${skiaDir}/out" EXCLUDE_FROM_ALL)
#set(SKIA_LIBRARY skia)


#target_compile_options(mushaf PUBLIC -Wno-error=unused-result)
#target_compile_definitions(mushaf PUBLIC HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED)
#target_compile_definitions(mushaf PUBLIC HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING)
target_compile_definitions(mushaf PUBLIC HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR)


target_include_directories(mushaf PRIVATE ${HARFBUZZ_DIR}/src ${VMF_DIR}/src/qurantext)


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

find_library( # Sets the name of the path variable.
        android-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        android)

find_library( # Sets the name of the path variable.
        jnigraphics-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        jnigraphics)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        mushaf

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} ${android-lib} ${jnigraphics-lib} skia)