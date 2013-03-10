#pragma warning(disable : 4786)

#ifndef _ASMDLL_H_
#define _ASMDLL_H_

#ifdef ASMMODEL_EXPORTS
    #define ASMMODEL_API __declspec(dllexport)
#else
    #define ASMMODEL_API __declspec(dllimport)
	#ifdef _DEBUG
		#pragma comment(lib, "./Lib/ASMModelD.lib")
	#else
		#pragma comment(lib, "./Lib/ASMModel.lib")
	#endif // _DEBUG
#endif // ASMMODEL_EXPORTS

/*
class ASMMODEL_API ASMSearchOption
{
public:
	ASMSearchOption();
	~ASMSearchOption();

	void SetConstrainedShape(std::vector<CPoint2> &vPoint2);
	void SetConstrainedPoints(std::vector<bool> &vbConstrained);

	bool m_fbUsedConstraint;
	char m_strImageFileName[256];
	char m_strModelFileName[256];

	std::vector<CPoint2> m_ConstrainedShape;
	std::vector<bool> m_vbConstrained; 
};
*/


class ASMSearchOption;
extern "C" ASMMODEL_API int myadd(int x, int y);
extern "C" ASMMODEL_API int gAsmSearch1( const char* strImageFileName, const char* strModelFileName, 
									     std::vector<CPoint2> &vPoint2, bool fbUseConstraint, 
									     std::vector<CPoint2>* pConstrainedShape=NULL, 
									     std::vector<bool>* pvbConstrained=NULL );
extern "C" ASMMODEL_API int gAsmSearch2(const ASMSearchOption &option, std::vector<CPoint2> &vPoint2);


#endif // ASMDLL_H
