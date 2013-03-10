#ifndef  UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#define MAX_BUF 1024
#define DEBUG
void bin2hexstr(const BYTE *bin, int len, char *buf);
// BOOL hexstr2bin(const char *buf, BYTE *bindata, int maxlen, int *len);
void LogWrite(const char * pLogData);
void MakePath(char * dir, char * file);
void GetCompressFileName(char *dst, char *src);

#endif
