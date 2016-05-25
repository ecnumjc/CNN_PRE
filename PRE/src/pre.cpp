#include <iostream>
#include "pre.h"
#include <string>
#include <string.h>
using namespace std;


void usage()
{
	cout << "usage:<options>\n"
		<< "-train\t\tuse word2vec config file as input\n"
		<< "-wdict\t\tuse file(re_data.csv) as input\n"
		<< "-wvdict\t\tuse worddict file and wordvec file as input\n";
}


int main(int argc,char* argv[])
{
	pcnndatapre pd;
	if (argc < 2)
	{
		usage();
		return 1;
	}

	if (strcmp(argv[1], "-train") == 0)
	{
		if(!pd.train_word2vec(argv[2]))
			cerr<<"train word2vec failed!"<<endl;
	}
	if (strcmp(argv[1], "-wdict") == 0)
	{
		if (!pd.get_dict(argv[2]))
			cerr << "get_dict failed!" << endl;
	}
	if (strcmp(argv[1], "-wvdict") == 0)
	{
		if (!pd.get_wv(argv[2],argv[3]))
			cerr << "get_wv failed!" << endl;
	}
	return 0;
}
