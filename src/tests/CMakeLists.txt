set(SOURCE_FILES
    ../utils.cpp
    ../utils.h
    ../types.cpp
    ../types.h
    ../adventure.cpp
    ../adventure.h)

add_executable(bottomlessBagTest bottomlessBagTest.cpp ${SOURCE_FILES})
add_executable(sandArrangementTest sandArrangementTest.cpp ${SOURCE_FILES})
add_executable(crystalSelectionTest crystalSelectionTest.cpp ${SOURCE_FILES})

target_link_libraries( bottomlessBagTest pthread )

target_link_libraries( sandArrangementTest pthread )

target_link_libraries( crystalSelectionTest pthread )

