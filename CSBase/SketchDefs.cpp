// SketchDefs.cpp: implementation of the SketchDefs.
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SketchDefs.h"

static char s_tmpbuf[256];

CStroke::CStroke(void)
{
    Clear();
}

CStroke::~CStroke(void)
{
    Clear();
}

const CStroke& CStroke::operator=(const CStroke& src)
{
	if ( this!= &src )
	{
		m_ucLineColor = src.m_ucLineColor;
		m_ucLineWidth = src.m_ucLineWidth;
		m_Path = src.m_Path;
		m_Type = src.m_Type;
	}

    return *this;
}

void CStroke::TranslateBall(double x, double y)
{
    {
        m_Path.PointX(0) += x;
        m_Path.PointY(0) += y;
    }
}

void CStroke::Translate(double x, double y)
{
    {
        m_Path.Translate(x, y);
    }
}

void CStroke::Scale(double sX, double sY)
{
	{
		m_Path.Scale(sX, sY);
	}
}

CStroke& CStroke::operator/=(double factor)
{
    assert(factor != 0);

    if (factor != 0)
    {
        if (0 != m_ucLineWidth)
        {
            m_ucLineWidth = (unsigned char)((double) (m_ucLineWidth) / factor);
            if (m_ucLineWidth < 1)
            {
                m_ucLineWidth = 1;
            }
        }
        m_Path /= factor;
    }

    return *this;
}

void CStroke::Clear(void)
{
    m_ucLineColor = 0;
    m_ucLineWidth = 0;
	m_Type = STROKE_STROKE;
    m_Path.Clear();
    //m_Spline.Clear();
}

void DrawControlledWidthLine(const CCubicBezierSpline& spline, int num, CPoint2 *points, const CStrokeCtrl &control, double widthfactor)
{
    if (num < 1) return;
	if (spline.NbSegment() == 0) return;

	double *pLength = new double[num * spline.NbSegment()+1];
	pLength[0] = 0.0;

	int totcount = 0;
	CPoint2 oldp, curp;

	int i,j;
	for (i = 0; i < spline.NbSegment(); i++)
	{
		const CCubicBezier &segment = spline.m_Segments[i];

		double delta;
		if (num > 1)
			delta = 1.0 / num;
		else
			delta = 1.0;
		double u = 0.0;

		for (j = 0; j < num; j++, u += delta, totcount++)
		{
			curp = segment.P(u);

			if (totcount == 0)
			{
				pLength[totcount] = 0;
			}
			else
			{
				CVector2 det = curp - oldp;
				pLength[totcount] = pLength[totcount - 1] + sqrt(det.X * det.X + det.Y * det.Y);
			}
			oldp = curp;
		}
	}
	{
		curp = spline.m_Segments[spline.NbSegment()-1].m_ControlPoints[3];
		CVector2 det = curp - oldp;
		pLength[totcount] = pLength[totcount - 1] + sqrt(det.X * det.X + det.Y * det.Y);
	}
	double totlength = pLength[spline.NbSegment() * num];

	for (i = 0; i < spline.NbSegment(); i++)
	{
		const CCubicBezier &segment = spline.m_Segments[i];

		double delta;
		if (num > 1)
			delta = 1.0 / num;
		else
			delta = 1.0;
		double u = 0.0;
		for (j = 0; j < num; j++, u += delta)
		{
			CPoint2 org = segment.P(u);
			CVector2 n = segment.Normal(u);
			double tempf;
			if (widthfactor > 0)
			{
				tempf = control.width1(pLength[i * num + j] / totlength) * widthfactor;
				points[i * num + j] = org + n * tempf;
			}
			else 
			{
				tempf = control.width2(pLength[i * num + j] / totlength) * widthfactor;
				points[i * num + j] = org + n * tempf;
			}
		}
	}

	points[spline.NbSegment() * num] = spline.m_Segments[spline.NbSegment() - 1].m_ControlPoints[3];
	delete []pLength;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CStroke& s)
{
 	ofs << ClassTag("CStroke") << sep_endl;
	ofs << (int)s.m_Type << (int)s.m_ucLineColor << (int)s.m_ucLineWidth << sep_endl;
	ofs << s.m_Path;
	return ofs;
}


CInputDataFile& operator>>(CInputDataFile& ifs, CStroke& s)
{
	ifs >> ClassTag("CStroke") >> sep_endl;
	int tempi1, tempi2, tempi3;
	ifs >> tempi1 >> tempi2 >> tempi3 >> sep_endl;
	*(int *)(&s.m_Type) = tempi1;
	s.m_ucLineColor = tempi2;
	s.m_ucLineWidth = tempi3;
	ifs >> s.m_Path;
	return ifs; 
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CStrokeSet& ss)
{
	ofs << (int)ss.size() << sep_endl;
	for (int i = 0; i< ss.size(); i++)
	{
		ofs << ss[i] << sep_endl;
	}
	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CStrokeSet& ss)
{
	int nStroke;
	ifs >> nStroke;
	ss.resize(nStroke);

	for (int i = 0; i< nStroke; i++)
	{
		ifs >> ss[i] >> sep_endl;
	}
	return ifs;
}



void CStrokeSet::Translate(double x, double y)
{
    {
        for (int i = 0; i < size(); i++)
        {
            (*this)[i].Translate(x, y);
        }
    }
}

void CStrokeSet::Scale(double sX, double sY)
{
	{
		for (int i = 0; i < size(); i++)
		{
			(*this)[i].Scale(sX, sY);
		}
	}
}

void CStrokeSet::TranslateEye(double x, double y)
{
    {
        for (int i = 0; i < size(); i++)
        {
			if (i == EYE_BALL || i == EYE_BRIGHT_POINT)
	            (*this)[i].TranslateBall(x, y);
			else if (i != EYE_DRAW_CTRL)
				(*this)[i].Translate(x, y);
        }
    }
}

CStrokeSet& CStrokeSet::operator/=(double factor)
{
    assert(factor != 0);

    if (factor != 0)
    {
        for (int i = 0; i < size(); i++)
        {
            (*this)[i] /= factor;
        }
    }

    return *this;
}

void CStrokeSet::Clear(void)
{
    clear();
}

CPartResult::CPartResult(void)
{

}

CPartResult::~CPartResult(void)
{
    Clear();
}

void CPartResult::Clear(void)
{
    m_vStrokeSet.clear();
}


void CPartResult::Translate(double x, double y)
{
    {
        for(int i = 0; i < m_vStrokeSet.size(); i++)
        {
            m_vStrokeSet[i].Translate(x, y);
        }
    }
}

void CPartResult::TranslateEye(double x, double y)
{
    {
        for(int i = 0; i < m_vStrokeSet.size(); i++)
        {
            m_vStrokeSet[i].TranslateEye(x, y);
        }
    }
}

CPartResult& CPartResult::operator /=(double factor)
{
    assert(0 != factor);
    if(0 != factor)
    {
        for(int i = 0; i < m_vStrokeSet.size(); i++)
        {
            m_vStrokeSet[i] /= factor;
        }
    }
    return *this;
}

