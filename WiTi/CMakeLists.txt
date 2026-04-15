add_executable (WiTi "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET WiTi PROPERTY CXX_STANDARD 20)
endif()
