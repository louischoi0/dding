#include "stdafx.h"
#include "butil.h"
using namespace std;

string block_to_string(blockContents b) {
	string str;

	str += "blockNum:" + to_string(b.blockNum);
	str += "parentHash:" + b.parentHash;
	str += "txnCount:" + to_string(b.txnCount);
	str += "txns:" + seriealize(b.txns);
	return str;
}

trans transacitonDumpBlock(string s, string r) {
	srand((unsigned int)time(NULL));
	int amount = rand();
	trans dumps;

	srand((unsigned int)time(NULL));

	std::array<int, 20> target = { -1, 1,1,1,1 - 1,-1,1,1,1, -1,-1, 1, 1, -1, 1,1,1,-1,1 };

	string lsd = s;
	string lre = r;

	for (int i = 0; i < BLOCK_SIZE; i += 2) {

		if (target[i] == 1)
		{
			lsd = s;
			lre = r;
		}

		else {
			lsd = r;
			lre = s;
		}

		dumps.push_back(fpair(lsd, -1 * rand()));
		dumps.push_back(fpair(lre, rand()));

	};

	return dumps;
}


void addTransaction(trans* trans, string s, string r, int amount) {
	trans->push_back(fpair(s, amount * -1));
	trans->push_back(fpair(r, amount));
}

state updateState(const state& pstate, const trans& txn) {
	auto tit = txn.begin();
	state lstate = pstate;

	for (auto tit = txn.begin(); tit != txn.end(); tit++)
	{
		if (lstate.find(tit->first) == lstate.end())
			lstate[tit->first] = tit->second;
		else
			lstate[tit->first] += tit->second;
	}

	return lstate;

}

int sumTransfered(const trans& ptxn) {
	int sum = 0;

	for (auto it = ptxn.begin(); it != ptxn.end(); it++)
		sum += it->second;

	return sum;
}

bool isValidTxn(const trans &ptxn, const state &pstate) {
	if (sumTransfered(ptxn) != 0)
		return false;

	for (auto it = ptxn.begin(); it != ptxn.end(); it++)
	{
		auto found_trans = pstate.find(it->first);

		if (found_trans != pstate.end()) {
			int account_balance = found_trans->second;
			int overflow_check = account_balance - it->second;
			if (overflow_check < 0) {
				return false;
			}

		}
	}
	return true;
}

void stateToTxn(trans* txn, const state& state) {
	auto it = state.begin();
	for (; it != state.end(); it++) {
		txn->push_back(*it);
	}
}

chain genesisTxnFactory(const state &initial) {
	trans gen_block_txn;
	state gen_state;

	auto it = initial.begin();
	for (; it != initial.end(); it++) {
		gen_block_txn.push_back(*it);
	}

	blockContents initial_block(1, "" , 1, gen_block_txn);
	string block_str = block_to_string(initial_block);
	string block_hash = sha256(block_str);

	return chain(initial_block);
}

chain appendChain(const chain &prev, const trans& trans) {
	blockContents block(prev.content.blockNum + 1, prev.hash, trans.size(), trans);
	return chain(block);
}

void mockValid() {

	state mock_state;
	trans mock_txn;
	mock_state.insert(fpair("louis", 5));
	mock_state.insert(fpair("bob", 5));

	genesisTxnFactory(mock_state);

	addTransaction(&mock_txn, "louis", "bob", 3);
	addTransaction(&mock_txn, "louis", "bob", 1);

	bool result = isValidTxn(mock_txn, mock_state);

	if (result)
		updateState(mock_state, mock_txn);

	cout << "txn valid : " << result << endl;
}