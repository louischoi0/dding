#pragma once

#include "stdafx.h"
#define BLOCK_SIZE 20

using namespace std;

typedef pair<string, int> fpair;
typedef array<fpair, BLOCK_SIZE> tblock;

typedef std::map<string, int> state;

typedef std::vector<fpair> trans;
typedef std::vector<fpair> content;

template <class T>
string seriealize(const T& container) {

	auto it = container.begin();
	string result;
	for (; it != container.end(); it++) {

		if (it != container.begin())
			result += ",";

		result += it->first;
		result += ":";
		result += to_string(it->second);
	}

	return result;
}


template <size_t N, class T>
array<T, N> make_array(const T &v) {
	array<T, N> ret;
	ret.fill(v);
	return ret;
}

struct blockContents {
	int blockNum;
	string parentHash;
	int txnCount;
	trans txns;

	blockContents() {
		this->blockNum = -1;
	}

	blockContents(int nblock, string pHash, int txn_count, trans txns) {
		this->blockNum = nblock;
		this->parentHash = pHash;
		this->txnCount = txn_count;
		this->txns = txns;
	}

	blockContents(string phash, const blockContents& parent, const trans& trans) {
		this->blockNum = parent.blockNum + 1;
		this->parentHash = phash;
		this->txnCount = trans.size();
		this->txns = trans;
	}

};

string block_to_string(blockContents b);

struct chain {
	blockContents content;
	string hash;
	
	chain(const blockContents& c) {
		this->content = c;
		this->hash = sha256(block_to_string(c));
	}
	
};

trans transacitonDumpBlock(string s, string r);
void addTransaction(trans* trans, string s, string r, int amount);
state updateState(const state& pstate, const trans& txn);

int sumTransfered(const trans& ptxn);
bool isValidTxn(const trans &ptxn, const state &pstate);
void stateToTxn(trans* txn, const state& state);

chain genesisTxnFactory(const state &initial);
void mockValid();