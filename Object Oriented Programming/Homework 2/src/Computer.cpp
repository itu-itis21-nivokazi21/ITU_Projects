/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#include <iostream>
#include <fstream>

#include "Computer.h"


Computer::Computer(){       //Computer constructor
    attachedCPU = NULL;     //sets the pointers to NULL and prints "Computer is ready"
    attachedGPU = NULL;
    std::cout << "Computer is ready"<< std::endl;
}

void Computer::operator+(const CPU& cpu){                   //Operator Overload of operator + where the reference of the CPU object gets
    if(!attachedCPU){                                       //attached to the attachedCPU pointer 
        attachedCPU = &cpu;                                 //The function also checks the availability of the pointer
        std::cout << "CPU is attached" << std::endl;        //(If the pointer is NULL or not)
    } else {                                                 
        std::cout << "There is already a CPU" << std::endl; 
    }
}

void Computer::operator+(const GPU& gpu){                   //Operator Overload of operator + where the reference of the GPU object
    if(!attachedGPU){                                       //gets attached to the attachedGPU pointer in the Computer object
        attachedGPU = &gpu;                                 //The function also checks the availability of the pointer
        std::cout << "GPU is attached" << std::endl;        //(If the pointer is NULL or not)
    } else {
        std::cout << "There is already a GPU" << std::endl; 
    }
}

void Computer::execute(std::string operation){
    // "subtract","render", "trainModel","add", "multiply"
    if (operation == "subtract" || operation == "add" || operation == "multiply"){ //The operations subtract, add and multiply are available in the
        attachedCPU->execute(operation);                                           //CPU object
    } else if (operation == "render" || operation == "trainModel"){                //Whereas the operations render and trainModel are only
        attachedGPU->execute(operation);                                           //available in the GPU object
    }
}

// cpu

CPU::CPU(){
}

CPU::CPU(int num){
    alu_object.set_num(num);                //In this CPU constructor the integer value gets assigned to the alu object to initialize numPerCores
    std::cout<<"CPU is ready"<<std::endl;   //Prints "CPU is ready"
}

void CPU::execute(std::string operation) const{
    std::cout<<"Enter two integers"<<std::endl; //Since the operations in CPU are subtract, multiply and add we need to take input from 
    int x,y;                                    //the user
    std::cin >> x;
    std::cin >> y;
    if (operation == "subtract"){                               //In each if statement we call a different method 
        std::cout << alu_object.subtract(x,y) << std::endl;     //of the ALU object that is associated with the operation
    }else if (operation == "multiply"){
        std::cout << alu_object.multiply(x,y) <<std::endl;
    }else if (operation == "add"){
        std::cout << alu_object.add(x,y) << std::endl;
    }
}

//gpu

GPU::GPU(){
}

GPU::GPU(int num){
    cuda_object.set_num(num);               //Here we initialize the numCores in the CUDA object
    std::cout<<"GPU is ready"<<std::endl;   
}

void GPU::execute(std::string operation) const{             //Here the method execute decides which method of the CUDA object is going to get
    if(operation == "render"){                              //called based on the operation input
        std::cout << cuda_object.render() << std::endl;
    }else if (operation == "trainModel"){
        std::cout << cuda_object.trainModel() << std::endl;
    }
}

//alu

void ALU::set_num(int num){                 //Method to set the numPerCores value in ALU
    numPerCores = num;
    std::cout<<"ALU is ready"<<std::endl;
}

int ALU::add(int x, int y) const{           //Add method in ALU
    return (x + y);
}
int ALU::subtract(int x, int y) const{      //Subtract method in ALU
    return (x - y);
}
int ALU::multiply(int x, int y) const{      //Multiply method in ALU
    return (x*y);
}

//cuda

void CUDA::set_num(int num){                //Method to initialize the numCores member in CUDA object
    numCores = num;
    std::cout<<"CUDA is ready"<<std::endl;
}

std::string CUDA::render() const{           //Render method in CUDA
    return "Video is rendered";
}

std::string CUDA::trainModel() const{       //trainModel method in CUDA
    return "AI Model is trained";
}

