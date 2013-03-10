/*
* 
* 文件名称：FileTool.h
* 文件描述：主要实现文件的加密存储、打开、关闭、读取、写入
* 
*/
#ifndef FILE_H
#define FILE_H

class FileTool  
{
public:
	DWORD GetFileSize();
	BOOL WriteEnd();
	BOOL WriteString(char *buf);
	BOOL WriteFile(char * inbuf, int len);
	BOOL CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName,  BOOL bFailIfExists);
	BOOL CopyDir(char * dst, char *src);
	BOOL WriteLine(char * inbuf, int len);
	BOOL CloseFile();
	HANDLE fHandle;
	
	int ReadLine(char * szFileName, char * outBuf);
	//解密文件,输入参数为文件名
	BOOL DeCryptFile(char * pFileName);
	//加密文件,输入参数为文件名
	BOOL EncryptFile(char * pFileName);
	BOOL OpenFile(char * szFileName);
	FileTool(); 
	virtual ~FileTool();

};

#endif
