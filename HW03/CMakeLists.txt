add_executable(hw03 main.cpp)
target_link_libraries(hw03 ${OpenCV_LIBS})

add_custom_target(copy_asset03
    COMMENT "Copying assets in HW03"
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:hw03>
    COMMAND ${CMAKE_COMMAND} -E copy_directory target $<TARGET_FILE_DIR:hw03>/target
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(hw03 copy_asset03)