/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#ifndef _student
#define _student

#pragma once
#include "university.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

class Student{
    private:
        std::string name = " ";       //Here I declared the private memers of my student class
        float GPA = 0.0;              //I set all the float values initially as 0
        float GRE = 0.0;
        float TOEFL = 0.0;
        mutable int applications = 0; //Since in main there is a declared const student object to be able to access that student's
                                      //appplications or keep track of them we need to declare applications as a mutable variable
    public:
        Student(std::string, float, int, int); //This constructer takes as parameters name, GPA, GRE, TOEFL
        Student(const Student &t);             //The copy constructer
        void set_name(std::string);            //This method serves as a setter for the Studnet's name in main it is used for student Lily
        void set_application(int) const;       //This method will be used in student evaluation to keep track of how many times the student applies
        int get_applications() const;          //Method to get the number of applications
        float get_gpa() const;                 //I declared these methods as const so that they could be used even if the object created is-
        float get_gre() const;                 //-a const object
        float get_toefl() const;
        std::string get_name() const;
        ~Student();                            //Destructor
};

#endif