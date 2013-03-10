#include "StdAfx.h"

#include "gifencoder.h"

#include <assert.h>
#include <math.h>
#include <tchar.h>

const static MY_ULONG gif_mask[16] = {1,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,0,0};
const static MY_ULONG gif_ptwo[16] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,0,0};

CGifEncoder::~CGifEncoder(void)
{
	if (NULL != m_fpResult)
	{
		fclose(m_fpResult);
		m_fpResult = NULL;
	}
}

// Begin to make an animation gif, add first frame into it.
bool CGifEncoder::StartGIF(const TCHAR * szGIFName, int nDelay)
{
	assert(NULL != szGIFName);
	if (NULL == szGIFName || szGIFName[0] == _T('\0'))
	{
		return false;
	}
	_tcsncpy(gif_file_name, szGIFName, 255);
	m_fpResult = _tfopen(szGIFName, _T("wb+"));
	if (NULL == m_fpResult)
	{
		return false;
	}

	this->delay = nDelay;
	m_fFirstFrame = true;
	return true;
}

// Add a new frame into animated gif with nRepeatCount times
bool CGifEncoder::AddFrame(const TCHAR * szFrame, const int nRepeatCount)
{
	assert(NULL != szFrame);
	if (NULL == szFrame || szFrame[0] == _T('\0') || NULL == m_fpResult)
	{
		return false;
	}

	for(int i = 0; i < nRepeatCount; i ++)
	{
		if (!GIF_Read_File(szFrame, m_fpResult))
		{
			fclose(m_fpResult);
			m_fpResult = NULL;
			return false;
		};
		m_fFirstFrame = false;
	}
	return true;
}

// Save current animated gif and release all resource
bool CGifEncoder::EndGIF(void)
{
	if (NULL == m_fpResult)
	{
		return false;
	}

	m_fFirstFrame = true;
	fputc(';', m_fpResult); /* image separator */
	fclose(m_fpResult);
	return true;
}

/*
* Read a GIF file, outputting to fname as we go.
* It would be faster to read and write the individual blocks,
* but eventually we'd like to optimize based on changes from
* previous images(ie only a small section of the image changed.
*/
bool CGifEncoder::GIF_Read_File(const TCHAR * fname, FILE * fpout)
{
	FILE *fp;
	int i;

	if ( (fp=_tfopen(fname, _T("rb")))==0)
	{ 
		return false;
	}

	GIF_Screen_Header(fp, fpout);

	/*** read until  ,  separator */
	do
	{
		i=fgetc(fp);
		if ( (i<0) && feof(fp))
		{
			fclose(fp);
			return false;
		}
	} while(i != ',');

	fputc(',', fpout); /* image separator */

	GIF_Image_Header(fp, fpout);

	/*** Setup ACTION for IMAGE */

	if (!GIF_Decompress(fp, fpout))
	{
		return false;
	}
	fputc(0, fpout);  /* block count of zero */

	fclose(fp);
	return true;
}

bool CGifEncoder::GIF_Decompress(FILE * fp, FILE * fpout)
{
	MY_ULONG code,old;

	pic_i = 0;
	bits=0;
	num_bits=0;
	gif_block_size=0;
	
	/* starting code size of LZW */
	root_code_size=(fgetc(fp) & 0xff);
	fputc(root_code_size, fpout);
	GIF_Clear_Table();                /* clear decoding symbol table */

	if (!GIF_Get_Code(fp, fpout, code))
	{
		return false;
	}

	if (code == CLEAR) 
	{
		GIF_Clear_Table(); 
		if (!GIF_Get_Code(fp, fpout, code))
		{
			return false;
		}
	}
	/* write code(or what it currently stands for) to file */
	if (!GIF_Send_Data(code))
	{
		return false;
	}

	old = code;
	if (!GIF_Get_Code(fp, fpout, code))
	{
		return false;
	}
	do
	{
		if (table[code].valid == 1)    /* if known code */
		{
			/* send it's associated string to file */
			if (!GIF_Send_Data(code))
			{
				return false;
			}
			if (!GIF_Get_Next_Entry(fp))	/* get next table entry (nextab) */
			{
				return false;
			}
			if (!GIF_Add_To_Table(old, code, nextab))  /* add old+code to table */
			{
				return false;
			}
			old = code;
		}
		else      /* code doesn't exist */
		{
			if (!GIF_Add_To_Table(old, old, code))   /* add old+old to table */
			{
				return false;
			}
			if (!GIF_Send_Data(code))
			{
				return false;
			}
			old = code;
		}
		if (!GIF_Get_Code(fp, fpout, code))
		{
			return false;
		}
		if (code == CLEAR)
		{ 
			GIF_Clear_Table();
			if (!GIF_Get_Code(fp, fpout, code))
			{
				return false;
			}
			if (!GIF_Send_Data(code))
			{
				return false;
			}
			old = code;
			if (!GIF_Get_Code(fp, fpout, code))
			{
				return false;
			}
		}
	} while(code != EOI);
	return true;
}

