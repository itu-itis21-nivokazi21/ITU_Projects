/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "university.h"
#include "student.h"

Student::Student(std::string name1, float GPA1, int GRE1, int TOEFL1){ 
    name = name1;
    GPA = GPA1;
    GRE = GRE1;
    TOEFL = TOEFL1;
    std::cout << name << " logged in to the system." << std::endl;  //Everytime this constructor is called it will show that the student
}                                                                   //has logged into the system

Student::Student(const Student &t){
    std::cout << t.name << " logged in to the system." << std::endl; //When the copy constructor is called it will again show that he has 
}                                                                    //logged in to the system

void Student::set_name(std::string name1){
    name = name1;                         //For the setters im using different names to reduce the risk of being ambiguous
}

void Student::set_application(int app) const{
    applications = app;
}

float Student::get_gpa() const{
    return GPA;
}

float Student::get_gre() const{
    return GRE;
}
        
float Student::get_toefl() const{
    return TOEFL;
}

int Student::get_applications() const{
    return applications;
}

std::string Student::get_name() const{
    return name;
}

Student::~Student(){
    std::cout<< name << " logged out of the system with " << applications << " application(s)." << std::endl; //After main finishes the desructor
}                                                                                                             //is called andon the terminal
                                                                                                              //we can see that the student logged out