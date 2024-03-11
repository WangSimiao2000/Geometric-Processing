#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cmath>

// University of leeds 2023-2024
// COMP 5812M Assignment 1
// Simiao Wang
// 201702881

// 时间复杂度：
// 主要的时间复杂度在 isManifold 函数中。时间复杂度取决于顶点数量和面数量，因此是 O(V + F)
// 函数在 edges 数组上执行循环，检查每个边是否满足流形条件。这部分的时间复杂度也取决于边的数量和面的数量，因此是 O(E + F)
// 函数循环遍历所有顶点，检查是否满足流形条件，时间复杂度是 O(V)
// 在主循环中，有两个嵌套的 for 循环用于读取面和边，它们的时间复杂度也是 O(F) 和 O(E)
// 总体时间复杂度为 O(F * E + V)
// 空间复杂度：
// 主要由 vertexToFaceMap、vertices、faces 和 edges 数组影响，空间复杂度是 O(V + F + E)

// Time Complexity:
// The main time complexity lies in the isManifold function. The time complexity depends on the number of vertices and faces, making it O(V + F)
// The function iterates over the edges array, checking if each edge satisfies the manifold condition. This part's time complexity also depends on the number of edges and faces, resulting in O(E + F)
// The function iterates over all vertices, checking if they satisfy the manifold condition, with a time complexity of O(V)
// In the main loop, there are two nested for loops for reading faces and edges, and their time complexities are O(F) and O(E), respectively
// The overall time complexity is O(F * E + V)
// Space Complexity:
// Mainly influenced by the vertexToFaceMap, vertices, faces, and edges arrays, the space complexity is O(V + F + E)

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

// 判断是否为流形
// Determine if it is a manifold
bool isManifold(const std::vector<Vertex>& vertices, const std::vector<Face>& faces, const std::vector<Edge>& edges){
    // 创建一个数据结构，用于记录每个顶点相邻的面
    // Create a data structure to record the faces adjacent to each vertex
    std::unordered_map<int, std::vector<int>> vertexToFaceMap;

    // 遍历每个面，将面与顶点之间的关系记录到 vertexToFaceMap 中
    // terate through each face and record the relationship between faces and vertices in the vertexToFaceMap
    for (const Face& face : faces) {
        vertexToFaceMap[face.v1].push_back(face.id);
        vertexToFaceMap[face.v2].push_back(face.id);
        vertexToFaceMap[face.v3].push_back(face.id);
    }

    // 检查每个边是否被一个或两个三角形使用
    // Check if each edge is used by one or two triangles
    for (const Edge& edge : edges) {
        if (vertexToFaceMap.find(edge.vStart) == vertexToFaceMap.end() || vertexToFaceMap.find(edge.vEnd) == vertexToFaceMap.end()) {
            std::cout << "Edge " << edge.edgeNum << " does not meet the requirements " << std::endl;
            return false; // 顶点不被任何面共享，边不符合要求 Vertices are not shared by any faces, and the edges do not meet the requirements
        }
        int sharedFaceCount = 0;
        for (int faceId : vertexToFaceMap[edge.vStart]) {
            if (std::find(vertexToFaceMap[edge.vEnd].begin(), vertexToFaceMap[edge.vEnd].end(), faceId) != vertexToFaceMap[edge.vEnd].end()) {
                sharedFaceCount++;
            }
        }
        if (sharedFaceCount < 1 || sharedFaceCount > 2) {
            std::cout << "Edge " << edge.edgeNum << " does not meet the requirements " << std::endl;
            return false; // 边不被一个或两个三角形使用，不符合要求 Edges are not used by one or two triangles, do not meet the requirements.
        }
    }

    // 检查每个顶点是否被一组相连的三角形共享
    // Check if each vertex is shared by a connected group of triangles
    for (const Vertex& vertex : vertices) {
        if (vertexToFaceMap.find(vertex.id) == vertexToFaceMap.end() || vertexToFaceMap[vertex.id].empty()) {
            std::cout << "Vertex " << vertex.id << " does not meet the requirements " << std::endl;
            return false; // 顶点没有相邻的面，不符合要求 Vertices have no adjacent faces, do not meet the requirements.
        }
    }

    return true; // 满足条件
}

int main() {
    std::string folderPath("faces");
    std::string inputFileExtension = ".face";

    // 创建输出文件
    //creat output file
    std::string outputFileName = "isManifold.txt";
    std::ofstream outputFile(outputFileName);

    outputFile << "# University of leeds 2023-2024" <<std::endl;
    outputFile << "# COMP 5812M Assignment 1" <<std::endl;
    outputFile << "# Simiao Wang" <<std::endl;
    outputFile << "# 201702881" <<std::endl;
    outputFile << "#" <<std::endl;

    // 遍历文件夹内容
    // Iterate through the contents of the folder.
    for (const auto& currentFile : std::filesystem::directory_iterator(folderPath)){
        if(currentFile.path().extension() == inputFileExtension){
            // 当前的进展情况
            // current progress
            std::cout << "Processing: " << currentFile.path() << std::endl;

            // 读取输入文件
            // read input file
            std::ifstream inputFile(currentFile.path());            

            std::vector<Vertex> vertices;
            std::vector<Face> faces;
            std::vector<Edge> edges;

            // 读取并存储面和顶点
            // Read and store faces and vertices.
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
            for (size_t i = 0; i < faces.size(); ++i){
                Edge edge1, edge2, edge3;

                edge1.vStart = faces[i].v1;
                edge1.vEnd = faces[i].v2;
                if (!edgeExists(edge1, edges)) {
                    edge1.edgeNum = edgeNum;
                    edges.push_back(edge1);
                    edgeNum++;
                }
                edge2.vStart = faces[i].v2;
                edge2.vEnd = faces[i].v3;
                if (!edgeExists(edge2, edges)){
                    edge2.edgeNum = edgeNum;
                    edges.push_back(edge2);
                    edgeNum++;
                }
                edge3.vStart = faces[i].v3;
                edge3.vEnd = faces[i].v1;
                if (!edgeExists(edge3, edges)){
                    edge3.edgeNum = edgeNum;
                    edges.push_back(edge3);
                    edgeNum++;
                }
            }
            inputFile.close();

            //输出
            if(isManifold(vertices, faces, edges)){          
                // 欧拉公式为：V - E + F = 2(1 - g)，其中V表示顶点数，E表示边数，F表示面数，g表示亏格
                // 立方体：顶点数 8 边数 18 面数 12
                int V, F, E, g;
                V = vertices.size();
                F = faces.size();
                E = edges.size()/2;
                std::cout << " V " << V << " F " << F << " E " << E << std::endl;
                g = 1- ((V-E+F)/2);
                if (g >= 0)
                {
                    outputFile << "Object " << currentFile.path().stem().string() << " is Manifold, the  genus is " << g << "." <<std::endl;
                }else{
                    outputFile << "Object " << currentFile.path().stem().string() << " is Manifold, the  genus is " << g << ", there are discontinuous models within this file." <<std::endl;
                }           
            }else{
                outputFile << "Object " << currentFile.path().stem().string() << " is not Manifold" <<std::endl;
            }            
        }
        if(currentFile.path().extension() != inputFileExtension){
            std::cout << "File: '" << currentFile.path() << "' has a suffix other than '" << inputFileExtension << "', so skipping." << std::endl;
        }
    }    
    outputFile.close();
    return 0;
}