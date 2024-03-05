add_executable(hw01 main.cpp)
target_link_libraries(hw01 ${OpenCV_LIBS})

add_custom_target(copy_asset
    COMMENT "Copying assets in HW01"
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:hw01>
    COMMAND ${CMAKE_COMMAND} -E copy Lenna.jpg $<TARGET_FILE_DIR:hw01>
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(hw01 copy_asset)