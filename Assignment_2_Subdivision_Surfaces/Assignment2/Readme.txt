# COMP 5812M Assignment 1

Name: Simiao Wang
Student ID: 201702881
University Email: sc23s2w@leeds.ac.uk
Personal Email: mickeymiao2023@163.com
Completion Date: November 4, 2023
University of Leeds 2023-2024

## Notes

1. Please place the files that need to be subdivided in the same level directory as this code.
2. This code allows you to specify the number of subdivisions. During multiple subdivisions, there is no need to output files; only the final result will be output.

## How to Execute My Code

1. Compile the program by executing the following command in the terminal: `make`
2. Run the compiled program by executing the following command: `./main`
3. The program will prompt you to enter the filename (excluding the file extension) of the `.bridgenormal` file that needs subdivision.
4. The program will prompt you to enter the number of surface subdivisions you desire.
5. The program will generate a new `.diredgenormal` file containing the subdivided surface geometry.

## Functionality of Each Code

### main.cpp
1. Accepts user input, including the filename (excluding the file extension) of the `.bridgenormal` file to be converted and the number of surface subdivisions.
2. Creates an instance of the `Model` class.
3. Loads the initial model from the corresponding `.diredgenormal` file.
4. Performs loop subdivision on the model for the specified number of times.
5. Outputs the final subdivided model to a new `.diredgenormal` file.

### Model.cpp
1. `loadModel(const std::string& filename)`: Loads the initial model from the specified filename.
2. `loopSubdivision()`: Implements the loop subdivision algorithm on the model.
3. `cleanAndStoreValue()`: Performs any necessary cleanup and storage operations after subdivision.
4. `outPutResult(const std::string& filename)`: Outputs the subdivided model to the specified filename.

## 注意事项

1. 请将需要细分的文件放在与本代码同级的文件夹内
2. 本代码可以设置需要细分的次数, 多次细分的过程中不需要输出文件, 只有最后结果会输出

## 如何执行我的代码

1. 在终端中执行以下命令编译程序：make
2. 通过执行以下命令运行编译后的程序：./main
3. 程序将提示您输入需要细分的 .bridgenormal 文件的文件名（不包括文件后缀）
4. 程序将提示您输入希望表面细分的次数
5. 程序将生成一个新的 .diredgenormal 文件，其中包含细分后的表面几何结构

## 各代码功能

### main.cpp
1. 接受用户输入，包括要转换的 .bridgenormal 文件的文件名（不包括文件后缀）和表面细分的次数。
2. 创建 Model 类的实例。
3. 从相应的 .diredgenormal 文件中加载初始模型。
4. 对模型执行指定次数的循环细分。
5. 将最终细分的模型输出到新的 .diredgenormal 文件。

### Model.cpp
1. loadModel(const std::string& filename)：从指定的文件名加载初始模型。
2. loopSubdivision()：在模型上实现循环细分算法。
3. cleanAndStoreValue()：在细分后执行任何必要的清理和存储操作。
4. outPutResult(const std::string& filename)：将细分后的模型输出到指定的文件名。