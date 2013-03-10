#pragma once

#ifndef __GXCOLOR_H
#define __GXCOLOR_H

/*==========================================================================;
//
//  Copyright (C) 1998 - 2000 Microsoft Corporation.  All Rights Reserved.
//
// File: gxcolor.h
// Description:
//     This header file defines packed and floating point ARGB and RGB color 
//  classes.  The packed classes are assumed to be gamma-corrected.  The
//  conversion routines between the floating point and packed representations
//  perform gamma-correction.
//
//
//**************************************************************************/


//-----------------------------------------------------------------------------
// Class: GXPackedRGBA
//-----------------------------------------------------------------------------
class GXPackedRGBA
{
public:
    GXPackedRGBA() {}
    GXPackedRGBA(BYTE gray)
        { uColor = (0xFF << 24) | (gray << 16) | (gray << 8) | gray; }
    GXPackedRGBA(BYTE r, BYTE g, BYTE b, BYTE a) 
        { uColor = (a << 24) | (r << 16) | (g << 8) | b; }
    GXPackedRGBA(UINT32 _uColor) 
        { uColor = _uColor; }

    friend bool operator == (const GXPackedRGBA &c1, const GXPackedRGBA &c2);
    friend bool operator != (const GXPackedRGBA &c1, const GXPackedRGBA &c2);

    BYTE    A()  { return (uColor >> 24); }
    BYTE    R()  { return (uColor >> 16) & 0xFF; }
    BYTE    G()  { return (uColor >> 8) & 0xFF; }
    BYTE    B()  { return uColor & 0xFF; }

public:
    UINT32  uColor;
};


//-----------------------------------------------------------------------------
// Class: GXColorRGB
//-----------------------------------------------------------------------------
class GXColorRGB
{
public:
    GXColorRGB() {}
    GXColorRGB( float gray);
    GXColorRGB( float _r, float _g, float _b );

    // assignment operators
    GXColorRGB& operator += ( const GXColorRGB& );
    GXColorRGB& operator -= ( const GXColorRGB& );
    GXColorRGB& operator *= ( const GXColorRGB& );
    GXColorRGB& operator /= ( const GXColorRGB& );
    GXColorRGB& operator *= ( float );
    GXColorRGB& operator /= ( float );

    // unary operators
    GXColorRGB operator + () const;
    GXColorRGB operator - () const;

    // access operators
    float operator[] (UINT i) { return (&r)[i]; }
    float operator[] (UINT i) const { return (&r)[i]; }

    // binary operators
    friend GXColorRGB operator + ( const GXColorRGB&, const GXColorRGB& );
    friend GXColorRGB operator - ( const GXColorRGB&, const GXColorRGB& );
    friend GXColorRGB operator * ( const GXColorRGB&, const GXColorRGB& );
    friend GXColorRGB operator / ( const GXColorRGB&, const GXColorRGB& );
    friend GXColorRGB operator * ( const GXColorRGB&, float );
    friend GXColorRGB operator * ( float, const GXColorRGB& );
    friend GXColorRGB operator / ( const GXColorRGB&, float );

    friend bool operator == ( const GXColorRGB&, const GXColorRGB& );
    friend bool operator != ( const GXColorRGB&, const GXColorRGB& );

    friend float        GXLuminance(const GXColorRGB &rgb);
    friend UINT         GXMaxComponent(const GXColorRGB &rgb);
    friend GXColorRGB   GXDesaturate(const GXColorRGB &rgb);
    friend GXColorRGB   GXLerp(const GXColorRGB &c1, const GXColorRGB &c2, float s);

public:
    float r, g, b;
};


//-----------------------------------------------------------------------------
// Class: GXColorRGBA
//-----------------------------------------------------------------------------
class GXColorRGBA
{
public:
    GXColorRGBA() { }
    GXColorRGBA( float gray );
    GXColorRGBA( float _r, float _g, float _b, float _a );     // assumed premultiplied
    GXColorRGBA( const GXColorRGB& clr );
    GXColorRGBA( const GXColorRGB& clr, float _a );

    // construction function
    GXColorRGBA PreMultiply( float r, float g, float b, float a );

