enable_language(CSharp)
set(CMAKE_CSharp_FLAGS "/platform:x64 /langversion:latest")
include(CSharpUtilities)

set(SRC 
    Program.cs
    UIInitializer.cs
)

set(BE_TARGET "ChartBenchmark")
add_executable(${BE_TARGET} ${SRC})
target_compile_options(${BE_TARGET} PRIVATE "/unsafe")

# target properties
find_program(NUGET nuget)
if(NOT NUGET)
    message(FATAL "CMake could not find the nuget command line tool. Please install it")
endif()

# Copy the nuget config file
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/packages.config ${CMAKE_CURRENT_BINARY_DIR}/packages.config COPYONLY)


#add_custom_command(TARGET ${BE_TARGET}
#    PRE_BUILD
#    COMMAND nuget restore ${CMAKE_CURRENT_BINARY_DIR}/packages.config -PackagesDirectory ./packages
#)

set_target_properties(${BE_TARGET} PROPERTIES 
                      DOTNET_TARGET_FRAMEWORK_VERSION ${DotNetVersion})
set_property(TARGET ${BE_TARGET} PROPERTY VS_DOTNET_PLATFORM "x64")
set_property(TARGET ${BE_TARGET} PROPERTY
                      VS_DOTNET_REFERENCES
                      "Microsoft.CSharp"
                      "PresentationCore"
                      "PresentationFramework"
                      "System"
                      "System.Core"
                      "System.Data"
                      "System.Data.DataSetExtensions"
                      "System.Net.Http"
                      "System.Xaml"
                      "System.Xml"
                      "System.Xml.Linq"
                      "WindowsBase")

target_link_libraries(${BE_TARGET} sfml-charts)
