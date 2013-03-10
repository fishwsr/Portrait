///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisCollection.cpp -- image collection implementation
//
// DESCRIPTION
//  ImageCollection is used to store and create collections of images,
//  e.g., multi-layer image compositions or image sequences.
//
// SEE ALSO
//  VisCollection.h      collection class and functionality description
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  05-Jun-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
//  27-Jun-96  Richard Szeliski (szeliski) at Microsoft
//  Major revision to i/o.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"

#if defined(_DEBUG) && defined(_AFXDLL)
#include <afxwin.h>         // MFC core and standard components
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

//
//  Text strings used in reading/writing descriptions
//

static char *collection_header = "# ImageCollection, frame = %s";



//
// Collection class constructor and destructor
//

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TPixel>::CVisCollection
//  
//  DECLARATION:
//      template <class TPixel>
//      CVisCollection<TPixel>::CVisCollection();
//  
//  DESCRIPTION:
//      Default constructor.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TPixel>
CVisCollection<TPixel>::CVisCollection(EVisSequence evissequence)
  : CVisFrameSequence<TPixel>(-1, evissequence)
{
//    m_n_frames      = 0;
//    m_current_frame = 0;
//    m_frame         = 0;
    strcpy(m_descriptor, "filename_only");
    m_working_dir[0] = 0;
//    m_invisible_pixel = -1;     // default is "off"
//    m_read_if_empty = 1;        // default is to read from file
}