    // assignment operators
    GXColorRGBA& operator += ( const GXColorRGBA& );
    GXColorRGBA& operator -= ( const GXColorRGBA& );
    GXColorRGBA& operator *= ( const GXColorRGBA& );
    GXColorRGBA& operator /= ( const GXColorRGBA& );
    GXColorRGBA& operator *= ( float );
    GXColorRGBA& operator /= ( float );

    // unary operators
    GXColorRGBA operator + () const;
    GXColorRGBA operator - () const;

    // access operators
    float operator[] (UINT i) { return (&r)[i]; }
    float operator[] (UINT i) const { return (&r)[i]; }

    // binary operators
    friend GXColorRGBA operator + ( const GXColorRGBA&, const GXColorRGBA& );
    friend GXColorRGBA operator - ( const GXColorRGBA&, const GXColorRGBA& );
    friend GXColorRGBA operator * ( const GXColorRGBA&, const GXColorRGBA& );
    friend GXColorRGBA operator / ( const GXColorRGBA&, const GXColorRGBA& );
    friend GXColorRGBA operator * ( const GXColorRGBA&, float );
    friend GXColorRGBA operator * ( float, const GXColorRGBA& );
    friend GXColorRGBA operator / ( const GXColorRGBA&, float );

    friend bool operator == ( const GXColorRGBA&, const GXColorRGBA& );
    friend bool operator != ( const GXColorRGBA&, const GXColorRGBA& );

    friend float        GXLuminance(const GXColorRGBA &rgba);
    friend UINT         GXMaxComponent( const GXColorRGBA &rgba);
    friend GXColorRGBA  GXDesaturate(const GXColorRGBA &rgba);
    friend GXColorRGBA  GXLerp(const GXColorRGBA &c1, const GXColorRGBA &c2, float s);

public:
    float r, g, b, a;
};


#ifdef _GXDEFINE_DLL_ENTRYPOINTS
extern "C" 
{
    HRESULT WINAPI GXConvertRGBAToPacked(GXPackedRGBA *rgDst, GXColorRGBA *rgSrc, UINT cColors);
    HRESULT WINAPI GXConvertRGBToPacked(GXPackedRGBA *rgDst, GXColorRGB *rgSrc, UINT cColors);
    HRESULT WINAPI GXConvertPackedToRGBA(GXColorRGBA *rgDst, GXPackedRGBA *rgSrc, UINT cColors);
    HRESULT WINAPI GXConvertPackedToRGB(GXColorRGB *rgDst, GXPackedRGBA *rgSrc, UINT cColors);
}
#endif  // #ifdef _GXDEFINE_DLL_ENTRYPOINTS


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Inline Functions for GXPackedRGBA
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

inline bool 
operator == (const GXPackedRGBA &c1, const GXPackedRGBA &c2)
{
    return (c1.uColor == c2.uColor);
}

