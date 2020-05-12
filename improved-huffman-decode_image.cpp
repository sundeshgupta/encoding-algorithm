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

bool flag[256];
int cnt = 256;

pixel* add(pixel* root, int i, string s, string label, int level = 0){

	if(root == NULL){
		root = new pixel(cnt++);
	}

	if(i == s.length()){
		cout << s << " " << (stoi(label)) << " " << label << endl; 
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
		cout << setfill(' ') << setw(3) << root->label 
			<< " -> " << setw(7) << s << endl;  
		return;
	}

	preoder(root->left, s + '0');
	preoder(root->right, s + '1');

	return;
}

int main(){
	ifstream in("improved_huffman_encoded.txt");
	int n;
	in>>n;
	pixel* root = new pixel(cnt++);
	for(int i=0; i<n; i++){
		string b, c;
		string a;
		in>>a>>b>>c;
		add(root, 0, c, a);
	}

	// for checking
	preoder(root, "");

	string w;
	in >> w;

	pixel* ptr = root;
	pixel* tmp = root;

	int height, width, num_channel;

	in>>height>>width>>num_channel;

	uint8_t* rgb_image;
    rgb_image = (uint8_t*) malloc(width*height*num_channel);
	
    int itr = 0;
	bool f = 0;
	int level = 0;
	for(auto i:w){
		if(f){
			if(i == '0'){
				// do nothing
			}
			else{
				ptr = tmp;
				level = 3;
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
			
			rgb_image[itr++] = ptr->label;
			
			level = 0;

			if(flag[ptr->label])
				f = 1;
			
			ptr = root;
		}
	}

	stbi_write_png("improved_image.png", width, height, num_channel, rgb_image, width*num_channel);


	return 0;
}