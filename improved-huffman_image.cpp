#include <bits/stdc++.h>
using namespace std;
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct pixel{
	float freq;
	int label;
	pixel* left, *right;

	pixel(int x, float y):	label(x), freq(y){
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

huffcode codes[256];

void preoder(pixel* root, string s, int level, int parent){
	if(root->label < 256){
		codes[root->label].parent = parent;
		codes[root->label].code = s;
		cout << setfill(' ') << setw(3) << root->label 
			<< " -> " << setw(20) << s << setw(4) << codes[root->label].parent << setw(20)<< fixed << setprecision(10) << root->freq << endl; 
		return;
	}
	// out << root->label << " -> " << root->left->label << " [ label = \"" << s+'0' << "\"];"<<endl;
	// out << root->label << " -> " << root->right->label << " [ label = \"" << s+'1' << "\"];" << endl; 
	
	if(level == 3)
		parent = root->label;

	preoder(root->left , s + '0', level+1, parent);
	preoder(root->right, s + '1', level+1, parent);

	return;
}



int main()
{
	int width, height, bpp;

	uint8_t* rgb_image = stbi_load("colombia.jpg", &width, &height, &bpp, 1);

	int image[height][width];
	int itr = 0;
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			image[i][j] = rgb_image[itr++];

	stbi_image_free(rgb_image);	

	int hist[256] = {0}; 
	  
	for (int i = 0; i < height; i++) 
	    for (int j = 0; j < width; j++) 
	        hist[image[i][j]] += 1;

	int totpix = height*width;

	float freq[256] = {0};
	
	for (int i = 0; i < 256; ++i)
		freq[i] = ((float)hist[i])/totpix;



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

	preoder(root, "", 0, -1);
	

	int compression_size = 0;
	int last = -1;
	bool f = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if(f == 0){
				compression_size += codes[image[i][j]].code.size();
			}
			else if(codes[image[i][j]].parent == codes[last].parent){
				compression_size += 1;
				string code = codes[image[i][j]].code;
				compression_size += code.substr(3, code.length()).size();
			}
			else{
				compression_size += 1;
				compression_size += codes[image[i][j]].code.size();
			}
			if(codes[image[i][j]].parent == -1)
				f = 0;
			else{
				last = image[i][j];
				f = 1;
			}
		}
	}

	cout << compression_size << " " << 256*256*8 << endl;


	return 0;
}

