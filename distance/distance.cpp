//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <map>
#include <algorithm>
#include <vector>
#include "Common.h"

using namespace std;

const long long max_size = 2000;         // max length of strings
const long long N = 40;                  // number of closest words that will be shown
const long long max_w = 50;              // max length of vocabulary entries

map<string, vector<float> > mpWordVtDimension;

typedef struct Node
{
    Node()
    {
        _sWord   = "";
        _fCosDis = 0.0;
    }

    string          _sWord;
    float           _fCosDis;
    vector<float>   _vtDim;

/*    bool operator < (const Node &s) const
    {   
        return _fCosDis < s._fCosDis;
    }*/
}Node;

bool greater_val(const Node & s1, const Node & s2)
{
    return s1._fCosDis > s2._fCosDis;
}


int calcCosDis(const vector<float> &vtDimen1, const vector<float> &vtDimen2, float &fCosDis)
{
    if(vtDimen1.size() != vtDimen2.size())
    {
        cout << "dimension is not equal, vtDimen1.size:" << vtDimen1.size() << ", vtDimen2.size:" << vtDimen2.size() << endl;
        fCosDis = -99999.9;
        return -1;
    }

    float fFenzi = 0.0;
    float fFenmu1 = 0.0;
    float fFenmu2 = 0.0;
    for(size_t i=0; i<vtDimen1.size(); i++)
    {
        fFenzi += (vtDimen1[i] * vtDimen2[i]);
        fFenmu1 += pow(vtDimen1[i], 2);
        fFenmu2 += pow(vtDimen2[i], 2);
    }
    float fFenmu = pow(fFenmu1, 0.5) * pow(fFenmu2, 0.5);
    fCosDis = fFenzi/fFenmu;

    return 0;
}

int calcEuclidDis(const vector<float> &vtDimen1, const vector<float> &vtDimen2, float &fEuclidDis)
{
    if(vtDimen1.size() != vtDimen2.size())
    {
        cout << "Euclid, dimension is not equal, vtDimen1.size:" << vtDimen1.size() << ", vtDimen2.size:" << vtDimen2.size() << endl;
        fEuclidDis = 99999.9;
        return -1;
    }

    float fSum = 0.0;
    for(size_t i=0; i<vtDimen1.size(); i++)
    {
        fSum += pow(vtDimen1[i]-vtDimen2[i], 2);
    }
    fEuclidDis = pow(fSum, 0.5);

    return 0;
}

int calcVal(const vector<string> &vtSen1, const vector<string> &vtSen2, float &fDis)
{
    float fSum = 0.0;
    float fNum = 0.0;
    for(size_t i=0; i<vtSen1.size(); i++)
    {
        if(mpWordVtDimension.find(vtSen1[i]) == mpWordVtDimension.end())
        {//not find
            cout << "not find, 1 word:" << vtSen1[i] << endl;
            continue;
        }
        float fMax = -99999.9;
        vector<float> vtDimen1 = mpWordVtDimension[vtSen1[i]];
        for(size_t j=0; j<vtSen2.size(); j++)
        {
            if(mpWordVtDimension.find(vtSen2[j]) == mpWordVtDimension.end())
            {//not find
                cout << "not find, 2 word:" << vtSen2[j] << endl;
                continue;
            }
            vector<float> vtDimen2 = mpWordVtDimension[vtSen2[j]];
            float fCosDis = 0.0;
            int iRet = calcCosDis(vtDimen1, vtDimen2, fCosDis);
            if(iRet < 0)
            {
                cout << "calcCosDis iRet:" << iRet << endl;
                continue;
            }
    //        cout << "word1:" << vtSen1[i] << ", word2:" << vtSen2[j] << ", fCosDis:" << fCosDis << endl;
            if(fCosDis > fMax)
            {
                fMax = fCosDis;
            }
        }

        //
        if(fMax > -90000.0)
        {
            fSum += fMax;
            fNum += 1.0;
        }
    }

    fDis = fSum / fNum;

    return 0;
}

