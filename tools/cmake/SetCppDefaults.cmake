cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# in lieu of specifying -fPIC manually to g++
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# export all symbols so we can dispense with __declspec pain (mostly)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
