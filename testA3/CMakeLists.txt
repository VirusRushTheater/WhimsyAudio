set(RELEVANT_FOLDERS . gui portaudio_engine core)

set(TESTA3_SRC)

foreach(FOLDER IN ITEMS ${RELEVANT_FOLDERS})
    file(GLOB TESTA3_SRC_LOOP
        "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}/*.h"
    )
    list(APPEND TESTA3_SRC ${TESTA3_SRC_LOOP})
endforeach(FOLDER)
 
add_executable(testA3 ${TESTA3_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(testA3 Qt5::Widgets ${PORTAUDIO_LIBRARIES})
