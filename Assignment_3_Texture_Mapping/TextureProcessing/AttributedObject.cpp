///////////////////////////////////////////////////
//
//  Hamish Carr
//  September, 2020
//
//  ------------------------
//  AttributedObject.cpp
//  ------------------------
//
//  Base code for rendering assignments.
//
//  Minimalist (non-optimised) code for reading and
//  rendering an object file
//
//  We will make some hard assumptions about input file
//  quality. We will not check for manifoldness or
//  normal direction, &c.  And if it doesn't work on
//  all object files, that's fine.
//
//	Variant on TexturedObject that stores explicit RGB
//	values for each vertex
//
///////////////////////////////////////////////////

// include the header file
#include "AttributedObject.h"

// include the C++ standard libraries we want
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include<unordered_map>
#include<unordered_set>
#include <algorithm>
#include<math.h>

// include the Cartesian 3- vector class
#include "Cartesian3.h"

#define MAXIMUM_LINE_LENGTH 1024
#define REMAP_TO_UNIT_INTERVAL(x) (0.5 + (0.5*(x)))
#define REMAP_FROM_UNIT_INTERVAL(x) (-1.0 + (2.0*(x)))

#define N_ITERATIONS 100000

// constructor will initialise to safe values
AttributedObject::AttributedObject()
    : centreOfGravity(0.0,0.0,0.0)
{ // AttributedObject()
    // force arrays to size 0
    vertices.resize(0);
    colours.resize(0);
    normals.resize(0);
    textureCoords.resize(0);
    firstDirectedEdge.resize(0);
    faceVertices.resize(0);
    otherHalf.resize(0);

    oriVertices.resize(0);
    oriColours.resize(0);
    halfEdges.resize(0);
    neighborPoints.resize(0);
    boundaryEdges.resize(0);
    boundaryPoints.resize(0);
} // AttributedObject()

// read routine returns true on success, failure otherwise
bool AttributedObject::ReadObjectStream(std::istream &geometryStream)
{ // ReadObjectStream()

    // create a read buffer
    char readBuffer[MAXIMUM_LINE_LENGTH];

    // the rest of this is a loop reading lines & adding them in appropriate places
    while (true)
    { // not eof
        // character to read
        char firstChar = geometryStream.get();

        //         std::cout << "Read: " << firstChar << std::endl;

        // check for eof() in case we've run out
        if (geometryStream.eof())
            break;

        // otherwise, switch on the character we read
        switch (firstChar)
        { // switch on first character
        case '#':       // comment line
            // read and discard the line
            geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
            break;

        case 'v':       // vertex data of some type
        { // some sort of vertex data
            // retrieve another character
            char secondChar = geometryStream.get();

            // bail if we ran out of file
            if (geometryStream.eof())
                break;

            // now use the second character to choose branch
            switch (secondChar)
            { // switch on second character
            case ' ':       // space - indicates a vertex
            { // vertex read
                Cartesian3 vertex;
                geometryStream >> vertex;
                vertices.push_back(vertex);
                //                         std::cout << "Vertex " << vertex << std::endl;
                break;
            } // vertex read
            case 'c':       // c indicates colour
            { // normal read
                Cartesian3 colour;
                geometryStream >> colour;
                colours.push_back(colour);
                //                         std::cout << "Colour " << colour << std::endl;
                break;
            } // normal read
            case 'n':       // n indicates normal vector
            { // normal read
                Cartesian3 normal;
                geometryStream >> normal;
                normals.push_back(normal);
                //                         std::cout << "Normal " << normal << std::endl;
                break;
            } // normal read
            case 't':       // t indicates texture coords
            { // tex coord
                Cartesian3 texCoord;
                geometryStream >> texCoord;
                textureCoords.push_back(texCoord);
                //                         std::cout << "Tex Coords " << texCoord << std::endl;
                break;
            } // tex coord
            default:
                break;
            } // switch on second character
            break;
        } // some sort of vertex data

        case 'f':       // face data
        { // face
            // make a hard assumption that we have a single triangle per line
            unsigned int vertexID;

            // read in three vertices
            for (unsigned int vertex = 0; vertex < 3; vertex++)
            { // per vertex
                // read a vertex ID
                geometryStream >> vertexID;

                // subtract one and store them (OBJ uses 1-based numbering)
                faceVertices.push_back(vertexID-1);
            } // per vertex
            break;
        } // face

        // default processing: do nothing
        default:
            break;

        } // switch on first character

    } // not eof

    // compute centre of gravity
    // note that very large files may have numerical problems with this
    centreOfGravity = Cartesian3(0.0, 0.0, 0.0);

    // if there are any vertices at all
    if (vertices.size() != 0)
    { // non-empty vertex set
        // sum up all of the vertex positions
        for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
            centreOfGravity = centreOfGravity + vertices[vertex];

        // and divide through by the number to get the average position
        // also known as the barycentre
        centreOfGravity = centreOfGravity / vertices.size();

        // start with 0 radius
        objectSize = 0.0;

        // now compute the largest distance from the origin to a vertex
        for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
        { // per vertex
            // compute the distance from the barycentre
            float distance = (vertices[vertex] - centreOfGravity).length();

            // now test for maximality
            if (distance > objectSize)
                objectSize = distance;

        } // per vertex
    } // non-empty vertex set

    // 	std::cout << "Centre of Gravity: " << centreOfGravity << std::endl;
    // 	std::cout << "Object Size:       " << objectSize << std::endl;

    // return a success code
    oriColours=colours;
    oriVertices =vertices;
    textureAndNormalMapSynthesis();
    return true;
} // ReadObjectStream()

