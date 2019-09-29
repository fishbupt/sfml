enable_language(CSharp)
set(CMAKE_CSharp_FLAGS "/platform:x64 /langversion:latest")
set(DotNetVersion "4.6.2")
include(CSharpUtilities)

set(INCROOT ./include)
set(SRCROOT ./src)

# all source files
set(SRC
    ${SRCROOT}/AssemblyInfo.cpp
    ${SRCROOT}/Annotation.cpp
    ${SRCROOT}/PointsShape.cpp
    ${SRCROOT}/ColorfulPointsShape.cpp
    ${SRCROOT}/GridShape.cpp
    ${SRCROOT}/SegmentPointsShape.cpp
    ${SRCROOT}/SegmentMinMaxShape.cpp
    ${SRCROOT}/SymbolShape.cpp
    ${SRCROOT}/ScatterChart.cpp
    ${SRCROOT}/ColorfulScatterChart.cpp
    ${SRCROOT}/SegmentChart.cpp
    ${SRCROOT}/SegmentMinMaxChart.cpp
    ${SRCROOT}/SymbolChart.cpp
    ${SRCROOT}/Marker.cpp
    ${SRCROOT}/Markers.cpp
    ${SRCROOT}/VertexArray.cpp
    ${INCROOT}/Marker.hpp
    ${INCROOT}/Markers.hpp
    ${INCROOT}/Annotation.hpp
    ${INCROOT}/PointsShape.hpp
    ${INCROOT}/ColorfulPointsShape.hpp
    ${INCROOT}/GridShape.hpp
    ${INCROOT}/SegmentPointsShape.hpp
    ${INCROOT}/SegmentMinMaxShape.hpp
    ${INCROOT}/SymbolShape.hpp
    ${INCROOT}/ScatterChart.hpp
    ${INCROOT}/ColorfulScatterChart.hpp
    ${INCROOT}/SegmentChart.hpp
    ${INCROOT}/SegmentMinMaxChart.hpp
    ${INCROOT}/SymbolChart.hpp
    ${INCROOT}/Drawable.hpp
    ${INCROOT}/ScopedPtr.hpp
    ${INCROOT}/Utils.hpp
    ${INCROOT}/ColorArray.hpp
    ${INCROOT}/OrbitCamera.hpp
    ${INCROOT}/VertexArray.hpp
    ${INCROOT}/Color.hpp
    ${INCROOT}/PrimitiveType.hpp
    ${INCROOT}/Vector3.hpp
    ${INCROOT}/Vertex.hpp
)
source_group("" FILES ${SRC})


# define the sfml-graphics target
set(CHART_LIB sfml-charts)
add_library(${CHART_LIB} SHARED ${SRC} )

#set properties
set(SIGN_KEY ${CMAKE_SOURCE_DIR}/chart/Agilent.SAD.snk)
set(SIGN_OPTION "/KEYFILE:${SIGN_KEY}")
target_compile_features(${CHART_LIB} PUBLIC cxx_std_17) 
target_compile_options(${CHART_LIB} PRIVATE /W4 /WX)

target_link_options(${CHART_LIB} PRIVATE ${SIGN_OPTION} "$<$<CONFIG:Debug>:/ASSEMBLYDEBUG>")

set_target_properties(${CHART_LIB} PROPERTIES
                      COMMON_LANGUAGE_RUNTIME ""
                      DOTNET_TARGET_FRAMEWORK_VERSION ${DotNetVersion}
                      VS_DOTNET_REFERENCES "System" "WindowsBase" "PresentationCore" "PresentationFramework" "System.Drawing")

target_include_directories(${CHART_LIB} PRIVATE include)
# setup dependencies
target_link_libraries(${CHART_LIB} PUBLIC sfml-graphics opengl32)

add_subdirectory(example)
if(SFML_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()
