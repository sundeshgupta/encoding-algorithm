#include <bits/stdc++.h>

using namespace std;

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
		// cout << s << " " << (stoi(label)) << " " << label << endl; 
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

	auto start = chrono::high_resolution_clock::now(); 

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
	// preoder(root, "");

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
	uint8_t* rgb_image_restore;
	rgb_image_restore = (uint8_t*) malloc(width*height*num_channel);
	itr = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			rgb_image_restore[itr++] = rgb_image[i*width + j];
			if (num_channel == 2 || num_channel == 3)
			{
				rgb_image_restore[itr++] = rgb_image[i*width + j + width*height];
			}
			if (num_channel == 3)
			{
				rgb_image_restore[itr++] = rgb_image[i*width + j + 2*width*height];
			}
		}
	}

	stbi_write_png("improved_image.bmp", width, height, num_channel, rgb_image_restore, width*num_channel);

	auto stop = chrono::high_resolution_clock::now(); 

	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);  

	cout<<(duration.count()/1000000.0)<<","<<getValue()<<"\n";


	return 0;
}