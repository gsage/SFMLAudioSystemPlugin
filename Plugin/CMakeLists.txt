set(LIB_NAME "SFMLAudioPlugin")

include_directories(include/)

FILE(GLOB headers include/*h include/*/*h)

FILE(GLOB sources src/*.cpp src/*/*.cpp)

include_directories(
  ${GSAGE_INCLUDE_DIRS}
  ${SFML_INCLUDE_DIR}
  ${LUAJIT_INCLUDE_DIR}
)

configure()
gsage_plugin(${LIB_NAME} ${sources})

target_link_libraries(${LIB_NAME} ${GSAGE_LIBRARIES} ${SFML_LIBRARIES} ${LUAJIT_LIBRARIES})

install(TARGETS ${LIB_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
install(TARGETS ${LIB_NAME} DESTINATION /usr/local/lib)
