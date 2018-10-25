#include <iostream>
#include <string>
#include <regex>
#include "CProcessUrl.h"

using namespace std;

int main(void)
{
	string input = "in.txt";
	string output = "out.txt";
	CProcessUrl a;
	a.SetInputFileName(input);
	a.SetOutputFileName(output);
	a.StartOfProcessing();
	return 0;
}