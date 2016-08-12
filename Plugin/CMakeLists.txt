set(LIB_NAME "SFMLAudioPlugin")

include_directories(include/)

FILE(GLOB headers include/*h include/*/*h)

FILE(GLOB sources src/*.cpp src/*/*.cpp)

include_directories(
  ${GSAGE_INCLUDE_DIRS}
  ${SFML_INCLUDE_DIR}
  ${LUABIND_INCLUDE_DIRS}
)

add_library(${LIB_NAME} SHARED ${headers} ${sources})

set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${LIB_NAME} ${GSAGE_LIBRARIES} ${SFML_LIBRARIES} ${LUABIND_LIBRARIES})

install(TARGETS ${LIB_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
install(TARGETS ${LIB_NAME} DESTINATION /usr/local/lib)
