set(RELEVANT_FOLDERS . gui portaudio_engine core)

set(TESTA1_SRC)

foreach(FOLDER IN ITEMS ${RELEVANT_FOLDERS})
    file(GLOB TESTA1_SRC_LOOP
        "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}/*.h"
    )
    list(APPEND TESTA1_SRC ${TESTA1_SRC_LOOP})
endforeach(FOLDER)
 
add_executable(testA1 ${TESTA1_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(testA1 Qt5::Widgets ${PORTAUDIO_LIBRARIES})
