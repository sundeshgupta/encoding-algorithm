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

struct huffcode{
	int label;
	int parent;
	string code;
};

struct compare{
	bool operator() (pixel* a, pixel* b){
		return a->freq > b->freq;
	}
};

ofstream out("huffman.txt");

huffcode codes[256];

void preoder(pixel* root, string s, int level, int parent){
	
	if(root->label < 256){
		// if(level >= 3){
		// 	codes[root->label].parent = parent;
		// }
		// else{
		// 	codes[root->label].parent = -1;
		// }
		codes[root->label].parent = parent;
		codes[root->label].code = s;
		cout << setfill(' ') << setw(3) << root->label 
			<< " -> " << setw(7) << s << setw(4) << codes[root->label].parent<< endl; 
		return;
	}

	out << root->label << " -> " << root->left->label << " [ label = \"" << s+'0' << "\"];"<<endl;
	out << root->label << " -> " << root->right->label << " [ label = \"" << s+'1' << "\"];" << endl; 
	
	if(level == 3)
		parent = root->label;

	preoder(root->left , s + '0', level+1, parent);
	preoder(root->right, s + '1', level+1, parent);

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

	int cnt = 256;

	while(pq.size() > 1){
		pixel* first = pq.top();
		pq.pop();
		pixel* second = pq.top();
		pq.pop();
		pixel* tmp = new pixel(cnt++, first->freq + second->freq);
		tmp->left = first;
		tmp->right = second;
		pq.push(tmp);
	}

	pixel* root = pq.top();

	out << "digraph{" << endl;

	preoder(root, "", 0 , -1);
	
	out << "}" << endl;

	// string str = "dot -Tpng huffman.txt -o huffman.png";	// running dot file through terminal
	// const char *command = str.c_str();					// system function is copied from https://www.geeksforgeeks.org/system-call-in-c/
	// system(command);


	string w;
	int last = -1;
	bool f = 0;

	for(int i = 0; i < s.length(); i++){
		if(f == 0){
			w += codes[s[i]].code;
		}
		else if(codes[s[i]].parent == codes[last].parent){
			w += '1';
			string code = codes[s[i]].code;
			w += code.substr(3, code.length());
		}
		else{
			w += '0';
			w += codes[s[i]].code;
		}
		if(codes[s[i]].parent == -1)
			f = 0;
		else{
			last = s[i];
			f = 1;
		}
	}

	cout << w << endl;
	cout << w.length() << " " << s.length() * 4 << endl;

	return 0;
}