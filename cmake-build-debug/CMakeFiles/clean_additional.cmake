# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Sem3Lab4_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Sem3Lab4_autogen.dir\\ParseCache.txt"
  "Sem3Lab4_autogen"
  )
endif()
