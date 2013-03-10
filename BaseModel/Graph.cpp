// Graph.cpp: implementation of the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Graph.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraph::CGraph(int nNode)
: m_nNode(nNode)
{
	
}

CGraph::~CGraph()
{

}

int CUndirectedGraph::vParentNode(int iNode, std::vector<int>& vPNode) const
{
	int nPNode = 0;
	vPNode.clear();

	for (int iEdge = 0; iEdge < m_nEgde; iEdge++ )
	{
		if ( m_vEdge[iEdge].m_iFrom == iNode)			
		{
			vPNode.push_back(m_vEdge[iEdge].m_iTo);
			nPNode++;
		}
		else if (m_vEdge[iEdge].m_iTo == iNode)			
		{
			vPNode.push_back(m_vEdge[iEdge].m_iFrom);
			nPNode++;
		}
	}
	return nPNode;
}

void  CUndirectedGraph::AddEdge(int iFrom, int iTo)
{
	// Check if current edge is in the edge set
	bool bExsit = false;
	for (int iEdge = 0; (iEdge < m_nEgde) && (!bExsit); iEdge++ )
	{
		if (( m_vEdge[iEdge].m_iFrom == iFrom) && ( m_vEdge[iEdge].m_iFrom == iTo))			
		{
			bExsit = true;
		}
		else if (( m_vEdge[iEdge].m_iFrom == iTo) && ( m_vEdge[iEdge].m_iFrom == iFrom))
		{
			bExsit = true;
		}
	}
	
	if (!bExsit)
	{
		m_vEdge.push_back(CGraphEdge(iFrom, iTo));
	}
}

int CUndirectedGraph::FindEdge(int iFrom, int iTo) const
{
	bool bExsit = false;
	int iEdgeFind;
	for (int iEdge = 0; (iEdge < m_nEgde) && (!bExsit); iEdge++ )
	{
		if (( m_vEdge[iEdge].m_iFrom == iFrom) && ( m_vEdge[iEdge].m_iFrom == iTo))			
		{
			iEdgeFind = iEdge;
			bExsit = true;
		}
		else if (( m_vEdge[iEdge].m_iFrom == iTo) && ( m_vEdge[iEdge].m_iFrom == iFrom))
		{
			iEdgeFind = iEdge;
			bExsit = true;
		}
	}
	if (bExsit)
	{
		return iEdgeFind;
	}
	else
	{
		return -1;
	}
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CGraphEdge& ge)
{
	ofs << ge.m_iFrom << ge.m_iTo;
	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CGraphEdge& ge)
{
	ifs >> ge.m_iFrom >> ge.m_iTo;
	return ifs;
}

COutputDataFile& Output(COutputDataFile& ofs, const CGraph& g)
{
	ofs << ClassTag("Graph") << sep_endl;
	ofs << g.m_nNode << g.m_nEgde << sep_endl;
	for (int iEdge = 0; iEdge < g.m_nEgde; iEdge++ )
	{
		ofs << g.m_vEdge[iEdge];
	}
	return ofs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CGraph& g)
{
	return Output(ofs, g);
}

CInputDataFile& Input(CInputDataFile& ifs, CGraph& g)
{
	ifs >> ClassTag("Graph") >> sep_endl;
	ifs >> g.m_nNode >> g.m_nEgde >> sep_endl;
	for (int iEdge = 0; iEdge < g.m_nEgde; iEdge++ )
	{
		ifs >> g.m_vEdge[iEdge];
	}
	return ifs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CGraph& g)
{
	return Input(ifs, g);
}
