// VisSDStream.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved

#ifndef _VIS_SD_STREAM_INL_
#define _VIS_SD_STREAM_INL_


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG



template <class T>
inline bool CVisSortable<T>::operator< (const T& a) const
{
    return (memcmp(this, &a, sizeof(T)) < 0);
}

template <class T>
inline bool CVisSortable<T>::operator==(const CVisSortable<T>& a) const
{
    return (memcmp(this, &a, sizeof(T)) ==0);
}

/*
inline CVisString::CVisString(void)
	{}
inline CVisString::CVisString(std::string s)
	{ *((std::string *) this) = s; }
inline CVisString::CVisString(const char *s)
	{ *((std::string *) this) = s; }
inline CVisString::operator const char*(void)
	{ return c_str(); }
inline char& CVisString::operator[](int i)
	{ return (*(std::string *) this)[i]; }
*/

// helper function
// UNDONE:  CHnage the name of this function!

inline void erase(CVisIntV& v, int i)
{
    CVisIntV::iterator it = std::find(v.begin(), v.end(), i);
    if (it != v.end())
        v.erase(it);
}



#ifdef VIS_INCLUDE_SDSTREAM



inline CVisIntV& CVisSDBase::ActiveFields(int read)
{
    return *(read ? &m_read_fields : &m_write_fields);
}
inline CVisSDBaseV& CVisSDBase::TypeList(void)
	{ return *type_list; }
inline int CVisSDBase::TypeListId(void)
	{ return m_type_list_id; }
inline int CVisSDBase::TypeListSize(void)
	{ return TypeList().size(); }


template <class T>
inline CVisSDObject<T>::CVisSDObject(void)
  : CVisSDBase(T::eftEnd)
{
    for (int i = 1; i < int(T::eftEnd); i++)   // eftName is not active
        InsertField(i);
    AdjustFields();
}

template <class T>
inline const char *CVisSDObject<T>::FieldDescriptor(int field_id) {
    return m_object.ReadWriteField(m_dummy, field_id);
}

template <class T>
inline void CVisSDObject<T>::InsertField(int f)
	{ m_write_fields.push_back(f); }

template <class T>
inline void CVisSDObject<T>::EraseField(int f)
	{ erase(m_write_fields, f); }

// Default implementation
template <class T>
inline void CVisSDObject<T>::AdjustFields(void)
	{ } // use all fields


inline CVisSDStream::CVisSDStream(int indent) :     // indent < 0  =>  no indentation
    m_status(Closed), m_stream(0), m_indent_level(indent) {}

inline FILE * CVisSDStream::Stream(void) { return m_stream; }
inline CVisSDStream::status CVisSDStream::Status(void) { return m_status; }

inline void CVisSDStream::CollectDescriptions(void)
		{ m_status = CollectDescription; }
inline void CVisSDStream::WriteOutDescriptions(void)
		{ CVisSDBase::WriteDescriptions(*this); }
inline void CVisSDStream::MarkDescription(int i, int v)
		{ m_write_description[i] |= v; }
inline int CVisSDStream::DescriptionMarked(int i)
		{ return m_write_description[i] & 1; }
inline int CVisSDStream::DescriptionWritten(int i)
		{ return m_write_description[i] & 2; }

inline int CVisSDStream::GetChar(void)
{
    int c = getc(m_stream);
    m_line_number += (c == '\n');
    return c;
}



// Default is provided for types that won't work with self-describing streams.
// (This is needed, for example, to avoid the need for Exemplar methods for
// images that use non-standard pixel types.)
template <class T>
inline CVisSDObject<T>& CVisSDObject<T>::Exemplar(void)
{
	assert(0);
	return (CVisSDObject<T>&) *((CVisSDObject<T> *) 0);
}


