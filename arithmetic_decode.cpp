#include <bits/stdc++.h>

using namespace std;

#define ll long long

const ll MIN = 0;
const ll MAX = 65535;

int lower[257];

int to_int(int _pos, string &encode){
	int n = 0;
	for (int i = _pos; i < sizeof(short) * 8 + _pos; i++)
	{
		n <<= 1;
		n |= encode[i] - '0';
	}
	return n;
}

ll add_bit(ll value, int count_taken, bool &flag, string &encode){
	bitset<16> a(value);

	if(flag == 1){
		a.reset(0);
	}
	else if(count_taken > encode.length()){
		a.set(0);
		flag = 1;
	}
	else if(encode[count_taken] == '1'){
		a.set(0);
	}
	else if(encode[count_taken] == '0'){
		a.reset(0);
	}

	value = (ll)(a.to_ulong());
	return value;
}

int main(){

	const ll ONE_QTR = MAX / 4 + 1;
	const ll HALF = 2 * ONE_QTR;
	const ll THREE_QTR = 3 * ONE_QTR;

	ll len;
	string code;

	cin >> len;
	for(int i=0; i<=256; i++)	cin >> lower[i];
	cin >> code;

	string decode = "";

	ll _low[len];
	ll _high[len];

	_low[0] = MIN;
	_high[0] = MAX;

	ll value = to_int(0, code);
	int count_taken = 16;
	bool flag = 0;

	for(int i=1;i<len;i++){
		ll range = (_high[i-1] - _low[i-1] + 1);
		ll nxt = (((value - _low[i-1]) + 1) * len - 1) / range;

		int symbol;
		for(symbol = 1; lower[symbol] <= nxt; symbol++);
		
		_low[i] = _low[i-1] + (range * lower[symbol-1]) / len;
		_high[i] = _low[i-1] + (range * lower[symbol]) / len - 1;

		if(symbol == 256)
			break;

		decode += char(symbol);

		for(;;){
			if(_high[i] >= HALF){
				if(_low[i] >= HALF){
					value -= HALF;
					_low[i] -= HALF;
					_high[i] -= HALF;
				}
				else if (_low[i] >= ONE_QTR && _high[i] < THREE_QTR)
				{
					value -= ONE_QTR;
					_low[i] -= ONE_QTR;
					_high[i] -= ONE_QTR;
				}
				else
				{
					break;
				}
			}
			_low[i] = 2 * _low[i];
			_high[i] = 2 * _high[i] + 1;
			value = add_bit(2 * value, count_taken, flag, code);
			count_taken++;
		}
	}

	cout << decode << endl;

}