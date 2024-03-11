#include <iostream>
#include "Model.h"

int main(){
    std::string filename;
    int times;

    std::cout << "Leeds COMP 5821M Autumn 2023" << std::endl;
    std::cout << "Wang Simiao 201702881" << std::endl;    
    std::cout << "Please place the surface files that need to be subdivided in the same level directory~" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Please enter the file name of the .bridgenormal file that needs to be converted (excluding the file suffix): " << std::endl;
    std::cin >> filename;    
    std::cout << "Please enter the number of times the surface needs to be subdivided:" << std::endl;
    std::cin >> times;

    std::string inputFilename = filename;
    Model model;
    model.loadModel(inputFilename + ".diredgenormal");
    int n = times;
    for(int i = 0; i < n; i++){
        model.loopSubdivision();
        model.cleanAndStoreValue();
    }
    std::string outputFilename = inputFilename + "_" + std::to_string(n) + "times.diredgenormal";
    model.outPutResult(outputFilename);
    std::cout << "The new file has been output and named: " << outputFilename << std::endl;

    return 0;
}