CStrokeCtrl::CStrokeCtrl(int PredefinedType)
{
    Clear();
	if (PredefinedType == 1)
	{
		std::vector<CPoint2> vKeyPoints;
		vKeyPoints.resize(11);

		vKeyPoints[0] = CPoint2(0, 0.1);
		vKeyPoints[1] = CPoint2(40, 0.40);
		vKeyPoints[2] = CPoint2(80, 0.45);
		vKeyPoints[4] = CPoint2(160, 0.49);
		vKeyPoints[3] = CPoint2(120, 0.48);
		vKeyPoints[5] = CPoint2(200, 0.50);
		vKeyPoints[6] = CPoint2(240, 0.49);
		vKeyPoints[7] = CPoint2(280, 0.48);
		vKeyPoints[8] = CPoint2(320, 0.45);
		vKeyPoints[9] = CPoint2(360, 0.40);
		vKeyPoints[10] = CPoint2(400, 0.1);
		
		m_Splines[0].Create_Catmull_Rom_Spline(vKeyPoints);
		m_Splines[1].Create_Catmull_Rom_Spline(vKeyPoints);
	}
}

CStrokeCtrl::~CStrokeCtrl(void)
{
    Clear();
}

const CStrokeCtrl& CStrokeCtrl::operator=(const CStrokeCtrl& src)
{
	if ( this!= &src )
	{
		m_Splines[0] = src.m_Splines[0];
		m_Splines[1] = src.m_Splines[1];
	}
    return *this;
}

CStrokeCtrl& CStrokeCtrl::operator/=(double factor)
{
    assert(factor != 0);

    if (factor != 0)
    {
        m_Splines[0] /= factor;
        m_Splines[1] /= factor;
    }

    return *this;
}

void CStrokeCtrl::Clear(void)
{
    m_Splines[0].Clear();
    m_Splines[1].Clear();
}

double CStrokeCtrl::width1(double u) const
{
	if (m_Splines[0].NbSegment()==0)
		return 0.5;

	if (u < 0.0) u = 0.0;
	int segnum = (int) (u * 10.0);
	if (segnum >= 10)
		segnum = 9;
	double au = u * 10.0 - segnum;
	if (au > 1.0) au = 1.0;

	const CCubicBezier &segment = m_Splines[0].m_Segments[segnum];
	CPoint2 p = segment.P(au);
	return  p.Y;
}

double CStrokeCtrl::width2(double u) const
{
	if (m_Splines[1].NbSegment()==0)
		return 0.5;

	if (u < 0.0) u = 0.0;
	int segnum = (int) (u * 10.0);
	if (segnum >= 10)
		segnum = 9;
	double au = u * 10.0 - segnum;
	if (au > 1.0) au = 1.0;

	const CCubicBezier &segment = m_Splines[1].m_Segments[segnum];
	CPoint2 p = segment.P(au);
	return  p.Y;
}

CStrokeCtrlSet& CStrokeCtrlSet::operator/=(double factor)
{
    assert(factor != 0);

    if (factor != 0)
    {
        for (int i = 0; i < size(); i++)
        {
            (*this)[i] /= factor;
        }
    }

    return *this;
}

void CStrokeCtrlSet::Clear(void)
{
    clear();
}


CPartResultCtrl::CPartResultCtrl(void)
{

}

CPartResultCtrl::~CPartResultCtrl(void)
{
    Clear();
}

void CPartResultCtrl::Clear(void)
{
    m_vStrokeCtrlSet.clear();
}

CPartResultCtrl& CPartResultCtrl::operator /=(double factor)
{
    assert(0 != factor);
    if(0 != factor)
    {
        for(int i = 0; i < m_vStrokeCtrlSet.size(); i++)
        {
            m_vStrokeCtrlSet[i] /= factor;
        }
    }
    return *this;
}

CSketchResult::CSketchResult(void)
{
    Clear();
}

CSketchResult::~CSketchResult(void)
{
    Clear();
}

void CSketchResult::Clear(void)
{
    m_shape.Clear();

    m_defPartRetCtrl.Clear();
    m_DrawControl.clear();
	m_DrawControl.resize(DT_CTRL_COUNT);
	for (int part = PART_FIRST; part <= PART_LAST; part++)
	{
        m_agPartRetCtrl[part].Clear();
        m_agPartRet[part].Clear();
	}
}





void CSketchResult::Translate(double x, double y)
{
    {
        m_shape.Translate(x, y);
        for(int part = PART_FIRST; part <= PART_LAST; part++)
        {
			if (part != PART_EYE)
				m_agPartRet[part].Translate(x, y);
			else
				m_agPartRet[part].TranslateEye(x, y);
        }
    }
}

void CSketchResult::Translate(const RECT & rect)
{
	double left, top, right, bottom;
	m_shape.AroundRect(left, top, right, bottom);
	double xRatio = (right - left) / (rect.right - rect.left);
	double yRatio = (bottom - top) / (rect.bottom - rect.top);
	double ratio = (xRatio > yRatio ? xRatio : yRatio);
	if (ratio != 0) (*this) /= ratio;
	m_shape.AroundRect(left, top, right, bottom);
	Translate(rect.left - left + (rect.right - rect.left - (right - left)) / 2, rect.top - top + (rect.bottom - rect.top - (bottom - top)) / 2);
}

CSketchResult& CSketchResult::operator/=(double factor)
{
    assert(0 != factor);
    if (factor > 0)
    {
        m_shape /= factor;
        for(int part = PART_FIRST; part <= PART_LAST; part++)
        {
            m_agPartRet[part] /= factor;
        }
		if (m_agPartRetCtrl[PART_TEMPLATE].m_vStrokeCtrlSet.size() >= 1)	//Has change-width hair
		{
			m_agPartRetCtrl[PART_TEMPLATE].m_vStrokeCtrlSet[TEMPLATE_HAIR] /= factor;
		}
    }
    return *this;
}

void CStrokeSet::AroundRect(double & left, double & top, double & right, double & bottom) const
{
    left = top = 10000.0;
    right = bottom = -10000.0;

    for(int i = 0; i < size(); i ++)
    {
        double dLeft, dTop, dRight, dBottom;

		if(255 == at(i).m_ucLineColor)
			continue;

        at(i).m_Path.AroundRect(dLeft, dTop, dRight, dBottom);
        left = (left < dLeft) ? left : dLeft;
        top = (top < dTop) ? top : dTop;
        right = (right > dRight) ? right : dRight;
        bottom = (bottom > dBottom) ? bottom : dBottom;
    }
}

void CSketchResult::AroundRect(double & left, double & top, double & right, double & bottom) const
{
    m_shape.AroundRect(left, top, right, bottom);
	for(int part = PART_FIRST; part <= PART_LAST; part ++)
	{
        if (part < PART_TEMPLATE && part != PART_EAR)
        {
            continue;
        }
		if (m_agPartRet[part].m_vStrokeSet.size() > 0)
		{
			double dLeft, dTop, dRight, dBottom;
			for(int i = 0; i < m_agPartRet[part].m_vStrokeSet.size(); i ++)
			{
				m_agPartRet[part].m_vStrokeSet[i].AroundRect(dLeft, dTop, dRight, dBottom);
				left = (left < dLeft) ? left : dLeft;
				top = (top < dTop) ? top : dTop;
				right = (right > dRight) ? right : dRight;
				bottom = (bottom > dBottom) ? bottom : dBottom;
			}
		}
	}
}


