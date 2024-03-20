set(SRC
  tst_filterlist.cpp)

add_executable(tst_filterlist ${SRC})

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH "${PROJECT_BINARY_DIR}/lib")

target_include_directories(tst_filterlist PRIVATE ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(tst_filterlist
  Qt5::Test
  voicecallfilter
  )

add_test(NAME tst_filterlist COMMAND tst_filterlist)
