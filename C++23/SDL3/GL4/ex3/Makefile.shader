all:
	glslc --target-env=opengl -fshader-stage=vert shaders/vertex.vert -o shaders/vertex.spv
	glslc --target-env=opengl -fshader-stage=frag shaders/frac.frag -o shaders/frac.spv	