//vector add 
int calcVal1(const vector<string> &vtSen1, const vector<string> &vtSen2, float &fDis)
{
    int iFlag = 0;
    vector<float> vtSum1;
    for(size_t i=0; i<vtSen1.size(); i++)
    {
        if(mpWordVtDimension.find(vtSen1[i]) == mpWordVtDimension.end())
        {//not find
            cout << "not find, 1 word:" << vtSen1[i] << endl;
            continue;
        }
        vector<float> vtDimen1 = mpWordVtDimension[vtSen1[i]];
        if(0==iFlag)
        {
            vtSum1 = vtDimen1;
            iFlag = 1;
        }
        else
        {
            for(size_t j=0; j<vtDimen1.size(); j++)
            {
                vtSum1[j] += vtDimen1[j];
            }
        }
    }

    iFlag = 0;
    vector<float> vtSum2;
    for(size_t i=0; i<vtSen2.size(); i++)
    {
        if(mpWordVtDimension.find(vtSen2[i]) == mpWordVtDimension.end())
        {//not find
            cout << "not find, 2 word:" << vtSen2[i] << endl;
            continue;
        }
        vector<float> vtDimen2 = mpWordVtDimension[vtSen2[i]];
        if(0==iFlag)
        {
            vtSum2 = vtDimen2;
            iFlag = 1;
        }
        else
        {
            for(size_t j=0; j<vtDimen2.size(); j++)
            {
                vtSum2[j] += vtDimen2[j];
            }
        }
    }

    //calc
    int iRet = calcCosDis(vtSum1, vtSum2, fDis);
    if(iRet < 0)
    {
        cout << "calcCosDis iRet:" << iRet << endl;
        return -1;
    }

    return 0;
}

int calcVal2(const vector<string> &vtSen1, const vector<string> &vtSen2, float &fDis)
{
    int iFlag = 0;
    vector<float> vtSum1;
    for(size_t i=0; i<vtSen1.size(); i++)
    {
        if(mpWordVtDimension.find(vtSen1[i]) == mpWordVtDimension.end())
        {//not find
            cout << "not find, 1 word:" << vtSen1[i] << endl;
            continue;
        }
        vector<float> vtDimen1 = mpWordVtDimension[vtSen1[i]];
        if(0==iFlag)
        {
            vtSum1 = vtDimen1;
            iFlag = 1;
        }
        else
        {
            for(size_t j=0; j<vtDimen1.size(); j++)
            {
                vtSum1[j] += vtDimen1[j];
            }
        }
    }
    for(size_t i=0; i<vtSum1.size(); i++)
    {
        vtSum1[i] = vtSum1[i] / (float)vtSen1.size();
    }

    iFlag = 0;
    vector<float> vtSum2;
    for(size_t i=0; i<vtSen2.size(); i++)
    {
        if(mpWordVtDimension.find(vtSen2[i]) == mpWordVtDimension.end())
        {//not find
            cout << "not find, 2 word:" << vtSen2[i] << endl;
            continue;
        }
        vector<float> vtDimen2 = mpWordVtDimension[vtSen2[i]];
        if(0==iFlag)
        {
            vtSum2 = vtDimen2;
            iFlag = 1;
        }
        else
        {
            for(size_t j=0; j<vtDimen2.size(); j++)
            {
                vtSum2[j] += vtDimen2[j];
            }
        }
    }
    for(size_t i=0; i<vtSum2.size(); i++)
    {
        vtSum2[i] = vtSum2[i] / (float)vtSen2.size();
    }

    //calc
    int iRet = calcEuclidDis(vtSum1, vtSum2, fDis);
    if(iRet < 0)
    {
        cout << "calcCosDis iRet:" << iRet << endl;
        return -1;
    }

    return 0;
}

int test1()
{
    string sWord = "市盈率";

    while(1)
    {
        if(sWord.empty())
        {
            continue;
        }

        map<string, vector<float> >::iterator it = mpWordVtDimension.find(sWord);
        if(it == mpWordVtDimension.end())
        {//not find
            cout << "sWord:" << sWord << " is not in mpWordVtDimension" << endl;
            continue;
        }

        vector<float> vtWord1Dim = it->second;

        vector<Node> vtNode;

        for(it=mpWordVtDimension.begin(); it!=mpWordVtDimension.end(); it++)
        {
            Node node;
            node._sWord = it->first;
            node._vtDim = it->second;

            calcCosDis(vtWord1Dim, it->second, node._fCosDis);

            vtNode.push_back(node);
        }

        sort(vtNode.begin(), vtNode.end(), greater_val);

        if(vtNode.size() < 50)
        {
            cout << "<50" << endl;
        }

        for(size_t i=0; i<vtNode.size() && i<60; i++)
        {
            cout << "word:" << vtNode[i]._sWord << ", dis:" << vtNode[i]._fCosDis << endl;
            for(size_t j=0; j<vtNode[i]._vtDim.size(); j++)
            {
                cout << vtNode[i]._vtDim[j] << " ";
            }
            cout << endl;
        }

        break;
    }

    return 0;
}

