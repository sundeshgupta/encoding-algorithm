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

pixel* add(pixel* root, int i, string s, string label){

	if(root == NULL){
		root = new pixel(-1);
	}

	if(i == s.length()){
		cout << s << " " << label << endl; 
		root->label = stoi(label);
		return root;
	}

	if(s[i] == '0'){
		root->left = add(root->left, i+1, s, label);
	}
	else{
		root->right = add(root->right, i+1, s, label);
	}

	return root;
}

void preoder(pixel* root, string s){
	
	if(root == NULL)
		return;

	if(root->label != -1){
		cout << setfill(' ') << setw(3) << root->label 
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
	pixel* root = new pixel(-1);
	for(int i=0; i<n; i++){
		string a,b,c;
		cin>>a>>b>>c;
		add(root, 0, c, a);
	}

	// for checking
	preoder(root, "");

	string w;
	cin >> w;

	pixel* ptr = root;

	vector<int> v;

	for(auto i:w){
		if(i=='0')
			ptr = ptr->left;
		else
			ptr = ptr->right;
		if(ptr->label != -1){
			v.push_back(ptr->label);
			ptr = root;
		}
	}

	for(auto i:v){
		cout << (char)i ;
	} cout << endl;

	return 0;
}

// blvbjeanvnkjvnjlearknjnakrnekndcjbhbllivuekrjhggefskvhfbvkjdbsrjgk