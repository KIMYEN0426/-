// CppTest04.cpp: �ܼ� ���� ���α׷��� �������� �����մϴ�.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	vector<char> vt;

	vt.push_back('a');
	vt.push_back('b');
	vt.push_back('c');
	vt.push_back('e');
	vt.push_back('c');
	vt.push_back('c');
	vt.push_back('d');

	cout << "Before remove 'c'" << endl;
	for (auto it = vt.begin(); it != vt.end(); ++it)
		cout << (*it) << " ";
	cout << endl << endl;

	auto rIter = std::remove_if(vt.begin(), vt.end(), [](char& c) {return c == 'c'; });

	cout << "remove 'c'" << endl;
	for (auto it = vt.begin(); it != vt.end(); ++it)
		cout << (*it) << " ";
	cout << endl << endl;

	vt.erase(rIter, vt.end());

	cout << "After erase 'c'"<<endl;
	for(auto it = vt.begin(); it!=vt.end(); ++it)
		cout << (*it) << " ";
	cout << endl << endl;

}


