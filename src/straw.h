/*
  The MIT License (MIT)

  Copyright (c) 2011-2016 Broad Institute, Aiden Lab

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/
#ifndef STRAW_H
#define STRAW_H

#include <fstream>
#include <set>
#include <utility>
#include <vector>
#include <map>

//hicmaptool
//#include "index.h"

// pointer structure for reading blocks or matrices, holds the size and position
struct indexEntry {
    long size;
    long position;
};

// sparse matrix entry
struct contactRecord {
  int binX;
  int binY;
    float counts;
};

// chromosome
struct chromosome {
    std::string name;
	int index;
    long length;
};

//struct for hic map tool
/*
struct INDEX_ELE{
	std::string chr;
	int start;
	int end;
	int count;
};
*/


bool readMagicString(std::ifstream &fin);

std::map<std::string, chromosome> readHeader(std::istream &fin, long &masterIndexPosition);

bool readFooter(std::istream &fin, long master, int c1, int c2, std::string matrix, std::string norm, std::string unit,
                int resolution, long &myFilePos, indexEntry &c1NormEntry, indexEntry &c2NormEntry,
                std::vector<double> &expectedValues);

std::map<int, indexEntry>
readMatrixZoomData(std::istream &fin, std::string myunit, int mybinsize, float &mySumCounts, int &myBlockBinCount,
                   int &myBlockColumnCount, bool &found);

std::map<int, indexEntry>
readMatrix(std::istream &fin, int myFilePosition, std::string unit, int resolution, float &mySumCounts,
           int &myBlockBinCount, int &myBlockColumnCount);

std::set<int>
getBlockNumbersForRegionFromBinPosition(int *regionIndices, int blockBinCount, int blockColumnCount, bool intra);

std::vector<contactRecord> readBlock(std::istream &fin, int blockNumber);

std::vector<double> readNormalizationVector(std::istream &fin, indexEntry entry);

std::vector<contactRecord>
straw(std::string matrix, std::string norm, std::string fname, std::string chr1loc, std::string chr2loc,
      std::string unit, int binsize);

int
getSize(std::string matrix, std::string norm, std::string fname, std::string chr1loc, std::string chr2loc,
        std::string unit, int binsize);

// for hic map tool below
bool compareXY(const contactRecord & record_1, const contactRecord & record_2); // comparison base for sorting contactRecord

void sortContactRecord (std::vector<contactRecord> & records);

std::map <std::string, chromosome> getAllChr(std::string fname);


#endif
