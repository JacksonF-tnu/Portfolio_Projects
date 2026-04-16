# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\sqlbrowser_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\sqlbrowser_autogen.dir\\ParseCache.txt"
  "sqlbrowser_autogen"
  )
endif()
