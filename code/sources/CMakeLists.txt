
LIST(APPEND PATATES Skybox.cpp Camera.cpp Shader.cpp Physics.cpp Game.cpp Object.cpp Vertices.cpp Mesh.cpp LightSource.cpp Car.cpp Model.cpp Mesh.cpp PlayerCamera.cpp WorldCamera.cpp Player.cpp Road.cpp Sun.cpp StreetLamp.cpp)
add_executable(racing_game main.cpp ${PATATES})

target_link_libraries(racing_game PUBLIC
									stb_image 
									OpenGL::GL
									glfw 
									glad 
									glm 
									assimp 
									${BULLET_LIBS}
)