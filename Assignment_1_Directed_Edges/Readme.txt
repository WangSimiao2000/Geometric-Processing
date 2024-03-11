# COMP 5812M Assignment 1

Name: Simiao Wang
Student ID: 201702881
University Email: sc23s2w@leeds.ac.uk
Personal Email: mickeymiao2023@163.com
Completion Date: November 4, 2023
University of Leeds 2023-2024

## Notes

1. Please place the files face2faceindex.cpp, faceindex2directededge.cpp, and MeshManifoldCheck.cpp in the same directory as the handout_models folder so that the code can run correctly.
2. The time complexity and space complexity of the algorithm are documented in the header comments at the beginning of each .cpp file.
3. This code will process all .tri files within the handout_models folder in one go. The progress will be displayed in the command line.
4. When executing MeshManifoldCheck, the command line will output the first item (edge or vertex) that does not satisfy the non-manifold condition. It will also display the number of vertices, faces, and edges in the manifold file.

## How to Run My Code

1. Start by using the `make` command to execute the Makefile and compile the three C++ files.
2. Execute the `./face2faceindex` command to run the `face2faceindex` executable, which converts .tri files to .face files.
3. Execute the `./faceindex2directededge` command to run the `faceindex2directededge` executable, which converts .face files to .tri files.
4. Execute the `./MeshManifoldCheck` command to run the `MeshManifoldCheck` executable, which checks whether the model is a manifold and calculates the genus.
5. If you need to recompile and run, first execute the `make clean` command to clean the file directories, and then proceed with the first step again.

## Functionality of Each Code

### face2faceindex.cpp

This code iterates through each .tri file in the handout_models folder, performing a conversion operation on each .tri file. The results of the conversion are generated in the "faces" folder in the directory of the `face2faceindex` executable, with the output files having the .face extension.

### faceindex2directededge.cpp

This code iterates through each .face file in the "faces" folder, performing a conversion operation on each .face file. The results of the conversion are generated in the "direct_edges" folder in the directory of the `faceindex2directededge` executable, with the output files having the .diredge extension.

### MeshManifoldCheck.cpp

This code iterates through each .face file in the "faces" folder, sequentially checking if each model is a manifold. For manifold models, it calculates the genus and generates the "isManifold.txt" file. The file indicates whether each model is a manifold and provides the genus for manifold models. Additionally, the terminal will output information about non-manifold models, including non-compliance items (edges or vertices), as well as vertex count, face count, and edge count for manifold models.

======== ======== ======== ======== ======== ======== ========= ======== ======== ======== ========

# COMP 5812M Assignment 1

姓名: 王思淼
学号: 201702881
学校邮箱: sc23s2w@leeds.ac.uk
生活邮箱: mickeymiao2023@163.com
完成日期: 2023年11月4日
University of leeds 2023-2024

## 注意事项

1. 请将face2faceindex.cpp, faceindex2directededge.cpp, MeshManifoldCheck.cpp放在与handout_models文件夹同级的目录, 以便代码能正常运行
2. 算法时间复杂度及空间复杂度的在每个代码的.cpp文件的开头注释部分
3. 本代码一次性会处理handout_models文件夹内的所有.tri文件, 命令行中会输出当前进度
4. 执行MeshManifoldCheck时, 命令行中会输出非流型文件的第一个不满足项(edge或vertex), 也会输出流行文件的vertex数, face数, 以及edge的数量

## 如何执行我的代码

1. 首先使用 make 命令, 执行Makefile文件, 以编译三个cpp代码
2. 执行 ./face2faceindex 命令, 以执行face2faceindex可执行文件, 进行将.tri转换成.face的操作
3. 执行 ./faceindex2directededge 命令, 以执行faceindex2directededge可执行文件, 进行将.face转换成.tri的操作
4. 执行 ./MeshManifoldCheck 命令, MeshManifoldCheck可执行文件, 进行确定模型是否为流型的检查及亏格计算的工作
5. 若需要重新编译运行, 请先执行 make clean 命令以清理文件目录, 然后再次执行第一步

## 各代码功能

### face2faceindex.cpp

此代码通过遍历handout_models文件夹内的每一个.tri后缀的文件, 依次对每一个.tri文件进行转换操作, 转换的结果会生成在face2faceindex可执行文件所在目录的faces文件夹内, 结果文件以.face为后缀

### faceindex2directededge.cpp

此代码通过遍历faces文件夹内的每一个.face后缀的文件, 依次对每一个.face文件进行转换操作, 转换的结果会生成在faceindex2directededge可执行文件所在目录的direct_edges文件夹内, 结果文件以..diredge为后缀

### MeshManifoldCheck.cpp

此代码通过遍历faces文件夹内的每一个.face后缀的文件, 依次判断每一个模型是否为流型, 对是流型的模型进行亏格计算, 最终生成isManifold.txt文件, 文件内会写明各个文件是否为流型, 以及流形文件的亏格, 
同时, 命令行终端会输出非流型文件的不满足项(edge或vertex), 也会输出流行文件的vertex数, face数, 以及edge的数量