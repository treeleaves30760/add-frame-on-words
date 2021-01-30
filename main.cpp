#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

typedef struct PIXEL
{
	unsigned char r, g, b;
	int is_Blank;
	int is_Converted;
} Pixels;

typedef struct IMG
{
	char header[3];
	int w, h;
	unsigned char l;
	Pixels **pic;
} Imgs;

int color_Table[12][3] = {
	255, 255, 255, // white
	255, 0, 0,	   // red
	255, 136, 0,   // tangerine
	255, 255, 0,   // yellow
	119, 255, 0,   // yellow green
	0, 255, 0,	   // green
	0, 255, 128,   // green blue
	0, 255, 255,   // light blue
	0, 128, 255,   // dark blue
	0, 0, 255,	   // blue
	153, 0, 255,   // purple
	255, 0, 255	   // light purple
};

char *color_Name[12] = {
	"white", "red", "tangerine", "yellow", "yellow green", "green", "green blue", "light blue", 
		"dark blue", "blue", "purple", "light purple"};

Imgs *open(char *Locate);

void PPMout(Imgs *p1, char *outFile);

Imgs *make_Frame(Imgs *p1, int color[3], int thickness);

void print(int d);

void setup(int (&Frame_color)[3], int &thickness, char (&file)[100], char (&out)[100]);

int main()
{
	int colors[3] = {255, 128, 0};
	int thickness = 8;
	Imgs *test;
	char file_Location[100] = ".\\test.ppm";
	char go_Location[100] = ".\\out.ppm";
	char check;

a:

	setup(colors, thickness, file_Location, go_Location);

	test = open(file_Location);
	printf("open success\n");

	test = make_Frame(test, colors, thickness);
	printf("make_Frame success\n");

	PPMout(test, go_Location);
	printf("Finish!\n");

	cout << "add another frame? (y/n):";
	cin >> check;
	if (check == 'y')
	{
		int i;
		for (i = 0; go_Location[i]; i++)
		{
			file_Location[i] = go_Location[i];
		}
		file_Location[i] = '\0';
		goto a;
	}

	return 0;
}

void setup(int (&Frame_color)[3], int &thickness, char (&file)[100], char (&out)[100])
{
	char check;

	cout << "Change Frame color and thickness or not (y/n):";
	cin >> check;
	if (check == 'y')
	{
		cout << "Use color table? (y/n):";
		cin >> check;
		if (check == 'n')
		{
			cout << "Frame_color[0] Frame_color[1] Frame_color[2] thickness\n";
			cin >> Frame_color[0] >> Frame_color[1] >> Frame_color[2] >> thickness;
		}
		else if (check == 'y')
		{
			cout << "Num R   G   B   Color\n";
			for (int i = 0; i < sizeof(color_Table) / (3 * sizeof(int)); i++)
			{
				printf("%3d %3d %3d %3d %s\n", i, color_Table[i][0], color_Table[i][1],
					   color_Table[i][2], color_Name[i]);
			}
			int c;
			cout << "what number to use:";
			cin >> c;
			cout << "you chose R:" << color_Table[c][0] << " G:"
				 << color_Table[c][1] << " B:" << color_Table[c][2] << endl;
			Frame_color[0] = color_Table[c][0];
			Frame_color[1] = color_Table[c][1];
			Frame_color[2] = color_Table[c][2];
			cout << "Please enter thickness:";
			cin >> thickness;
		}
	}

	cout << "Change infile path or not (Now path: " << file << " ) (y/n):";
	cin >> check;
	if (check == 'y')
	{
		cout << "relative path from now\n";
		cin >> file;
	}

	cout << "Change outfile path or not (Now path: " << out << " ) (y/n):";
	cin >> check;
	if (check == 'y')
	{
		cout << "relative path from now\n";
		cin >> out;
	}
}

Imgs *open(char *Locate)
{
	FILE *fin;
	Imgs *re;
	int i, j;

	printf("Locate = %s\n", Locate);

	fin = fopen(Locate, "r");
	//	if (fin == NULL) {
	//		printf("Fail to open\n");
	//		return NULL;
	//	}

	re = (Imgs *)malloc(sizeof(Imgs));
	fscanf(fin, "%s", re->header);
	fscanf(fin, "%d %d\n", &re->w, &re->h);
	fscanf(fin, "%d\n", &re->l);

	//	printf("ok header\n");

	re->pic = (Pixels **)malloc(sizeof(Pixels *) * re->w);

	for (i = 0; i < re->w; i++)
	{
		re->pic[i] = (Pixels *)malloc(sizeof(Pixels) * re->h);
	}

	//	printf("ok locate\n");

	for (j = 0; j < re->h; j++)
	{
		for (i = 0; i < re->w; i++)
		{
			re->pic[i][j].r = getc(fin);
			re->pic[i][j].g = getc(fin);
			re->pic[i][j].b = getc(fin);
			if (re->pic[i][j].r == re->pic[i][j].g && re->pic[i][j].r == re->pic[i][j].b && re->pic[i][j].r == 0)
				re->pic[i][j].is_Blank = 1;
			else
				re->pic[i][j].is_Blank = 0;
			re->pic[i][j].is_Converted = 0;
		}
	}

	printf("%s\n%d %d\n%d\n", re->header, re->w, re->h, re->l);

	fclose(fin);
	return re;
}

void PPMout(Imgs *p1, char *outFile)
{
	FILE *fout;					// the pointer to the output file
	int i, j;					// loop variable
	fout = fopen(outFile, "w"); // open a file with write mode

	// output the PPM format
	fprintf(fout, "%s\n%d %d\n%d\n", p1->header, p1->w, p1->h, p1->l);
	// output the images contents
	for (i = 0; i < p1->h; i++) {
		for (j = 0; j < p1->w; j++) {
			fprintf(fout, "%c%c%c", p1->pic[j][i].b, p1->pic[j][i].g,
					p1->pic[j][i].r);
		}
	}
	// close fout
	fclose(fout);
}

Imgs *make_Frame(Imgs *p1, int color[3], int thickness)
{
	int x, y;
	int i, j;
	int to_Convert;
	for (y = 0; y < p1->h; y++)
	{
		for (x = 0; x < p1->w; x++)
		{
			to_Convert = 0;
			if (p1->pic[x][y].is_Blank == 0)
			{
				//				printf("(%d, %d) is not blank\n", x, y);
				continue;
			}
			for (i = -thickness; i < thickness && !to_Convert; i++)
			{
				for (j = -thickness; j < thickness && !to_Convert; j++)
				{
					if (x + i >= 0 && y + j >= 0 && x + i < p1->w && y + j < p1->h &&
						p1->pic[x + i][y + j].is_Blank == 0 &&
						p1->pic[x + i][y + j].is_Converted == 0)
					{
						to_Convert = 1;
						//						printf("(%d, %d) need Covert\n", x, y);
					}
				}
			}

			if (to_Convert)
			{
				p1->pic[x][y].r = 255;	// color[0];
				p1->pic[x][y].g = 0;	// color[1];
				p1->pic[x][y].b = 128;	// color[2];
				p1->pic[x][y].is_Blank = 0;
				p1->pic[x][y].is_Converted = 1;
			}
			else
			{
				//				printf("(%d, %d) no need convert\n", x, y);
			}
		}
	}

	return p1;
}

void print(int d)
{
	printf("%d\n", d);
}
