/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#ifndef _university
#define _university

#pragma once
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "student.h"

class University{
    
        std::string name = "";
        float gpa_weigh = 0.0;   //I set all the float variables to initially be 0
        float gre_weigh = 0.0;
        float toefl_weigh = 0.0;
        float bias = 0;
        std::string country = "";

    public:
        University(std::string, float, float, float, float, std::string); //I've declared two constructors since in the main we have two different types
        University(std::string, float, float, float, float);              //of university declarations one with the country included and one without
        void evaluate_student(const Student&);                            //Since in the main function we have a const student declaration I used cost Student&
                                                                          //as a parameter for this function, and I also made a call by reference to avoid the 
                                                                          //avoid creating unnecessary objects again and to actually be able to modify the objects
                                                                          //created in main.
                                                                          //I didn't add a lot of methods to these classes since it was
                                                                          //instructed to only use what is going to be needed for main.
        //bool uni_perceptron(const Student&);
};

#endif