add_executable(racing_game main.cpp)

target_link_libraries(racing_game PUBLIC	
									LightSource 
									Car 
									Mesh 
									Model 
									Game 
									Object 
									Shader 
									Texture 
									stb_image 
									OpenGL::GL
									glfw 
									glad 
									glm 
									assimp 
									${BULLET_LIBS}
)