/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AL_BITMAP_H
#define _AL_BITMAP_H

#ifdef	_WINDOWS
namespace ATL {
	class CImage;
}
using namespace ATL;
#endif

class ADvBitmap : public AGm {
public:
	ADvBitmap();
	~ADvBitmap();

	void SetFileName(const char* file_name);
	int Load(const char* dir);
	ADvBitmap* Copy();
	int Write(AGmFile*);
	static ADvBitmap* Read(AGmFile*);
	
	AGmNode* SaveToBMP(const char*);
	AGmNode* SaveToPNG(const char*);
	AGmNode* SaveToGIF(const char*);
	AGmNode* SaveToJPEG(const char*);

	AGmString* FileName() { return _file_name; }
	long Width() { return _width; }
	long Height() { return _height; }
protected:
	AGmString *_file_name;
	long _width, _height;

#ifdef	_WINDOWS
public:
	CImage *Buffer() { return _image; }
protected:
	CImage *_image;
#endif	// _WINDOWS
#ifdef	_LINUX
public:
	GdkPixbuf *Buffer() { return _pixbuf; }
protected:
	GdkPixbuf *_pixbuf;
#endif
};

#endif	/* _AL_BITMAP_H */

