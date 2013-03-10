#pragma once

//#define LONG int
#define MY_ULONG unsigned int
//#define BYTE char
#define MY_UBYTE unsigned char
//#define SHORT short
//#define USHORT unsigned short
//#define WORD short int
//#define UWORD unsigned short int

typedef struct
{
	int width;
	int height;
	MY_UBYTE m;
	MY_UBYTE cres;
	MY_UBYTE pixbits;
	MY_UBYTE bc;
} GIF_Screen_Hdr; 

typedef union 
{
	struct
	{
		MY_UBYTE red;
		MY_UBYTE green;
		MY_UBYTE blue;
		MY_UBYTE pad;
	} cmap;
	MY_ULONG pixel;
} GIF_Color;

typedef struct
{
	int left;
	int top;
	int width;
	int height;
	MY_UBYTE m;
	MY_UBYTE i;
	MY_UBYTE pixbits;
	MY_UBYTE reserved;
} GIF_Image_Hdr;

typedef struct 
{
	MY_UBYTE valid;
	MY_UBYTE data;
	MY_UBYTE first;
	MY_UBYTE res;
	int last;
} GIF_Table;

class CGifEncoder
{
public:
	CSBASE_API CGifEncoder(void)
	{
		imagec = 0;
		pic_i;

		disposal = 2;
		repeats = 0;
		delay = 50;
		transp = -1;
		pos_set = 0;
		xpos = 0;
		ypos = 0;

		m_fpResult = NULL;
		m_fFirstFrame = true;
	}

	CSBASE_API ~CGifEncoder(void);

private:
	const static unsigned int MAXVAL	= 4100;
	const static unsigned int MAXVALP	= 4200;

	TCHAR gif_file_name[256];

	GIF_Color gif_cmap[256];

	GIF_Screen_Hdr gifscrn;
	GIF_Image_Hdr gifimage;
	GIF_Table table[MAXVALP];

	MY_ULONG root_code_size,code_size,CLEAR,EOI,INCSIZE;
	MY_ULONG nextab;
	
	MY_UBYTE gif_buff[MAXVALP];
	MY_ULONG gif_block_size;

	int num_bits,bits;

	int imagec;
	int pic_i;

	int disposal;
	int repeats;
	int delay;
	int transp;
	int pos_set;
	int xpos, ypos;

	FILE * m_fpResult;
	bool m_fFirstFrame;

public:
	// Begin to make an animated gif, allocate necessary resource and assign some parameters.
	CSBASE_API bool StartGIF(const TCHAR * szGIFName, int nDelay = 50);
	
	// Add a new frame into animated gif with nRepeatCount times
	CSBASE_API bool AddFrame(const TCHAR * szFrame, const int nRepeatCount = 1);
	
	// Save current animated gif and release all resource
	CSBASE_API bool EndGIF(void);

private:
	/*
	* Read a GIF file, outputting to fname as we go.
	* It would be faster to read and write the individual blocks,
	* but eventually we'd like to optimize based on changes from
	* previous images(ie only a small section of the image changed.
	*/
	bool GIF_Read_File(const TCHAR * fname, FILE * fpout);
	bool GIF_Decompress(FILE * fp, FILE * fpout);
	bool GIF_Get_Next_Entry(FILE * fp);
	/*  body is associated string next is code to add to that string to form associated string for index */
	bool GIF_Add_To_Table(MY_ULONG body, MY_ULONG next, MY_ULONG index);
	bool GIF_Send_Data(int index);
	void GIF_Init_Table(void);
	void GIF_Clear_Table(void);   
	bool GIF_Get_Code(FILE * fp, FILE * fpout, MY_ULONG & code); /* get code depending of current LZW code size */
	/* read GIF header, do some converting and write it into output stream	*/
	void GIF_Screen_Header(FILE * fp, FILE * fout);
	void GIF_Image_Header(FILE * fp, FILE * fout);
	int GIF_Get_Short(FILE * fp, FILE * fout, bool fFirst);
	void GIF_Put_Short(FILE * fout, unsigned int data);
};
