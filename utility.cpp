#include "stdafx.h"
#include "utility.h"
//inline u_char hexchar2char(u_char ch)
//{
//	if (ch >= '0' && ch <= '9')
//		return ch - '0';
//	ch = toupper(ch);
//	if (ch >= 'A' && ch <= 'Z')
//		return ch - 'A' + 10;
//	return 0;
//}
//
//BOOL hexstr2bin(const char *buf, BYTE *bindata, int maxlen, int *len)
//{
//	for (*len=0; buf[0] && buf[1] && *len < maxlen; buf+=2, (*len)++)
//	{
//		bindata[*len] = hexchar2char(buf[0]) << 4 | hexchar2char(buf[1]);
//	}
//	return	TRUE;
//}
static char *hexstr = "0123456789ABCDEF";
void bin2hexstr(const BYTE *bindata, int len, char *buf)
{

	for (int cnt=0; cnt < len; cnt++)
	{
		*buf++ = hexstr[bindata[cnt] >> 4];
		*buf++ = hexstr[bindata[cnt] & 0x0f];
	}
	*buf = 0;
}
//在路经后面加上文件名
//dir   目录名
//file  文件名
void MakePath(char * dir, char * file)
{
	if(dir[strlen(dir) -1] != '\\')
	{
		strcat(dir, "\\");
	}
	strcat(dir, file);
}
void LogWrite(const char * pLogData)
{
#ifdef DEBUG
	FILE * p = fopen("log.txt","a");
	fwrite(pLogData,strlen(pLogData),1,p);
	fwrite("\n",strlen("\n"),1,p);
	fflush(p);
	fclose(p);
#endif
}
//src     a.exe a.lib
//dst     a.liz
void GetCompressFileName(char *dst, char *src)
{
	char FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	char *p;
	memcpy(FileName, src, strlen(src));
	p = strstr(FileName, ".");
	*p = 0;
	strcpy(dst, FileName);
	strcat(dst, ".liz");
}
