add_executable(racing_game main.cpp)

target_link_libraries(racing_game PUBLIC	
									${PROJECT_LIBS}
									stb_image 
									OpenGL::GL
									glfw 
									glad 
									glm 
									assimp 
									${BULLET_LIBS}
)