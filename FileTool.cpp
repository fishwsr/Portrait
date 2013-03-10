/*
* Copyright (c) 2005，安胜联
* 
* 文件名称：FileTool.cpp
* 文件描述：主要实现文件的加密存储、打开、关闭、读取、写入
* 
* 当前版本：1.0
* 作    者：卫向军
* 完成日期：2006年1月11日
*
*/
#include "stdafx.h"
#include "FileTool.h"
#include "blowfish.h"   //加密算法
#include "utility.h"

// Construction/Destruction
FileTool::FileTool()
{
	
}

FileTool::~FileTool()
{
	
}


BOOL FileTool::EncryptFile(char *pFileName)
{	
	BOOL	ret = FALSE;
	BYTE	buffer[MAX_BUF];
	int i;	
	HANDLE		fh;
	DWORD		nBytesToRead,nBytesRead;
	DWORD		nBytes;    //取整后的
	//加密函数
	CBlowFish bl;
	if ((fh = ::CreateFile(pFileName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)) == INVALID_HANDLE_VALUE)
	{		
		return FALSE;
	}
	
	//循环读取文件内容,每次读取512字节,然后加密.直到读到文件尾为止.	
	//文件内容必须为8的整数倍
	do{
		ret = ReadFile(fh, buffer, 512, &nBytesRead, NULL) ; 
		// Check for end of file. 
		if (ret &&  (nBytesRead == 0) ) 
		{ 
			break;
		}	
		BYTE skey[33] = "13026367186135812505441234567890";
//		bl.Initialize(skey, 32, CBlowFish::CBC);
//		bin2hexstr(buffer,nBytesRead,szBuffer);
//		p=(BYTE *)szBuffer;
//		bl.Encode(p, p, nBytesRead*2);
//		hexstr2bin((const char *)p,buffer,nBytesRead,&len);
		//文件内容必须为8的整数倍
		if(nBytesRead%8 != 0)
		{
			i=nBytesRead%8 + nBytesRead /8 * 8;
			nBytes = (nBytesRead/8 + 1) * 8;
			for(; i < nBytes; i++)
			{
				buffer[i] = 0x00;
			}
			bl.Encode(buffer, buffer, nBytesRead);
			//往后移动27个
			SetFilePointer(fh, - nBytesRead, NULL, FILE_CURRENT);
			//重新写32个
			nBytesRead = nBytes;
		}
		else
		{
			bl.Encode(buffer, buffer, nBytesRead);
			SetFilePointer(fh, - nBytesRead, NULL, FILE_CURRENT);
		}		
		ret = ::WriteFile(fh, buffer, nBytesRead, &nBytesToRead, NULL);					
		if(!ret)
		{
			return FALSE;
		}
	}while(TRUE);
	CloseHandle(fh);
	return	TRUE;
	
}

BOOL FileTool::DeCryptFile(char *pFileName)
{	
	BOOL	ret = FALSE;
	BYTE	buffer[MAX_BUF];
	HANDLE		fh;
	DWORD		nBytesToRead,nBytesRead;

	if (pFileName == NULL)
		return	FALSE;
	//加密函数
	CBlowFish bl;
	if ((fh = ::CreateFile(pFileName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	//循环读取文件内容,每次读取512字节,然后加密.直到读到文件尾为止.	
	// Attempt a synchronous read operation. 
	do{
		ret = ReadFile(fh, buffer, 512, &nBytesRead, NULL) ; 
		// Check for end of file. 
		if (ret &&  (nBytesRead == 0) ) 
		{ 
			break;
		}	
		BYTE skey[33] = "13026367186135812505441234567890";
		bl.Decode(buffer,buffer,nBytesRead);
		SetFilePointer(fh,- nBytesRead,NULL,FILE_CURRENT);
		ret = ::WriteFile(fh,buffer,nBytesRead,&nBytesToRead,NULL);		
		if(!ret)
		{
			return FALSE;
		}
	}while(TRUE);
	CloseHandle(fh);
	return	TRUE;
	
}
//outbuf  应该置为全0
//返回值为0 说明已经到了文件末尾
//
int FileTool::ReadLine(char * szfilename, char * outBuf)
{
	char buf[20000];
	int i=0;
	DWORD nBytesRead;
	BOOL ret = FALSE;
	//printf("reading a line\n");
	while (i<20000) 
	{
		ReadFile(fHandle, buf+i, 1, &nBytesRead, NULL);
		if (nBytesRead!=1) 
		{
			return 0;
  		}
		if (buf[i]=='\n')
		{
			buf[i+1]=0;
			printf("line read=%s\n",buf);
			//读取到的数据中可能包含0x00,所以内存拷贝时长度不能用strlen
			memcpy(outBuf, buf, i+2);
			return strlen(buf);
		}
		i++;		
	}
	memcpy(outBuf, buf, i+2);
	return strlen(buf); 
}
BOOL FileTool::OpenFile(char *szFileName)
{
	HANDLE		fh;
	if ((fh = ::CreateFile(szFileName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)) == INVALID_HANDLE_VALUE)
	{		
		return FALSE;
	}
	fHandle = fh;
	return TRUE;
}


BOOL FileTool::CloseFile()
{
	CloseHandle(fHandle);
	return TRUE;
}

BOOL FileTool::WriteLine(char *inbuf, int len)
{
	
	DWORD nBytesWrited;
	BOOL ret = FALSE;

	ret = ::WriteFile(fHandle, inbuf, len, &nBytesWrited, NULL);
	if (ret = FALSE) 
	{
		return FALSE;
  	}
	ret = ::WriteFile(fHandle, "\r\n", 2, &nBytesWrited, NULL);
	if (ret = FALSE) 
	{
		return FALSE;
  	}
	return TRUE; 
}

BOOL FileTool::CopyDir(char *dst, char *src)
{
	SHFILEOPSTRUCT OpStruc;
	{
		OpStruc.hwnd = NULL;
		OpStruc.wFunc = FO_COPY;  //FO_COPY, FO_MOVE, FO_DELETE
		OpStruc.fFlags = NULL;
		OpStruc.pFrom = src;
		OpStruc.pTo = dst;
		OpStruc.lpszProgressTitle = NULL;
	}
	SHFileOperation(&OpStruc);
	return TRUE;
}

BOOL FileTool::CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists)
{
	return CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
}

BOOL FileTool::WriteFile(char *inbuf, int len)
{
	DWORD  nBytesWrited;
	return ::WriteFile(fHandle, inbuf, len, &nBytesWrited, NULL);	
}

BOOL FileTool::WriteString(char *buf)
{
	DWORD  nBytesWrited;
	return ::WriteFile(fHandle, buf, strlen(buf)+1, &nBytesWrited, NULL);	
}

BOOL FileTool::WriteEnd()
{
	DWORD  nBytesWrited;
	return ::WriteFile(fHandle, "\r\n", 2, &nBytesWrited, NULL);
}

DWORD FileTool::GetFileSize()
{
	return	::GetFileSize(fHandle, NULL);
}


