#include <iostream>
#include <set>
#include "Model.h"

//读取文件
void Model::loadModel(const std::string& filename){
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "Vertex") {
            // 顶点
            OriginVertex vertex;
            vertex.n = 0;
            iss >> vertex.id >> vertex.location.x >> vertex.location.y >> vertex.location.z;
            oriVertices.push_back(vertex);
        }else if (token == "Normal") {
            int VertexNum;
            iss >> VertexNum;
            iss >> oriVertices[VertexNum].normal.x >> oriVertices[VertexNum].normal.y >> oriVertices[VertexNum].normal.z;
        }else if (token == "Face") {
            //三角面
            Face face;
            iss >> face.id >> face.vertexA >> face.vertexB >> face.vertexC;
            faces.push_back(face);

            //有向边
            DirectedEdge dirEdge0,dirEdge1,dirEdge2;

            dirEdge0.startVertexID = face.vertexC;
            dirEdge0.endVertexID = face.vertexA;
            dirEdge0.faceID = face.id;
            directedEdges.push_back(dirEdge0);

            dirEdge1.startVertexID = face.vertexA;
            dirEdge1.endVertexID = face.vertexB;
            dirEdge1.faceID = face.id;
            directedEdges.push_back(dirEdge1);

            dirEdge2.startVertexID = face.vertexB;
            dirEdge2.endVertexID = face.vertexC;
            dirEdge2.faceID = face.id;
            directedEdges.push_back(dirEdge2);
        }else if (token == "OtherHalf")
        {
            int edgeID, otherHalfID;
            iss >> edgeID >> otherHalfID;
            directedEdges[edgeID].id = edgeID;
            directedEdges[edgeID].otherHalfID = otherHalfID;
        }        
    }
    file.close();
    std::cout<< "file read finished" <<std::endl;
}

