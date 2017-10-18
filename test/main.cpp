
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char ** argv)
{
  
	unsigned char * bla = new unsigned char[50];

	for(int i =0;i<50;i++){
		bla[i] = 'A';
	}


	cout << bla << endl;

	

  return 0;
}

