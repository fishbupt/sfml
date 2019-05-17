enable_language(CSharp)
set(CMAKE_CSharp_FLAGS "/platform:x64 /langversion:latest")
include(CSharpUtilities)

set(INCROOT ./include)
set(SRCROOT ./src)

# all source files
set(SRC
    ${SRCROOT}/PointsShape.cpp
    ${SRCROOT}/GridShape.cpp
    ${SRCROOT}/SegmentPointsShape.cpp
    ${SRCROOT}/SegmentMinMaxShape.cpp
    ${SRCROOT}/ScatterChart.cpp
    ${SRCROOT}/SegmentChart.cpp
    ${SRCROOT}/SegmentMinMaxChart.cpp
    ${INCROOT}/PointsShape.hpp
    ${INCROOT}/GridShape.hpp
    ${INCROOT}/SegmentPointsShape.hpp
    ${INCROOT}/SegmentMinMaxShape.hpp
    ${INCROOT}/ScatterChart.hpp
    ${INCROOT}/SegmentChart.hpp
    ${INCROOT}/SegmentMinMaxChart.hpp
    ${INCROOT}/Drawable.hpp
    ${INCROOT}/ScopedPtr.hpp
    ${INCROOT}/Utils.hpp
    ${INCROOT}/ColorArray.hpp
    ${INCROOT}/OrbitCamera.hpp
)
source_group("" FILES ${SRC})


# define the sfml-graphics target
set(CHART_LIB sfml-charts)
add_library(${CHART_LIB} SHARED ${SRC} )

#set properties
set(SIGN_KEY ${CMAKE_SOURCE_DIR}/chart/Agilent.SAD.snk)
set(SIGN_OPTION "/KEYFILE:${SIGN_KEY}")
target_compile_features(${CHART_LIB} PUBLIC cxx_std_14) # vs2017 has a bug to build with c++17 is enabled
target_link_options(${CHART_LIB} PRIVATE ${SIGN_OPTION} "$<$<CONFIG:Debug>:/ASSEMBLYDEBUG>")

set_target_properties(${CHART_LIB} PROPERTIES
                      COMMON_LANGUAGE_RUNTIME ""
                      DOTNET_TARGET_FRAMEWORK_VERSION "v4.5.2"
                      VS_DOTNET_REFERENCES "System" "WindowsBase" "PresentationCore" "PresentationFramework" "System.Drawing")

target_include_directories(${CHART_LIB} PRIVATE include)
# setup dependencies
target_link_libraries(${CHART_LIB} PUBLIC sfml-graphics opengl32)

add_subdirectory(example)