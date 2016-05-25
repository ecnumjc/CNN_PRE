#include<iostream>
#include<fstream>
#include<set>
#include<string>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<exception>
using namespace std;

class pcnndatapre
{
public:
	pcnndatapre();
	~pcnndatapre();
	//word2vec训练
	bool train_word2vec(char* filename);
	//词典
	bool get_dict(char* rawdatapath_jieba);
	//词典对应的向量
	bool get_wv(char* dictpath,char* word2vecfilepath);
	//查询词典dict.txt文件和wv.txt文件，得到pcnn测试和训练格式的语料
	bool get_pcnndata(string dictpath, string wvpath);
private:

};

pcnndatapre::pcnndatapre()
{
}

pcnndatapre::~pcnndatapre()
{
}

bool pcnndatapre::train_word2vec(char* filename)
{
	ifstream fin(filename);
	if(!fin)
	{
		cerr<<"open file failed!"<<endl;
		return false;
	}
        string line;
	while(getline(fin,line))
	{
		system(line.c_str());
	}
        return true;
}

bool pcnndatapre::get_dict(char* rawdatapath_jieba)
{
	ifstream fin(rawdatapath_jieba);//打开用于实体关系抽取文件
	ofstream fout("dict.txt");
	if (!fin)
	{
		cerr << "file open failed!" << endl;
		return false;
	}
	//去重
	set<string> dictset;
	string line;//一行，举例：第七/m ，/x 超越/v 《/x <Nf>宿醉2</Nf> 》/x (/x 8594/m 万/m )/x 位列/v <Na>布莱德利-库珀</Na> 主演/n 影片/n 开画/v 榜/n 第一位/m
	char* p;
	string str_Na = "<Na>", str_lNa = "</Na>";
	string str_Nab = "<Nab>", str_lNab = "</Nab>";
	string str_Nf = "<Nf>", str_lNf = "</Nf>";
	string str_Nc = "<Nc>", str_lNc = "</Nc>";
	string str_Nd = "<Nd>", str_lNd = "</Nd>";
	string str_Nl = "<Nl>", str_lNl = "</Nl>";
	string str_Nt = "<Nt>", str_lNt = "</Nt>";
	int i = 0;
	while (getline(fin, line))
	{
		i++;
		//cout << line << endl;
		try
		{
			p = strtok((char*)line.c_str(), " ");
			while (p)
			{
				string temp(p);
				string temp_top4 = temp.substr(0, 4);
				if (temp_top4 == str_Na)
				{
					temp.replace(temp.find(str_Na), str_Na.length(), "");  //replace 用法 
					temp.replace(temp.rfind(str_lNa), str_lNa.length(), ""); //repalce 用法，将最后一个"</Na>"替换掉
					dictset.insert(temp);
				}
				else if (temp_top4 == str_Nab)
				{
					temp.replace(temp.find(str_Nab), str_Nab.length(), "");  //replace 用法 
					temp.replace(temp.rfind(str_lNab), str_lNab.length(), ""); //repalce 用法，将最后一个"</Nab>"替换掉
					dictset.insert(temp);
				}
				else if (temp_top4 == str_Nf)
				{
					temp.replace(temp.find(str_Nf), str_Nf.length(), "");  //replace 用法 
					temp.replace(temp.rfind(str_lNf), str_lNf.length(), ""); //repalce 用法，将最后一个"</Nf>"替换掉
					dictset.insert(temp);
				}
				else if (temp_top4 == str_Nc)
				{
					temp.replace(temp.find(str_Nc), str_Nc.length(), "");  //replace 用法 
					temp.replace(temp.rfind(str_lNc), str_lNc.length(), ""); //repalce 用法，将最后一个"</Nc>"替换掉
					dictset.insert(temp);
				}
				else if (temp_top4 == str_Nd)
				{
					temp.replace(temp.find(str_Nd), str_Nd.length(), "");  //replace 用法 
					temp.replace(temp.rfind(str_lNd), str_lNd.length(), ""); //repalce 用法，将最后一个"</Nd>"替换掉
					dictset.insert(temp);
				}
				else if (temp_top4 == str_Nl)
				{
					temp.replace(temp.find(str_Nl), str_Nl.length(), "");  //replace 用法 
					temp.replace(temp.rfind(str_lNl), str_lNl.length(), ""); //repalce 用法，将最后一个"</Nl>"替换掉
					dictset.insert(temp);
				}
				else if (temp_top4 == str_Nt)
				{
					temp.replace(temp.find(str_Nt), str_Nt.length(), "");  //replace 用法 
					temp.replace(temp.rfind(str_lNt), str_lNt.length(), ""); //repalce 用法，将最后一个"</Nt>"替换掉
					dictset.insert(temp);
				}
				else
				{
					dictset.insert(temp.substr(0, temp.rfind("/")));
				}
				p = strtok(NULL, " ");
			}
		}
		catch (exception& e)
		{
			cerr << e.what() <<line<<i<< endl;
		}	
	}
	fin.close();
	set<string>::iterator it;
	for (it = dictset.begin(); it !=dictset.end(); it++)
	{
		fout << (*it) << endl;
	}
	fout.close();
	return true;
}