bool CGifEncoder::GIF_Get_Next_Entry(FILE * fp)
{
	/* table walk to empty spot */
	while(  (table[nextab].valid == 1)
		&&(nextab < MAXVAL)
		) nextab++;
	/* 
	* Ran out of space??!?  Something's roached 
	*/
	if (nextab >= MAXVAL)    
	{ 
		return false;
	}
	if (nextab == INCSIZE)   /* go to next table size (and LZW code size ) */
	{
		/* fprintf(stderr,"GetNext INCSIZE was %ld ",nextab); */
		code_size++;
		INCSIZE = (INCSIZE * 2) + 1;
		if (code_size >= 12)
		{
			code_size = 12;
		}
	}
	return true;
}

/*  body is associated string next is code to add to that string to form associated string for index */     
bool CGifEncoder::GIF_Add_To_Table(MY_ULONG body, MY_ULONG next, MY_ULONG index)
{
	if (index>MAXVAL)
	{ 
		return false;
	}
	else
	{
		table[index].valid=1;
		table[index].data=table[next].first;
		table[index].first=table[body].first;
		table[index].last=body;
		return true;
	}
}

bool CGifEncoder::GIF_Send_Data(int index)
{
	int i,j;
	i=0;
	
	do         /* table walk to retrieve string associated with index */
	{ 
		gif_buff[i] = table[index].data; 
		i++;
		index = table[index].last;
		if (i > MAXVAL)
		{ 
			return false;
		}
	} while(index>=0);

	/* now invert that string since we retreived it backwards */
	i--;
	for(j=i;j>=0;j--)
	{
		/*pic[pic_i] = gif_buff[j] | gif_pix_offset;*/
		pic_i++;
	}
	return true;
}

/* 
* initialize string table 
*/
void CGifEncoder::GIF_Init_Table(void)
{
	register int maxi,i;

	maxi=gif_ptwo[root_code_size];
	for(i=0; i<maxi; i++)
	{
		table[i].data=i;   
		table[i].first=i;
		table[i].valid=1;  
		table[i].last = -1;
	}
	CLEAR=maxi; 
	EOI=maxi+1; 
	nextab=maxi+2;
	INCSIZE = (2*maxi)-1;
	code_size=root_code_size+1;
}


/* 
* clear table 
*/
void CGifEncoder::GIF_Clear_Table(void)   
{
	register int i;
	for(i=0;i<MAXVAL;i++) table[i].valid=0;
	GIF_Init_Table();
}

bool CGifEncoder::GIF_Get_Code(FILE * fp, FILE * fpout, MY_ULONG &code) /* get code depending of current LZW code size */
{
	register int tmp;

	while((unsigned int)num_bits < code_size)
	{
		/**** if at end of a block, start new block */
		if (gif_block_size==0) 
		{
			tmp = fgetc(fp);
			if (tmp >= 0 )
			{
				fputc(tmp, fpout);
				gif_block_size=(MY_ULONG)(tmp);
			}
			else
			{
				return false;
			}
		}

		tmp = fgetc(fp);   gif_block_size--;
		if (tmp >= 0)
		{
			fputc(tmp, fpout);
			bits |= ( ((MY_ULONG)(tmp) & 0xff) << num_bits );
			num_bits+=8;
		}
		else
		{
			return false;
		}
	}

	code = bits & gif_mask[code_size];
	bits >>= code_size;
	num_bits -= code_size; 


	if (code>MAXVAL)
	{ 
		code=EOI;
	}

	if (code==INCSIZE)
	{
		if (code_size<12)
		{
			code_size++; INCSIZE=(INCSIZE*2)+1;
		}
	}

	return true;
}

