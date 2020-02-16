/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#include "stdafx.h"

#include <gdm/gdm.h>
#include <gdm/string.h>
#include <gdm/file.h>
#include <gdm/error.h>
#include <gdm/pict.h>
#include <dev/advwnd.h>
#include <dev/bitmap.h>
#include <altair/strres.h>

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifdef _WINDOWS
#undef T
#include <atlimage.h>
#endif

ADvBitmap::ADvBitmap() {
	_file_name = NULL;
#ifdef _WINDOWS
	_image = NULL;
#endif
#ifdef _LINUX
	_pixbuf = NULL;
#endif
}
ADvBitmap::~ADvBitmap() {
	if (_file_name) delete _file_name;
#ifdef _WINDOWS
	if (_image) _image->Destroy();
#endif
#ifdef _LINUX
	if (_pixbuf) g_object_unref(_pixbuf);
#endif
}

void ADvBitmap::SetFileName(const char *file_name) {
	if (_file_name) delete _file_name;
	_file_name = NULL;
	if (file_name) _file_name = new AGmString(file_name);
}

#ifdef	_WINDOWS
int ADvBitmap::Load(const char *dir) {
	if (dir == NULL || _file_name == NULL) {
		return 1;
	}
	if (_image) _image->Destroy();
	_image = NULL;
	AGmString *path = new AGmString(dir);
	path->Append("/");
	path->Append(_file_name);
	_image = new CImage();
	HRESULT hr = _image->Load(path->Str());
	delete path;
	if (hr) return 1;
	_width = _image->GetWidth();
	_height = _image->GetHeight();
	return 0;
}
ADvBitmap *ADvBitmap::Copy() {
	AGmString *path = AGmFile::GetTempFile(AGmFile::GetTempDir());
	AGmFile *f = new AGmFile();
	f->Create(path, AGM_FILE_WRITE);
	int err = Write(f);
	delete f;
	if (err) { AGmFile::Remove(path->Str()); delete path; return NULL; }
	f = new AGmFile();
	f->Create(path, AGM_FILE_READ);
	ADvBitmap *bitmap = Read(f);
	delete f; AGmFile::Remove(path->Str()); delete path;
	return bitmap;
}
int ADvBitmap::Write(AGmFile *f) {
	if (_file_name == NULL || _image == NULL) {
		f->WriteQString("$empty"); f->WriteString("\n");
		return 0;
	}
	f->WriteQString(_file_name); f->WriteString(" ");

	AGmString *path = AGmFile::GetTempFile(AGmFile::GetTempDir());
	HRESULT hr = _image->Save(path->Str(), Gdiplus::ImageFormatBMP);
	if (hr) { AGmFile::Remove(path->Str()); delete path; return 1; }
	struct stat buf;
	int err = AGmFile::GetStat(path->Str(), &buf);
	if (err) { AGmFile::Remove(path->Str()); delete path; return 1; }
	long size = buf.st_size;
	f->WriteInteger(size); f->WriteString("\n");

	FILE *in = art_fopen(path->Str(), "rb");
	if (in == NULL) { AGmFile::Remove(path->Str()); delete path; return 1; }
	unsigned char *buff = new unsigned char[size];
	int left, cc;
	for (left = size; left > 0; left -= cc) {
		cc = art_read(art_fileno(in), buff, left);
		if (cc <= 0) break;
	}
	fclose(in); AGmFile::Remove(path->Str()); delete path;
	if (left > 0) return 1;

	long count = 0;
	for (long i = 0; i < size; i++) {
		if (count++ < 50) {
			f->WriteString(" ");
		} else {
			count = 0;
			f->WriteString("\n");
		}
		f->WriteInteger(buff[i]);
	}
	delete [] buff;

	return 0;
}
ADvBitmap *ADvBitmap::Read(AGmFile *f) {
	AGmString *s;
	if (f->ReadQString(s)) return NULL;
	ADvBitmap *bitmap = new ADvBitmap;
	if (s->Compare("$empty") == 0) {
		delete s; return bitmap;
	}
	bitmap->_file_name = s;

#if	_WINDOWS && _64BIT
	long long size;
#else
	long size;
#endif
	if (f->ReadInteger(size)) { delete bitmap; return NULL; }
	unsigned char *buff = new unsigned char[size];

	for (long i = 0; i < size; i++) {
#if	_WINDOWS && _64BIT
		long long d;
#else
		long d;
#endif
		if (f->ReadInteger(d)) { delete [] buff; delete bitmap; }
		buff[i] = (unsigned char)d;
	}

	AGmString *path = AGmFile::GetTempFile(AGmFile::GetTempDir());
	FILE *out = art_fopen(path->Str(), "wb");
	if (out == NULL) { delete [] buff; delete path; return bitmap; }
	int left, cc;
	for (left = (int)size; left > 0; left -= cc) {
		cc = art_write(art_fileno(out), buff, left);
		if (cc <= 0) break;
	}
	fclose(out); delete [] buff;
	if (left > 0) { AGmFile::Remove(path->Str()); delete path; return bitmap; }

	bitmap->_image = new CImage();
	HRESULT hr = bitmap->_image->Load(path->Str());
	AGmFile::Remove(path->Str()); delete path;
	if (hr) { bitmap->_image->Destroy(); bitmap->_image = NULL; return bitmap; }

	bitmap->_width = bitmap->_image->GetWidth();
	bitmap->_height = bitmap->_image->GetHeight();

	return bitmap;
}
AGmNode* ADvBitmap::SaveToBMP(const char* path) {
	if (_image == NULL) return String("image is empty.");
	HRESULT hr = _image->Save(path, Gdiplus::ImageFormatBMP);
	if (hr) {
		char msg[256];
		art_sprintf_1(msg, 256, "fail to save BMP: error code = %d.", hr);
		return String(msg);
	}
	return NULL;
}
AGmNode* ADvBitmap::SaveToPNG(const char* path) {
	if (_image == NULL) return String("image is empty.");
	HRESULT hr = _image->Save(path, Gdiplus::ImageFormatPNG);
	if (hr) {
		char msg[256];
		art_sprintf_1(msg, 256, "fail to save PNG: error code = %d.", hr);
		return String(msg);
	}
	return 0;
}
AGmNode* ADvBitmap::SaveToGIF(const char* path) {
	if (_image == NULL) return String("image is empty.");
	HRESULT hr = _image->Save(path, Gdiplus::ImageFormatGIF);
	if (hr) {
		char msg[256];
		art_sprintf_1(msg, 256, "fail to save GIF: error code = %d.", hr);
		return String(msg);
	}
	return 0;
}
AGmNode* ADvBitmap::SaveToJPEG(const char* path) {
	if (_image == NULL) return String("image is empty.");
	HRESULT hr = _image->Save(path, Gdiplus::ImageFormatJPEG);
	if (hr) {
		char msg[256];
		art_sprintf_1(msg, 256, "fail to save JPEG: error code = %d.", hr);
		return String(msg);
	}
	return 0;
}
#endif	// _WINDOWS