// 实现loopSubdivision函数
// Implement the loopSubdivision function
void Model::loopSubdivision() {
    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;
    std::cout << "Directed edge endpoint assignment strat..." <<std::endl;
    // 边的端点赋值
    // Assigning endpoints of edges
    for (auto& edge : directedEdges){
        // 根据端点的id给它赋值
        // 遍历所有顶点, 如果该有向边的起点与顶点相同, 则将以该顶点为起点的有向边数量+1
        // Assign a value to the endpoint based on its ID
        // Traverse all vertices, if the starting point of the directed edge is the same as the vertex, then the number of directed edges starting from that vertex will be increased by 1
        for (auto& oriV : oriVertices)
        {
            if (edge.startVertexID == oriV.id)
            {
                edge.startLoac = oriV.location;
                oriV.n = oriV.n + 1;
            }else if(edge.endVertexID == oriV.id){
                edge.endLoac = oriV.location;
            }          
        }
    }
    std::cout<< "Directed edge endpoint assignment completed.\n" <<std::endl;

    std::cout<< "Calculate the midpoint based on each directed edge..." <<std::endl;
    std::vector<NewVertex> newVertices;
    // 对于每条有向边计算其中点, 并且在pushback的时候避免重复, 以此计算新增顶点
    //Calculate the midpoint for each directed edge and avoid repetition during pushback to calculate new vertices
    for (const auto& edge : directedEdges)
    {
        // 计算中点
        // Calculate midpoint
        NewVertex newV;
        newV.location = CenterPoint(oriVertices[edge.startVertexID].location,oriVertices[edge.endVertexID].location);
        
        // 其他信息
        newV.vertexA = edge.startVertexID;
        newV.vertexB = edge.endVertexID;
        newV.faceA = edge.faceID;
        newV.faceB = directedEdges[edge.otherHalfID].faceID;
        
        // 如果有相同的就不添加了
        // If there are identical ones, don't add them anymore
        bool isExist = false;
        for (const auto& n: newVertices)
        {
            if(n.location.x == newV.location.x && n.location.y == newV.location.y && n.location.z == newV.location.z ){
                isExist = true;
                break;
            }            
        }
        if (isExist == false)
        {
            newVertices.push_back(newV);
        }
    }
    std::cout<< "The midpoint calculation for each directed edge is completed.\n" << std::endl;

    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;

    std::cout<< "Update the positions of all newly added vertices..." << std::endl;
    // std::cout << "newVertices:" << newVertices.size() << std::endl;
    // std::cout << "oriVertices:" << oriVertices.size() << std::endl;
    // 新增的顶点进行变化
    // Change the newly added vertices
    for (auto& n : newVertices)
    {
        Vertex A,B,C,D;
        int A_id,B_id,C_id,D_id;
        A_id = oriVertices[n.vertexA].id;
        A = oriVertices[n.vertexA].location; // 所在边端点A

        B_id = oriVertices[n.vertexB].id;
        B = oriVertices[n.vertexB].location; // 所在边端点B

        if((A_id == faces[n.faceA].vertexA && B_id == faces[n.faceA].vertexB) || (A_id == faces[n.faceA].vertexB && B_id == faces[n.faceA].vertexA)){
            C_id = faces[n.faceA].vertexC;
        }else if((A_id == faces[n.faceA].vertexB && B_id == faces[n.faceA].vertexC) || (A_id == faces[n.faceA].vertexC && B_id == faces[n.faceA].vertexB)){
            C_id = faces[n.faceA].vertexA;
        }else if((A_id == faces[n.faceA].vertexC && B_id == faces[n.faceA].vertexA)||(A_id == faces[n.faceA].vertexA && B_id == faces[n.faceA].vertexC)){
            C_id = faces[n.faceA].vertexB;
        }
        C = oriVertices[C_id].location;

        if((A_id == faces[n.faceB].vertexA && B_id == faces[n.faceB].vertexB) || (A_id == faces[n.faceB].vertexB && B_id == faces[n.faceB].vertexA)){
            D_id = faces[n.faceB].vertexC;
        }else if((A_id == faces[n.faceB].vertexB && B_id == faces[n.faceB].vertexC) || (A_id == faces[n.faceB].vertexC && B_id == faces[n.faceB].vertexB)){
            D_id = faces[n.faceB].vertexA;
        }else if((A_id == faces[n.faceB].vertexC && B_id == faces[n.faceB].vertexA)||(A_id == faces[n.faceB].vertexA && B_id == faces[n.faceB].vertexC)){
            D_id = faces[n.faceB].vertexB;
        }
        D = oriVertices[D_id].location;

        n.location.x = SumPoint(MulPoint(SumPoint(A,B),0.375), MulPoint(SumPoint(C,D),0.125)).x;
        n.location.y = SumPoint(MulPoint(SumPoint(A,B),0.375), MulPoint(SumPoint(C,D),0.125)).y;
        n.location.z = SumPoint(MulPoint(SumPoint(A,B),0.375), MulPoint(SumPoint(C,D),0.125)).z;
    }
    std::cout<< "The positions of all newly added vertices have been updated.\n" << std::endl;

    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;

    std::cout<< "Start modifying old vertices..." << std::endl;
    
    // 修改原来的顶点
    // Modify the original vertex
    for (auto& oriV : oriVertices)
    {
        // 计算周围的原始顶点的
        // Calculate the surrounding original vertices
        Vertex sum;
        sum.x = 0.0f;
        sum.y = 0.0f;
        sum.z = 0.0f;
        for(const auto& dirEdge : directedEdges){
            if (oriV.id == dirEdge.startVertexID)
            {
                sum = SumPoint(sum,dirEdge.endLoac);
            }            
        }
        float u;
        if (oriV.n == 3)
        {
            u = 3.0f/16.0f;
        }else{
            u = 3.0f/(8.0f*static_cast<float>(oriV.n));
        }
        oriV.location = SumPoint(MulPoint(oriV.location,(1.0f-static_cast<float>(oriV.n)*u)),MulPoint(sum,u));
    }
    std::cout<< "Old vertex modification completed.\n" << std::endl;
    
    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;

    std::cout<< "Add new vertices after old vertices..." << std::endl;
    // 顶点
    // old Vertex
    for (const auto& newV : newVertices) {
        OriginVertex ov;
        ov.id = oriVertices.size();
        ov.location.x = newV.location.x;
        ov.location.y = newV.location.y;
        ov.location.z = newV.location.z;
        oriVertices.push_back(ov);
    }
    std::cout << "The new vertex has been added after the old vertex.\n" << std::endl;

    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;

    std::cout<< "Add the ID of all newly added vertices in the old vertex vector to the value of the new vertex vector..." << std::endl;
    // 给所有newVertices的id赋上在oriVertices里的值
    // Assign values in oriVertices to the IDs of all newVertices
    for (auto& newV : newVertices)
    {
        for(const auto& v : oriVertices){
            if (newV.location.x == v.location.x && newV.location.y == v.location.y && newV.location.z == v.location.z)
            {
                newV.id = v.id;
            }
        }
    }
    std::cout<< "ID value added completed.\n" << std::endl;

    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;

    // debug
    // for(auto& face : faces){
    //     std::cout << "faceID:" << face.id << " VA:" << face.vertexA << " VB:" << face.vertexB << " VC:" << face.vertexC << std::endl;
    // }
    // std::cout << "" << std::endl;

    // debug
    // for(const auto& v : newVertices){
    //     std::cout << "newVertex:" << v.id << " edgePointA:" << v.vertexA << " edgePointB:" << v.vertexB << " (" << v.location.x << "," << v.location.y << "," << v.location.z << ")" << " faceA:" << v.faceA << " faceB:" << v.faceB << std::endl;
    // }
    // std::cout << "" << std::endl;
    
    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;
    
    std::cout<< "Subdivide each face based on newly added points..." << std::endl;
    // 面 
    // Face
    // 新建一个临时容器存储新建的面
    // Create a new temporary container to store the newly created face
    std::vector<Face> newFaces;
    // std::cout << "faces:" << faces.size() << std::endl;
    // 先储存中心面
    // Store the central plane first
    for (const auto& face : faces){
        // face: A, B, C
        /*
                 A
                 /\
                /  \
             C'/____\B'
              / \  / \
            B/___\/___\C
                 A'
        */
        Face newf;
        for (const auto& newV : newVertices){
            //如果新增点的左右顶点是AB,则将大三角形的C设置为新增点C'
            //If the left and right vertices of the newly added point are AB, set the C of the large triangle to the newly added point C '
            if ((newV.vertexA == face.vertexA && newV.vertexB == face.vertexB) || (newV.vertexA == face.vertexB && newV.vertexB == face.vertexA)){
                newf.vertexC = newV.id;
            }
            //如果新增点的左右顶点是AC,则将大三角形的B设置为新增点
            //If the left and right vertices of the new point are AC, set B of the large triangle as the new point
            else if((newV.vertexA == face.vertexA && newV.vertexB == face.vertexC) || (newV.vertexA == face.vertexC && newV.vertexB == face.vertexA)){
                newf.vertexB = newV.id;
            }
            //如果新增点的左右顶点是CB,则将大三角形的A设置为新增点
            //If the left and right vertices of the new point are CB, set A of the large triangle as the new point
            else if((newV.vertexA == face.vertexC && newV.vertexB == face.vertexB) || (newV.vertexA == face.vertexB && newV.vertexB == face.vertexC)){
                newf.vertexA = newV.id;
            }       
        }
        newf.id = face.id;  
        newFaces.push_back(newf);
    }
    //处理每个面剩下的三个面
    //Process the remaining three faces for each face
    for (const auto& face : faces){        
        // face: A, B, C
        /*
                 A
                 /\
                /  \
             C'/____\B'
              / \  / \
            B/___\/___\C
                 A'
        */

        // 新建三个面, 因为原本的一个面被分成了4份,但中间那个的序号是原来的
        // Create three new faces because the original one has been divided into four parts, but the middle one has the same serial number as the original one
        Face Acb, Bac, Cba;
        Acb.vertexA = face.vertexA;
        Bac.vertexA = face.vertexB;
        Cba.vertexA = face.vertexC;

        for (const auto& newV : newVertices){
            //如果新增点的左右顶点是AB,则将大三角形的C设置为新增点C'
            //If the left and right vertices of the newly added point are AB, set the C of the large triangle to the newly added point C '
            if ((newV.vertexA == face.vertexA && newV.vertexB == face.vertexB) || (newV.vertexA == face.vertexB && newV.vertexB == face.vertexA)){
                Acb.vertexB = newV.id;
                Bac.vertexC = newV.id;
            }
            //如果新增点的左右顶点是AC,则将大三角形的B设置为新增点
            //If the left and right vertices of the new point are AC, set B of the large triangle as the new point
            else if((newV.vertexA == face.vertexA && newV.vertexB == face.vertexC) || (newV.vertexA == face.vertexC && newV.vertexB == face.vertexA)){
                Cba.vertexB = newV.id;
                Acb.vertexC = newV.id;
            }
            //如果新增点的左右顶点是CB,则将大三角形的A设置为新增点
            //If the left and right vertices of the new point are CB, set A of the large triangle as the new point
            else if((newV.vertexA == face.vertexC && newV.vertexB == face.vertexB) || (newV.vertexA == face.vertexB && newV.vertexB == face.vertexC)){
                Bac.vertexB = newV.id;
                Cba.vertexC = newV.id;
            }            
        }       
        Acb.id = newFaces.size();
        newFaces.push_back(Acb);

        Bac.id = newFaces.size();
        newFaces.push_back(Bac);

        Cba.id = newFaces.size();
        newFaces.push_back(Cba);
    }
    // std::cout << "newFaces:" << newFaces.size() << std::endl;
    faces.clear();
    faces = newFaces;
    newFaces.clear();
    
    std::cout<< "Subdivide each face based on newly added points, complete.\n" << std::endl;
    //debug
    // for(auto& face : faces){
    //     std::cout << "faceID:" << face.id << " VA:" << face.vertexA << " VB:" << face.vertexB << " VC:" << face.vertexC << std::endl;
    // }
    // std::cout << "" << std::endl;
    
    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;
    
    std::cout<< "Calculating Directed Edges Based on Faces..." << std::endl;
    //新建有向边vector, 将计算得出的新的有向边赋值给它
    //Create a new directed edge vector and assign the calculated new directed edge value to it
    std::vector<DirectedEdge> newDirectedEdges;
    // std::cout << "faces:" << faces.size() << std::endl;
    // std::cout << "newDirectedEdges:" << newDirectedEdges.size() << std::endl;
    for (const auto& face : faces) {
        DirectedEdge e1,e2,e3;

        e1.startVertexID = face.vertexC;
        e1.endVertexID =  face.vertexA;
        e1.id = newDirectedEdges.size();        
        newDirectedEdges.push_back(e1);

        e2.startVertexID = face.vertexA;
        e2.endVertexID = face.vertexB;
        e2.id = newDirectedEdges.size();
        newDirectedEdges.push_back(e2);

        e3.startVertexID = face.vertexB;
        e3.endVertexID = face.vertexC;
        e3.id = newDirectedEdges.size();
        newDirectedEdges.push_back(e3);        
    }   
    std::cout<< "Directed edge calculation completed.\n" << std::endl;

    // for (auto& dirEdge : directedEdges){
    //     DirectedEdge e1,e2;
    //     e1.startVertexID = dirEdge.startVertexID;
    //     e2.endVertexID = dirEdge.endVertexID;
    //     for (const auto& newV : newVertices)
    //     {
    //         //找到两个端点是此边的两端点的新增点
    //         if ((dirEdge.startVertexID == newV.vertexA && dirEdge.endVertexID == newV.vertexB)||(dirEdge.startVertexID == newV.vertexB && dirEdge.endVertexID == newV.vertexA))
    //         {
    //             e1.endVertexID = newV.id;
    //             e2.startVertexID = newV.id;
    //         }
    //     }
    //     e1.id = newDirectedEdges.size();
    //     newDirectedEdges.push_back(e1);
    //     e2.id = newDirectedEdges.size();
    //     newDirectedEdges.push_back(e2);
    // }
    
    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;
    
    std::cout << "Calculate other half for each directed edge..." << std::endl;
    // std::cout << "newDirectedEdges:" << newDirectedEdges.size() << std::endl;
    //给新的有向边确定其otherHalf
    //Determine the otherHalf of a new directed edge
    for (size_t i = 0; i < newDirectedEdges.size()-1; i++)
    {
        for (size_t j = i+1; j < newDirectedEdges.size(); j++)
        {
            if (newDirectedEdges[i].startVertexID == newDirectedEdges[j].endVertexID && newDirectedEdges[i].endVertexID == newDirectedEdges[j].startVertexID)
            {
                newDirectedEdges[i].otherHalfID = newDirectedEdges[j].id;
                newDirectedEdges[j].otherHalfID = newDirectedEdges[i].id;
            }            
        }        
    }
    std::cout<< "Other half calculation completed.\n" << std::endl;

    
    std::cout<< "Calculate the center of the model based on the average of all points..." << std::endl;
    // 计算模型的中心点
    //Calculate the center point of the model
    Vertex modelCenter;
    for (const auto& v: oriVertices)
    {
        modelCenter = SumPoint(modelCenter,v.location);
    }
    modelCenter = DivPoint(modelCenter,oriVertices.size());    
    std::cout<< "Calculation model center completed.\n" << std::endl;
    // debug
    // std::cout<<modelCenter.x << " " << modelCenter.y << " " << modelCenter.z <<std::endl;

    
    std::cout<< "Calculate the normal vector for each point based on the center point..." << std::endl;
    //计算每个点的法向量
    //Calculate the normal vector for each point
    for (auto& v : oriVertices){
        v.normal = SubPoint(v.location,modelCenter);
    }
    std::cout<< "The normal vector calculation for each point is completed.\n" << std::endl;
    
    // std::cout << "Vertex0 (" << oriVertices[0].location.x << "," << oriVertices[0].location.y << "," << oriVertices[0].location.z << ")" << std::endl;

    std::cout<< "Clean up temporary vectors..." << std::endl;
    directedEdges.clear();//临时储存用的vector删掉
    newVertices.clear();//临时储存用的vector删掉
    directedEdges = newDirectedEdges;
    newDirectedEdges.clear();//临时储存用的vector删掉
    std::cout<< "Temporary vector cleaning completed.\n" << std::endl;
    // for(const auto& dirEdge : directedEdges){
    //     std::cout<< "edgeID:" << dirEdge.id << " startV:" << dirEdge.startVertexID << " endV:" << dirEdge.endVertexID <<std::endl;
    // }
    
    std::cout<< "loop subdivision finished.\n" <<std::endl;
}

