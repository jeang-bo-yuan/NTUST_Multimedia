add_executable(hw04 main.cpp)
target_link_libraries(hw04 ${OpenCV_LIBS})

add_custom_target(copy_asset04
    COMMENT "Copying assets in HW04"
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:hw04>
    COMMAND ${CMAKE_COMMAND} -E copy image.jpg $<TARGET_FILE_DIR:hw04>
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(hw04 copy_asset04)