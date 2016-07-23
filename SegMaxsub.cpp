// SegMaxsub.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <time.h>
using namespace std;

typedef __int64 mytype;
struct node
{
	mytype maxi, lmaxi, rmaxi, sum;
}tree[50000000], outtree;                 //tree's szie should be large50000009

int g_data_num = 0;
const int g_data_line = 10000000;
int data[g_data_line + 1];

fstream file;
void GetData(string filename, int start, int end)
{
	ifstream file;
	char line[8];
	file.open(filename, ios::in);
	int i = 0;
	while (i <= end && !file.eof())
	{
		i++;
		file.getline(line, 10);
		if (i >= start)
		{
			g_data_num++;
			data[g_data_num] = atoi(line);
		}
	}
	file.close();
}

mytype max(mytype a, mytype b)
{
	return a>b ? a : b;
}

void MergeBranch(int rt)
{
	tree[rt].sum = tree[2 * rt].sum + tree[2 * rt + 1].sum;
	tree[rt].maxi = max(tree[2 * rt].maxi, max(tree[2 * rt + 1].maxi, tree[2 * rt].rmaxi + tree[2 * rt + 1].lmaxi));
	tree[rt].lmaxi = max(tree[2 * rt].lmaxi, tree[2 * rt].sum + tree[2 * rt + 1].lmaxi);
	tree[rt].rmaxi = max(tree[2 * rt + 1].rmaxi, tree[2 * rt + 1].sum + tree[2 * rt].rmaxi);
}

int nodenum = 0;
void CreateTree(int l, int r, int rt, int x[])
{
    //nodenum++;
	if (l == r)
	{
        //nodenum++;
		printf("%d tree.sum is:  %d \n", l, x[l]);
		tree[rt].maxi = tree[rt].lmaxi = tree[rt].rmaxi = tree[rt].sum = x[l];
        nodenum = rt > nodenum ? nodenum = rt : nodenum = nodenum;
		return;
	}
	int mid = (l + r) / 2;
	CreateTree(l, mid, 2 * rt, x);
	CreateTree(mid + 1, r, 2 * rt + 1, x);
	MergeBranch(rt);
}

struct node QueryInMemory(int l, int r, int aa, int bb, int rt)
{
	if (aa <= l && bb >= r)
		return tree[rt];
	int mid = (l + r) / 2;
	struct node ka, kb, res;
	int flag1 = 0;
	int flag2 = 0;
	if (aa <= mid)
	{
		ka = QueryInMemory(l, mid, aa, bb, 2 * rt);
		flag1 = 1;
	}
	if (bb > mid)
	{
		kb = QueryInMemory(mid + 1, r, aa, bb, 2 * rt + 1);
		flag2 = 1;
	}
	if (flag1 && flag2)
	{
		res.sum = ka.sum + kb.sum;
		res.lmaxi = max(ka.lmaxi, ka.sum + kb.lmaxi);
		res.rmaxi = max(kb.rmaxi, kb.sum + ka.rmaxi);
		res.maxi = max(ka.rmaxi + kb.lmaxi, max(ka.maxi, kb.maxi));
	}
	else
	{
		if (flag1)  //left
			res = ka;
		else
			res = kb;
	}
	return res;
}

int ionum = 0;
struct node QueryInDisk(int l, int r, int aa, int bb, int rt)
{
	if (aa <= l && bb >= r)
    {
		ionum++;
        file.seekg(sizeof(node)*(rt), ios::beg);
        file.read((char*)&outtree, sizeof(node));
        return outtree;
    }
	int mid = (l + r) / 2;
	struct node ka, kb, res;
	int flag1 = 0;
	int flag2 = 0;
	if (aa <= mid)
	{
		ka = QueryInDisk(l, mid, aa, bb, 2 * rt);
		flag1 = 1;
	}
	if (bb > mid)
	{
		kb = QueryInDisk(mid + 1, r, aa, bb, 2 * rt + 1);
		flag2 = 1;
	}
	if (flag1 && flag2)
	{
		res.sum = ka.sum + kb.sum;
		res.lmaxi = max(ka.lmaxi, ka.sum + kb.lmaxi);
		res.rmaxi = max(kb.rmaxi, kb.sum + ka.rmaxi);
		res.maxi = max(ka.rmaxi + kb.lmaxi, max(ka.maxi, kb.maxi));
	}
	else
	{
		if (flag1)  //left
			res = ka;
		else
			res = kb;
	}
	return res;
}

void WriteIndexFile()
{

	fstream file;
	file.open("index.dat", ios::out | ios::trunc | ios::binary);  //create index file
	if (!file)
		cout << "error! can't create file" << endl;
	file.write((char*)tree, sizeof(node)*nodenum+1);                  //write data to index file
	file.close();
}

int main()
{
	clock_t start_time, end_time;
	int left, right;
	int a = 1;                                             
	int b = g_data_line;
	//GetData("1000w.txt", a, b);
	//printf("total data sum is %d\n", g_data_num);
	
	//CreateTree(a, b, 1, data);//build a to b segmenttree
   // WriteIndexFile();
	//cout << "node number is " << nodenum << endl;
    file.open("index.dat", ios::in | ios::binary);
	
	while (1)
	{
		printf("Please input range: \n");
		cin >> left >> right;
		start_time = clock();
        //struct node res1 = QueryInMemory(a, b, left, right, 1);        // query aa to bb 's maxsub
		//cout << "maxsub sum is " << res1.maxi << " in memory" << endl;
		struct node res2 = QueryInDisk(a, b, left, right, 1);        // query aa to bb 's maxsub
		cout << "maxsub sum is " << res2.maxi << " in disk" << endl;
		cout << "I/O number is " << ionum << endl;
		end_time = clock();
		double duration_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		cout << "duration :" << duration_time << endl;
	}
	return 0;
}