bool pcnndatapre::get_wv(char* dictpath, char* word2vecfilepath)
{
	const long long max_size = 2000;         // max length of strings
	const long long N = 40;                  // number of closest words that will be shown
	const long long max_w = 50;              // max length of vocabulary entries

	FILE *f;
	char st1[max_size];
	char *bestw[N];
	char file_name[max_size], st[100][max_size];
	float vec[max_size];
	long long words, size, a, b, c, cn, bi[100];
	float *M;
	char *vocab;

	strcpy(file_name, word2vecfilepath);
	f = fopen(file_name, "rb");
	if (f == NULL) 
	{
		printf("Input file not found\n");
		return false;
	}
	fscanf(f, "%lld", &words);
	fscanf(f, "%lld", &size);
	vocab = (char *)malloc((long long)words * max_w * sizeof(char));
	for (a = 0; a < N; a++) bestw[a] = (char *)malloc(max_size * sizeof(char));
	M = (float *)malloc((long long)words * (long long)size * sizeof(float));
	if (M == NULL)
	{
		printf("Cannot allocate memory: %lld MB %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
		return -1;
	}
	for (b = 0; b < words; b++)
	{
		a = 0;
		while (1)
		{
			vocab[b * max_w + a] = fgetc(f);
			if (feof(f) || (vocab[b * max_w + a] == ' ')) break;
			if ((a < max_w) && (vocab[b * max_w + a] != '\n')) a++;
		}
		vocab[b * max_w + a] = 0;
		for (a = 0; a < size; a++)
		{
			fread(&M[a + b * size], sizeof(float), 1, f);
		}
	}
	fclose(f);
	//查询
	ifstream fin(dictpath);
	ofstream fout("wvdict.txt");
	if (!fin)
	{
		cerr << "file open failed!" << endl;
		return false;
	}
	string line;
	while (getline(fin, line))
	{
		strcpy(st1, line.c_str());
		a = line.size();
		st1[a] = 0;
		cout << st1 << endl;
		cn = 0;
		b = 0;
		c = 0;
		while (1)
		{
			st[cn][b] = st1[c];
			b++;
			c++;
			st[cn][b] = 0;
			if (st1[c] == 0) break;
			if (st1[c] == ' ')
			{
				cn++;
				b = 0;
				c++;
			}
		}
		cn++;
		for (a = 0; a < cn; a++)
		{
			for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], st[a])) break;
			if (b == words) b = -1;
			bi[a] = b;
			printf("\nWord: %s  Position in vocabulary: %lld\n", st[a], bi[a]);
			if (b == -1)
			{
				printf("Out of dictionary word!\n");
				break;
			}
		}
		if (b == -1) continue;
		for (a = 0; a < size; a++)
			vec[a] = 0;
		for (b = 0; b < cn; b++)
		{
			if (bi[b] == -1)
				continue;
			for (a = 0; a < size; a++)
			{
				fout << M[a + bi[b] * size] << " ";
				printf("%f ", M[a + bi[b] * size]);
			}
			fout << endl;
			printf("\n");
		}
	}
	fin.close();
	fout.close();
	return true;
}
