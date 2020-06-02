#include <bits/stdc++.h>

using namespace std;

#define ll long long

const ll MIN = 0;
const ll MAX = 65535;

int lower[257];

string write_bits(bool bit, int bit_to_fall){
	string tmp;
	tmp += to_string(bit);
	while(bit_to_fall){
		tmp += to_string(!bit);
		bit_to_fall -= 1;
	}
	return tmp;
}

int main(){

	const ll ONE_QTR = MAX / 4 + 1;
	const ll HALF = 2 * ONE_QTR;
	const ll THREE_QTR = 3 * ONE_QTR;

	string text;
	cin >> text;

	int len = text.length() + 1;
	ll low[len];
	ll high[len];
	low[0] = MIN;
	high[0] = MAX;
	int current = 1;
	int i = 0;
	
	for(auto i: text){
		lower[i] += 1;
	}lower[256] = 1;

	for(int i=1; i<=256; i++){
		lower[i] += lower[i-1];
	}

	int bit_to_fall = 0;
	string code = "";

	while(i < len){
		if(i == len-1)	current = 256;
		else current = text[i];
		i += 1;

		ll range = high[i-1] - low[i-1] + 1;
		low[i] = low[i-1] + (range * lower[current-1]) / len;
		high[i] = low[i-1] + (range * lower[current]) / len - 1;

		while(true){
			if(high[i] < HALF){
				code += write_bits(0, bit_to_fall);
				bit_to_fall = 0;
			}
			else if(low[i] >= HALF){
				code += write_bits(1, bit_to_fall);
				bit_to_fall = 0;
				low[i] -= HALF;
				high[i] -= HALF;
			}
			else if(low[i] >= ONE_QTR and high[i] < THREE_QTR){
				bit_to_fall += 1;
				low[i] -= ONE_QTR;
				high[i] -= ONE_QTR;
			}
			else
				break;
			low[i] = 2 * low[i];
			high[i] = 2 * high[i] + 1;
		}
	}

	ll p = low[i-1];

	for(int j=15; j>=0; j--){
		if((p & (1LL<<j)))	code += '1';
		else code += '0';
	}

	cout << len << endl;

	for(int i=0; i<=256; i++)
		cout << lower[i] << endl;

	cout << code << endl;	

}

// qhnwnkuewhsqmgbbuqcljjivswmdkqtbxixmvtrrbljptnsnfwzqfjmafadrrwsofsbcnuvqhffbsaqxwpqcaceh