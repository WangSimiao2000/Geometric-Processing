#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

// University of leeds 2023-2024
// COMP 5812M Assignment 1
// Simiao Wang
// 201702881

// 时间复杂度：
// 1. for 循环迭代次数与 faces 数组的大小成正比，因此主要循环的时间复杂度是 O(faces.size())
// 2. 在主循环中，还有一个内部循环用于查找已存在的边，内部循环的时间复杂度也是 O(edges.size())
// 3. 总体时间复杂度是 O(faces.size() * edges.size())
// 空间复杂度：
// 1. vertices、faces 和 edges 数组用于存储顶点、面和边的数据
// 2. 因此，空间复杂度分别为 O(vertices.size())、O(faces.size()) 和 O(edges.size())

// Time Complexity:
// 1. The number of iterations in the main loop is proportional to the size of the 'faces' array, making the time complexity of the main loop O(faces.size())
// 2. Within the main loop, there is an internal loop for searching existing edges, and the time complexity of this internal loop is also O(edges.size())
// 3. The overall time complexity is O(faces.size() * edges.size())
// Space Complexity:
// 1. The arrays 'vertices,' 'faces,' and 'edges' are used to store data for vertices, faces, and edges
// 2. Therefore, the space complexity is O(vertices.size()), O(faces.size()), and O(edges.size()), respectively

struct Vertex {
  int id;
  float x, y, z;
};

struct Face {
  int id, v1, v2, v3;
};

struct Edge {
  int edgeNum;
  int vStart;
  int vEnd;
};

bool edgeExists(const Edge& newEdge, const std::vector<Edge>& edges){
  for(const Edge& existingEdge : edges){
    if(newEdge.vStart == existingEdge.vStart && newEdge.vEnd == existingEdge.vEnd){
      return true;
    }
  }
  return false;
}

int main() {  
  std::string folderPath("faces");
  std::string inputFileExtension = ".face";

  std::filesystem::create_directory("direct_edges");

  for (const auto& currentFile : std::filesystem::directory_iterator(folderPath)){
    if(currentFile.path().extension() == inputFileExtension){
      // 当前的进展情况
      // current progress
      std::cout << "Processing: " << currentFile.path() << std::endl;

      // 读取输入文件
      // read input file
      std::ifstream inputFile(currentFile.path());

      // 创建输出文件
      // creat output file
      std::string outputFolderPath = "direct_edges/";
      std::string outputFileName = outputFolderPath + currentFile.path().stem().string() + ".diredge";
      std::ofstream outputFile(outputFileName);

      std::vector<Vertex> vertices;
      std::vector<Face> faces;
      std::vector<Edge> edges;

      std::string line;
      while (std::getline(inputFile, line)) {
        // 检查是否以井号开头
        // Check if it starts with a hashtag
        if (!line.empty() && line[0] == '#') {
            continue;
        }

        std::istringstream temp(line);

        std::string token;
        temp >> token;// 每一行开头的单词 The first word at the beginning of each line

        if (token == "Vertex") {
            Vertex vertex;
            temp >> vertex.id >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (token == "Face") {
            Face face;
            temp >> face.id >> face.v1 >> face.v2 >> face.v3;
            faces.push_back(face);
        }
      }

      // 读取并存储边
      // Read and store edges
      int edgeNum = 0;
      for (size_t i = 0; i < faces.size(); ++i)
      {
        Edge edge1, edge2, edge3;

        edge1.vStart = faces[i].v1;
        edge1.vEnd = faces[i].v2;
        if (!edgeExists(edge1, edges))
        {
          edge1.edgeNum = edgeNum;
          edges.push_back(edge1);
          edgeNum++;
        }        

        edge2.vStart = faces[i].v2;
        edge2.vEnd = faces[i].v3;
        if (!edgeExists(edge2, edges))
        {
          edge2.edgeNum = edgeNum;
          edges.push_back(edge2);
          edgeNum++;
        }

        edge3.vStart = faces[i].v3;
        edge3.vEnd = faces[i].v1;
        if (!edgeExists(edge3, edges))
        {
          edge3.edgeNum = edgeNum;
          edges.push_back(edge3);
          edgeNum++;
        }
      }
      
      outputFile << "# University of leeds 2023-2024" <<std::endl;
      outputFile << "# COMP 5812M Assignment 1" <<std::endl;
      outputFile << "# Simiao Wang" <<std::endl;
      outputFile << "# 201702881" <<std::endl;
      outputFile << "#" <<std::endl;
      outputFile << "# Object Name:" << currentFile.path().stem().string() <<std::endl;
      outputFile << "# Vertices=" << vertices.size() << " Faces=" << faces.size() <<std::endl;
      outputFile << "#" <<std::endl;

      // 遍历 vertices 并将数据按格式写入文件
      // Iterate through vertices and write the data to a file in the specified format
      for (const Vertex& vertex : vertices) {
        outputFile << "Vertex " << vertex.id << " " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
      }

      // 将列出顶点的ID，以及每个顶点的第一个有向边的ID
      // List the IDs of vertices and the IDs of their first directed edges
      for (const Vertex& vertex : vertices) {
        for(const Edge& existingEdge : edges){
          if(existingEdge.vStart == vertex.id){
            outputFile << "FirstDirectedEdge " << vertex.id << " " << existingEdge.edgeNum << std::endl;;
            break;
          }
        }
      }

      // 每个面都会以编号及其面索引的形式列在单独的一行上，按逆时针的顺序列出
      // Each face will be listed on a separate line with its number and face index, listed counterclockwise.
      for (const Face& face : faces){
        outputFile << "Face " << face.id << " " << face.v1 << " " << face.v2 << " " << face.v3 << std::endl;
      }
      
      // 每个有向边都会以编号的形式列在单独的一行上，同时列出与之相对方向的另一有向边的编号
      // Each directed edge will be listed on a separate line with its number, along with the number of the directed edge in the opposite direction
      for(const Edge& existingEdge1 : edges){
        for(const Edge& existingEdge2 : edges){
          if(existingEdge1.vStart == existingEdge2.vEnd && existingEdge1.vEnd == existingEdge2.vStart){
            outputFile << "OtherHalf " << existingEdge1.edgeNum << " " << existingEdge2.edgeNum << std::endl;;
            break;
          }
        }
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