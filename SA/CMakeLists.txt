add_executable (SA "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SA PROPERTY CXX_STANDARD 20)
endif()
