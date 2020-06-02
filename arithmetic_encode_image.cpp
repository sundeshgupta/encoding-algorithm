#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const ll MIN = 0;
const ll MAX = 4294967295;

ll lower[257];
vector <ll> low, high;

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

string write_bits(bool bit, int bit_to_fall){
	string tmp;
	tmp += to_string(bit);
	while(bit_to_fall){
		tmp += to_string(!bit);
		bit_to_fall -= 1;
	}
	return tmp;
}

int to_int(int _pos, string &encode){
	int n = 0;
	for (int i = _pos; i < sizeof(short) * 8 + _pos; i++)
	{
		n <<= 1;
		n |= encode[i] - '0';
	}
	return n;
}

ll add_bit(ll value, int count_taken, bool &flag, string &encode){
	bitset<16> a(value);

	if(flag == 1){
		a.reset(0);
	}
	else if(count_taken > encode.length()){
		a.set(0);
		flag = 1;
	}
	else if(encode[count_taken] == '1'){
		a.set(0);
	}
	else if(encode[count_taken] == '0'){
		a.reset(0);
	}

	value = (ll)(a.to_ulong());
	return value;
}

ofstream out("arithmetic_encoded.txt");

string image_pixels = "";
string code = "";
uint8_t* rgb_image;

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

	rgb_image = stbi_load(filename.c_str(), &width, &height, &bpp, num_channel);

	const ll ONE_QTR = MAX / 4 + 1;
	const ll HALF = 2 * ONE_QTR;
	const ll THREE_QTR = 3 * ONE_QTR;

	int len = height*width*num_channel + 1;

	low.resize(len+1);
	high.resize(len+1);
	
	low[0] = MIN;
	high[0] = MAX;
	
	int current = 1;
	int i = 0;
	
	int itr = 0;
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			for(int k = 0; k < num_channel; ++k){
				if (int(rgb_image[itr])==0)
					rgb_image[itr] = char(1);
				lower[int(rgb_image[itr++])] += 1;
			}

	lower[256] = 1;

	for(int i=1; i<=256; i++){
		lower[i] += lower[i-1];
	}

	int bit_to_fall = 0;

	while(i < len){
		if(i == len-1)	current = 256;
		else current = rgb_image[i];
		i += 1;

		ll range = high[i-1] - low[i-1] + 1;
		low[i] = low[i-1] + (range * lower[current-1]) / len;
		high[i] = low[i-1] + (range * lower[current]) / len - 1;

		while(true){
			if(high[i] < HALF){
				code += write_bits(0, bit_to_fall);
				bit_to_fall = 0;
			}
			else if(low[i] >= HALF){
				code += write_bits(1, bit_to_fall);
				bit_to_fall = 0;
				low[i] -= HALF;
				high[i] -= HALF;
			}
			else if(low[i] >= ONE_QTR and high[i] < THREE_QTR){
				bit_to_fall += 1;
				low[i] -= ONE_QTR;
				high[i] -= ONE_QTR;
			}
			else
				break;
			low[i] = 2 * low[i];
			high[i] = 2 * high[i] + 1;
		}

	}

	ll p = low[i-1];

	for(int j=31; j>=0; j--){
		if((p & (1LL<<j)))	code += '1';
		else code += '0';
	}

	out << len << " ";

	for(int i=0; i<=256; i++)
		out << lower[i] << " ";

	out << code << "\n";
	
	out << height << " " << width << " " << num_channel << "\n";	

	auto stop = chrono::high_resolution_clock::now(); 
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start); 

	cout<<filename<<",";
	float NoBpp = ((float)code.size())/(height*width*num_channel);
	float cp = (1 - ((float)code.size())/(height*width*num_channel*8))*100;
	cout<<height*width*num_channel<<","<<cp<<",";
	cout<<NoBpp<<","<<(duration.count()/1000000.0)<<",";
	cout<<getValue()<<",";

	return 0;	
}