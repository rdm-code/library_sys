#pragma once
#include <string>

class Prompt
{
private:
	static void printMsg(std::string);
public:   
   static void input(std::string , int &);
   static void input(std::string , double &);
   static void input(std::string , std::string &);
};
