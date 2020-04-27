#include <stdio.h>
#include <string>
#include <iostream>

int main (int argc, char** argv) {
    int32_t innum = 1000000;
	float *indata = (float*)malloc(innum*sizeof(float));
    FILE * infile;
	infile = fopen ("temp.bin","rb");
	innum = fread(indata,sizeof(float),innum,infile)/5;
	for (int32_t i=0; i<innum; i++) {
        std::cout << "i: " << indata[5*(i)] 
        << " x: " << indata[5*(i) + 1] 
        << " y: " << indata[5*(i) + 2] 
        << " z: " << indata[5*(i) + 3] 
        << " r: " << indata[5*(i) + 4] 
        << std::endl;
    }
    fclose(infile);
}