int test2()
{
    string sRs = "";
    vector<float> vtWord1Dim;
    ifstream f;
    f.open("rs", ios::in);
    while(f >> sRs)
    {
        cout << "sRs:" << sRs << endl;
        vtWord1Dim = TC_Common::sepstr<float>(sRs, ",");
        cout << "vtWord1Dim.size():" << vtWord1Dim.size() << endl;
        break;
    }
    f.close();


    string sWord;
    vector<Node> vtNode;
    ifstream myifstream;

    myifstream.open("word2", ios::in);
    while(myifstream >> sWord)
    {
        if(sWord.empty())
        {
            continue;
        }

        map<string, vector<float> >::iterator it = mpWordVtDimension.find(sWord);
        if(it == mpWordVtDimension.end())
        {//not find
            cout << "sWord:" << sWord << " is not in mpWordVtDimension" << endl;
            continue;
        }

        Node node;
        node._sWord = it->first;
        node._vtDim = it->second;

        calcCosDis(vtWord1Dim, it->second, node._fCosDis);

        vtNode.push_back(node);
    }
    myifstream.close();

    sort(vtNode.begin(), vtNode.end(), greater_val);

    for(size_t i=0; i<vtNode.size(); i++)
    {
        cout << "word:" << vtNode[i]._sWord << ", dis:" << vtNode[i]._fCosDis << endl;
    }

    return 0;
}


int test3_input_regular(const string &sInput)
{
    //计算距离

    //输入解析
    vector<string> vtSenInput = TC_Common::sepstr<string>(sInput, "|");
//    cout << "sInput:" << sInput << ", vtSenInput.size:" << vtSenInput.size() << endl;
  //  cout << "----------------------------------------------" << endl;;

    //读入规则
    string sLine("");
    ifstream f;
    f.open("invest3.regular", ios::in);
    while(f >> sLine)
    {
        if(sLine.empty())
        {
            continue;
        }

        vector<string> vtSenRegular = TC_Common::sepstr<string>(sLine, "|");

        float fDis = 0.0;
        int iRet = calcVal1(vtSenInput, vtSenRegular, fDis);
        //cout << "sLine:" << sLine << ", vtSenRegular.size:" << vtSenRegular.size() << endl;
        //cout << "iRet:" << iRet << ", fDis:" << fDis << endl;
        cout << sLine << "" << vtSenRegular.size() << "" << iRet << "" << fDis << endl;
        //cout << "----------------------------------------------" << endl;;
    }
    f.close();


    return 0;
}
    
int test4_input_regular_euclid(const string &sInput)
{
    //计算距离

    //输入解析
    vector<string> vtSenInput = TC_Common::sepstr<string>(sInput, "|");
//    cout << "sInput:" << sInput << ", vtSenInput.size:" << vtSenInput.size() << endl;
  //  cout << "----------------------------------------------" << endl;;

    //读入规则
    string sLine("");
    ifstream f;
    f.open("invest3.regular", ios::in);
    while(f >> sLine)
    {
        if(sLine.empty())
        {
            continue;
        }

        vector<string> vtSenRegular = TC_Common::sepstr<string>(sLine, "|");

        float fDis = 0.0;
        int iRet = calcVal2(vtSenInput, vtSenRegular, fDis);
        //cout << "sLine:" << sLine << ", vtSenRegular.size:" << vtSenRegular.size() << endl;
        //cout << "iRet:" << iRet << ", fDis:" << fDis << endl;
        cout << sLine << "" << vtSenRegular.size() << "" << iRet << "" << fDis << endl;
        //cout << "----------------------------------------------" << endl;;
    }
    f.close();


    return 0;
}