/* read GIF header, do some converting and write it into output stream	*/
void CGifEncoder::GIF_Screen_Header(FILE * fp, FILE * fout)
{
	int temp,i;

	for(i = 0; i < 6; i++)
	{
		temp = fgetc(fp);
	}	//Skip the "GIF89a"
	
	if(m_fFirstFrame) 
	{
		fputs("GIF89a",fout);	//put "GIF89a"
	}
	gifscrn.width  = GIF_Get_Short(fp, fout, m_fFirstFrame);
	gifscrn.height = GIF_Get_Short(fp, fout, m_fFirstFrame);
	temp=fgetc(fp);
	if (m_fFirstFrame)
	{
		fputc(temp,	fout);
	}
	gifscrn.m       =  temp & 0x80;
	gifscrn.cres    = (temp & 0x70) >> 4;
	gifscrn.pixbits =  temp & 0x07;
	gifscrn.bc  = fgetc(fp);
	if (m_fFirstFrame)
	{
		fputc(gifscrn.bc,fout);
	}
	temp=fgetc(fp);
	if (m_fFirstFrame)
	{
		fputc(temp,fout);
	}
	imagec=gif_ptwo[(1+gifscrn.pixbits)];

	if (gifscrn.m)
	{
		for(i=0;i<imagec;i++)
		{
			gif_cmap[i].cmap.red   = temp = fgetc(fp); 
			if (m_fFirstFrame)
			{
				fputc(temp,fout);
			}
			gif_cmap[i].cmap.green = temp = fgetc(fp); 
			if (m_fFirstFrame)
			{
				fputc(temp,fout);
			}
			gif_cmap[i].cmap.blue  = temp = fgetc(fp); 
			if (m_fFirstFrame)
			{
				fputc(temp,fout);
			}
		}
	}
	if(gifscrn.m&&(transp>=0||delay>=0))
	{
		int ix = 0, max_dist = 3*256;
		if(transp>=0)
		{
			for(i=0; i<imagec; i++)
			{
				int dist = abs(gif_cmap[i].cmap.red-(transp&255)) +
							abs(gif_cmap[i].cmap.green-((transp>>8)&255)) +
							abs(gif_cmap[i].cmap.blue-(transp>>16));
				if(dist<max_dist)
				{
					ix = i;
					max_dist = dist;
				}
			} 
		}
		fputc(0x21,fout);
		fputc(0xF9,fout);
		fputc(0x04,fout);
		fputc((transp>=0?0x01:0x00)|(disposal<<2),fout);
		fputc(delay&255,fout);
		fputc((unsigned)delay>>8,fout);
		fputc(ix,fout);
		fputc(0x00,fout);
	}
	if(m_fFirstFrame && repeats>=0)
	{ /* documentation look at */
		fputc(0x21,fout);          /* http://www.reiworld.com/royalef/gifabout.htm */
		fputc(0xFF,fout);
		fputc(0x0B,fout);
		fputs("NETSCAPE2.0",fout);
		fputc(0x03,fout);
		fputc(0x01,fout);
		fputc(repeats&255,fout);
		fputc((unsigned)repeats>>8,fout);
		fputc(0x00,fout);
	}
}

void CGifEncoder::GIF_Image_Header(FILE * fp, FILE * fout)
{
	int temp,tnum,i,tmp;

	tmp = GIF_Get_Short(fp,fout, false); if(!pos_set) xpos = tmp;
	gifimage.left   = xpos; GIF_Put_Short(fout,xpos);
	tmp = GIF_Get_Short(fp,fout, false); if(!pos_set) ypos = tmp;
	gifimage.top    = ypos; GIF_Put_Short(fout,ypos);
	gifimage.width  = GIF_Get_Short(fp,fout, true);
	gifimage.height = GIF_Get_Short(fp,fout, true);
	temp=fgetc(fp); 

	gifimage.m        = temp & 0x80;
	gifimage.i        = temp & 0x40;
	gifimage.pixbits  = temp & 0x07;

	if (!m_fFirstFrame)
	{
		temp |= 0x80;
	}
	temp &= 0xf8;
	temp |= gifscrn.pixbits;
	fputc(temp,fout);

	tnum=gif_ptwo[(1+gifimage.pixbits)];

	/* if there is an image cmap, then use it */
	if (gifimage.m)
	{
		for(i=0;i<tnum;i++)
		{
			gif_cmap[i].cmap.red   = temp = fgetc(fp); fputc(temp,fout);
			gif_cmap[i].cmap.green = temp = fgetc(fp); fputc(temp,fout);
			gif_cmap[i].cmap.blue  = temp = fgetc(fp); fputc(temp,fout);
		}
	}  /* else if screen was last not 1st time */
	else if (!m_fFirstFrame)
	{
		for(i=0;i<imagec;i++)
		{
			fputc(gif_cmap[i].cmap.red  ,fout);
			fputc(gif_cmap[i].cmap.green,fout);
			fputc(gif_cmap[i].cmap.blue ,fout);
		}
	}
}

int CGifEncoder::GIF_Get_Short(FILE * fp, FILE * fout, bool fFirst)
{
	register int temp,tmp1;
	temp=fgetc(fp);
	if (fFirst)
	{
		fputc(temp,fout);
	}
	tmp1=fgetc(fp);
	if (fFirst)
	{
		fputc(tmp1,fout);
	}
	return(temp|( (tmp1) << 8 ));
}

void CGifEncoder::GIF_Put_Short(FILE * fout, unsigned int data)
{
	fputc((unsigned char)(data&255),fout);      /* lo */
	fputc((unsigned char)((data>>8)&255),fout); /* hi */
}