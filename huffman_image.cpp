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

ofstream out("huffman_encoded.txt");

void preoder(pixel* root, string s){
	
	if(root->label != -1){
		// cout << setfill(' ') << setw(3) << root->label 
		// 	<< " -> " << setw(15) << s << setw(20) << fixed << setprecision(8) << root->freq << endl;  
		out << root->label << " -> " << s << "\n";
		code[root->label] = s; 
		return;
	}

	preoder(root->left, s + '0');
	preoder(root->right, s + '1');

	return;
}


int main(int argc, char** argv)
{
	int width, height, bpp, num_channel = 1;


	if (argc>2)
	{
		cout<<"Error: Too many arguments\n";
		return 0;
	}
	if (argc==2)
	{
		if (string(argv[1])!="grey" && string(argv[1])!="rgb")
		{
			cout<<"Error: Invalid Argument\n";
			return 0;
		}
		if (string(argv[1]) == "rgb")
			num_channel = 3;
	}
	
	string filename;
	cin>>filename;

	auto start = chrono::high_resolution_clock::now();

	uint8_t* rgb_image = stbi_load(filename.c_str(), &width, &height, &bpp, num_channel);

	uint8_t image[height][width][num_channel];

	int itr = 0;
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			for(int k = 0; k < num_channel; ++k)
				image[i][j][k] = rgb_image[itr++];

	stbi_image_free(rgb_image);	

	int hist[256] = {0}; 
	  
	for (int i = 0; i < height; ++i) 
	    for (int j = 0; j < width; ++j)
	    	for (int k = 0; k < num_channel; ++k) 
	        	hist[image[i][j][k]] += 1;

	int nodes = 0; 
	for (int i = 0; i < 256; i++)  
	    if (hist[i] != 0) 
	        nodes += 1; 

	out << nodes << "\n";

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
			for (int k = 0; k < num_channel; ++k)
			{
				compression_size += code[image[i][j][k]].size();
				out << code[image[i][j][k]];
			}
		}
	}
	// cout<<compression_size<<"\n";
	out << "\n";
	out << height << " " << width << " " << num_channel << "\n";
	
	auto stop = chrono::high_resolution_clock::now(); 
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start); 

	float NoBpp = ((float)compression_size)/(height*width*num_channel);
	float cp = (1 - ((float)compression_size)/(height*width*num_channel*8))*100;
	cout<<height<<"x"<<width<<"x"<<num_channel<<", "<<cp<<", ";
	cout<<NoBpp<<", "<<(duration.count()/1000000.0)<<", ";


	return 0;
}

