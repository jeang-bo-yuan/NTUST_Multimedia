add_executable(hw02 main.cpp)
target_link_libraries(hw02 ${OpenCV_LIBS})

add_custom_target(copy_asset02
    COMMENT "Copying assets in HW02"
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:hw02>
    COMMAND ${CMAKE_COMMAND} -E copy Lenna.jpg lenna_noise.jpg $<TARGET_FILE_DIR:hw02>/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(hw02 copy_asset02)