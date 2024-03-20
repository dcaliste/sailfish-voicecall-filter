set(SRC
  plugin.c
  filterservice.c)

add_library(voicecallfilterplugin SHARED ${SRC})

add_definitions(-DVERSION=\"${PROJECT_VERSION}\")

target_compile_options(voicecallfilterplugin PRIVATE ${DBUS_CFLAGS})
target_include_directories(voicecallfilterplugin PRIVATE ${PROJECT_SOURCE_DIR}/lib)

set_target_properties(voicecallfilterplugin PROPERTIES PREFIX "")
target_link_libraries(voicecallfilterplugin
  PRIVATE
  PkgConfig::GLIB
  )

pkg_get_variable(OFONO_PLUGINDIR ofono plugindir)
install(TARGETS voicecallfilterplugin
	DESTINATION ${OFONO_PLUGINDIR})

install(FILES org.sailfishos.voicecallfilter.conf
	DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/dbus-1/system.d)