// write routine
void AttributedObject::WriteObjectStream(std::ostream &geometryStream)
{ // WriteObjectStream()
    geometryStream << "# " << (faceVertices.size()/3) << " triangles" << std::endl;
    geometryStream << std::endl;

    // output the vertex coordinates
    geometryStream << "# " << vertices.size() << " vertices" << std::endl;
    for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
        geometryStream << "v  " << std::fixed << vertices[vertex] << std::endl;

    // output the vertex colours
    geometryStream << "# " << colours.size() << " vertex colours" << std::endl;
    for (unsigned int vertex = 0; vertex < colours.size(); vertex++)
        geometryStream << "vc " << std::fixed << colours[vertex] << std::endl;

    // output the vertex normals
    geometryStream << "# " << normals.size() << " vertex normals" << std::endl;
    for (unsigned int vertex = 0; vertex < normals.size(); vertex++)
        geometryStream << "vn " << std::fixed << normals[vertex] << std::endl;

    // output the vertex coords
    geometryStream << "# " << textureCoords.size() << " vertex tex coords" << std::endl;
    for (unsigned int vertex = 0; vertex < textureCoords.size(); vertex++)
        geometryStream << "vt " << std::fixed << textureCoords[vertex] << std::endl;

    // and the faces
    for (unsigned int face = 0; face < faceVertices.size(); face+=3)
    { // per face
        geometryStream << "f";

        // loop through # of vertices
        for (unsigned int vertex = 0; vertex < 3; vertex++)
        { // per vertex
            geometryStream << " ";
            geometryStream << faceVertices[face+vertex] + 1;
        } // per vertex
        // end the line
        geometryStream << std::endl;
    } // per face
} // WriteObjectStream()

// routine to render
void AttributedObject::Render(RenderParameters *renderParameters)
{ // Render()
    // make sure that textures are disabled
    glDisable(GL_TEXTURE_2D);

    float scale = renderParameters->zoomScale;
    scale /= objectSize;
    // Scale defaults to the zoom setting
    glTranslatef(-centreOfGravity.x * scale, -centreOfGravity.y * scale, -centreOfGravity.z * scale);

    if (renderParameters->useWireframe)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    else
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    if (renderParameters->useTexCoords)
    {
        for(size_t i = 0; i < textureCoords.size(); i++)
        {
            colours[i] = Cartesian3(textureCoords[i].x, textureCoords[i].y, textureCoords[i].z);
        }
    }
    else
    {
        for(size_t i = 0; i < textureCoords.size(); i++)
        {
            colours[i] = oriColours[i];
        }
    }

    if (renderParameters->renderNormalMap)
    {
        for(size_t i = 0; i < textureCoords.size(); i++)
        {
            colours[i] = Cartesian3((normals[i].x+1.0f)*0.5f,(normals[i].y+1.0f)*0.5f,(normals[i].z+1.0f)*0.5f);
        }
    }

    if(renderParameters->renderTexture)
    {
        vertices = textureCoords;
    }
    else
    {
        vertices = oriVertices;
    }

    // start rendering
    glBegin(GL_TRIANGLES);

    // loop through the faces: note that they may not be triangles, which complicates life
    for (unsigned int face = 0; face < faceVertices.size(); face+=3)
    { // per face

        // now do a loop over three vertices
        for (unsigned int vertex = 0; vertex < 3; vertex++)
        { // per vertex
            // set colour using vertex ID

            glColor3f
                (
                    colours[faceVertices[face+vertex]].x,
                    colours[faceVertices[face+vertex]].y,
                    colours[faceVertices[face+vertex]].z
                    );

            // use scaled xyz for vertex position
            glVertex3f
                (
                    scale * vertices[faceVertices[face+vertex]].x,
                    scale * vertices[faceVertices[face+vertex]].y,
                    scale * vertices[faceVertices[face+vertex]].z
                    );
        } // per vertex
    } // per face

    // close off the triangles
    glEnd();

    // revert render mode
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

} // Render()

