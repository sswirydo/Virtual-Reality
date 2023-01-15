
LIST(APPEND PATATES Skybox.cpp Camera.cpp Shader.cpp Physics.cpp Window.cpp Object.cpp Vertices.cpp Mesh.cpp LightSource.cpp Car.cpp Model.cpp Mesh.cpp PlayerCamera.cpp WorldCamera.cpp Player.cpp Road.cpp Sun.cpp StreetLamp.cpp InstancedObject.cpp Sound.cpp CarRenderer.cpp Font.cpp Text.cpp)
add_executable(${PROJECT_NAME} main.cpp ${PATATES})

target_link_libraries(${PROJECT_NAME} PUBLIC
									stb_image 
									OpenGL::GL
									glfw 
									glad 
									glm 
									assimp 
									${BULLET_LIBS}
									${OTHER_LIBS}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/libs/irrKlang/build"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)