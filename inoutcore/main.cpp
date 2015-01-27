//
//  main.cpp
//  inoutcore
//
//  Created by vic on 09/01/2014.
//  Copyright (c) 2014 vixac. All rights reserved.
//

#include <iostream>
#include <gtest/gtest.h>
int main(int argc,  char * argv[])
{

    // insert code here...
   // std::cout << "Hello, World!\n";
   
    std::cout << "Starting LetterStackComponents gtests:"<<std::endl;
    ::testing::InitGoogleTest(&argc, argv);
  //  ::testing::Ini
    return RUN_ALL_TESTS();

}