void Model::cleanAndStoreValue() {    
    std::vector<OriginVertex> tempOriVertices; 
    std::vector<DirectedEdge> tempDirectedEdges;
    std::vector<Face> tempFaces;

    for(const auto& v : oriVertices) {
        OriginVertex vertex;
        vertex.n = 0;
        vertex.id = v.id;
        vertex.location.x = v.location.x;
        vertex.location.y = v.location.y;
        vertex.location.z = v.location.z;
        tempOriVertices.push_back(vertex);
    }
    // 顶点 Vertex
    for(const auto& v : oriVertices) {
        int VertexNum = v.id;
        tempOriVertices[VertexNum].normal.x = v.normal.x;
        tempOriVertices[VertexNum].normal.y = v.normal.y;
        tempOriVertices[VertexNum].normal.z = v.normal.z;
    }
    // 面 Face
    for(const auto& f : faces) {
        Face face;
        face.id = f.id;
        face.vertexA = f.vertexA;
        face.vertexB = f.vertexB;
        face.vertexC = f.vertexC;
        tempFaces.push_back(face);

        DirectedEdge dirEdge0,dirEdge1,dirEdge2;

        dirEdge0.startVertexID = face.vertexC;
        dirEdge0.endVertexID = face.vertexA;
        dirEdge0.faceID = face.id;
        tempDirectedEdges.push_back(dirEdge0);

        dirEdge1.startVertexID = face.vertexA;
        dirEdge1.endVertexID = face.vertexB;
        dirEdge1.faceID = face.id;
        tempDirectedEdges.push_back(dirEdge1);

        dirEdge2.startVertexID = face.vertexB;
        dirEdge2.endVertexID = face.vertexC;
        dirEdge2.faceID = face.id;
        tempDirectedEdges.push_back(dirEdge2);
    }
    // 有向边 Directed Edge
    for(const auto& dirEdge : directedEdges) {
        tempDirectedEdges[dirEdge.id].id = dirEdge.id;
        tempDirectedEdges[dirEdge.id].otherHalfID = dirEdge.otherHalfID;
    }

    oriVertices.clear();
    oriVertices = tempOriVertices;
    tempOriVertices.clear();

    directedEdges.clear();
    directedEdges = tempDirectedEdges;
    tempDirectedEdges.clear();

    faces.clear();
    faces = tempFaces;
    tempFaces.clear();
}

