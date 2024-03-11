#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

// University of leeds 2023-2024
// COMP 5812M Assignment 1
// Simiao Wang
// 201702881

// 时间复杂度：
// 1. for 循环迭代次数与输入文件中的顶点数量 n 成正比，因此主要循环的时间复杂度是 O(n)。
// 2. 输出循环的时间复杂度也与顶点数量和面的数量成正比，因此它们的时间复杂度分别为 O(vertices.size()) 和 O(faces.size())。
// 空间复杂度:
// 1. vertices 数组存储了输入文件中的所有唯一顶点，空间复杂度为 O(n)
// 2. faces 数组存储了处理后的面，空间复杂度 为O(n)

// Time Complexity:
// 1. The main loop's iteration count is proportional to the number of vertices 'n' in the input file, so the time complexity of the main loop is O(n)
// 2. The time complexity of the output loops is also proportional to the number of vertices and faces, resulting in time complexities of O(vertices.size()) and O(faces.size()), respectively
// Space Complexity:
// 1. The 'vertices' array stores all the unique vertices from the input file, leading to a space complexity of O(n) since it's proportional to the number of vertices
// 2. The 'faces' array stores the processed faces, and its space complexity is O(n) because it's proportional to the number of faces

struct Vertex {
  float x, y, z;
};

struct Face {
  int v1, v2, v3;
};

bool vertexExists(const Vertex& newVertex, const std::vector<Vertex>& vertices){
  for(const Vertex& existingVertex : vertices){
    if(newVertex.x == existingVertex.x && newVertex.y == existingVertex.y && newVertex.z == existingVertex.z){
      return true;
    }
  }
  return false;
}

int main(){
  std::string folderPath("handout_models");
  std::string inputFileExtension = ".tri";

  std::filesystem::create_directory("faces");

  for(const auto& currentFile : std::filesystem::directory_iterator(folderPath)){
    if(currentFile.path().extension() == inputFileExtension){
      // 当前的进展情况
      // current progress
      std::cout << "Processing: " << currentFile.path() << std::endl;

      // 读取输入文件
      // read input file
      std::ifstream inputFile(currentFile.path());

      // 创建输出文件
      // creat output file
      std::string outputFolderPath = "faces/";
      std::string outputFileName = outputFolderPath + currentFile.path().stem().string() + ".face";
      std::ofstream outputFile(outputFileName);

      // 处理数据并写入输出文件
      // process data and write to output file
      int n;
      inputFile >> n;

      std::vector<Vertex> vertices;
      std::vector<Face> faces;

      for(int i = 0; i < n; ++i){
	      Vertex v1, v2, v3;
	      inputFile >> v1.x >> v1.y >> v1.z;
	      inputFile >> v2.x >> v2.y >> v2.z;
	      inputFile >> v3.x >> v3.y >> v3.z;

        // 将顶点添加到顶点列表
      	// add vertex to vertives
	      if(!vertexExists(v1, vertices)){
	        vertices.push_back(v1);
	      }
	      if(!vertexExists(v2, vertices)){
	        vertices.push_back(v2);
	      }
      	if(!vertexExists(v3, vertices)){
	        vertices.push_back(v3);
	      }

        // 计算当前面的各个顶点的序号
        // Calculate the indices of the vertices for the current face
      	int v1Index, v2Index, v3Index;
        for(int i = 0; i <vertices.size(); ++i){
          if(vertices[i].x == v1.x && vertices[i].y == v1.y && vertices[i].z == v1.z) {
            v1Index = i;
            continue;
          }
          if(vertices[i].x == v2.x && vertices[i].y == v2.y && vertices[i].z == v2.z) {
            v2Index = i;
            continue;
          }
          if(vertices[i].x == v3.x && vertices[i].y == v3.y && vertices[i].z == v3.z) {
            v3Index = i;
            continue;
          }
        }

        Face face = {v1Index, v2Index, v3Index};
        faces.push_back(face);
      }
      
      outputFile << "# University of leeds 2023-2024" <<std::endl;
      outputFile << "# COMP 5812M Assignment 1" <<std::endl;
      outputFile << "# Simiao Wang" <<std::endl;
      outputFile << "# 201702881" <<std::endl;
      outputFile << "#" <<std::endl;
      outputFile << "# Object Name:" << currentFile.path().stem().string() <<std::endl;
      outputFile << "# Vertices=" << vertices.size() << " Faces=" << faces.size() <<std::endl;
      outputFile << "#" <<std::endl;
      
      for(size_t i = 0; i < vertices.size(); ++i){
	      const Vertex& vertex = vertices[i];
	      outputFile << "Vertex " << i << " " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
      }
  
      for(size_t i = 0; i < faces.size(); ++i){
	      const Face& face = faces[i];
	      outputFile << "Face " << i << " " << face.v1 << " " << face.v2 << " " << face.v3 << std::endl;
      }  
      inputFile.close();
      outputFile.close();
    }
    if(currentFile.path().extension() != inputFileExtension){
      std::cout << "File: '" << currentFile.path() << "' has a suffix other than '" << inputFileExtension << "', so skipping." << std::endl;
    }
  }
  return 0;
}
