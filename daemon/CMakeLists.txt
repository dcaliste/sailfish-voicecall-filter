set(SRC
  main.cpp
  filterhandler.cpp)

set(HEADERS
  filterhandler.h)

add_executable(voicecallfilterd ${SRC} ${HEADERS})

target_include_directories(voicecallfilterd PRIVATE ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(voicecallfilterd
  PRIVATE
  Qt5::Core
  Qt5::DBus
  voicecallfilter
  )

install(TARGETS voicecallfilterd
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES systemd/voicecallfilterd.service
	DESTINATION "lib/systemd/user/")
