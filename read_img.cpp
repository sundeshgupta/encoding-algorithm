//https://github.com/nothings/stb

#include <bits/stdc++.h>
using namespace std;
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main()
{
	int width, height, bpp;

	uint8_t* rgb_image = stbi_load("/home/sundesh/Downloads/colombia.jpg", &width, &height, &bpp, 3);
	cout<<width<<" "<<height<<" "<<bpp<<"\n";
	int a[256][256][3];
	int cnt = 0;
	for (int i=0;i<256;++i)
	{
		for (int j=0;j<256;++j)
		{
			for (int k=0;k<3;++k)
			{
				a[i][j][k] = rgb_image[cnt];
				// cout<<a[i][j][k]<<" ";
				++cnt;
			}
			// cout<<"\n";
		}
		// cout<<"\n";
	}
    stbi_image_free(rgb_image);	

    return 0;
}