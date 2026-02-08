all:
	glslc --target-env=opengl -fshader-stage=vert shaders/static.vert -o shaders/static.vert.spv
	glslc --target-env=opengl -fshader-stage=frag shaders/static.frag -o shaders/static.frag.spv	