template <class T>
inline CVisSDStream& VisSDReadWriteClass(CVisSDStream& s, T& v)
{
	CVisSDObject<T>& sdobjectExemplar = CVisSDObject<T>::Exemplar();

    int read = s.Status() == CVisSDStream::Read;
    int type_list_id = sdobjectExemplar.TypeListId();
    if (s.Status() == CVisSDStream::CollectDescription &&
        ! s.DescriptionWritten(type_list_id))
        s.MarkDescription(type_list_id);
    if (s.Status() == CVisSDStream::Write && ! s.DescriptionWritten(type_list_id)) {
        s.CollectDescriptions();    // start collecting descriptions to write
        VisSDReadWriteClass(s, v);  // call self recursively
        s.WriteOutDescriptions();   // finish the writing process
    }
    CVisIntV& f = sdobjectExemplar.ActiveFields(read);
    s.OpenParen();
    int n = f.size(), i = 0;
    for (CVisIntV::iterator it = f.begin(); it != f.end(); it++, i++) {
        if (*it < 0) 
            s.SkipItem();
        else
            v.ReadWriteField(s, *it);
        if (i < n-1)   // no comma after name field
            s.Comma();
    }
    s.CloseParen();
    return s;
}


template <class T, class E>
inline CVisSDStream& VisSDReadWriteContainer(CVisSDStream& s, T& v, E def)
{
    // Read/write a container class
    int read = (s.Status() == CVisSDStream::Read);
    s.OpenParen();
    if (s.Status() == CVisSDStream::Read) {
        v.clear();
        while (s.PeekChar() != '}') {
            s << def;
            v.insert(v.end(), def);
            if (s.SomeChar(",}") == ',')
                s.Comma();
        }
    } else {
        int n = v.size(), i = 0;
		if (sizeof(E) > 16)     // hack constant, 16 bytes
        	s.NewLine();
        for (T::iterator it = v.begin(); it != v.end(); it++, i++) {
            s << *it;
            if (i < n-1)
                s.Comma();
            if (sizeof(E) > 16)     // hack constant, 16 bytes
                s.NewLine();
        if (s.Status() == CVisSDStream::CollectDescription)
            break;  // only need to check first one in list
        }
    }
    s.CloseParen();
    return s;
}

template <class T, class E>
inline CVisSDStream& VisSDReadWriteContainerP(CVisSDStream& s, T& v, E *ptr)
{
    // Read/write a container class full of pointers
    int read = (s.Status() == CVisSDStream::Read);
    s.OpenParen();
	s.NewLine();
    if (s.Status() == CVisSDStream::Read) {
        v.clear();
        while (s.PeekChar() != '}') {
            ptr = new E;
            s << *ptr;
            v.insert(v.end(), ptr);
            if (s.SomeChar(",}") == ',')
                s.Comma();
        }
    } else {
        int n = v.size(), i = 0;
        for (T::iterator it = v.begin(); it != v.end(); it++, i++) {
            if (*it == 0)
                continue;
            s << **it;
            if (i < n-1)
                s.Comma();
            s.NewLine();
            if (s.Status() == CVisSDStream::CollectDescription)
                break;  // only need to check first one in list
        }
    }
	s.NewLine();
    s.CloseParen();
    return s;
}

template <class T>
inline CVisSDStream& operator<<(CVisSDStream& s, std::vector<T,
		std::allocator<T> >& v)
{
    return VisSDReadWriteContainer(s, v, T());
}

template <class T>
inline CVisSDStream& operator<<(CVisSDStream& s,
		std::list<T, std::allocator<T> >& v)
{
    return VisSDReadWriteContainer(s, v, T());
}

template <class T>
inline CVisSDStream& operator<<(CVisSDStream& s,
		std::deque<T, std::allocator<T> >& v)
{
    return VisSDReadWriteContainer(s, v, T());
}

template <class T>
inline CVisSDStream& operator<<(CVisSDStream& s, 
		std::set<T, std::less<T>, std::allocator<T> >& v)
{
    return VisSDReadWriteContainer(s, v, T());
}


#endif // VIS_INCLUDE_SDSTREAM



#ifdef _DEBUG
#undef new
#endif // _DEBUG

#endif // _VIS_SD_STREAM_INL_