inline bool 
operator != (const GXPackedRGBA &c1, const GXPackedRGBA &c2)
{
    return (c1.uColor != c2.uColor);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Inline Functions for GXColorRGB
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

inline
GXColorRGB::GXColorRGB(float gray)
{
    r = g = b = gray;
}

inline
GXColorRGB::GXColorRGB(float _r, float _g, float _b)
{
    r = _r; g = _g; b = _b;
}

inline GXColorRGB&
GXColorRGB::operator += ( const GXColorRGB& c )
{
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

inline GXColorRGB&
GXColorRGB::operator -= ( const GXColorRGB& c )
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
}

inline GXColorRGB&
GXColorRGB::operator *= ( const GXColorRGB& c )
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

inline GXColorRGB&
GXColorRGB::operator /= ( const GXColorRGB& c )
{
    r /= c.r;
    g /= c.g;
    b /= c.b;
    return *this;
}

inline GXColorRGB&
GXColorRGB::operator *= ( float s )
{
    r *= s;
    g *= s;
    b *= s;
    return *this;
}

inline GXColorRGB&
GXColorRGB::operator /= ( float s )
{
    r /= s;
    g /= s;
    b /= s;
    return *this;
}

inline GXColorRGB
GXColorRGB::operator + () const
{
    return *this;
}

inline GXColorRGB
GXColorRGB::operator - () const
{
    return GXColorRGB( -r, -g, -b );
}

inline GXColorRGB
operator + ( const GXColorRGB& x, const GXColorRGB& y )
{
    return GXColorRGB( x.r+y.r, x.g+y.g, x.b+y.b );
}

inline GXColorRGB
operator - ( const GXColorRGB& x, const GXColorRGB& y )
{
    return GXColorRGB( x.r-y.r, x.g-y.g, x.b-y.b );
}

inline GXColorRGB
operator * ( const GXColorRGB& x, const GXColorRGB& y )
{
    return GXColorRGB( x.r*y.r, x.g*y.g, x.b*y.b );
}

inline GXColorRGB
operator * ( const GXColorRGB& c, float s )
{
    return GXColorRGB( c.r*s, c.g*s, c.b*s );
}

inline GXColorRGB
operator * ( float s, const GXColorRGB& c )
{
    return GXColorRGB( c.r*s, c.g*s, c.b*s );
}

inline GXColorRGB
operator / ( const GXColorRGB& c, float s )
{
    return GXColorRGB( c.r/s, c.g/s, c.b/s );
}

inline GXColorRGB
operator / ( const GXColorRGB& x, const GXColorRGB& y )
{
    return GXColorRGB( x.r/y.r, x.g/y.g, x.b/y.b );
}

inline bool
operator == ( const GXColorRGB& x, const GXColorRGB& y )
{
    return x.r==y.r && x.g==y.g && x.b==y.b;
}

inline bool
operator != ( const GXColorRGB& x, const GXColorRGB& y )
{
    return ! (x==y);
}

inline float
GXLuminance(const GXColorRGB &c)
{
    return 0.229f * c.r + 0.587f * c.g + 0.114f * c.b;
}

inline UINT 
GXMaxComponent(const GXColorRGB &c)
{
    UINT iMax;
    iMax = (c.g > c.r);
    if (c.b > c[iMax])
        iMax = 2;
    return iMax;
}

inline GXColorRGB 
GXLerp(const GXColorRGB &c1, const GXColorRGB &c2, float s)
{
    return c1 + s * (c2 - c1);
}

inline GXColorRGB
GXDesaturate(const GXColorRGB &rgb)
{
    static const GXColorRGB s_rgbWhite(1.f, 1.f, 1.f);

    float fMaxChan = rgb[GXMaxComponent(rgb)];
    if (fMaxChan > 1.f) 
    {
        float fYOld = GXLuminance(rgb);
        if (fYOld >= 1.f)
            return s_rgbWhite;
        // scale color to preserve hue
        GXColorRGB &rgbNew = rgb/fMaxChan;
        float fYNew = GXLuminance(rgbNew);
        // add white to preserve luminance
        return GXLerp(rgbNew, s_rgbWhite, (fYOld - fYNew) / (1.f - fYNew));
    }
    return rgb;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Inline Functions for GXColorRGBA
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

inline
GXColorRGBA::GXColorRGBA( float gray )
{
    r = g = b = gray; 
    a = 1.0f;
}

// assumed premultiplied
inline
GXColorRGBA::GXColorRGBA( float _r, float _g, float _b, float _a )
{ 
    r = _r; 
    g = _g; 
    b = _b; 
    a = _a;
}

inline
GXColorRGBA::GXColorRGBA( const GXColorRGB& clr )
{ 
    r = clr.r; 
    g = clr.g; 
    b = clr.b; 
    a = 1.0f; 
}

inline
GXColorRGBA::GXColorRGBA( const GXColorRGB& clr, float _a )
{ 
    r = clr.r; 
    g = clr.g; 
    b = clr.b; 
    a = _a; 
}

inline GXColorRGBA
PreMultiply( float r, float b, float g, float a )
{
    return GXColorRGBA( r*a, g*a, b*a, a );
}

inline GXColorRGBA&
GXColorRGBA::operator += ( const GXColorRGBA& c )
{
    r += c.r;
    g += c.g;
    b += c.b;
    a += c.a;
    return *this;
}

inline GXColorRGBA&
GXColorRGBA::operator -= ( const GXColorRGBA& c )
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    a -= c.a;
    return *this;
}

inline GXColorRGBA&
GXColorRGBA::operator *= ( const GXColorRGBA& c )
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    a *= c.a;
    return *this;
}

