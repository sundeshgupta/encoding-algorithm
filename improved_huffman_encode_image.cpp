#include <bits/stdc++.h>
using namespace std;
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

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

ofstream out("improved_huffman_encoded.txt");

void preoder(pixel* root, string s, int level, int parent){
	if(root->label < 256){
		codes[root->label].parent = parent;
		codes[root->label].code = s;
		// cout << setfill(' ') << setw(3) << root->label 
			// << " -> " << setw(20) << s << setw(4) << codes[root->label].parent << setw(20)<< fixed << setprecision(10) << root->freq << endl; 

		out << root->label << " -> " << s << "\n";

		return;
	}
	
	if(level == 3)
		parent = root->label;

	preoder(root->left , s + '0', level+1, parent);
	preoder(root->right, s + '1', level+1, parent);

	return;
}

string code_str = "";

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
			for (int k = 0; k < num_channel; ++k)
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
	for (int k = 0; k < num_channel; ++k)
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				if(f == 0){
					compression_size += codes[image[i][j][k]].code.size();
					out << codes[image[i][j][k]].code;
					code_str += codes[image[i][j][k]].code;
				}
				else if(codes[image[i][j][k]].parent == codes[last].parent){
					compression_size += 1;
					out << "1";
					code_str += "1";
					string code = codes[image[i][j][k]].code;
					out << code.substr(3, code.length());
					code_str += code.substr(3, code.length());
					compression_size += code.substr(3, code.length()).size();
				}
				else{
					compression_size += 1;
					out << "0";
					code_str += "0";
					compression_size += codes[image[i][j][k]].code.size();
					out << codes[image[i][j][k]].code;
					code_str += codes[image[i][j][k]].code;
				}
				if(codes[image[i][j][k]].parent == -1)
					f = 0;
				else{
					last = image[i][j][k];
					f = 1;
				}
			}
		}
	}
	out << "\n";
	out << height << " " << width << " " << num_channel << "\n";
	// cout << compression_size << " " << 256*256*8 << endl;


	cout<<filename<<",";

	auto stop = chrono::high_resolution_clock::now(); 
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start); 

	float NoBpp = ((float)compression_size)/(height*width*num_channel);
	float cp = (1 - ((float)compression_size)/(height*width*num_channel*8))*100;
	cout<<height*width*num_channel<<","<<cp<<",";
	cout<<NoBpp<<","<<(duration.count()/1000000.0)<<",";
	cout<<getValue()<<",";



	return 0;
}

