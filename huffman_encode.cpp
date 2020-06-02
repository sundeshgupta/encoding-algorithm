#include <bits/stdc++.h>

using namespace std;

struct pixel{
	int freq;
	int label;
	pixel* left, *right;

	pixel(int x, int y):	label(x), freq(y){
		left = NULL;
		right = NULL;
	}

	void print(){
		cout << label << " " << freq << endl;
	}
};

struct compare{
	bool operator() (pixel* a, pixel* b){
		return a->freq > b->freq;
	}
};

string code[256];

void preoder(pixel* root, string s){
	
	if(root->label != -1){
		cout << setfill(' ') << setw(3) << root->label 
			<< " -> " << setw(7) << s << endl;  
		code[root->label] = s; 
		return;
	}

	preoder(root->left, s + '0');
	preoder(root->right, s + '1');

	return;
}

int main(){
	// Uploading Image

	string s;
	cin >> s;

	int freq[256] = {0};

	for(auto i: s){
		freq[i] += 1;
	}

	priority_queue<pixel*, vector<pixel*>, compare> pq;

	for(int i = 0; i < 256; i++){
		if (freq[i] > 0){
			pixel* tmp = new pixel(i, freq[i]);
			pq.push(tmp);
		}
	}

	while(pq.size() > 1){
		pixel* first = pq.top();
		pq.pop();
		pixel* second = pq.top();
		pq.pop();
		pixel* tmp = new pixel(-1, first->freq + second->freq);
		tmp->left = first;
		tmp->right = second;
		pq.push(tmp);
	}

	pixel* root = pq.top();

	preoder(root, "");
	
	string w;

	for(auto i:s){
		w += code[i];
	}

	cout << w << endl;
	cout << w.length() << " " << s.length() * 4 << endl;

	return 0;
}