# SA SFML Chart

## Overview
SFML Chart is an library to provide WPF chart control, consists of 2D and 3D scatter charts. 
It uses [SFML](https://www.sfml-dev.org/)(underlying it uses OpenGL) to render chart elements.

## Build
To build C++/CLI project, make sure you have cmake > 3.14.0 and Visual Studio 2017 have been installed
``` shell 
cd 'to source dir'  
mkdir build  
cd build  
cmake .. -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF 
start SFML.sln 

```

## Benchmark

To run benchmark, you need to turn on SFML_BUILD_BENCHMARK option for cmake

```shell
cd 'to source dir'  
mkdir build  
cd build  
cmake .. -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DSFML_BUILD_BENCHMARK=ON  
start SFML.sln 
```
After open SFML solution, select ChartBenchmark project, and open Package Manager Console by using the **Tools** > **NuGet Package Manager** > **Package Manager Console** command. Then run following command
```shell
Install-Package BenchmarkDotNet
```