
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 5.02.0221 */
/* at Tue Jul 06 15:25:51 1999
 */
/* Compiler settings for ..\dxmgr7.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data , no_format_optimization
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AXP64)

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IEnumDirectDraws = {0x5FF13307,0xFCB9,0x11D1,{0xB2,0x33,0x00,0x00,0xF8,0x1F,0xDF,0x9E}};


const IID IID_IEnumPixelFormats = {0x5FF13337,0xFCB9,0x11D1,{0xB2,0x33,0x00,0x00,0xF8,0x1F,0xDF,0x9E}};


const IID IID_IEnumDisplayModes = {0xB4E847C0,0xFF1A,0x11D1,{0xB2,0x33,0x00,0x00,0xF8,0x1F,0xDF,0x9E}};


const IID IID_IDXMgr7 = {0x65E51761,0xEBA8,0x11D1,{0xAA,0x78,0x00,0x60,0x08,0x14,0xAA,0xE9}};


const IID LIBID_DXMGRLib = {0x06fd7238,0x0978,0x11d3,{0x99,0x69,0x00,0x00,0xf8,0x1f,0xdf,0x9e}};


const CLSID CLSID_DXMgr7 = {0xb8549cec,0x0977,0x11d3,{0x99,0x69,0x00,0x00,0xf8,0x1f,0xdf,0x9e}};


const CLSID CLSID_EnumDirectDraws = {0x5FF13308,0xFCB9,0x11D1,{0xB2,0x33,0x00,0x00,0xF8,0x1F,0xDF,0x9E}};


const CLSID CLSID_EnumPixelFormats = {0x5FF13338,0xFCB9,0x11D1,{0xB2,0x33,0x00,0x00,0xF8,0x1F,0xDF,0x9E}};


const CLSID CLSID_EnumDisplayModes = {0xB4E847C1,0xFF1A,0x11D1,{0xB2,0x33,0x00,0x00,0xF8,0x1F,0xDF,0x9E}};


#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AXP64)*/

