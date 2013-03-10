// VisRefCnt.h
//
// Base class for reference-counted objects.
//
// Copyright © 1997 - 2000 Microsoft Corporation, All Rights Reserved


// @class
class CVisRefCnt
{
public:
    //------------------------------------------------------------------
	// @group Constructor and Destructor
  
    // @cmember
	// Construct this object and set the reference count to one.
	CVisRefCnt(void);

    // @cmember
	// Destroy this object.  This must be virtual to make sure that
	// derived classes are properly destroyed.
	virtual ~CVisRefCnt(void);

    //------------------------------------------------------------------
	// @group Reference counting

	// @cmember
	// Add a reference to this object by increasing the internal
	// reference count.
	unsigned long AddRef(void);

	// @cmember
	// Release a reference to this object by decreasing the internal
	// reference count.  If the reference count falls to zero, this
	// object will be deleted.
	unsigned long Release(void);


private:
    // @cmember:(IMPL)
	// Reference count for this object.
    unsigned long m_cRef;
};


////////////////////////////////////////////////////////////////////////////
//
// @mfunc  | CVisRefCnt | CVisRefCnt |
//
// Constructor
//
////////////////////////////////////////////////////////////////////////////
inline CVisRefCnt::CVisRefCnt(void)
  : m_cRef(1)
{
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc  | CVisRefCnt | ~CVisRefCnt |
//
// Destructor
//
////////////////////////////////////////////////////////////////////////////
inline CVisRefCnt::~CVisRefCnt(void)
{
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc unsigned long | CVisRefCnt | AddRef |
//
// Add a reference to this object by increasing the internal
// reference count.
//
////////////////////////////////////////////////////////////////////////////
inline unsigned long CVisRefCnt::AddRef(void)
{
	return (unsigned long) InterlockedIncrement((LONG *) &m_cRef);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc unsigned long | CVisRefCnt | Release |
//
// Release a reference to this object by decreasing the internal
// reference count.  If the reference count falls to zero, this
// object will be deleted.
//
////////////////////////////////////////////////////////////////////////////
inline unsigned long CVisRefCnt::Release(void)
{
	unsigned long cRef
			= (unsigned long) InterlockedDecrement((LONG *) &m_cRef);

	if (cRef == 0)
		delete this;

	return cRef;
}
