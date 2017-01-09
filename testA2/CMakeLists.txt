set(RELEVANT_FOLDERS . gui portaudio_engine core)

set(TESTA2_SRC)

foreach(FOLDER IN ITEMS ${RELEVANT_FOLDERS})
    file(GLOB TESTA2_SRC_LOOP
        "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}/*.h"
    )
    list(APPEND TESTA2_SRC ${TESTA2_SRC_LOOP})
endforeach(FOLDER)
 
add_executable(testA2 ${TESTA2_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(testA2 Qt5::Widgets ${PORTAUDIO_LIBRARIES})