#ifdef	_LINUX
int ADvBitmap::Load(const char *dir) {
	if (dir == NULL || _file_name == NULL) {
		return 1;
	}
	if (_pixbuf) g_object_unref(_pixbuf);
	_pixbuf = NULL;
	AGmString *path = new AGmString(dir);
	path->Append("/");
	path->Append(_file_name);
	GError *err = NULL;
	_pixbuf = gdk_pixbuf_new_from_file(path->Str(), &err);
	delete path;
	if (err) {
		g_error_free(err); _pixbuf = NULL; return 1;
	}
	_width = gdk_pixbuf_get_width(_pixbuf);
	_height = gdk_pixbuf_get_height(_pixbuf);
	return 0;
}
ADvBitmap *ADvBitmap::Copy() {
	ADvBitmap *bitmap = new ADvBitmap();
	if (_file_name) bitmap->_file_name = new AGmString(_file_name);
	if (_pixbuf == NULL) return bitmap;
	bitmap->_pixbuf = gdk_pixbuf_copy(_pixbuf);
	bitmap->_width = _width;
	bitmap->_height = _height;
	return bitmap;
}
int ADvBitmap::Write(AGmFile *f) {
	if (_file_name == NULL || _pixbuf == NULL) {
		f->WriteQString("$empty"); f->WriteString("\n");
		return 0;
	}
	f->WriteQString(_file_name);	f->WriteString("\n");

	long rowstride, n_channels, bits_per_sample, colorspace, has_alpha;
	rowstride = gdk_pixbuf_get_rowstride(_pixbuf);
	n_channels = gdk_pixbuf_get_n_channels(_pixbuf);
	bits_per_sample = gdk_pixbuf_get_bits_per_sample(_pixbuf);
	colorspace = gdk_pixbuf_get_colorspace(_pixbuf);
	has_alpha = gdk_pixbuf_get_has_alpha(_pixbuf);

	int size = ((_height - 1) * rowstride + _width * ((n_channels * bits_per_sample + 7) / 8));

	f->WriteInteger(size + 7);	f->WriteString("\n");

	f->WriteInteger(_width);			f->WriteString(" ");
	f->WriteInteger(_height);			f->WriteString(" ");
	f->WriteInteger(rowstride); 		f->WriteString(" ");
	f->WriteInteger(n_channels);		f->WriteString(" ");
	f->WriteInteger(bits_per_sample);	f->WriteString(" ");
	f->WriteInteger(colorspace);		f->WriteString(" ");
	f->WriteInteger(has_alpha); 		f->WriteString("\n");

	guchar* buf = gdk_pixbuf_get_pixels(_pixbuf);

	long count = 0;
	for (long i = 0; i < size; i++) {
		if (count++ < 50) {
			f->WriteString(" ");
		} else {
			count = 0;
			f->WriteString("\n");
		}
		f->WriteInteger(buf[i]);
	}

	return 0;
}
ADvBitmap *ADvBitmap::Read(AGmFile *f) {
	AGmString *s;
	if (f->ReadQString(s)) return NULL;
	ADvBitmap *bitmap = new ADvBitmap;
	if (s->Compare("$empty") == 0) {
		delete s; return bitmap;
	}
	bitmap->_file_name = s;

	long size, width, height, rowstride, n_channels, bits_per_sample, colorspace, has_alpha;
	if (f->ReadInteger(size) || f->ReadInteger(width) || f->ReadInteger(height)
			|| f->ReadInteger(rowstride) || f->ReadInteger(n_channels) || f->ReadInteger(bits_per_sample)
			|| f->ReadInteger(colorspace) || f->ReadInteger(has_alpha)) {
		return bitmap;
	}
	bitmap->_width = width; bitmap->_height = height;

	size -= 7;
	guchar* buf = (guchar*)g_malloc(sizeof(guchar) * size);

	for (long i = 0; i < size; i++) {
		long d;
		if (f->ReadInteger(d)) { g_free(buf); return bitmap; }
		buf[i] = (guchar)d;
	}

	bitmap->_pixbuf = gdk_pixbuf_new_from_data(buf, (GdkColorspace)colorspace,
		has_alpha, bits_per_sample, width, height, rowstride, NULL, NULL);

	return bitmap;
}
AGmNode* ADvBitmap::SaveToBMP(const char* path) {
	if (_pixbuf == NULL) return String("image is empty.");
	GError *err = NULL;
	if (gdk_pixbuf_save(_pixbuf, path, "bmp", &err, NULL) == FALSE) {
		gchar *c = g_locale_from_utf8(err->message, -1, NULL, NULL, NULL);
		char msg[256];
		art_sprintf_1(msg, 256, "fail to save BMP: message = %s.", c);
		g_free(c); g_error_free(err);
		return String(msg);
	}
	return NULL;
}
AGmNode* ADvBitmap::SaveToPNG(const char* path) {
	if (_pixbuf == NULL) return String("image is empty.");
	GError *err = NULL;
	if (gdk_pixbuf_save(_pixbuf, path, "png", &err, NULL) == FALSE) {
		gchar *c = g_locale_from_utf8(err->message, -1, NULL, NULL, NULL);
		char msg[256];
		art_sprintf_1(msg, 256, "fail to save PNG: message = %s.", c);
		g_free(c); g_error_free(err);
		return String(msg);
	}
	return NULL;
}
AGmNode* ADvBitmap::SaveToGIF(const char* path) {
	if (_pixbuf == NULL) return String("image is empty.");
	GError *err = NULL;
	if (gdk_pixbuf_save(_pixbuf, path, "gif", &err, NULL) == FALSE) {
		gchar *c = g_locale_from_utf8(err->message, -1, NULL, NULL, NULL);
		char msg[256];
		art_sprintf_1(msg, 256, "fail to save GIF: message = %s.", c);
		g_free(c); g_error_free(err);
		return String(msg);
	}
	return NULL;
}
AGmNode* ADvBitmap::SaveToJPEG(const char* path) {
	if (_pixbuf == NULL) return String("image is empty.");
	GError *err = NULL;
	if (gdk_pixbuf_save(_pixbuf, path, "jpeg", &err, NULL) == FALSE) {
		gchar *c = g_locale_from_utf8(err->message, -1, NULL, NULL, NULL);
		char msg[256];
		art_sprintf_1(msg, 256, "fail to save JPEG: message = %s.", c);
		g_free(c); g_error_free(err);
		return String(msg);
	}
	return NULL;
}
#endif
