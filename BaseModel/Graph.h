// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__EF8653DF_2B42_4D47_AB52_72118E056580__INCLUDED_)
#define AFX_GRAPH_H__EF8653DF_2B42_4D47_AB52_72118E056580__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct BASEMODEL_API CGraphEdge{
	CGraphEdge(int iFrom, int iTo) : m_iFrom(iFrom), m_iTo(iTo) {};
	int m_iFrom;
	int m_iTo;
};

class BASEMODEL_API CGraph  
{
public:
	CGraph(int nNode);
	virtual ~CGraph();

	int NNode() const {return m_nNode;};
	virtual int vParentNode(int iNode, std::vector<int>& vPNode) const = 0;

	virtual void AddEdge(int iFrom, int iTo) = 0;
	virtual int FindEdge(int iFrom, int iTo) const = 0;

	friend COutputDataFile& Output(COutputDataFile& ofs, const CGraph& g);
	friend CInputDataFile& Input(CInputDataFile& ifs, CGraph& g);
protected:
	int m_nNode;
	int m_nEgde;
	std::vector<CGraphEdge> m_vEdge;
};

class BASEMODEL_API CUndirectedGraph : public CGraph
{
public:
	virtual int vParentNode(int iNode, std::vector<int>& vPNode) const;
	virtual void AddEdge(int iFrom, int iTo);
	virtual int FindEdge(int iFrom, int iTo) const;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CGraphEdge& ge);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CGraphEdge& ge);

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CGraph& g);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CGraph& g);


#endif // !defined(AFX_GRAPH_H__EF8653DF_2B42_4D47_AB52_72118E056580__INCLUDED_)
