add_executable(hw05 main.cpp)
target_link_libraries(hw05 ${OpenCV_LIBS})

add_custom_target(copy_asset05
    COMMENT "Copy assets in HW05"
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:hw05>
    COMMAND ${CMAKE_COMMAND} -E copy_directory target/ $<TARGET_FILE_DIR:hw05>/target/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_dependencies(hw05 copy_asset05)