// XML support
/*
bool CSketchResult::XMLLoad(const WCHAR *fname)
{
    bool ret = false;

	HRESULT hr;
	MSXML2::IXMLDOMDocument2Ptr pXMLDoc;

	hr = pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));
	if (FAILED(hr)) goto exit;

	pXMLDoc->async = VARIANT_FALSE;

	if (!pXMLDoc->load(fname)) goto exit;

	{
		Clear();

		MSXML2::IXMLDOMNodePtr pnPerson = pXMLDoc->selectSingleNode("/person");
		if (pnPerson == NULL) goto exit;
		MSXML2::IXMLDOMElementPtr pePerson = pnPerson;
		if (pePerson == NULL) goto exit;

		wcscpy(m_name,  _bstr_t(pePerson->getAttribute("name")));

		{
			MSXML2::IXMLDOMNodePtr pnBaseface = pePerson->selectSingleNode("baseface");
			if (pnBaseface == NULL) goto exit;
			MSXML2::IXMLDOMElementPtr peBaseface = pnBaseface;
			if (peBaseface == NULL) goto exit;

			wcscpy(m_id,  _bstr_t(peBaseface->getAttribute("id")));
			//strcpy(m_renderstyle, _bstr_t(peBaseface->getAttribute("renderstyle")));
            m_nConfig = atoi(_bstr_t(peBaseface->getAttribute("config")));
            m_nRenderStyle = atoi(_bstr_t(peBaseface->getAttribute("renderstyle")));
			wcscpy(m_linestyle, _bstr_t(peBaseface->getAttribute("linestyle")));
            m_nHairCatID = atoi(_bstr_t(peBaseface->getAttribute("haircategory")));
            m_nHairTempID = atoi(_bstr_t(peBaseface->getAttribute("hairtemplate")));
            m_nGlassesCatID = atoi(_bstr_t(peBaseface->getAttribute("glassescategory")));
            m_nGlassesID = atoi(_bstr_t(peBaseface->getAttribute("glassestemplate")));
            m_nBeardCatID = atoi(_bstr_t(peBaseface->getAttribute("beardcategory")));
            m_nBeardID = atoi(_bstr_t(peBaseface->getAttribute("beardtemplate")));

			{
				m_defPartRetCtrl.m_vStrokeCtrlSet.resize(1);
				m_defPartRetCtrl.m_vStrokeCtrlSet[0].resize(1);

				MSXML2::IXMLDOMNodeListPtr pnRenderctrls = peBaseface->selectNodes("renderctrl");
				for (int i = 0; i < pnRenderctrls->Getlength(); i++)
				{
					_bstr_t role;

					MSXML2::IXMLDOMNodePtr pnRenderctrl = pnRenderctrls->Getitem(i);
					if (pnRenderctrl == NULL) goto exit;
					MSXML2::IXMLDOMElementPtr peRenderctrl = pnRenderctrl;
					if (peRenderctrl == NULL) goto exit;
					
					role = peRenderctrl->getAttribute("role");
					if (stricmp(role, "stroke ctrl") == 0)
					{
						if (!XMLLoadStrokeCtrl(pXMLDoc, peRenderctrl, "stroke ctrl", m_defPartRetCtrl.m_vStrokeCtrlSet[0][0])) goto exit;
					}
				}

				MSXML2::IXMLDOMNodeListPtr pnShapes = peBaseface->selectNodes("shape");
				for (int i = 0; i < pnShapes->Getlength(); i++)
				{
					_bstr_t role;

					MSXML2::IXMLDOMNodePtr pnShape = pnShapes->Getitem(i);
					if (pnShape == NULL) goto exit;
					MSXML2::IXMLDOMElementPtr peShape = pnShape;
					if (peShape == NULL) goto exit;
					
					role = peShape->getAttribute("role");
					if (stricmp(role, "face shape") == 0)
					{
						if (!XMLLoadShape(pXMLDoc, peShape, "face shape", m_shape)) goto exit;
					}
				}

				m_agPartRet[PART_CONTOUR].m_vStrokeSet.resize(1);
				m_agPartRet[PART_CONTOUR].m_vStrokeSet[0].resize(1);

				m_agPartRet[PART_EAR].m_vStrokeSet.resize(2);
				m_agPartRet[PART_EAR].m_vStrokeSet[0].resize(1);
				m_agPartRet[PART_EAR].m_vStrokeSet[1].resize(1);

				m_agPartRet[PART_BROW].m_vStrokeSet.resize(2);

			    m_agPartRet[PART_EYE].m_vStrokeSet.resize(2);

			    m_agPartRet[PART_MOUTH].m_vStrokeSet.resize(1);
				
				m_agPartRet[PART_NOSE].m_vStrokeSet.resize(0);

				MSXML2::IXMLDOMNodeListPtr pnFaceparts = peBaseface->selectNodes("facepart");
				for (int i = 0; i < pnFaceparts->Getlength(); i++)
				{
					_bstr_t role;

					MSXML2::IXMLDOMNodePtr pnFacepart = pnFaceparts->Getitem(i);
					if (pnFacepart == NULL) goto exit;
					MSXML2::IXMLDOMElementPtr peFacepart = pnFacepart;
					if (peFacepart == NULL) goto exit;
					
					role = peFacepart->getAttribute("role");
					if (stricmp(role, "face line") == 0)
					{
						if (!XMLLoadOther(pXMLDoc, peFacepart, "face line", m_agPartRet[PART_CONTOUR].m_vStrokeSet[0])) goto exit;
					}
					else if (stricmp(role, "left ear") == 0)
					{
						if (!XMLLoadOther(pXMLDoc, peFacepart, "left ear", m_agPartRet[PART_EAR].m_vStrokeSet[0])) goto exit;
					}
					else if (stricmp(role, "right ear") == 0)
					{
						if (!XMLLoadOther(pXMLDoc, peFacepart, "right ear", m_agPartRet[PART_EAR].m_vStrokeSet[1])) goto exit;
					}
					else if (stricmp(role, "left brow") == 0)
					{
						if (!XMLLoadOther(pXMLDoc, peFacepart, "left brow", m_agPartRet[PART_BROW].m_vStrokeSet[0])) goto exit;
					}
					else if (stricmp(role, "right brow") == 0)
					{
						if (!XMLLoadOther(pXMLDoc, peFacepart, "right brow", m_agPartRet[PART_BROW].m_vStrokeSet[1])) goto exit;
					}
					else if (stricmp(role, "left eye") == 0)
					{
						if (!XMLLoadEye(pXMLDoc, peFacepart, "left eye", m_agPartRet[PART_EYE].m_vStrokeSet[0])) goto exit;
					}
					else if (stricmp(role, "right eye") == 0)
					{
						if (!XMLLoadEye(pXMLDoc, peFacepart, "right eye", m_agPartRet[PART_EYE].m_vStrokeSet[1])) goto exit;
					}
					else if (stricmp(role, "nose") == 0)
					{
						if (!XMLLoadNose(pXMLDoc, peFacepart, "nose", m_agPartRet[PART_NOSE])) goto exit;
					}
					else if (stricmp(role, "mouth") == 0)
					{
						if (!XMLLoadOther(pXMLDoc, peFacepart, "mouth", m_agPartRet[PART_MOUTH].m_vStrokeSet[0])) goto exit;
					}
				}
			}
		}
	}

	pXMLDoc.Release();

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLLoadStrokeCtrl(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peRenderctrl, const char *role, CStrokeCtrl & sc)
{
	bool ret = false;

	MSXML2::IXMLDOMNodeListPtr pnSplines = peRenderctrl->selectNodes("spline");

	for (int i = 0; i < pnSplines->Getlength(); i++)
	{
		_bstr_t role;

		MSXML2::IXMLDOMNodePtr pnSpline = pnSplines->Getitem(i);
		if (pnSpline == NULL) goto exit;
		MSXML2::IXMLDOMElementPtr peSpline = pnSpline;
		if (peSpline == NULL) goto exit;
					
		role = peSpline->getAttribute("role");
		if (stricmp(role, "spline 1") == 0)
		{
			if (!XMLLoadSpline(pXMLDoc, peSpline, role, sc.m_Splines[0])) goto exit;
		}
		else if (stricmp(role, "spline 2") == 0)
		{
			if (!XMLLoadSpline(pXMLDoc, peSpline, role, sc.m_Splines[1])) goto exit;
		}
	}

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLLoadShape(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peShape, const char *role, CShape & shape)
{
	bool ret = false;

	_bstr_t points;

	points = peShape->getAttribute("points");

	char *tmp1 = new char[strlen(points) + 1];
	strcpy(tmp1, points);
	char *tmp2 = new char[strlen(points) + 1];
	strcpy(tmp2, points);

	int count = 0;
	char *token = strtok(tmp1, ",");
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, ",");
	}

	shape.Resize(count / 2);
	count = 0;
	token = strtok(tmp2, ",");
	while (token != NULL)
	{
		if (count % 2)
			shape.PointY(count / 2) = strtod(token, NULL);
		else
			shape.PointX(count / 2) = strtod(token, NULL);
		count++;
		token = strtok(NULL, ",");
	}

    ret = true;
    return ret;
}

bool CSketchResult::XMLLoadEye(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, CStrokeSet & ss)
{
	bool ret = false;

	ss.resize(EYE_COUNT);
	ss[EYE_DRAW_CTRL].m_Path.Resize(EYE_DRAW_CTRL_COUNT);
	ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_DOUBLE_LINE) = 0;
	ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_BALL) = 0;
	ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_BRIGHT_POINT) = 0;
	ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_EYE_LINE) = 0;
	ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE) = 0;
	ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE1) = 0;

	MSXML2::IXMLDOMNodeListPtr pnLines = peFacepart->selectNodes("line");
	MSXML2::IXMLDOMNodeListPtr pnOvals = peFacepart->selectNodes("oval");

	for (int i = 0; i < pnLines->Getlength(); i++)
	{
		_bstr_t role;

		MSXML2::IXMLDOMNodePtr pnLine = pnLines->Getitem(i);
		if (pnLine == NULL) goto exit;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
		MSXML2::IXMLDOMElementPtr peLine = pnLine;
		if (peLine == NULL) goto exit;
					
		role = peLine->getAttribute("role");
		if (stricmp(role, "main top") == 0)
		{
			if (!XMLLoadLine(pXMLDoc, peLine, role, ss[EYE_UP_LINE])) goto exit;
			ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_EYE_LINE) = 1;
		}
		else if (stricmp(role, "main bottom") == 0)
		{
			if (!XMLLoadLine(pXMLDoc, peLine, role, ss[EYE_BELOW_LINE])) goto exit;
			ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_EYE_LINE) = 1;
		}
		else if (stricmp(role, "extra top") == 0)
		{
            //Fix bug#327914
			if (!XMLLoadLine(pXMLDoc, peLine, role, ss[EYE_DOUBLE_LINE])) goto exit;
			ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_DOUBLE_LINE) = 1;
		}
		else if (stricmp(role, "extra side") == 0)
		{
			if (!XMLLoadLine(pXMLDoc, peLine, role, ss[EYE_TEST_LINE])) goto exit;
			ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE) = 1;
		}
		else if (stricmp(role, "extra bottom") == 0)
		{
			if (!XMLLoadLine(pXMLDoc, peLine, role, ss[EYE_TEST_LINE1])) goto exit;
			ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE1) = 1;
		}
	}

	for (int i = 0; i < pnOvals->Getlength(); i++)
	{
		_bstr_t role;

		MSXML2::IXMLDOMNodePtr pnOval = pnOvals->Getitem(i);
		if (pnOval == NULL) goto exit;
		MSXML2::IXMLDOMElementPtr peOval = pnOval;
		if (peOval == NULL) goto exit;
					
		role = peOval->getAttribute("role");
		if (stricmp(role, "main pupil") == 0)
		{
			if (!XMLLoadOval(pXMLDoc, peOval, role, ss[EYE_BALL])) goto exit;
			ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_BALL) = 1;
		}
		else if (stricmp(role, "main highlight") == 0)
		{
			if (!XMLLoadOval(pXMLDoc, peOval, role, ss[EYE_BRIGHT_POINT])) goto exit;
			ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_BRIGHT_POINT) = 1;
		}
	}

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLLoadNose(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, CPartResult & pr)
{
	bool ret = false;

	CStroke stroke;
	CStrokeSet ss1, ss2;

	MSXML2::IXMLDOMNodeListPtr pnLines = peFacepart->selectNodes("line");
	for (int i = 0; i < pnLines->Getlength(); i++)
	{
		_bstr_t role;

		MSXML2::IXMLDOMNodePtr pnLine = pnLines->Getitem(i);
		if (pnLine == NULL) goto exit;
		MSXML2::IXMLDOMElementPtr peLine = pnLine;
		if (peLine == NULL) goto exit;
					
		role = peLine->getAttribute("role");
		if (!XMLLoadLine(pXMLDoc, peLine, role, stroke)) goto exit;

		if (strnicmp(role, "main", 4) == 0)
			ss1.push_back(stroke);
		else if (strnicmp(role, "extra", 5) == 0)
			ss2.push_back(stroke);
	}

	pr.m_vStrokeSet.push_back(ss1);
	if (ss2.size() > 0) pr.m_vStrokeSet.push_back(ss2);

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLLoadOther(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, CStrokeSet & ss)
{
	bool ret = false;

	MSXML2::IXMLDOMNodeListPtr pnLines = peFacepart->selectNodes("line");
	ss.resize(pnLines->Getlength());
	for (int i = 0; i < pnLines->Getlength(); i++)
	{
		_bstr_t role;

		MSXML2::IXMLDOMNodePtr pnLine = pnLines->Getitem(i);
		if (pnLine == NULL) goto exit;
		MSXML2::IXMLDOMElementPtr peLine = pnLine;
		if (peLine == NULL) goto exit;
					
		role = peLine->getAttribute("role");
		if (!XMLLoadLine(pXMLDoc, peLine, role, ss[i])) goto exit;
	}

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLLoadSpline(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peSpline, const char *role, CCubicBezierSpline & spline)
{
	bool ret = false;

	_bstr_t points;

	points = peSpline->getAttribute("points");

	char *tmp1 = new char[strlen(points) + 1];
	strcpy(tmp1, points);
	char *tmp2 = new char[strlen(points) + 1];
	strcpy(tmp2, points);

	int count = 0;
	char *token = strtok(tmp1, ",");
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, ",");
	}

	spline.m_Segments.resize(count / 8);
	count = 0;
	token = strtok(tmp2, ",");
	while (token != NULL)
	{
		if (count % 2)
        {
			spline.m_Segments[count / 8].m_ControlPoints[(count / 2) % 4].Y = (REAL)strtod(token, NULL);
        }
		else
        {
			spline.m_Segments[count / 8].m_ControlPoints[(count / 2) % 4].X = (REAL)strtod(token, NULL);
        }
		count++;
		token = strtok(NULL, ",");
	}

    ret = true;
    return ret;
}

bool CSketchResult::XMLLoadLine(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peLine, const char *role, CStroke & stroke)
{
	bool ret = false;

	_bstr_t points;

	stroke.m_ucLineColor = peLine->getAttribute("linecolor");
	stroke.m_ucLineWidth = peLine->getAttribute("linewidth");
	if (STROKE_REGION_CATMULLROM == stroke.m_ucLineWidth)
	{
		stroke.m_Type = STROKE_REGION_CATMULLROM;
	}
	else if (STROKE_REGION_CUBICBEZIERS == stroke.m_ucLineWidth)
	{
		stroke.m_Type = STROKE_REGION_CUBICBEZIERS;
	}
	else if (stroke.m_ucLineWidth < STROKE_SIMPLELINE)
	{
		stroke.m_Type = STROKE_STROKE;
	}
	else
	{
		stroke.m_Type = STROKE_SIMPLELINE;
		stroke.m_ucLineWidth -= STROKE_SIMPLELINE;
	}
	
	points = peLine->getAttribute("points");

	char *tmp1 = new char[strlen(points) + 1];
	strcpy(tmp1, points);
	char *tmp2 = new char[strlen(points) + 1];
	strcpy(tmp2, points);

	int count = 0;
	char *token = strtok(tmp1, ",");
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, ",");
	}

	stroke.m_Path.Resize(count / 2);
	count = 0;
	token = strtok(tmp2, ",");
	while (token != NULL)
	{
		if (count % 2)
			stroke.m_Path.PointY(count / 2) = strtod(token, NULL);
		else
			stroke.m_Path.PointX(count / 2) = strtod(token, NULL);
		count++;
		token = strtok(NULL, ",");
	}

    ret = true;
    return ret;
}

bool CSketchResult::XMLLoadOval(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peOval, const char *role, CStroke & stroke)
{
	bool ret = false;

	_bstr_t points;

	stroke.m_ucLineColor = peOval->getAttribute("fillcolor");
	points = peOval->getAttribute("points");

	double tmp[4];
	char *tmp1 = new char[strlen(points) + 1];
	strcpy(tmp1, points);

	int count = 0;
	char *token = strtok(tmp1, ",");
	while (token != NULL && count < 4)
	{
		tmp[count] = strtod(token, NULL);
		count++;
		token = strtok(NULL, ",");
	}
	if (count < 4) goto exit;

	stroke.m_Path.Resize(2);
	stroke.m_Path.PointX(0) = (tmp[0] + tmp[2]) / 2.0;
	stroke.m_Path.PointY(0) = (tmp[1] + tmp[3]) / 2.0;
	stroke.m_Path.PointX(1) = (tmp[2] - tmp[0]) / 2.0;

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLSave(const WCHAR *fname)
{
    bool ret = false;

	HRESULT hr;
	MSXML2::IXMLDOMDocument2Ptr pXMLDoc;

	hr = pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));
	if (FAILED(hr)) goto exit;

	pXMLDoc->preserveWhiteSpace = VARIANT_TRUE;
   
	// Create a processing instruction targeted for xml.
	{
		MSXML2::IXMLDOMProcessingInstructionPtr pi;
		pi = pXMLDoc->createProcessingInstruction("xml", "version=\"1.0\"");
		if (pi == NULL) goto exit;
		pXMLDoc->appendChild((MSXML2::IXMLDOMProcessingInstruction *)pi);
	}

	// Create a comment for the document.
	{
		MSXML2::IXMLDOMCommentPtr pc;
		pc = pXMLDoc->createComment("cartoon xml file created using cartoonwiz.");
		if (pc == NULL) goto exit;
		pXMLDoc->appendChild(pc);
	}

	{
		MSXML2::IXMLDOMElementPtr pePerson;
		pePerson = pXMLDoc->createElement("person");
		if (pePerson == NULL) goto exit;

		MSXML2::IXMLDOMAttributePtr pa;
		pa = pXMLDoc->createAttribute("name");
		if (pa == NULL) goto exit;
		pa->value = m_name;
		pePerson->setAttributeNode(pa);

		pXMLDoc->appendChild(pePerson);

		{
			MSXML2::IXMLDOMElementPtr peBaseface;
			peBaseface = pXMLDoc->createElement("baseface");
			if (peBaseface == NULL) goto exit;

			MSXML2::IXMLDOMAttributePtr pa;

			pa = pXMLDoc->createAttribute("id");
			if (pa == NULL) goto exit;
			pa->value = m_id;
			peBaseface->setAttributeNode(pa);

			pa = pXMLDoc->createAttribute("coords");
			if (pa == NULL) goto exit;
			pa->value = "0,0,512,512";
			peBaseface->setAttributeNode(pa);

			pa = pXMLDoc->createAttribute("renderstyle");
			if (pa == NULL) goto exit;
			//pa->value = m_renderstyle;
            pa->value = m_nRenderStyle;
			peBaseface->setAttributeNode(pa);

            pa = pXMLDoc->createAttribute("config");
			if (pa == NULL) goto exit;
            pa->value = m_nConfig;
			peBaseface->setAttributeNode(pa);

            pa = pXMLDoc->createAttribute("haircategory");
			if (pa == NULL) goto exit;
            pa->value = m_nHairCatID;
			peBaseface->setAttributeNode(pa);

            pa = pXMLDoc->createAttribute("hairtemplate");
			if (pa == NULL) goto exit;
            pa->value = m_nHairTempID;
			peBaseface->setAttributeNode(pa);

            pa = pXMLDoc->createAttribute("glassescategory");
			if (pa == NULL) goto exit;
            pa->value = m_nGlassesCatID;
			peBaseface->setAttributeNode(pa);

            pa = pXMLDoc->createAttribute("glassestemplate");
			if (pa == NULL) goto exit;
            pa->value = m_nGlassesID;
			peBaseface->setAttributeNode(pa);

            pa = pXMLDoc->createAttribute("beardcategory");
			if (pa == NULL) goto exit;
            pa->value = m_nBeardCatID;
			peBaseface->setAttributeNode(pa);

            pa = pXMLDoc->createAttribute("beardtemplate");
			if (pa == NULL) goto exit;
            pa->value = m_nBeardID;
			peBaseface->setAttributeNode(pa);

            pa = pXMLDoc->createAttribute("linestyle");
			if (pa == NULL) goto exit;
			pa->value = m_linestyle;
			peBaseface->setAttributeNode(pa);

			{
				if (m_defPartRetCtrl.m_vStrokeCtrlSet.size() != 1) goto exit;
				if (m_defPartRetCtrl.m_vStrokeCtrlSet[0].size() != 1) goto exit;
				if (!XMLSaveStrokeCtrl(pXMLDoc, peBaseface, "stroke ctrl", m_defPartRetCtrl.m_vStrokeCtrlSet[0][0])) goto exit;

				if (!XMLSaveShape(pXMLDoc, peBaseface, "face shape", m_shape)) goto exit;

				if (m_agPartRet[PART_CONTOUR].m_vStrokeSet.size() != 1) goto exit;
				if (m_agPartRet[PART_CONTOUR].m_vStrokeSet[0].size() != 1) goto exit;
				if (!XMLSaveOther(pXMLDoc, peBaseface, "face line", m_agPartRet[PART_CONTOUR].m_vStrokeSet[0])) goto exit;

				if (m_agPartRet[PART_EAR].m_vStrokeSet.size() != 2) goto exit;
				if (m_agPartRet[PART_EAR].m_vStrokeSet[0].size() != 1) goto exit;
				if (m_agPartRet[PART_EAR].m_vStrokeSet[1].size() != 1) goto exit;
				if (!XMLSaveOther(pXMLDoc, peBaseface, "left ear", m_agPartRet[PART_EAR].m_vStrokeSet[0])) goto exit;
				if (!XMLSaveOther(pXMLDoc, peBaseface, "right ear", m_agPartRet[PART_EAR].m_vStrokeSet[1])) goto exit;

				if (m_agPartRet[PART_BROW].m_vStrokeSet.size() != 2)
				{
					goto exit;
				}
				if (!XMLSaveOther(pXMLDoc, peBaseface, "left brow", m_agPartRet[PART_BROW].m_vStrokeSet[0])) goto exit;
				if (!XMLSaveOther(pXMLDoc, peBaseface, "right brow", m_agPartRet[PART_BROW].m_vStrokeSet[1])) goto exit;

			    if (m_agPartRet[PART_EYE].m_vStrokeSet.size() != 2) goto exit;
				if (!XMLSaveEye(pXMLDoc, peBaseface, "left eye", m_agPartRet[PART_EYE].m_vStrokeSet[0])) goto exit;
				if (!XMLSaveEye(pXMLDoc, peBaseface, "right eye", m_agPartRet[PART_EYE].m_vStrokeSet[1])) goto exit;

			    if (m_agPartRet[PART_NOSE].m_vStrokeSet.size() != 1 && m_agPartRet[PART_NOSE].m_vStrokeSet.size() != 2) goto exit;
				if (!XMLSaveNose(pXMLDoc, peBaseface, "nose", m_agPartRet[PART_NOSE])) goto exit;

			    if (m_agPartRet[PART_MOUTH].m_vStrokeSet.size() != 1) goto exit;
				if (!XMLSaveOther(pXMLDoc, peBaseface, "mouth", m_agPartRet[PART_MOUTH].m_vStrokeSet[0])) goto exit;

				peBaseface->appendChild(pXMLDoc->createTextNode("\n\t"));
			}

			pXMLDoc->documentElement->appendChild(pXMLDoc->createTextNode("\n\t"));
			pXMLDoc->documentElement->appendChild(peBaseface);
			pXMLDoc->documentElement->appendChild(pXMLDoc->createTextNode("\n"));
		}
	}

	hr = pXMLDoc->save(fname);
	if (FAILED(hr)) goto exit;

	pXMLDoc.Release();

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLSaveStrokeCtrl(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CStrokeCtrl & sc)
{
    bool ret = false;

	MSXML2::IXMLDOMElementPtr peRenderctrl;
	MSXML2::IXMLDOMAttributePtr pa;

	peRenderctrl = pXMLDoc->createElement("renderctrl");
	if (peRenderctrl == NULL) goto exit;

	pa = pXMLDoc->createAttribute("role");
	if (pa == NULL) goto exit;
	pa->value = role;
	peRenderctrl->setAttributeNode(pa);

	if (!XMLSaveSpline(pXMLDoc, peRenderctrl, "spline 1", sc.m_Splines[0])) goto exit;
	if (!XMLSaveSpline(pXMLDoc, peRenderctrl, "spline 2", sc.m_Splines[1])) goto exit;

	peRenderctrl->appendChild(pXMLDoc->createTextNode("\n\t\t"));

	peBaseface->appendChild(pXMLDoc->createTextNode("\n\t\t"));
	peBaseface->appendChild(peRenderctrl);

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLSaveShape(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CShape & shape)
{
    bool ret = false;

	MSXML2::IXMLDOMElementPtr peShape;
	MSXML2::IXMLDOMAttributePtr pa;

	char tmp[32];
	char buf[2048];

	buf[0] = '\0';
	if (shape.PointNum() < 2) goto exit;
	for (int i = 0; i < shape.PointNum(); i++)
	{
		sprintf(tmp, "%d,%d,", (int)shape.PointX(i), (int)shape.PointY(i));
		strcat(buf, tmp);
	}
	buf[strlen(buf) - 1] = '\0';

	peShape = pXMLDoc->createElement("shape");
	if (peShape == NULL) goto exit;

	pa = pXMLDoc->createAttribute("role");
	if (pa == NULL) goto exit;
	pa->value = role;
	peShape->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("points");
	if (pa == NULL) goto exit;
	pa->value = buf;
	peShape->setAttributeNode(pa);

	peShape->appendChild(pXMLDoc->createTextNode("\n\t\t"));

	peBaseface->appendChild(pXMLDoc->createTextNode("\n\t\t"));
	peBaseface->appendChild(peShape);

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLSaveEye(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CStrokeSet & ss)
{
    bool ret = false;

	MSXML2::IXMLDOMElementPtr peFacepart;
	MSXML2::IXMLDOMAttributePtr pa;

	peFacepart = pXMLDoc->createElement("facepart");
	if (peFacepart == NULL) goto exit;

	pa = pXMLDoc->createAttribute("role");
	if (pa == NULL) goto exit;
	pa->value = role;
	peFacepart->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("type");
	if (pa == NULL) goto exit;
	pa->value = "custom";
	peFacepart->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("position");
	if (pa == NULL) goto exit;
	pa->value = "auto";
	peFacepart->setAttributeNode(pa);

	if (!XMLSaveLine(pXMLDoc, peFacepart, "main top", ss[EYE_UP_LINE])) goto exit;
	if (!XMLSaveLine(pXMLDoc, peFacepart, "main bottom", ss[EYE_BELOW_LINE])) goto exit;

	if (ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_DOUBLE_LINE))
	{
        //Fix bug#327914
		if (!XMLSaveLine(pXMLDoc, peFacepart, "extra top", ss[EYE_DOUBLE_LINE])) goto exit;
	}

	if (ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE))
	{
		if (!XMLSaveLine(pXMLDoc, peFacepart, "extra side", ss[EYE_TEST_LINE])) goto exit;
	}

	if (ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE1))
	{
		if (!XMLSaveLine(pXMLDoc, peFacepart, "extra bottom", ss[EYE_TEST_LINE1])) goto exit;
	}

	if (ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_BALL))
	{
		if (!XMLSaveOval(pXMLDoc, peFacepart, "main pupil", ss[EYE_BALL])) goto exit;
	}

	if (ss[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_BRIGHT_POINT))
	{
		if (!XMLSaveOval(pXMLDoc, peFacepart, "main highlight", ss[EYE_BRIGHT_POINT])) goto exit;
	}

	peFacepart->appendChild(pXMLDoc->createTextNode("\n\t\t"));

	peBaseface->appendChild(pXMLDoc->createTextNode("\n\t\t"));
	peBaseface->appendChild(peFacepart);

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLSaveNose(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CPartResult & pr)
{
    bool ret = false;

	int i;
	char tmp[32];
	MSXML2::IXMLDOMElementPtr peFacepart;
	MSXML2::IXMLDOMAttributePtr pa;

	peFacepart = pXMLDoc->createElement("facepart");
	if (peFacepart == NULL) goto exit;

	pa = pXMLDoc->createAttribute("role");
	if (pa == NULL) goto exit;
	pa->value = role;
	peFacepart->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("type");
	if (pa == NULL) goto exit;
	pa->value = "custom";
	peFacepart->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("position");
	if (pa == NULL) goto exit;
	pa->value = "auto";
	peFacepart->setAttributeNode(pa);

	if (pr.m_vStrokeSet.size() >= 1)
	{
		for (i = 0; i < pr.m_vStrokeSet[0].size(); i++)
		{
			sprintf(tmp, "main %d", i + 1);
			if (!XMLSaveLine(pXMLDoc, peFacepart, tmp, pr.m_vStrokeSet[0][i])) goto exit;
		}
	}

	if (pr.m_vStrokeSet.size() >= 2)
	{
		for (i = 0; i < pr.m_vStrokeSet[1].size(); i++)
		{
			sprintf(tmp, "extra %d", i + 1);
			if (!XMLSaveLine(pXMLDoc, peFacepart, tmp, pr.m_vStrokeSet[1][i])) goto exit;
		}
	}

	peFacepart->appendChild(pXMLDoc->createTextNode("\n\t\t"));

	peBaseface->appendChild(pXMLDoc->createTextNode("\n\t\t"));
	peBaseface->appendChild(peFacepart);

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLSaveOther(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CStrokeSet & ss)
{
    bool ret = false;

	int i;
	char tmp[32];
	MSXML2::IXMLDOMElementPtr peFacepart;
	MSXML2::IXMLDOMAttributePtr pa;

	peFacepart = pXMLDoc->createElement("facepart");
	if (peFacepart == NULL) goto exit;

	pa = pXMLDoc->createAttribute("role");
	if (pa == NULL) goto exit;
	pa->value = role;
	peFacepart->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("type");
	if (pa == NULL) goto exit;
	pa->value = "custom";
	peFacepart->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("position");
	if (pa == NULL) goto exit;
	pa->value = "auto";
	peFacepart->setAttributeNode(pa);

	for (i = 0; i < ss.size(); i++)
	{
		sprintf(tmp, "main %d", i + 1);
		if (!XMLSaveLine(pXMLDoc, peFacepart, tmp, ss[i])) goto exit;
	}

	peFacepart->appendChild(pXMLDoc->createTextNode("\n\t\t"));

	peBaseface->appendChild(pXMLDoc->createTextNode("\n\t\t"));
	peBaseface->appendChild(peFacepart);

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLSaveSpline(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peRenderctrl, const char *role, const CCubicBezierSpline & spline)
{
    bool ret = false;

	MSXML2::IXMLDOMElementPtr peSpline;
	MSXML2::IXMLDOMAttributePtr pa;

	char tmp[32];
	char buf[2048];

	buf[0] = '\0';
	int iSegments = spline.NbSegment();
	if (iSegments < 1) goto exit;
	for (int i = 0; i < iSegments; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sprintf(tmp, "%f,%f,", spline.m_Segments[i].m_ControlPoints[j].X, spline.m_Segments[i].m_ControlPoints[j].Y);
			strcat(buf, tmp);
		}
	}
	buf[strlen(buf) - 1] = '\0';

	peSpline = pXMLDoc->createElement("spline");
	if (peSpline == NULL) goto exit;

	pa = pXMLDoc->createAttribute("role");
	if (pa == NULL) goto exit;
	pa->value = role;
	peSpline->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("points");
	if (pa == NULL) goto exit;
	pa->value = buf;
	peSpline->setAttributeNode(pa);

	peRenderctrl->appendChild(pXMLDoc->createTextNode("\n\t\t\t"));
	peRenderctrl->appendChild(peSpline);

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLSaveLine(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, const CStroke & stroke)
{
    bool ret = false;

	MSXML2::IXMLDOMElementPtr peLine;
	MSXML2::IXMLDOMAttributePtr pa;

	char tmp[32];
	char buf[2048];

	buf[0] = '\0';
	if (stroke.m_Path.PointNum() < 2) goto exit;
	for (int i = 0; i < stroke.m_Path.PointNum(); i++)
	{
		sprintf(tmp, "%d,%d,", (int)stroke.m_Path.PointX(i), (int)stroke.m_Path.PointY(i));
		strcat(buf, tmp);
	}
	buf[strlen(buf) - 1] = '\0';
	
	peLine = pXMLDoc->createElement("line");
	if (peLine == NULL) goto exit;

	pa = pXMLDoc->createAttribute("role");
	if (pa == NULL) goto exit;
	pa->value = role;
	peLine->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("linecolor");
	if (pa == NULL) goto exit;
	pa->value = stroke.m_ucLineColor;
	peLine->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("linewidth");
	if (pa == NULL) goto exit;

	unsigned char ucLineWidth = stroke.m_ucLineWidth;
	if (STROKE_STROKE == stroke.m_Type)
	{
		if (ucLineWidth<1)
			ucLineWidth = 1;
		if (ucLineWidth>=STROKE_SIMPLELINE)
			ucLineWidth = STROKE_SIMPLELINE-1;
	}
	else if (STROKE_SIMPLELINE == stroke.m_Type)
	{
		if (ucLineWidth<1)
			ucLineWidth = 1;
		if (ucLineWidth>=STROKE_SIMPLELINE)
			ucLineWidth = STROKE_SIMPLELINE-1;
		ucLineWidth += STROKE_SIMPLELINE;
	}
	else if (STROKE_REGION_CATMULLROM == stroke.m_Type)
	{
		ucLineWidth = 0;
	}
	else if (STROKE_REGION_CUBICBEZIERS == stroke.m_Type)
	{
		ucLineWidth = 255;
	}

	pa->value = ucLineWidth;
	peLine->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("points");
	if (pa == NULL) goto exit;
	pa->value = buf;
	peLine->setAttributeNode(pa);

	peFacepart->appendChild(pXMLDoc->createTextNode("\n\t\t\t"));
	peFacepart->appendChild(peLine);

    ret = true;

exit:

    return ret;
}

bool CSketchResult::XMLSaveOval(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, const CStroke & stroke)
{
    bool ret = false;

	MSXML2::IXMLDOMElementPtr peOval;
	MSXML2::IXMLDOMAttributePtr pa;

	char buf[2048];
	sprintf(buf, "%d,%d,%d,%d", (int)stroke.m_Path.PointX(0) - (int)stroke.m_Path.PointX(1), (int)stroke.m_Path.PointY(0) - (int)stroke.m_Path.PointX(1), (int)stroke.m_Path.PointX(0) + (int)stroke.m_Path.PointX(1), (int)stroke.m_Path.PointY(0) + (int)stroke.m_Path.PointX(1));
	
	peOval = pXMLDoc->createElement("oval");
	if (peOval == NULL) goto exit;

	pa = pXMLDoc->createAttribute("role");
	if (pa == NULL) goto exit;
	pa->value = role;
	peOval->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("fillcolor");
	if (pa == NULL) goto exit;
	pa->value = stroke.m_ucLineColor;
	peOval->setAttributeNode(pa);

	pa = pXMLDoc->createAttribute("points");
	if (pa == NULL) goto exit;
	pa->value = buf;
	peOval->setAttributeNode(pa);

	peFacepart->appendChild(pXMLDoc->createTextNode("\n\t\t\t"));
	peFacepart->appendChild(peOval);

    ret = true;

exit:

    return ret;
}
*/

