set(LIB "PortAudioSystemPlugin")

include_directories(include/)

FILE(GLOB headers include/*h include/*/*h)

FILE(GLOB sources src/*.cpp src/*/*.cpp)

include_directories(${PORTAUDIO2_INCLUDE_DIRS})

add_library(${LIB} SHARED ${headers} ${sources})

set_target_properties(${LIB} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${LIB} ${PORTAUDIO_LIBRARIES})

