#pragma once


#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include<fstream>  //ifstream
#include<string>     //包含getline()

#include<iostream>  
#include<string>  
#include<vector>  

#include <opencv.hpp>

using namespace std; 

inline bool is_exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}


//inline bool is_exists(const std::string& name) {
//	if (FILE* file = fopen(name.c_str(), "r")) {
//		fclose(file);
//		return true;
//	}
//	else {
//		return false;
//	}
//}

//
//inline bool is_exists(const std::string& name) {
//	ifstream f(name.c_str());
//	return f.good();
//}

/*
-0.08510 -0.19482 0.00664
-0.03645 0.23987 -0.00568
-0.03862 0.25657 -0.00406
-0.03081 0.25651 -0.00609

*/
cv::Mat open_pts_file(std::string file_path)
{
	ifstream input;
	input.open(file_path);
	std::string str;
	std::vector<std::vector<float>> pts;
	float x, y, z;
	
	for (int i = 0; !input.eof(); i++)
	{
		input >> x >> y >> z;
		pts.push_back({x, y, z});
	}

	cv::Mat pt_mat(pts.size(), 3, CV_32F);
	float* p;
	for (int i = 0; i < pts.size(); i++)
	{
		p = pt_mat.ptr<float>(i);
		for (int j = 0; j < 3; j++) p[j] = pts[i][j];
	}
	//std::cout << pt_mat << std::endl;
	return pt_mat;
}

std::vector<std::string> listdir(std::string file_dir)
{
	intptr_t Handle;
	struct _finddata_t FileInfo;
	string p;
	std::vector<std::string> file_name_list;
	if ((Handle = _findfirst(p.assign(file_dir).append("\\*").c_str(), &FileInfo)) == -1)
		printf("not exists!\n");
	else
	{
		//printf("%s\n", FileInfo.name);
		while (_findnext(Handle, &FileInfo) == 0)
			if (strcmp(FileInfo.name, "..") != 0)
				file_name_list.push_back(FileInfo.name);
		_findclose(Handle);
	}
	return file_name_list;
}
string strip(const string& str, char ch = ' ')
{
	//除去str两端的ch字符
	int i = 0;
	while (str[i] == ch)// 头部ch字符个数是 i
		i++;
	int j = str.size() - 1;
	while (str[j] == ch) //
		j--;
	return str.substr(i, j + 1 - i);
}
vector<string> split(const string& str, string ch = " ")
{
	//以 ch 为分割字符，把 cstr 分割为多个元素存到vector
	vector<string>ret;
	int pos = 0;
	int start = 0;
	while ((pos = str.find(ch, start)) != string::npos)
	{
		//cout << "pos:" << pos << endl;
		if (pos > start)
			ret.push_back(str.substr(start, pos - start));
		start = pos + ch.size();
	}
	if (str.size() > start)
		ret.push_back(str.substr(start));
	return ret;
}
//int main()
//{
//	string str = ".. first .. second  .. third ..";
//
//	for (string str : split(str, ".")) //单个字符
//		cout << "-" << str << "-" << endl;
//	for (string str : split(str, "..")) //多个字符
//		cout << "-" << str << "-" << endl;
//	for (string str : split(str))   //默认用空格分割
//		cout << "-" << str << "-" << endl;
//
//	cout << "strip():" << strip(str, '.') << "-" << endl;
//	return 0;
//}