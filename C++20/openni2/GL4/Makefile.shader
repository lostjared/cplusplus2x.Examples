all:
	glslc --target-env=opengl vertex.vert -o vert.spv
	glslc --target-env=opengl fragment.frag -o frag.spv
	glslc --target-env=opengl compute.comp -o compute.spv
	glslc --target-env=opengl trails.comp -o trails.spv
	glslc --target-env=opengl depth_demo.comp -o depth_demo.spv
	glslc --target-env=opengl aura.comp -o aura.spv
	glslc --target-env=opengl aura2.comp -o aura2.spv
	glslc --target-env=opengl lidar.comp -o lidar.spv