void AttributedObject::textureAndNormalMapSynthesis()
{
    calculateHalfEdgesAndPoints();
    findBoundaryPointAndSortThem();
    createSquareTexture();
    calculateAndDoFloater();
    calculateNormals();
} // textureAndNormalMapSynthesis()

void AttributedObject::calculateHalfEdgesAndPoints()
{
    std::vector<std::unordered_set<unsigned int>> neighborpoints;
    neighborpoints.resize(vertices.size());

    // Calculate half edges and neighbor points
    std::cout<<"Calculate half edges and neighbor points"<<std::endl;
    for(size_t i=0;i<faceVertices.size();i+=3)
    {
        //get edge
        halfEdges.push_back({faceVertices[i],faceVertices[i+1]});
        halfEdges.push_back({faceVertices[i+1],faceVertices[i+2]});
        halfEdges.push_back({faceVertices[i+2],faceVertices[i]});

        //neighbor
        neighborpoints[faceVertices[i]].insert(faceVertices[i+1]);
        neighborpoints[faceVertices[i]].insert(faceVertices[i+2]);
        neighborpoints[faceVertices[i+1]].insert(faceVertices[i]);
        neighborpoints[faceVertices[i+1]].insert(faceVertices[i+2]);
        neighborpoints[faceVertices[i+2]].insert(faceVertices[i]);
        neighborpoints[faceVertices[i+2]].insert(faceVertices[i+1]);
    }
    for (const auto& pointSet : neighborpoints)
    {
        neighborPoints.push_back(std::vector<unsigned int>( pointSet.begin(),  pointSet.end()));
    }

    std::cout<<"Calculate other half"<<std::endl;
    otherHalf.clear();
    for (size_t i=0; i < halfEdges.size(); ++i)
    {
        int flag = 0;// Check if it has otherhalf
        for (size_t j=0; j < halfEdges.size(); ++j)
        {
            if(halfEdges[i].first == halfEdges[j].second && halfEdges[i].second == halfEdges[j].first){
                flag = 1;
                otherHalf.push_back(j);
                break;
            }
        }
        if(flag == 0){
            otherHalf.push_back(-1);
        }
    }
} // calculateHalfEdgesAndPoints()

void AttributedObject::findBoundaryPointAndSortThem()
{
    //find boundary edges
    std::cout<<"find boundary edges"<<std::endl;
    boundaryEdges.clear();
    for (size_t i = 0; i < otherHalf.size(); ++i) {
        if (otherHalf[i] == -1) {
            boundaryEdges.push_back(i);
        }
    }

    //find boundary points
    std::cout<<"find boundary points"<<std::endl;
    std::set<unsigned int> boundaryPointSet;
    for(size_t i =0;i<otherHalf.size();++i)
    {
        if(otherHalf[i] == -1)
        {
            boundaryPointSet.insert(halfEdges[i].first);
            boundaryPointSet.insert(halfEdges[i].second);
        }
    }

    boundaryPoints.clear();
    boundaryPoints.insert(boundaryPoints.begin(), boundaryPointSet.begin(), boundaryPointSet.end());

    //sort boundary points
    std::cout<<"sort boundary points"<<std::endl;
    if (boundaryPoints.empty()) return;

    std::vector<unsigned int> sorted;
    std::unordered_set<unsigned int> visited;
    unsigned int start = boundaryPoints[0];
    sorted.push_back(start);
    unsigned int current = start;

    for(size_t i =0;i<boundaryPoints.size();i++)
    {
        for(size_t j =0;j<boundaryEdges.size();j++)
        {
            if(halfEdges[boundaryEdges[j]].first==current&&halfEdges[boundaryEdges[j]].second!=start)
            {
                if (visited.find(boundaryEdges[j]) == visited.end())
                {
                    sorted.push_back(halfEdges[boundaryEdges[j]].second);
                    current=halfEdges[boundaryEdges[j]].second;
                    visited.insert(boundaryEdges[j]);
                    break;
                }
            }
        }
    }
    boundaryPoints=sorted;
} // findBoundaryPointAndSortThem()

