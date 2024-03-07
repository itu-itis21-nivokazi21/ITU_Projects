/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#ifndef _student
#define _student

#pragma once

class CUDA{
    private:
    int numCores;
    public:
    void set_num(int);              //Function used by GPU to set the numberCores of the CUDA
    std::string render() const;     //Render function of CUDA class
    std::string trainModel() const; //trainModel function of CUDA class
};

class ALU{
    private:
    int numPerCores;
    public:
    void set_num(int);              //Function used by the CPU to initialize the numPerCores part of the ALU
    int add(int, int) const;        
    int subtract(int, int) const;
    int multiply(int, int) const;
};

class GPU{
    private:
    CUDA cuda_object;                   //The GPU contains CUDA object
    public:
    GPU();                              
    GPU(int num);                       //GPU Constructor with the number of CUDAs given
    void execute(std::string) const;    //Execute function
};

class CPU{
    private:
    ALU alu_object;                     //The CPU contains ALU object
    public:
    CPU();
    CPU(int);                           //Constructor of CPU with the number of ALU's in it
    void execute(std::string) const;    //Execute function
};

class Computer{
    private:
    const CPU* attachedCPU;             //Here I made two pointers to CPU and GPU so we can connect them to a CPU or GPU object
    const GPU* attachedGPU;
    public:
    Computer();
    void operator+(const CPU&);         //Here I am setting an operator overload so that whenever (Computer object) + (CPU/GPU object)
    void operator+(const GPU&);         //is written the CPU/GPU object is attached to the pointers in the Computer object
    void execute(std::string);          //The execute function
};

#endif