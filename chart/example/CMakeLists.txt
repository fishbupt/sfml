enable_language(CSharp)
set(CMAKE_CSharp_FLAGS "/platform:x64 /langversion:latest")
include(CSharpUtilities)

set(DESIGNER_FILES
    Properties/AssemblyInfo.cs
    Properties/Resources.Designer.cs
    Properties/Resources.resx 
    Properties/Settings.Designer.cs
    Properties/Settings.settings
)

set(XAML_FILES
    App.xaml
    App.xaml.cs
    MainWindow.xaml
    MainWindow.xaml.cs
)

set(SRC 
    App.config
    ScatterChartControl.cs
    FPS.cs
)

set(EX_TARGET "ChartExample")
add_executable(${EX_TARGET} ${SRC} ${DESIGNER_FILES} ${XAML_FILES})

target_compile_options(${EX_TARGET} PRIVATE "/unsafe")
# set app properties files as program entry point
set_source_files_properties(App.xaml PROPERTIES VS_XAML_TYPE "ApplicationDefinition")
# target properties
set_target_properties(${EX_TARGET} PROPERTIES 
                      DOTNET_TARGET_FRAMEWORK_VERSION "v4.5.2")
set_property(TARGET ${EX_TARGET} PROPERTY VS_DOTNET_PLATFORM "x64")
set_property(TARGET ${EX_TARGET} PROPERTY
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
target_link_libraries(${EX_TARGET} sfml-charts)