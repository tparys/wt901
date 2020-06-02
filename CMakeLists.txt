set(CMAKE_C_FLAGS "-Os -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

add_library(
  wt901_common
  serial.cpp
  bindump.cpp
  WT901.cpp
  )

add_executable(readout readout.cpp)
target_link_libraries(readout wt901_common)

add_executable(poke poke.cpp)
target_link_libraries(poke wt901_common)
