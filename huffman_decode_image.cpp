#include <bits/stdc++.h>
using namespace std;

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
		// cout << s << " " << label << endl; 
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

int main()
{
	auto start = chrono::high_resolution_clock::now(); 

	ifstream in("huffman_encoded.txt");
	int n;
	in>>n;
	pixel* root = new pixel(-1);
	for(int i=0; i<n; i++){
		string a,b,c;
		in>>a>>b>>c;
		add(root, 0, c, a);
	}

	// for checking
	// preoder(root, "");

	string w;
	in >> w;

	pixel* ptr = root;

	int height, width, num_channel;

	in>>height>>width>>num_channel;

	uint8_t* rgb_image;
    rgb_image = (uint8_t*) malloc(width*height*num_channel);
	
    int itr = 0;

	for(auto i:w){
		if(i=='0')
			ptr = ptr->left;
		else
			ptr = ptr->right;
		if(ptr->label != -1){
			rgb_image[itr++] = char(ptr->label);
			ptr = root;
		}
	}

	stbi_write_png("image.bmp", width, height, num_channel, rgb_image, width*num_channel);

	auto stop = chrono::high_resolution_clock::now(); 

	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);  

	cout<<(duration.count()/1000000.0)<<", ";


	return 0;
}