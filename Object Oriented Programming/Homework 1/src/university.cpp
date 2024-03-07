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


University::University(std::string name1, float gpa, float gre, float toefl, float bias1){  //Constructor that is needed for main
    name = name1;                                                                           //This constructor takes as parameters
    gpa_weigh = gpa;                                                                        //the name of the uni, gpa weigh, gre weigh
    gre_weigh = gre;                                                                        //toefl weigh and the bias
    toefl_weigh = gre;
    bias = bias1;
}

University::University(std::string name1, float gpa, float gre, float toefl, float bias1, std::string country1){ //Constructor that is needed for main, including the country
    name = name1;
    gpa_weigh = gpa;
    gre_weigh = gre;
    toefl_weigh = gre;
    bias = bias1;
    country = country1;
}

void University::evaluate_student(const Student& stud){                                                 //This function evaluates whether a student
    stud.set_application(stud.get_applications() + 1);                                                  //is eligible to enter a uni based on their        
    if((stud.get_gpa()*gpa_weigh + stud.get_gre()*gre_weigh + stud.get_toefl()*toefl_weigh + bias)>1){  //performance in GPA, GRE, TOEFL, we multiply
        std::cout << stud.get_name() << " is admitted to " << name << "." << std::endl;                 //these numbers with the weigh that each uni  
    }else{                                                                                              //has given these values and in the end we 
        std::cout << stud.get_name() << " is rejected from " << name << "." << std::endl;               //add the bias. If the value remaining 
    }                                                                                                   //is bigger than 1 then the student is admitted
}                                                                                                       //otherwise they're rejected


/*
bool University::uni_perceptron(const Student& stud){                                                   //In this function I implemented a code
    if((stud.get_gpa()*gpa_weigh + stud.get_gre()*gre_weigh + stud.get_toefl()*toefl_weigh + bias)>1){  //that will either return one or zero
        return 1;                                                                                       //it will return 1 when the value of the 
    }else{                                                                                              //gpa_weigh mutliplied with the student's gpa
        return 0;                                                                                       //and so forth plus the bias of the university
    }                                                                                                   //is bigger than 1, otherwise it'll return 0
}
*/