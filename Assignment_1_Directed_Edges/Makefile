# 编译器和选项
CXX = g++
CXXFLAGS = -std=c++17 -lstdc++fs

# 目标文件和对应的源文件
TARGETS = face2faceindex faceindex2directededge MeshManifoldCheck
SRCS = face2faceindex.cpp faceindex2directededge.cpp MeshManifoldCheck.cpp

# 生成的目标文件和文件夹
OBJS = $(TARGETS:%=%.o)
GENERATED_DIRS = faces direct_edges
GENERATED_FILES = isManifold.txt

# 默认目标，构建所有可执行文件
all: $(TARGETS)

# 构建每个可执行文件
$(TARGETS): % : %.o
	$(CXX) $(CXXFLAGS) -o $@ $< -lstdc++fs

# 编译每个源文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清除生成的文件和文件夹
clean:
	rm -f $(TARGETS) $(OBJS)
	rm -rf $(GENERATED_DIRS) $(GENERATED_FILES)

.PHONY: all clean