inline GXColorRGBA&
GXColorRGBA::operator /= ( const GXColorRGBA& c )
{
    r /= c.r;
    g /= c.g;
    b /= c.b;
    a /= c.a;
    return *this;
}


inline GXColorRGBA&
GXColorRGBA::operator *= ( float s )
{
    r *= s;
    g *= s;
    b *= s;
    a *= s;
    return *this;
}

inline GXColorRGBA&
GXColorRGBA::operator /= ( float s )
{
    float fScale = 1.f / s;
    return (*this *= fScale);
}

inline GXColorRGBA
GXColorRGBA::operator + () const
{
    return *this;
}

inline GXColorRGBA
GXColorRGBA::operator - () const
{
    return GXColorRGBA( -r, -g, -b, -a );
}

inline GXColorRGBA
operator + ( const GXColorRGBA& x, const GXColorRGBA& y )
{
    return GXColorRGBA( x.r+y.r, x.g+y.g, x.b+y.b, x.a+y.a );
}

inline GXColorRGBA
operator - ( const GXColorRGBA& x, const GXColorRGBA& y )
{
    return GXColorRGBA( x.r-y.r, x.g-y.g, x.b-y.b, x.a-y.a );
}

inline GXColorRGBA
operator * ( const GXColorRGBA& x, const GXColorRGBA& y )
{
    return GXColorRGBA( x.r*y.r, x.g*y.g, x.b*y.b, x.a*y.a );
}

inline GXColorRGBA
operator / ( const GXColorRGBA& x, const GXColorRGBA& y )
{
    return GXColorRGBA( x.r/y.r, x.g/y.g, x.b/y.b, x.a/y.a );
}

inline GXColorRGBA
operator * ( float s, const GXColorRGBA& clr )
{
    return GXColorRGBA( clr.r*s, clr.g*s, clr.b*s, clr.a*s );
}

inline GXColorRGBA
operator * ( const GXColorRGBA& clr, float s )
{
    return s * clr;
}

inline GXColorRGBA
operator / ( const GXColorRGBA& clr, float s )
{
    float fScale = 1.f / s;
    return clr * fScale;
}

inline bool
operator == ( const GXColorRGBA& x, const GXColorRGBA& y )
{
    return x.r==y.r && x.g==y.g && x.b==y.b && x.a==y.a;
}

inline bool
operator != ( const GXColorRGBA& x, const GXColorRGBA& y )
{
    return ! (x==y);
}

inline float
GXLuminance(const GXColorRGBA &c)
{
    return (0.229f * c.r) + (0.587f * c.g) + (0.114f * c.b);
}

inline UINT 
GXMaxComponent(const GXColorRGBA &c)
{
    UINT iMax;

    // ignore alpha
    iMax = (c.g > c.r);
    if (c.b > c[iMax])
        iMax = 2;
    return iMax;
}

inline GXColorRGBA 
GXLerp(const GXColorRGBA &c1, const GXColorRGBA &c2, float s)
{
    return c1 + s * (c2 - c1);
}

inline GXColorRGBA
GXDesaturate(const GXColorRGBA &rgba)
{
    float alpha = rgba.a;
    if (alpha > 1.f)
        alpha = 1.f;
    float fMaxChan = rgba[GXMaxComponent(rgba)];
    if (fMaxChan > alpha) 
    {
        GXColorRGB rgbGray(alpha);
        float fYOld = GXLuminance(rgba);
        if (fYOld >= alpha)
            return rgbGray;
        // scale color to preserve hue
        GXColorRGB rgbNew;
        float fInvMaxChan = 1.f / fMaxChan;
        rgbNew.r = rgba.r * fInvMaxChan;
        rgbNew.g = rgba.g * fInvMaxChan;
        rgbNew.b = rgba.b * fInvMaxChan;
        float fYNew = GXLuminance(rgbNew);
        // add gray to preserve luminance
        return GXColorRGBA(GXLerp(rgbNew, rgbGray, (fYOld - fYNew) / (alpha - fYNew)), alpha);
    }
    return rgba;
}


#endif // #ifndef __GXCOLOR_H