void ApplyTransform(const CStroke& orgStroke, CStroke& desStroke, const CGeomTransform& geomTrans, const bool fNotInv, const bool fResample)
{
	// Check input
	if ( &orgStroke == &desStroke )
	{
		printf("\n Error: orgStroke is the same with desStroke!");
		return;
	}
	if ( ( fNotInv && ( !geomTrans.TransReady() ) ) || ( (!fNotInv) && (!geomTrans.InvTransReady())))
	{
		printf("\n Error: geomtrans is not initialized!");
		desStroke = orgStroke;
		return;
	}
	if (fResample)
	{
		printf("\n Warning: not support resample now!");
	}
	
	// GeomTrans
	desStroke = orgStroke;
    ApplyTransform(orgStroke.m_Path, desStroke.m_Path, geomTrans, fNotInv);
}

void ApplyTransform(const CStrokeSet& orgStrokeSet, CStrokeSet& desStrokeSet, const CGeomTransform& geomTrans, const bool fNotInv, const bool fResample)
{
	// Check input
	if ( &orgStrokeSet == &desStrokeSet )
	{
		printf("\n Error: orgStroke is the same with desStroke!");
		return;
	}
	if ( ( fNotInv && ( !geomTrans.TransReady() ) ) || ( (!fNotInv) && (!geomTrans.InvTransReady())))
	{
		printf("\n Error: geomtrans is not initialized!");
		desStrokeSet = orgStrokeSet;
		return;
	}
	if (fResample)
	{
		printf("\n Warning: not support resample now!");
	}
	
	// GeomTrans
	desStrokeSet = orgStrokeSet;
	for (int iStroke = 0; iStroke < orgStrokeSet.size(); iStroke++)
	{
		ApplyTransform(orgStrokeSet[iStroke], desStrokeSet[iStroke], geomTrans, fNotInv, fResample);
	}
}

