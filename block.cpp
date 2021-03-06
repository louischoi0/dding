// block.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

#include <iostream>
#include "sha256.h"
#include "butil.h"

using std::string;
using std::cout;
using std::endl;


int main(int argc, char *argv[])
{
	string input = "grape";
	string output1 = sha256(input);
	
	trans init;
	init.push_back(fpair("louis", 5000000));
	init.push_back(fpair("elice", 2000000));
	
	state initial = updateState(state() , init);
	
	auto tdummy = transacitonDumpBlock("elice", "louis");
	string strings = seriealize(tdummy);
	cout << strings << endl;
	
	auto initial_state = updateState(initial,tdummy);
	chain c0 = genesisTxnFactory(initial_state);
	chain c1 = appendChain();

	return 0;
}