int main(int argc, char **argv) {
  FILE *f;
//  char *bestw[N];
  char file_name[max_size];
  float len;
  long long words, size, a, b;
  float *M;
  char *vocab;
  if (argc < 2) {
    printf("Usage: ./distance <FILE>\nwhere FILE contains word projections in the BINARY FORMAT\n");
    return 0;
  }
  strcpy(file_name, argv[1]);
  f = fopen(file_name, "rb");
  if (f == NULL) {
    printf("Input file not found\n");
    return -1;
  }
  fscanf(f, "%lld", &words);    //单词个数        vector.bin文件的第一个位置
  fscanf(f, "%lld", &size);     //表示单词的维度  vector.bin文件的第二个位置
  vocab = (char *)malloc((long long)words * max_w * sizeof(char));      //存放所有词汇的地方  max_w表示的是词的最大长度(50个字符)
//  for (a = 0; a < N; a++) bestw[a] = (char *)malloc(max_size * sizeof(char));
  M = (float *)malloc((long long)words * (long long)size * sizeof(float));  //存放所有词汇及其维度的内存
  if (M == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
    return -1;
  }
  for (b = 0; b < words; b++) {
    a = 0;
    while (1) {
      vocab[b * max_w + a] = fgetc(f);
      if (feof(f) || (vocab[b * max_w + a] == ' ')) break;
      if ((a < max_w) && (vocab[b * max_w + a] != '\n')) a++;
    }
    vocab[b * max_w + a] = 0;
    for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, f);    //以下5行为取数据的时候进行归一化
    len = 0;
    for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
    len = sqrt(len);
    for (a = 0; a < size; a++) M[a + b * size] /= len;
  }
  fclose(f);
  ///////////////////////////////////////////////////////////////
//  printf("words:%d\n", words);
//  printf("size :%d\n", size);
//  printf("----------------------------------------------\n");

////////////////////////////////////////////////////////////////////

//    map<string, vector<float> > mpWordVtDimension;
    for(int i=0; i<words; i++)
    {
        string sWord = vocab + i*max_w;
//        cout << sWord << endl;

        vector<float> vtDimension;
        for(int j=0; j<size; j++)
        {
            float fDimension = M[i*size+j];
            vtDimension.push_back(fDimension);
        }

        //set into map
        mpWordVtDimension[sWord] = vtDimension;
    }

//    cout << "----------------------------------------------" << endl;;
/*    for(map<string, vector<float> >::iterator it=mpWordVtDimension.begin(); it!=mpWordVtDimension.end(); it++)
    {
        cout << it->first << endl;
        for(size_t i=0; i<it->second.size(); i++)
        {
            cout << it->second[i] << ", ";
        }
        cout << endl;
    }*/
/*    for(map<string, vector<float> >::iterator it=mpWordVtDimension.begin(); it!=mpWordVtDimension.end(); it++)
    {
        float fSum = 0.0;
        cout << it->first << endl;
        for(size_t i=0; i<it->second.size(); i++)
        {
            fSum += pow(it->second[i], 2);
        }
        cout << pow(fSum, 0.5) << endl;
        cout << endl;
    }*/

////////////////////////////////////////////////////////////////////////////

//    test1();
//    test2();
//    test3_input_regular(argv[2]);
    test4_input_regular_euclid(argv[2]);

/*    string sWord;
    ifstream myifstream;

    myifstream.open("word2", ios::in);
    while(myifstream >> sWord)
    {
        if(sWord.empty())
        {
            continue;
        }

        map<string, vector<float> >::iterator it = mpWordVtDimension.find(sWord);
        if(it == mpWordVtDimension.end())
        {//not find
            cout << "sWord:" << sWord << " is not in mpWordVtDimension" << endl;
            continue;
        }

        vector<float> vtWord1Dim = it->second;

        vector<Node> vtNode;

        for(it=mpWordVtDimension.begin(); it!=mpWordVtDimension.end(); it++)
        {
            Node node;
            node._sWord = it->first;
            node._vtDim = it->second;

            calcCosDis(vtWord1Dim, it->second, node._fCosDis);

            vtNode.push_back(node);
        }

        sort(vtNode.begin(), vtNode.end(), greater_val);

        if(vtNode.size() < 50)
        {
            cout << "<50" << endl;
        }*/
/*
        for(size_t i=0; i<vtNode.size() && i<50; i++)
        {
            //cout << "word:" << vtNode[i]._sWord << ", dis:" << vtNode[i]._fCosDis << endl;
            for(size_t j=0; j<vtNode[i]._vtDim.size(); j++)
            {
                cout << vtNode[i]._vtDim[j] << " ";
            }
            cout << endl;
        }
*/
//    }
  //  myifstream.close();


/////////////////////////////////////////////////////////////////
    return 0;
}