//modified by lin begin, 2003-5
void Apply2DRTSTransform(CVisTransform4x4& transM, CStroke& DesStroke)
{
	CShape& StrokePath = DesStroke.m_Path;
	Apply2DRTSTransform(transM, StrokePath);
}

void Apply2DRTSTransform(CVisTransform4x4& transM, CStrokeSet& DesStrokeSet)
{
	for(int j=0; j<DesStrokeSet.size(); j++)
	{
		Apply2DRTSTransform(transM, DesStrokeSet[j]);		
	}
}

void Apply2DRTSTransform(CVisTransform4x4& transM, std::vector<CStrokeSet>& vecDesStrokeSet)
{
	for(int i=0; i<vecDesStrokeSet.size(); i++)
		Apply2DRTSTransform(transM, vecDesStrokeSet[i]);		
}

void Apply2DRTSTransform(CVisTransform4x4& transM, CPartResult& PartResult)
{
	std::vector<CStrokeSet>& StrokeSet = PartResult.m_vStrokeSet;
	Apply2DRTSTransform(transM, StrokeSet);
}

//modified by lin end, 2003-5

void SmoothCurve(CShape& shape, double m_dSmooth_Param1, double m_dSmooth_Param2)
{
	if (shape.PointNum()<3) return;

	std::vector<CPoint2> vPoints;
	std::vector<CPoint2> vSmoothedPoints;

	Shape2vPoint2(shape, vPoints);
	if ( shape.GetPoint(0)==shape.GetPoint(shape.PointNum()-1) ) // CONTOUR_CLOSED
	{
		vPoints.insert(vPoints.begin(), shape.GetPoint(shape.PointNum()-1));
	}
			
	// Smooth
	vSmoothedPoints.resize(vPoints.size());
	vSmoothedPoints[0] = vPoints[0];
	vSmoothedPoints[vPoints.size()-1] = vPoints[vPoints.size()-1];

	for (int i=1; i<vPoints.size()-1; i++)
	{
		CPoint2 tempP = vPoints[i] * (1 - m_dSmooth_Param1) + ((CPoint2)(vPoints[i-1] + vPoints[i+1]))*0.5*m_dSmooth_Param1;
		vSmoothedPoints[i] = tempP;
	}
			
	// push
	std::vector<CPoint2> vDet;
	vDet.resize(vPoints.size());
	for (i=0; i<vPoints.size(); i++)
	{
		vDet[i] = (CPoint2)(vSmoothedPoints[i] - vPoints[i]);
	}
			
	CPoint2 SumDet;
	for (i=1; i<vPoints.size()-1; i++)
	{
		SumDet = ((CPoint2)(vDet[i] + vDet[i-1] + vDet[i+1]))/3;
		CPoint2 tempP = vSmoothedPoints[i] - SumDet * m_dSmooth_Param2;
		vPoints[i] = tempP;
	}

	if ( shape.GetPoint(0)==shape.GetPoint(shape.PointNum()-1) ) // CONTOUR_CLOSED
	{
		vSmoothedPoints.erase(vSmoothedPoints.begin());
	}
	vPoint22Shape(vPoints, shape);
}

void SmoothCurveSet(std::vector<CShape>& vShape, double m_dSmooth_Param1, double m_dSmooth_Param2)
{
	for (int i = 0; i<vShape.size(); i++)
	{
		SmoothCurve(vShape[i], m_dSmooth_Param1, m_dSmooth_Param2);
	}
}

