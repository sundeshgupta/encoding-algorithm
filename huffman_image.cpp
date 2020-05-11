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

struct compare{
	bool operator() (pixel* a, pixel* b){
		return a->freq > b->freq;
	}
};

string code[256];

void preoder(pixel* root, string s){
	
	if(root->label != -1){
		cout << setfill(' ') << setw(3) << root->label 
			<< " -> " << setw(15) << s << setw(20) << fixed << setprecision(8) << root->freq << endl;  
		code[root->label] = s; 
		return;
	}

	preoder(root->left, s + '0');
	preoder(root->right, s + '1');

	return;
}


int main()
{
	int width, height, bpp;

	uint8_t* rgb_image = stbi_load("/home/sundesh/Downloads/colombia.jpg", &width, &height, &bpp, 1);

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
	
	int compression_size = 0;

	for (int i = 0;i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			compression_size += code[image[i][j]].size();
		}
	}

	cout << compression_size << " " << 256*256*8 << endl;


	return 0;
}