#ifdef TAKEOUT

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TPixel>::~CVisCollection
//  
//  DECLARATION:
//      template <class TPixel>
//      CVisCollection<TPixel>::~CVisCollection();
//  
//  DESCRIPTION:
//      Destructor.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
CVisCollection<TFrame>::~CVisCollection()
{
    DeleteAll();
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::CVisCollection
//  
//  DECLARATION:
//      template <class TFrame>
//      CVisCollection<TFrame>::CVisCollection(const CVisCollection<TFrame>& c);
//  
//  DESCRIPTION:
//      Copy constructor.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
CVisCollection<TFrame>::CVisCollection(const CVisCollection<TFrame>& c)
{
    m_n_frames      = 0;
    m_current_frame = 0;
    m_frame         = 0;
    *this = c;
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      operator=
//  
//  DECLARATION:
//      template <class TFrame>
//      CVisCollection<TFrame>& CVisCollection<TFrame>::operator=(const CImageCollecti
//                  on<TFrame>& c);
//  
//  DESCRIPTION:
//      Assignment operator.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
CVisCollection<TFrame>& CVisCollection<TFrame>::operator=(const CVisCollection<TFrame>& c)
{
    DeleteAll();
    m_n_frames      = c.m_n_frames;
    m_current_frame = c.m_current_frame;
    m_frame = new TFrame* [c.m_n_frames];
    for (int i = 0; i < m_n_frames; i++)
        m_frame[i] = new TFrame(*c.m_frame[i]);
    m_invisible_pixel = c.m_invisible_pixel;
    m_read_if_empty = c.m_read_if_empty;
    return *this;
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::CreateCollection
//  
//  DECLARATION:
//     template <class TFrame>
//     void CVisCollection<TFrame>::CreateCollection(int n_frames, 
//									               int n_rows, int n_cols)
//      
//  
//  DESCRIPTION:
//		Initializes a collection to consists of n_frames images 
//	 of the same size.
//  
//////////////////////////////////////////////////////////////////////////\/

template <class TFrame>
void CVisCollection<TFrame>::CreateCollection(int n_frames, 
									  int n_rows, int n_cols)
// Polina Golland, t-poling, 06/20/96. Create a new collection
// with all the images of the same size.
// Rick Szeliski, szeliski, 06/27/96: modified to reflect new frame structure
// Rick Szeliski, szeliski, 05/07/97: modified to call (virtual) Insert
{
	if ( m_n_frames > 0 )
		return;
    CVisShape shape(0, 0, n_cols, n_rows, 1);
    for ( int i = 0; i < n_frames; i++ ) {
        PushBack(shape);
    }
}

//
// Collection operations
//

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::Insert
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::Insert(TFrame &frame, int index, int new_geometry);
//  
//  PARAMETERS:
//      frame - 
//          CVisFrame to be inserted.
//  
//      index - 
//          Index of this frame in the ersulting collection.
//
//      new_geometry -
//          Allocate a new geometry (don't share with frame parameter):
//            1 (or positive): initialize to identity
//           -1 (or negative): copy transformation chain
//  
//  DESCRIPTION:
//      Inserts a frame into a collection.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::Insert(TFrame &frame, int index, int new_geometry)
{
    // Make a copy of the input frame
    TFrame *new_frame = new TFrame(frame);
    if (new_geometry < 0)
        new_frame->TransformChain() = frame.TransformChain();
    if (new_geometry > 0)
        new_frame->TransformChain() = CVisTransformChain();

    // Expand the list, copy old list, and insert new frame
    TFrame **new_list = new TFrame* [m_n_frames + 1];
    if (index < 0)
        index = m_n_frames;
    for (int i = 0; i < index; i++)
        new_list[i  ] = m_frame[i];
    new_list[index  ] = new_frame;
    for (i = index; i < m_n_frames; i++)
        new_list[i+1] = m_frame[i];

    // Swap lists and delete the old one
    delete m_frame;
    m_frame = new_list;
    m_n_frames += 1;

    // Special check for valid current_frame
    if (m_current_frame < 0)
        m_current_frame = 0;
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::Insert
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::Insert(const char *filename, int index);
//  
//  PARAMETERS:
//      filename - 
//          image file name
//  
//      index - 
//          image file name
//  
//  DESCRIPTION:
//      Builds a new frame and interst it into a collection.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::Insert(const char *filename, int index)
{
    TFrame frame(filename);
    Insert(frame, index);
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::Raise
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::Raise(int index, int to_top);
//  
//  PARAMETERS:
//      index - 
//          Index of a frame to be raised
//  
//      to_top - 
//			A flag that tells if the frame should be raised by 1 or all
//			the way to the top.
//          
//  
//  DESCRIPTION:
//		Raises a frame either by one or to the top.
//      
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::Raise(int index, int to_top)
{
    if (index > 0) {
        int dst_index = (to_top) ? 0 : index-1;
        TFrame *save = m_frame[index];
        for (int i = index; i > dst_index; i--)
            m_frame[i] = m_frame[i-1];
        m_frame[dst_index] = save;
    }
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::Lower
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::Lower(int index, int to_bottom);
//  
//  PARAMETERS:
//      index - 
//          Index of a frame to be lowered.
//  
//      to_bottom - 
//          A flag that tells if the frame should be lowered by 1 or all
//			the way to the bottom.
//          
//  
//  DESCRIPTION:
//		Lowers a frame either by 1 or all the way to the bottom.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::Lower(int index, int to_bottom)
{
    if (index < m_n_frames-1) {
        int dst_index = (to_bottom) ? m_n_frames-1 : index+1;
        TFrame *save = m_frame[index];
        for (int i = index; i < dst_index; i++)
            m_frame[i] = m_frame[i+1];
        m_frame[dst_index] = save;
    }
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::Delete
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::Delete(int index);
//  
//  PARAMETERS:
//      index - 
//          Index of a frame to be deleted.
//  
//  DESCRIPTION:
//      Deletes a frame from collection.
//      Updates current frame to point to valid frame,
//      unless the sequence is empty.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::Delete(int index)
{
    assert(0 <= index && index < m_n_frames);
    delete m_frame[index];
    for (int i = index; i < m_n_frames-1; i++)
        m_frame[i] = m_frame[i+1];
    m_frame[m_n_frames-1] = 0;
    m_n_frames -= 1;
    if (m_current_frame >= m_n_frames && m_current_frame > 0)
        m_current_frame  = m_n_frames-1;
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::DeleteAll
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::DeleteAll();
//  
//  DESCRIPTION:
//      Deletes all the frames.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::DeleteAll()
{
    for (int i = NFrames()-1; i >= 0; i--)
        Delete(i);
    delete [] m_frame;
    m_frame = 0;
}



//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      operator[]
//  
//  DECLARATION:
//      template <class TFrame>
//      TFrame& CVisCollection<TFrame>::operator[](int index);
//  
//  RETURN VALUE:
//      CVisFrame at the index place in collection.
//  
//  PARAMETERS:
//      index - 
//          CVisFrame index.
//  
//  DESCRIPTION:
//      Reference operator.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
TFrame& CVisCollection<TFrame>::operator[](int index)
{
    assert(0 <= index && index < m_n_frames && m_frame[index]);
    TFrame &f = *m_frame[index];
    if (f.Height() == 0 && m_read_if_empty) {
        f.SetPixelInvisible(InvisiblePixel());
        f.ReadFile();  // just in time reading
    }
    return f;
}



//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      operator[]
//  
//  DECLARATION:
//      template <class TFrame>
//      TFrame& CVisCollection<TFrame>::operator[] (int index);
//  
//  RETURN VALUE:
//      CVisFrame at the index place in collection.
//  
//  PARAMETERS:
//      index - 
//          CVisFrame index.
//  
//  DESCRIPTION:
//      Reference operator.
//
// Polina Golland, 07/22/96. A const version of the operator,
// that does not check for the file being read, but returns
// a frame as it is.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
TFrame& CVisCollection<TFrame>::operator[] (int index)const
{
    assert(0 <= index && index < m_n_frames && m_frame[index]);
    TFrame &f = *m_frame[index];
    return f;
}

#endif // TAKEOUT



//
// File I/O
//  Write the collection of images into a descriptive text file,
//  and optionally write out (back) the constituent files.
//

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TPixel>::ReadCollection
//  
//  DECLARATION:
//      template <class TPixel>
//      void CVisCollection<TPixel>::ReadCollection(const char *filename,
//                  int read_files);
//  
//  PARAMETERS:
//      filename - 
//          File name.
//  
//      read_files - 
//          A flag that tells if the read operation is to be delayed.
//  
//  DESCRIPTION:
//      Read collection from file.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TPixel>
void CVisCollection<TPixel>::ReadCollection(const char *filename, int read_files,
                                       FILE *istream)
{
    // Read the descriptive collection file
    FILE *stream = istream;
    if (istream == 0) {
    if (filename == 0 || filename[0] == 0 || strcmp(filename, "-") == 0)
        stream = stdin;
    else
        stream = fopen(filename, "r");
    if (stream == 0)
        throw CVisFileIOError(filename, "could not open file for reading",
                eviserrorOpen, "CVisCollection::ReadCollection()",
                __FILE__, __LINE__);
    }

    // Set the working directory
    m_working_dir[0] = 0;
    if (stream != stdin) {
        char *e1 = strrchr(filename, '/'), *e2 = strrchr(filename, '\\');
        int i = (e2) ? e2 - filename : (e1) ? e1 - filename : -1;
        strcpy(m_working_dir, filename);
        m_working_dir[i+1] = 0;
    }

    // Read in the file header and the frames
    fscanf(stream, collection_header, m_descriptor);
    if (m_descriptor[strlen(m_descriptor)-1] == ',')
    {
        m_descriptor[strlen(m_descriptor)-1] = 0;   // remove trailing ,
		int invisible_pixel = - 1;
        fscanf(stream, " invisible_pixel = %d\n", &invisible_pixel);
		SetInvisiblePixel(invisible_pixel);
    }
    else
    {
        fscanf(stream, "\n"); 
    }
    ReadCollection(stream);
    ReadMetaData(stream);

    // Close the file
    if (stream && stream != stdin && istream == 0)
        fclose(stream);
    // Optionally read in the constituent frames
    if (read_files)
        ReadFiles(false);	// 25-Aug-99 (szeliski/t-rdror): force reading even if allocated
}




//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TPixel>::WriteCollection
//  
//  DECLARATION:
//      template <class TPixel>
//      void CVisCollection<TPixel>::WriteCollection(const char *filename,
//                  const char *desc_type,
//                  int write_files);
//  
//  PARAMETERS:
//      filename - 
//          File name.
//  
//      desc_type - 
//          type of a frame: image only, or image plus geometry
//  
//      write_files - 
//          A flag that tells if the files should be written as well as the 
//          description.
//  
//  DESCRIPTION:
//      Writes collection (its description and maybe files) into a file.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TPixel>
void CVisCollection<TPixel>::WriteCollection(const char *filename,
                                        const char *desc_type,
                                        int write_files, FILE *ostream)
{
    // Write the descriptive collection file
    FILE *stream = ostream;
    if (ostream == 0) {
    if (filename == 0 || filename[0] == 0 || strcmp(filename, "-") == 0)
        stream = stdout;
    else
        stream = fopen(filename, "w");
    if (stream == 0)
        throw CVisFileIOError(filename, "could not open file for writing",
                eviserrorOpen, "CVisCollection::WriteCollection()",
                __FILE__, __LINE__);
    }

    // Set the working directory
    m_working_dir[0] = 0;
    if (stream != stdin) {
        char *e1 = strrchr(filename, '/'), *e2 = strrchr(filename, '\\');
        int i = (e2) ? e2 - filename : (e1) ? e1 - filename : -1;
        strcpy(m_working_dir, filename);
        m_working_dir[i+1] = 0;
    }

    // Write out the file header and the frames
    if (desc_type)
        strcpy(m_descriptor, desc_type);
    fprintf(stream, collection_header, m_descriptor);
    if (HasPixelInvisible())
        fprintf(stream, ", invisible_pixel = %d", InvisiblePixel());
    fprintf(stream, "\n");
    WriteCollection(stream);
    WriteMetaData(stream);

    // Close the file
    if (stream && stream != stdout && ostream == 0)
        fclose(stream);

    // Optionally write out the constituent frames
    if (write_files)
        WriteFiles();
}



//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TPixel>::ReadCollection
//  
//  DECLARATION:
//      template <class TPixel>
//      void CVisCollection<TPixel>::ReadCollection(FILE *stream);
//  
//  PARAMETERS:
//      stream - 
//          File
//  
//  DESCRIPTION:
//      Read collection from file.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TPixel>
void CVisCollection<TPixel>::ReadCollection(FILE *stream)
{
    while (1) {
        // Test for end of file or # separator
        int c;
        while (isspace(c = getc(stream)) && c != EOF);
        if (c == EOF || c == '#')
            break;
        else
            ungetc(c, stream);

        // Read in the frame descriptor
        PushBack();
        T_Frame &f = Back();
        f.ReadDescription(stream, m_descriptor, m_working_dir);
    }

    OpenStreams();
}


//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TPixel>::WriteCollection
//  
//  DECLARATION:
//      template <class TPixel>
//      void CVisCollection<TPixel>::WriteCollection(FILE *stream);
//  
//  PARAMETERS:
//      stream - 
//          File
//  
//  DESCRIPTION:
//      Writes collection (its description and maybe files) into a file.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TPixel>
void CVisCollection<TPixel>::WriteCollection(FILE *stream)
{
    // Placeholder for true collection description
    for (int i = 0; i < Length(); i++) {
        T_Frame &f = (*this)[i];
        f.WriteDescription(stream, m_descriptor, m_working_dir);
    }
}
//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TPixel>::ReadMetaData
//  
//  DECLARATION:
//      template <class TPixel>
//      void CVisCollection<TPixel>::ReadMetaData(FILE *stream);
//  
//  PARAMETERS:
//      stream - 
//          File
//  
//  DESCRIPTION:
//      Reads meta data (geometry etc. ) from the file.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TPixel>
void CVisCollection<TPixel>::ReadMetaData(FILE *stream)
{
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TPixel>::WriteMetaData
//  
//  DECLARATION:
//      template <class TPixel>
//      void CVisCollection<TPixel>::WriteMetaData(FILE *stream);
//  
//  PARAMETERS:
//      stream - 
//          File
//  
//  DESCRIPTION:
//      Writes meta data (geometry etc. ) to the file.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TPixel>
void CVisCollection<TPixel>::WriteMetaData(FILE *stream)
{
}



#ifdef TAKEOUT

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::ReadFiles
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::ReadFiles();
//  
//  DESCRIPTION:
//      Reads image files.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::ReadFiles()
{
    for (int i = 0; i < Length(); i++) {
        m_frame[i]->SetPixelInvisible(InvisiblePixel());
        m_frame[i]->ReadFile();
    }
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::WriteFiles
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::WriteFiles();
//  
//  DESCRIPTION:
//      Writes image files.
//  
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::WriteFiles()
{
    for (int i = 0; i < Length(); i++) {
        m_frame[i]->SetPixelInvisible(InvisiblePixel());
        m_frame[i]->WriteFile();
    }
}

//
//  Adding files by name or renaming the sequence/collection
//

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::AddFile
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::AddFile(const char *filename, int read_file);
//  
//  PARAMETERS:
//      filename - 
//          File name.
//  
//      read_file - 
//			A flag that controls delay of the reading.
//          
//  
//  DESCRIPTION:
//		Inserts a new frame with a give file name into collection
//		and (optianally) reads the file in.
//      
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::AddFile(const char *filename, int read_file)
{
    PushBack(filename);
    TFrame &f = Back();
    if (read_file) {
        f.SetPixelInvisible(InvisiblePixel());
        f.ReadFile();
    }
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      CVisCollection<TFrame>::ReNameSequence
//  
//  DECLARATION:
//      template <class TFrame>
//      void CVisCollection<TFrame>::ReNameSequence(const char *stem,
//          const char *extension, int sequence_digits = 4);
//  
//  PARAMETERS:
//      stem - 
//          Filename prefix
//
//      extension
//          Filename suffix
//
//      sequence_digits -
//          number of digits in sequence number
//          
//  
//  DESCRIPTION:
//		Name all of the frames in the sequence to stem<nnnn>extension.
//      
//////////////////////////////////////////////////////////////////////////\/
template <class TFrame>
void CVisCollection<TFrame>::ReNameSequence(const char *stem,
                                       const char *extension,
                                       int sequence_digits)
{
    char format[128], filename[1024];
    sprintf(format, "%%s%%0%dd%%s", sequence_digits);
    for (int i = 0; i < m_n_frames; i++) {
        TFrame &f = *m_frame[i];
        sprintf(filename, format, stem, i, extension);
        f.SetName(filename);
    }
}

#endif // TAKEOUT

#ifdef TAKEOUT  // In the VisMatrix project.
//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      VisMakePixelsInvisible
//  
//  DECLARATION:
//      static void VisMakePixelsInvisible(CVisRGBAByteImage &img, int v);
//  
//  PARAMETERS:
//      img - 
//          Image
//  
//      v - 
//			A value of the pixel to be set invisible.
//          
//  
//  DESCRIPTION:
//      
//		Sets all the pixels equal to (v,v,v,255) to be invisible (0,0,0,0).
//			
//  
//////////////////////////////////////////////////////////////////////////\/
void VisMakePixelsInvisible(CVisRGBAByteImage &img, int v)
{
    int rows = img.Height(), cols = img.Width()*img.NBands();
    for (int r = 0; r < rows; r++) {
        CVisRGBABytePixel input(v, v, v, 255);
        int iv = *(int *) &input;
        int *p = (int *) img.StartOfRow(r + img.SRow());
        for (int c = 0; c < cols; c++)
            if (p[c] == iv)
                p[c] = 0;
    }
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      VisBackInvisiblePixels
//  
//  DECLARATION:
//      static void VisBackInvisiblePixels(CVisRGBAByteImage &img, int v);
//  
//  PARAMETERS:
//      img - 
//          Image to which backing color should be added
//  
//      v - 
//			Gray-level value of backing pixel.
//          
//  
//  DESCRIPTION:
//      
//		Sets out to in OVER (v,v,v,255).
//      Also, sets any (v,v,v,255) value to (v+e,v+e,v+e,255), e = +/- 1
//			
//  
//////////////////////////////////////////////////////////////////////////\/
void VisBackInvisiblePixels(CVisRGBAByteImage &img, int v)
{
    int rows = img.Height(), cols = img.Width()*img.NBands();
    int ic, e = (v < 128) ? 1 : -1;
    *(CVisRGBABytePixel *) &ic = CVisRGBABytePixel(v, v, v, 255);
    int i0 = ic;
    *(CVisRGBABytePixel *) &ic = CVisRGBABytePixel(v+e, v+e, v+e, 255);
    int i1 = ic;
    CVisRGBABytePixel a_mask;
    a_mask.r = a_mask.g = a_mask.b = 0, a_mask.a = 255;
    int ia_mask = *(int *) &a_mask;
    for (int r = 0; r < rows; r++) {
        CVisRGBABytePixel *p = img.StartOfRow(r + img.SRow());
        int *ip = (int *) p;
        for (int c = 0; c < cols; c++) {
#ifdef FASTER
            // Test 8 pixels at a time (speedup)
            if ((c & 7) == 0 && (c+7) < cols) {
                int *f = &ip[c];
                int f255 = f[0] & f[1] & f[2] & f[3] & 
                           f[4] & f[5] & f[6] & f[7];
                if ((f255 & ia_mask) == ia_mask) {
                    c += 7;
                    continue;   // all 8 pixels are opaque
                }
                int f000 = f[0] | f[1] | f[2] | f[3] | 
                           f[4] | f[5] | f[6] | f[7];
                if ((f000  & ia_mask) == 0) {
                    // all 8 pixels are transparent
                    f[0] = f[1] = f[2] = f[3] =
                    f[4] = f[5] = f[6] = f[7] = i0;
                    c += 7;
                    continue;
                }
            }
#endif
            if (ip[c] == 0)
                ip[c] = i0;
            else if (ip[c] == i0)
                ip[c] = i1;
            else if (p[c].A() != 255) {
                int w = ((255 - p[c].A())*v + 127)/255;
                p[c].R() += w, p[c].G() += w, p[c].B() += w, p[c].A() = 255;
            }
        }
    }
}
#endif // TAKEOUT


//
// Virtual CVisSequence methods
// 

template <class TPixel>
const type_info& CVisCollection<TPixel>::ImageTypeInfo(void) const
{
	return typeid(T_Frame);
}

template <class TPixel>
const type_info& CVisCollection<TPixel>::ObjectTypeInfo(int iLevel) const
{
	if ((iLevel < 0) || (iLevel >= eilevelCollection))
		return typeid(CVisCollection<TPixel>);

	return CVisSequence<T_Pixel>::ObjectTypeInfo(iLevel);
}


template <class TPixel>
CVisSequenceBase *CVisCollection<TPixel>::Clone(bool fCopyThis) const
{
	if (fCopyThis)
		return new CVisCollection<TPixel>(*this);

	return new CVisCollection<TPixel>;
}


template <class TPixel>
void CVisCollection<TPixel>::SDReadWriteProperties(CVisSDStream& s,
		bool fAddComma)
{
	// UNDONE
	CVisSequence<T_Pixel>::SDReadWriteProperties(s, fAddComma);
}



// Specialized PropList helper functions for common types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCollection<BYTE> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<int> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCollection<int> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<float> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCollection<float> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<double> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCollection<double> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCollection<CVisRGBA<BYTE> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCollection<CVisRGBA<int> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCollection<CVisRGBA<float> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCollection<CVisRGBA<double> > >(0, fAlwaysTypedef); }


//
// Explicit template instantiation
//

#pragma warning(disable : 4660)  // template class already instantiated

//template CVisFrame<CVisRGBAByteImage>;
//template CVisFrame<CVisByteImage>;

template CVisCollection<CVisBytePixel>;
template CVisCollection<CVisRGBABytePixel>;
template CVisCollection<CVisFloatPixel>;
template CVisCollection<CVisDoublePixel>;
template CVisCollection<CVisIntPixel>;
template CVisCollection<CVisRGBAIntPixel>;
template CVisCollection<CVisRGBAFloatPixel>;
template CVisCollection<CVisRGBADoublePixel>;

#pragma warning(default : 4660)
