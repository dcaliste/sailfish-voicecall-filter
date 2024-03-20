set(SRC
  filterservice_adaptor.cpp
  filterservice_proxy.cpp
  filterlist.cpp
  ignoredlist.cpp
  blockedlist.cpp
  filter.cpp)

set(HEADERS
  filteraction.h
  filterservice_adaptor.h
  filterservice_proxy.h
  filterlist.h
  ignoredlist.h
  blockedlist.h
  filter.h)

add_custom_command(OUTPUT filterservice_adaptor.h filterservice_adaptor.cpp
  COMMAND qdbusxml2cpp -a filterservice_adaptor -i filteraction.h filterservice.xml
  DEPENDS filterservice.xml)

add_custom_command(OUTPUT filterservice_proxy.h filterservice_proxy.cpp
  COMMAND qdbusxml2cpp -p filterservice_proxy -i filteraction.h filterservice.xml
  DEPENDS filterservice.xml)

add_library(voicecallfilter SHARED ${SRC} ${HEADERS})

set_target_properties(voicecallfilter PROPERTIES
  SOVERSION ${PROJECT_VERSION_MAJOR}
  VERSION ${PROJECT_VERSION})
set_property(TARGET voicecallfilter
  PROPERTY PUBLIC_HEADER ${HEADERS})

target_link_libraries(voicecallfilter
  PRIVATE
  Qt5::Core
  Qt5::DBus
  PkgConfig::MLITE
  )

install(TARGETS voicecallfilter
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/voicecallfilter")

pkg_get_variable(GIO_SCHEMASDIR gio-2.0 schemasdir)
install(FILES sailfish.voicecall.filter.gschema.xml
	DESTINATION ${GIO_SCHEMASDIR})
