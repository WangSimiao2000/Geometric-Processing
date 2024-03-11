#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

struct Vertex
{
    float x,y,z;
};

// 定义原本的顶点数据结构
// Define the original vertex data structure
struct OriginVertex{
    Vertex location;
    Vertex normal;
    int id;
    int n;//由此点为起点的有向边数量 The number of directed edges starting from this point
};

// 定义新增的顶点数据结构
//Define new vertex data structures
struct NewVertex{
    Vertex location;
    int vertexA, vertexB;
    int faceA, faceB;
    int id;
};

// 定义有向边数据结构
//Defining Directed Edge Data Structures
struct DirectedEdge {
    int startVertexID;
    Vertex startLoac;
    int endVertexID;
    Vertex endLoac;
    int id;
    int otherHalfID;
    int faceID;
};

// 面
//Face
struct Face {
    int vertexA, vertexB, vertexC;
    int id;
};

// 定义模型类
//Defining Model Classes
class Model {
public:
    std::vector<OriginVertex> oriVertices;    
    std::vector<DirectedEdge> directedEdges;
    std::vector<Face> faces;


    // 从文件加载模型数据
    //Load model data from a file
    void loadModel(const std::string& filename);

    // Loop细分
    //Loop segmentation
    void loopSubdivision();

    // 输出结果
    // Output results
    void outPutResult(const std::string& filename);

    // 将结果储存并赋值给原有的vector里面
    // Store and assign the results to the original vector
    void cleanAndStoreValue();

    //检查精度是否有问题
    //Check if there are any issues with the accuracy
    void checkResult();

    Vertex CenterPoint(Vertex A, Vertex B);//求中点
    Vertex SumPoint(Vertex A, Vertex B);   // +
    Vertex SubPoint(Vertex A, Vertex B);   // -
    Vertex DivPoint(Vertex A, float d);    // /
    Vertex MulPoint(Vertex A, float m);    // *
};

#endif // MODEL_H