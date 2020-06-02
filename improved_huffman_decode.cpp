#include <bits/stdc++.h>

using namespace std;

struct pixel{
	int label;
	pixel* left, *right;

	pixel(int x):	label(x){
		left = NULL;
		right = NULL;
	}

};

bool flag[256];
int cnt = 256;

pixel* add(pixel* root, int i, string s, string label, int level = 0){

	if(root == NULL){
		root = new pixel(cnt++);
	}

	if(i == s.length()){
		cout << s << " " << char(stoi(label)) << " " << label << endl; 
		root->label = stoi(label);
		if(level > 3){
			flag[root->label] = 1;
		}
		return root;
	}

	if(s[i] == '0'){
		root->left = add(root->left, i+1, s, label, level+1);
	}
	else{
		root->right = add(root->right, i+1, s, label, level+1);
	}

	return root;
}

void preoder(pixel* root, string s){
	
	if(root == NULL)
		return;

	if(root->label < 256){
		cout << setfill(' ') << setw(3) << (char)root->label 
			<< " -> " << setw(7) << s << endl;  
		return;
	}

	preoder(root->left, s + '0');
	preoder(root->right, s + '1');

	return;
}

int main(){
	int n;
	cin>>n;
	pixel* root = new pixel(cnt++);
	for(int i=0; i<n; i++){
		string b, c;
		string a;
		cin>>a>>b>>c;
		add(root, 0, c, a);
	}

	// for checking
	preoder(root, "");

	string w;
	cin >> w;

	pixel* ptr = root;
	pixel* tmp = root;

	vector<int> v;
	bool f = 0;
	int level = 0;

	for(auto i:w){
		
		if(f){
			if(i == '0'){
				// do nothing
			}
			else{
				ptr = tmp;
			}
			f = 0;
			continue;
		}

		if(i=='0')
			ptr = ptr->left;
		else
			ptr = ptr->right;
		
		level += 1;

		if(level == 3)
			tmp = ptr;

		if(ptr->label < 256){
			
			v.push_back(ptr->label);
			
			level = 0;

			// cout << ptr->label << endl;
			// cout << char(ptr->label) << flag[ptr->label] << endl;

			if(flag[ptr->label])
				f = 1;
			
			ptr = root;
		}
	}

	for(auto i:v){
		cout << (char)i ;
	} cout << endl;

	return 0;
}

// blvbjeanvnkjvnjlearknjnakrnekndcjbhbllivuekrjhggefskvhfbvkjdbsrjgk