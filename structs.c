typedef struct pix{
	float R;
	float G;
	float B;
	float alpha;
	int Y;
	int binPixel;
}pixel;
typedef struct imgStruct{
	int nPixeles;
	pixel** pixeles;
}imgStruct;