void Model::checkResult(){
    for(const auto& v : oriVertices)
    {
        std::cout << "Vector " << v.id << " " << v.location.x << " " << v.location.y << " " << v.location.z << std::endl;        
    }
    for(const auto& v : oriVertices)
    {
        std::cout << "Normal " << v.id << " " << v.normal.x << " " << v.normal.y << " " << v.normal.z << std::endl;        
    }
    for (const auto& f : faces)
    {   
        std::cout << "Face " << f.id << " " << f.vertexA << " " << f.vertexB << " " << f.vertexC << std::endl;
    }    
    for(const auto& dirEdge : directedEdges) 
    {
        std::cout << "OtherHalf " << dirEdge.id << " " << dirEdge.otherHalfID << std::endl;
    }
    std::cout << "" <<std::endl;
}

void Model::outPutResult(const std::string& filename) {

    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << "#" << std::endl;
        outFile << "# Leeds COMP 5821M Autumn 2023" << std::endl;
        outFile << "# Wang Simiao" << std::endl;
        outFile << "# 201702881" << std::endl;        
        outFile << "# Surface vertices=" << oriVertices.size() << " faces=" << faces.size() << std::endl;
        outFile << "#" << std::endl;
        // 顶点 Vertex
        for(const auto& v : oriVertices) {
            outFile << "Vertex " << v.id << " " << v.location.x << " " << v.location.y << " " << v.location.z << std::endl;
        }
        // 顶点 Vertex
        for(const auto& v : oriVertices) {
            outFile << "Normal " << v.id << " " << v.normal.x << " " << v.normal.y << " " << v.normal.z << std::endl;
        }
        // 每个顶点的第一个有向边
        // The first directed edge of each vertex
        for(const auto& v : oriVertices) {
            for(const auto& dirEdge : directedEdges) {
                if(dirEdge.startVertexID == v.id){
                    outFile << "FirstDirectedEdge " << v.id << " " << dirEdge.id << std::endl;
                    break;
                }
            }
        }
        // 面 Face
        for(const auto& f : faces) {
            outFile << "Face " << f.id << " " << f.vertexA << " " << f.vertexB << " " << f.vertexC << std::endl;
        }
        // 有向边 directed edge
        for(const auto& dirEdge : directedEdges) {
            outFile << "OtherHalf " << dirEdge.id << " " << dirEdge.otherHalfID << std::endl;
        }

        // 关闭文件
        outFile.close();
    }
    std::cout<< "file output finished.\n" <<std::endl;
}

Vertex Model::CenterPoint(Vertex A, Vertex B){
    Vertex result;
    result.x = (A.x + B.x)/2.0;
    result.y = (A.y + B.y)/2.0;
    result.z = (A.z + B.z)/2.0;
    return result;
}

Vertex Model::SumPoint(Vertex A, Vertex B){
    Vertex result;
    result.x = A.x + B.x;
    result.y = A.y + B.y;
    result.z = A.z + B.z;
    return result;
}

Vertex Model::SubPoint(Vertex A, Vertex B){
    Vertex result;
    result.x = A.x - B.x;
    result.y = A.y - B.y;
    result.z = A.z - B.z;
    return result;
}

Vertex Model::DivPoint(Vertex A, float d){
    Vertex result;
    result.x = A.x/d;
    result.y = A.y/d;
    result.z = A.z/d;
    return result;
}

Vertex Model::MulPoint(Vertex A, float m){
    Vertex result;
    result.x = A.x*m;
    result.y = A.y*m;
    result.z = A.z*m;
    return result;
}