void AttributedObject::createSquareTexture() {

    std::cout<<"Create Square Texture"<<std::endl;
    size_t N = boundaryPoints.size();
    textureCoords.resize(vertices.size());

    float step = 1.0f / (N - 1);
    float quarter = 0.25f;

    // Calculate texture coordinates based on quadrant
    for (size_t i = 0; i < N; ++i) {
        float s = step * i;
        float x = 0.0f, y = 0.0f;

        // square and edges:
        //  A _____ B
        //   |     |
        //   |     |
        //  C|_____|D
        if (i < N / 4) // edge: CD
        {
            x = s * 4;
            y = 0.0f;
        }
        else if (i < N / 2) // edge: BD
        {
            x = 1.0f;
            y = (s - quarter) * 4;
        }
        else if (i < 3 * N / 4) // edge: AB
        {
            x = 1.0f - (s - 2 * quarter) * 4;
            y = 1.0f;
        }
        else // edge: AC
        {
            x = 0.0f;
            y = 1.0f - (s - 3 * quarter) * 4;
        }
        textureCoords[boundaryPoints[i]] = Cartesian3(x, y, 0.0);
    }

    // Assign default texture coordinates to non-boundary points
    for(size_t i=0; i<vertices.size(); i++)
    {
        if (!checkIsBoundaryPoint(i))
        {
            textureCoords[i] = Cartesian3(0.5, 0.5, 0.0);
        }
    }
} // createSquareTexture()

bool AttributedObject::checkIsBoundaryPoint(unsigned int v)
{
    for(size_t i=0;i<boundaryPoints.size();++i)
    {
        if(v==boundaryPoints[i])
            return true;
    }
    return false;
} // checkIsBoundaryPoint(unsigned int v)

void AttributedObject::calculateAndDoFloater()
{
    std::cout<<"calculate And Do Floater"<<std::endl;
    for (size_t iteration =0 ;iteration<100;iteration++)
    {
        std::vector<Cartesian3> tempTextureCoords = textureCoords;
        for(size_t i=0;i<textureCoords.size();++i)
        {
            if(!neighborPoints[i].empty()&&!checkIsBoundaryPoint(i))
            {
                float x=0,y=0,z=0;
                for(size_t j=0;j<neighborPoints[i].size();++j)
                {
                    x +=textureCoords[neighborPoints[i][j]].x;
                    y +=textureCoords[neighborPoints[i][j]].y;
                    z = 0;
                }
                tempTextureCoords[i].x=x/neighborPoints[i].size();
                tempTextureCoords[i].y=y/neighborPoints[i].size();
                tempTextureCoords[i].z=z;

                //hack way to do floater
                Cartesian3 finalPos(0.0f, 0.0f, 0.0f);
                for(size_t q=0;q<neighborPoints[i].size();++q)
                {
                    float weight = 1.0f/neighborPoints[i].size();
                    finalPos=finalPos + weight*tempTextureCoords[neighborPoints[i][q]];
                }
                textureCoords[i]=finalPos;
            }
        }
    }
} // calculateAndDoFloater()

void AttributedObject::calculateNormals()
{
    std::cout<<"calculate normals"<<std::endl;
    normals.resize(faceVertices.size());
    for (size_t i=0;i<faceVertices.size();i+=3)
    {
        Cartesian3 A,B,C,faceNormal;
        A = vertices[faceVertices[i]];
        B = vertices[faceVertices[i+1]];
        C = vertices[faceVertices[i+2]];
        faceNormal = (B-A).cross(C-A)/(B-A).cross(C-A).length();

        normals[faceVertices[i]] = normals[faceVertices[i]]+faceNormal;
        normals[faceVertices[i+1]] = normals[faceVertices[i+1]]+faceNormal;
        normals[faceVertices[i+2]] = normals[faceVertices[i+2]]+faceNormal;
    }
    for (size_t i=0;i<normals.size();++i)
    {
        normals[i] = normals[i]/neighborPoints[i].size();
    }
}
