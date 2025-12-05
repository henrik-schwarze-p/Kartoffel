//
//  IC.c written without any love
//
//  Created by Enrico on 03.04.20.
//  Copyright © 2020 Enrico. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/**************************************************************

	QDBMP - Quick n' Dirty BMP

	v1.0.0 - 2007-04-07
	http://qdbmp.sourceforge.net


	The library supports the following BMP variants:
	1. Uncompressed 32 BPP (alpha values are ignored)
	2. Uncompressed 24 BPP
	3. Uncompressed 8 BPP (indexed color)

	QDBMP is free and open source software, distributed
	under the MIT licence.

	Copyright (c) 2007 Chai Braudo (braudo@users.sourceforge.net)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

**************************************************************/

/* Type definitions */
#ifndef UINT
#define UINT unsigned long int
#endif

#ifndef USHORT
#define USHORT unsigned short
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

/* Version */
#define QDBMP_VERSION_MAJOR 1
#define QDBMP_VERSION_MINOR 0
#define QDBMP_VERSION_PATCH 1

/* Error codes */
typedef enum
{
	BMP_OK = 0,				/* No error */
	BMP_ERROR,				/* General error */
	BMP_OUT_OF_MEMORY,		/* Could not allocate enough memory to complete the operation */
	BMP_IO_ERROR,			/* General input/output error */
	BMP_FILE_NOT_FOUND,		/* File not found */
	BMP_FILE_NOT_SUPPORTED, /* File is not a supported BMP variant */
	BMP_FILE_INVALID,		/* File is not a BMP image or is an invalid BMP */
	BMP_INVALID_ARGUMENT,	/* An argument is invalid or out of range */
	BMP_TYPE_MISMATCH,		/* The requested action is not compatible with the BMP's type */
	BMP_ERROR_NUM
} BMP_STATUS;

/* Bitmap image */
typedef struct _BMP BMP;

/*********************************** Public methods **********************************/

/* Construction/destruction */
BMP *BMP_Create(UINT width, UINT height, USHORT depth);
void BMP_Free(BMP *bmp);

/* I/O */
BMP *BMP_ReadFile(const char *filename);
void BMP_WriteFile(BMP *bmp, const char *filename);

/* Meta info */
UINT BMP_GetWidth(BMP *bmp);
UINT BMP_GetHeight(BMP *bmp);
USHORT BMP_GetDepth(BMP *bmp);

/* Pixel access */
void BMP_GetPixelRGB(BMP *bmp, UINT x, UINT y, UCHAR *r, UCHAR *g, UCHAR *b);
void BMP_SetPixelRGB(BMP *bmp, UINT x, UINT y, UCHAR r, UCHAR g, UCHAR b);
void BMP_GetPixelIndex(BMP *bmp, UINT x, UINT y, UCHAR *val);
void BMP_SetPixelIndex(BMP *bmp, UINT x, UINT y, UCHAR val);

/* Palette handling */
void BMP_GetPaletteColor(BMP *bmp, UCHAR index, UCHAR *r, UCHAR *g, UCHAR *b);
void BMP_SetPaletteColor(BMP *bmp, UCHAR index, UCHAR r, UCHAR g, UCHAR b);

/* Error handling */
BMP_STATUS BMP_GetError();
const char *BMP_GetErrorDescription();

/* Useful macro that may be used after each BMP operation to check for an error */
#define BMP_CHECK_ERROR(output_file, return_value)                            \
	if (BMP_GetError() != BMP_OK)                                             \
	{                                                                         \
		fprintf((output_file), "BMP error: %s\n", BMP_GetErrorDescription()); \
		return (return_value);                                                \
	}

/* Bitmap header */
typedef struct _BMP_Header
{
	USHORT Magic;		  /* Magic identifier: "BM" */
	UINT FileSize;		  /* Size of the BMP file in bytes */
	USHORT Reserved1;	  /* Reserved */
	USHORT Reserved2;	  /* Reserved */
	UINT DataOffset;	  /* Offset of image data relative to the file's start */
	UINT HeaderSize;	  /* Size of the header in bytes */
	UINT Width;			  /* Bitmap's width */
	UINT Height;		  /* Bitmap's height */
	USHORT Planes;		  /* Number of color planes in the bitmap */
	USHORT BitsPerPixel;  /* Number of bits per pixel */
	UINT CompressionType; /* Compression type */
	UINT ImageDataSize;	  /* Size of uncompressed image's data */
	UINT HPixelsPerMeter; /* Horizontal resolution (pixels per meter) */
	UINT VPixelsPerMeter; /* Vertical resolution (pixels per meter) */
	UINT ColorsUsed;	  /* Number of color indexes in the color table that are actually used by the bitmap */
	UINT ColorsRequired;  /* Number of color indexes that are required for displaying the bitmap */
} BMP_Header;

/* Private data structure */
struct _BMP
{
	BMP_Header Header;
	UCHAR *Palette;
	UCHAR *Data;
};

/* Holds the last error code */
static BMP_STATUS BMP_LAST_ERROR_CODE = 0;

/* Error description strings */
static const char *BMP_ERROR_STRING[] =
	{
		"",
		"General error",
		"Could not allocate enough memory to complete the operation",
		"File input/output error",
		"File not found",
		"File is not a supported BMP variant (must be uncompressed 8, 24 or 32 BPP)",
		"File is not a valid BMP image",
		"An argument is invalid or out of range",
		"The requested action is not compatible with the BMP's type"};

/* Size of the palette data for 8 BPP bitmaps */
#define BMP_PALETTE_SIZE (256 * 4)

/*********************************** Forward declarations **********************************/
int ReadHeader(BMP *bmp, FILE *f);
int WriteHeader(BMP *bmp, FILE *f);

int ReadUINT(UINT *x, FILE *f);
int ReadUSHORT(USHORT *x, FILE *f);

int WriteUINT(UINT x, FILE *f);
int WriteUSHORT(USHORT x, FILE *f);

/*********************************** Public methods **********************************/

/**************************************************************
	Creates a blank BMP image with the specified dimensions
	and bit depth.
**************************************************************/
BMP *BMP_Create(UINT width, UINT height, USHORT depth)
{
	BMP *bmp;
	int bytes_per_pixel = depth >> 3;
	UINT bytes_per_row;

	if (height <= 0 || width <= 0)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return NULL;
	}

	if (depth != 8 && depth != 24 && depth != 32)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_SUPPORTED;
		return NULL;
	}

	/* Allocate the bitmap data structure */
	bmp = calloc(1, sizeof(BMP));
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		return NULL;
	}

	/* Set header' default values */
	bmp->Header.Magic = 0x4D42;
	bmp->Header.Reserved1 = 0;
	bmp->Header.Reserved2 = 0;
	bmp->Header.HeaderSize = 40;
	bmp->Header.Planes = 1;
	bmp->Header.CompressionType = 0;
	bmp->Header.HPixelsPerMeter = 0;
	bmp->Header.VPixelsPerMeter = 0;
	bmp->Header.ColorsUsed = 0;
	bmp->Header.ColorsRequired = 0;

	/* Calculate the number of bytes used to store a single image row. This is always
	rounded up to the next multiple of 4. */
	bytes_per_row = width * bytes_per_pixel;
	bytes_per_row += (bytes_per_row % 4 ? 4 - bytes_per_row % 4 : 0);

	/* Set header's image specific values */
	bmp->Header.Width = width;
	bmp->Header.Height = height;
	bmp->Header.BitsPerPixel = depth;
	bmp->Header.ImageDataSize = bytes_per_row * height;
	bmp->Header.FileSize = bmp->Header.ImageDataSize + 54 + (depth == 8 ? BMP_PALETTE_SIZE : 0);
	bmp->Header.DataOffset = 54 + (depth == 8 ? BMP_PALETTE_SIZE : 0);

	/* Allocate palette */
	if (bmp->Header.BitsPerPixel == 8)
	{
		bmp->Palette = (UCHAR *)calloc(BMP_PALETTE_SIZE, sizeof(UCHAR));
		if (bmp->Palette == NULL)
		{
			BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
			free(bmp);
			return NULL;
		}
	}
	else
	{
		bmp->Palette = NULL;
	}

	/* Allocate pixels */
	bmp->Data = (UCHAR *)calloc(bmp->Header.ImageDataSize, sizeof(UCHAR));
	if (bmp->Data == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		free(bmp->Palette);
		free(bmp);
		return NULL;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return bmp;
}

/**************************************************************
	Frees all the memory used by the specified BMP image.
**************************************************************/
void BMP_Free(BMP *bmp)
{
	if (bmp == NULL)
	{
		return;
	}

	if (bmp->Palette != NULL)
	{
		free(bmp->Palette);
	}

	if (bmp->Data != NULL)
	{
		free(bmp->Data);
	}

	free(bmp);

	BMP_LAST_ERROR_CODE = BMP_OK;
}

/**************************************************************
	Reads the specified BMP image file.
**************************************************************/
BMP *BMP_ReadFile(const char *filename)
{
	BMP *bmp;
	FILE *f;

	if (filename == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return NULL;
	}

	/* Allocate */
	bmp = calloc(1, sizeof(BMP));
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		return NULL;
	}

	/* Open file */
	f = fopen(filename, "rb");
	if (f == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_FOUND;
		free(bmp);
		return NULL;
	}

	/* Read header */
	if (ReadHeader(bmp, f) != BMP_OK || bmp->Header.Magic != 0x4D42)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_INVALID;
		fclose(f);
		free(bmp);
		return NULL;
	}

	/* Verify that the bitmap variant is supported */
	if ((bmp->Header.BitsPerPixel != 32 && bmp->Header.BitsPerPixel != 24 && bmp->Header.BitsPerPixel != 8) || bmp->Header.CompressionType != 0 || bmp->Header.HeaderSize != 40)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_SUPPORTED;
		fclose(f);
		free(bmp);
		return NULL;
	}

	/* Allocate and read palette */
	if (bmp->Header.BitsPerPixel == 8)
	{
		bmp->Palette = (UCHAR *)malloc(BMP_PALETTE_SIZE * sizeof(UCHAR));
		if (bmp->Palette == NULL)
		{
			BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
			fclose(f);
			free(bmp);
			return NULL;
		}

		if (fread(bmp->Palette, sizeof(UCHAR), BMP_PALETTE_SIZE, f) != BMP_PALETTE_SIZE)
		{
			BMP_LAST_ERROR_CODE = BMP_FILE_INVALID;
			fclose(f);
			free(bmp->Palette);
			free(bmp);
			return NULL;
		}
	}
	else /* Not an indexed image */
	{
		bmp->Palette = NULL;
	}

	/* Allocate memory for image data */
	bmp->Data = (UCHAR *)malloc(bmp->Header.ImageDataSize);
	if (bmp->Data == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		fclose(f);
		free(bmp->Palette);
		free(bmp);
		return NULL;
	}

	/* Read image data */
	if (fread(bmp->Data, sizeof(UCHAR), bmp->Header.ImageDataSize, f) != bmp->Header.ImageDataSize)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_INVALID;
		fclose(f);
		free(bmp->Data);
		free(bmp->Palette);
		free(bmp);
		return NULL;
	}

	fclose(f);

	BMP_LAST_ERROR_CODE = BMP_OK;

	return bmp;
}

/**************************************************************
	Writes the BMP image to the specified file.
**************************************************************/
void BMP_WriteFile(BMP *bmp, const char *filename)
{
	FILE *f;

	if (filename == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return;
	}

	/* Open file */
	f = fopen(filename, "wb");
	if (f == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_FOUND;
		return;
	}

	/* Write header */
	if (WriteHeader(bmp, f) != BMP_OK)
	{
		BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
		fclose(f);
		return;
	}

	/* Write palette */
	if (bmp->Palette)
	{
		if (fwrite(bmp->Palette, sizeof(UCHAR), BMP_PALETTE_SIZE, f) != BMP_PALETTE_SIZE)
		{
			BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
			fclose(f);
			return;
		}
	}

	/* Write data */
	if (fwrite(bmp->Data, sizeof(UCHAR), bmp->Header.ImageDataSize, f) != bmp->Header.ImageDataSize)
	{
		BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
		fclose(f);
		return;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;
	fclose(f);
}

/**************************************************************
	Returns the image's width.
**************************************************************/
UINT BMP_GetWidth(BMP *bmp)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return -1;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return (bmp->Header.Width);
}

/**************************************************************
	Returns the image's height.
**************************************************************/
UINT BMP_GetHeight(BMP *bmp)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return -1;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return (bmp->Header.Height);
}

/**************************************************************
	Returns the image's color depth (bits per pixel).
**************************************************************/
USHORT BMP_GetDepth(BMP *bmp)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return -1;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return (bmp->Header.BitsPerPixel);
}

/**************************************************************
	Populates the arguments with the specified pixel's RGB
	values.
**************************************************************/
void BMP_GetPixelRGB(BMP *bmp, UINT x, UINT y, UCHAR *r, UCHAR *g, UCHAR *b)
{
	UCHAR *pixel;
	UINT bytes_per_row;
	UCHAR bytes_per_pixel;

	if (bmp == NULL || x < 0 || x >= bmp->Header.Width || y < 0 || y >= bmp->Header.Height)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}
	else
	{
		BMP_LAST_ERROR_CODE = BMP_OK;

		bytes_per_pixel = bmp->Header.BitsPerPixel >> 3;

		/* Row's size is rounded up to the next multiple of 4 bytes */
		bytes_per_row = bmp->Header.ImageDataSize / bmp->Header.Height;

		/* Calculate the location of the relevant pixel (rows are flipped) */
		pixel = bmp->Data + ((bmp->Header.Height - y - 1) * bytes_per_row + x * bytes_per_pixel);

		/* In indexed color mode the pixel's value is an index within the palette */
		if (bmp->Header.BitsPerPixel == 8)
		{
			pixel = bmp->Palette + *pixel * 4;
		}

		/* Note: colors are stored in BGR order */
		if (r)
			*r = *(pixel + 2);
		if (g)
			*g = *(pixel + 1);
		if (b)
			*b = *(pixel + 0);
	}
}

/**************************************************************
	Sets the specified pixel's RGB values.
**************************************************************/
void BMP_SetPixelRGB(BMP *bmp, UINT x, UINT y, UCHAR r, UCHAR g, UCHAR b)
{
	UCHAR *pixel;
	UINT bytes_per_row;
	UCHAR bytes_per_pixel;

	if (bmp == NULL || x < 0 || x >= bmp->Header.Width || y < 0 || y >= bmp->Header.Height)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 24 && bmp->Header.BitsPerPixel != 32)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		BMP_LAST_ERROR_CODE = BMP_OK;

		bytes_per_pixel = bmp->Header.BitsPerPixel >> 3;

		/* Row's size is rounded up to the next multiple of 4 bytes */
		bytes_per_row = bmp->Header.ImageDataSize / bmp->Header.Height;

		/* Calculate the location of the relevant pixel (rows are flipped) */
		pixel = bmp->Data + ((bmp->Header.Height - y - 1) * bytes_per_row + x * bytes_per_pixel);

		/* Note: colors are stored in BGR order */
		*(pixel + 2) = r;
		*(pixel + 1) = g;
		*(pixel + 0) = b;
	}
}

/**************************************************************
	Gets the specified pixel's color index.
**************************************************************/
void BMP_GetPixelIndex(BMP *bmp, UINT x, UINT y, UCHAR *val)
{
	UCHAR *pixel;
	UINT bytes_per_row;

	if (bmp == NULL || x < 0 || x >= bmp->Header.Width || y < 0 || y >= bmp->Header.Height)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 8)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		BMP_LAST_ERROR_CODE = BMP_OK;

		/* Row's size is rounded up to the next multiple of 4 bytes */
		bytes_per_row = bmp->Header.ImageDataSize / bmp->Header.Height;

		/* Calculate the location of the relevant pixel */
		pixel = bmp->Data + ((bmp->Header.Height - y - 1) * bytes_per_row + x);

		if (val)
			*val = *pixel;
	}
}

/**************************************************************
	Sets the specified pixel's color index.
**************************************************************/
void BMP_SetPixelIndex(BMP *bmp, UINT x, UINT y, UCHAR val)
{
	UCHAR *pixel;
	UINT bytes_per_row;

	if (bmp == NULL || x < 0 || x >= bmp->Header.Width || y < 0 || y >= bmp->Header.Height)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 8)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		BMP_LAST_ERROR_CODE = BMP_OK;

		/* Row's size is rounded up to the next multiple of 4 bytes */
		bytes_per_row = bmp->Header.ImageDataSize / bmp->Header.Height;

		/* Calculate the location of the relevant pixel */
		pixel = bmp->Data + ((bmp->Header.Height - y - 1) * bytes_per_row + x);

		*pixel = val;
	}
}

/**************************************************************
	Gets the color value for the specified palette index.
**************************************************************/
void BMP_GetPaletteColor(BMP *bmp, UCHAR index, UCHAR *r, UCHAR *g, UCHAR *b)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 8)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		if (r)
			*r = *(bmp->Palette + index * 4 + 2);
		if (g)
			*g = *(bmp->Palette + index * 4 + 1);
		if (b)
			*b = *(bmp->Palette + index * 4 + 0);

		BMP_LAST_ERROR_CODE = BMP_OK;
	}
}

/**************************************************************
	Sets the color value for the specified palette index.
**************************************************************/
void BMP_SetPaletteColor(BMP *bmp, UCHAR index, UCHAR r, UCHAR g, UCHAR b)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 8)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		*(bmp->Palette + index * 4 + 2) = r;
		*(bmp->Palette + index * 4 + 1) = g;
		*(bmp->Palette + index * 4 + 0) = b;

		BMP_LAST_ERROR_CODE = BMP_OK;
	}
}

/**************************************************************
	Returns the last error code.
**************************************************************/
BMP_STATUS BMP_GetError()
{
	return BMP_LAST_ERROR_CODE;
}

/**************************************************************
	Returns a description of the last error code.
**************************************************************/
const char *BMP_GetErrorDescription()
{
	if (BMP_LAST_ERROR_CODE > 0 && BMP_LAST_ERROR_CODE < BMP_ERROR_NUM)
	{
		return BMP_ERROR_STRING[BMP_LAST_ERROR_CODE];
	}
	else
	{
		return NULL;
	}
}

/*********************************** Private methods **********************************/

/**************************************************************
	Reads the BMP file's header into the data structure.
	Returns BMP_OK on success.
**************************************************************/
int ReadHeader(BMP *bmp, FILE *f)
{
	if (bmp == NULL || f == NULL)
	{
		return BMP_INVALID_ARGUMENT;
	}

	/* The header's fields are read one by one, and converted from the format's
	little endian to the system's native representation. */
	if (!ReadUSHORT(&(bmp->Header.Magic), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.FileSize), f))
		return BMP_IO_ERROR;
	if (!ReadUSHORT(&(bmp->Header.Reserved1), f))
		return BMP_IO_ERROR;
	if (!ReadUSHORT(&(bmp->Header.Reserved2), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.DataOffset), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.HeaderSize), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.Width), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.Height), f))
		return BMP_IO_ERROR;
	if (!ReadUSHORT(&(bmp->Header.Planes), f))
		return BMP_IO_ERROR;
	if (!ReadUSHORT(&(bmp->Header.BitsPerPixel), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.CompressionType), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.ImageDataSize), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.HPixelsPerMeter), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.VPixelsPerMeter), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.ColorsUsed), f))
		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.ColorsRequired), f))
		return BMP_IO_ERROR;

	return BMP_OK;
}

/**************************************************************
	Writes the BMP file's header into the data structure.
	Returns BMP_OK on success.
**************************************************************/
int WriteHeader(BMP *bmp, FILE *f)
{
	if (bmp == NULL || f == NULL)
	{
		return BMP_INVALID_ARGUMENT;
	}

	/* The header's fields are written one by one, and converted to the format's
	little endian representation. */
	if (!WriteUSHORT(bmp->Header.Magic, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.FileSize, f))
		return BMP_IO_ERROR;
	if (!WriteUSHORT(bmp->Header.Reserved1, f))
		return BMP_IO_ERROR;
	if (!WriteUSHORT(bmp->Header.Reserved2, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.DataOffset, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.HeaderSize, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.Width, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.Height, f))
		return BMP_IO_ERROR;
	if (!WriteUSHORT(bmp->Header.Planes, f))
		return BMP_IO_ERROR;
	if (!WriteUSHORT(bmp->Header.BitsPerPixel, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.CompressionType, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.ImageDataSize, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.HPixelsPerMeter, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.VPixelsPerMeter, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.ColorsUsed, f))
		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.ColorsRequired, f))
		return BMP_IO_ERROR;

	return BMP_OK;
}

/**************************************************************
	Reads a little-endian unsigned int from the file.
	Returns non-zero on success.
**************************************************************/
int ReadUINT(UINT *x, FILE *f)
{
	UCHAR little[4]; /* BMPs use 32 bit ints */

	if (x == NULL || f == NULL)
	{
		return 0;
	}

	if (fread(little, 4, 1, f) != 1)
	{
		return 0;
	}

	*x = (little[3] << 24 | little[2] << 16 | little[1] << 8 | little[0]);

	return 1;
}

/**************************************************************
	Reads a little-endian unsigned short int from the file.
	Returns non-zero on success.
**************************************************************/
int ReadUSHORT(USHORT *x, FILE *f)
{
	UCHAR little[2]; /* BMPs use 16 bit shorts */

	if (x == NULL || f == NULL)
	{
		return 0;
	}

	if (fread(little, 2, 1, f) != 1)
	{
		return 0;
	}

	*x = (little[1] << 8 | little[0]);

	return 1;
}

/**************************************************************
	Writes a little-endian unsigned int to the file.
	Returns non-zero on success.
**************************************************************/
int WriteUINT(UINT x, FILE *f)
{
	UCHAR little[4]; /* BMPs use 32 bit ints */

	little[3] = (UCHAR)((x & 0xff000000) >> 24);
	little[2] = (UCHAR)((x & 0x00ff0000) >> 16);
	little[1] = (UCHAR)((x & 0x0000ff00) >> 8);
	little[0] = (UCHAR)((x & 0x000000ff) >> 0);

	return (f && fwrite(little, 4, 1, f) == 1);
}

/**************************************************************
	Writes a little-endian unsigned short int to the file.
	Returns non-zero on success.
**************************************************************/
int WriteUSHORT(USHORT x, FILE *f)
{
	UCHAR little[2]; /* BMPs use 16 bit shorts */

	little[1] = (UCHAR)((x & 0xff00) >> 8);
	little[0] = (UCHAR)((x & 0x00ff) >> 0);

	return (f && fwrite(little, 2, 1, f) == 1);
}

// End of library

BMP *bmp;

int imageHeight = 100;
int imageWidth = 100;

unsigned char black(int i, int j)
{
	i = i * 2;
	j = j * 2;
	unsigned char r, g, b;
	BMP_GetPixelRGB(bmp, i, j, &r, &g, &b);
	return g < 128;
}

int color(int i, int j)
{
	unsigned char r, g, b;
	BMP_GetPixelRGB(bmp, i, j, &r, &g, &b);
	unsigned char d = r * 100 / 256;

	if (d < 5)
		d = 5;
	if (d > 80)
		d = 0;
	return d;
}

int ccolor(int r, int g, int b)
{
	// RRRR RGGG GGGB BBBB
	r = (31 * r / 255) << 11;
	g = (63 * g / 255) << 5;
	b = (31 * b / 255);
	return r + g + b;
}

int correctedAlpha(int z, int a)
{
	return (z * a + 235 * (255 - a)) / 255;
}

int toDig(char p)
{
	if (p >= 'A')
		return p - 'A' + 10;
	return p - '0';
}

int toHex(char p, char q)
{
	return toDig(p) * 16 + toDig(q);
}

void createIcon(const char *filen, const char *id)
{
	int cont = 0;
	FILE *file = fopen(filen, "r");
	int i = 0;

	int width = 0;
	int height = 0;

	int col[50000], row[50000], r[50000], g[50000], b[50000];
	int alpha[50000];
	char p[100];
	while (!feof(file))
	{
		int _col, _row, _r, _g, _b;
		float _alpha;
		fscanf(file, "%d", &_col);
		fscanf(file, "%d", &_row);
		fscanf(file, "%s", p);
		if (_col > width)
			width = _col;
		if (_row > height)
			height = _row;
		col[cont] = _col;
		row[cont] = _row;

		r[cont] = toHex(p[0], p[1]);
		g[cont] = toHex(p[2], p[3]);
		b[cont] = toHex(p[4], p[5]);
		alpha[cont++] = toHex(p[6], p[7]);
	}
	width += 1;
	height += 1;
	fclose(file);

	printf("const PROGMEM int _Icon%s[] = {%d,%d\n", id, width, height);
	int colx = 0;
	for (int rw = 0; rw < height; rw++)
	{
		int reps = 0;
		int lastcol = -1;
		int c = 0;
		while (c < width)
		{
			int i = c + rw * width;
			colx = 0;
			if (alpha[i] > 10)
			{
				colx = ccolor(correctedAlpha(r[i], alpha[i]), correctedAlpha(g[i], alpha[i]), correctedAlpha(b[i], alpha[i]));
				if (!colx)
					colx = 1;
			}
			if (colx != lastcol && c)
			{
				if (reps == 1 && lastcol >= 100)
					printf(",%d", lastcol);
				else
					printf(",%d,%d", reps, lastcol);
				reps = 1;
			}
			else
			{
				reps++;
			}
			lastcol = colx;
			c++;
		}
		printf(",%d,%d", reps, lastcol);
	}
	printf("};");
}

// Fast Font

void generate(char c, int *buffer, int matrixWidth, int matrixHeight)
{
	if (c == '\\')
		printf("int draw%d(int x, int y) { //   \n", c);
	else
		printf("int draw%d(int x, int y) { // CHAR: %c   \n", c, c);
	int firstColumnWithAtLeastOnePixel = -1;
	int lastColumnWithAtLeastOnePixel = 0;
	for (int i = 0; i < matrixWidth; i++)
	{
		for (int j = 0; j < matrixHeight; j++)
		{
			if (buffer[j * matrixWidth + i])
			{
				if (firstColumnWithAtLeastOnePixel == -1)
					firstColumnWithAtLeastOnePixel = i;
				if (i > lastColumnWithAtLeastOnePixel)
					lastColumnWithAtLeastOnePixel = i;
			}
		}
	}
	while (1)
	{
		int horSegmentCount = 0;
		int horSegmentOffset = 0;
		int hrow = 0;
		for (int row = 0; row < matrixHeight; row++)
		{
			int state = 0;
			int c = 0;
			int maxHorCount = 0;
			int maxHorOffset = -1;
			int x = 0;
			for (int col = 0; col < matrixWidth; col++)
			{
				int n = buffer[row * matrixWidth + col];
				if (state == 0 && n == 1)
				{
					x = col;
					state = 1;
					c = 1;
				}
				else if (state == 1 && n == 1)
				{
					c++;
				}
				else if (state == 0 && n == 0)
				{
				}
				else if (state == 1 && n == 0)
				{
					if (c > maxHorCount)
					{
						maxHorCount = c;
						maxHorOffset = x;
					}
					state = 0;
				}
			}
			if (maxHorCount > horSegmentCount)
			{
				horSegmentCount = maxHorCount;
				horSegmentOffset = maxHorOffset;
				hrow = row;
			}
			for (int i = 0; i < matrixWidth; i++)
			{
				char cc = ' ';
				if (buffer[row * matrixWidth + i])
				{
					cc = '*';
				}
			}
		}
		int vc = 0;
		int vx = 0;
		int vcol = 0;
		for (int i = 0; i < matrixWidth; i++)
		{
			int state = 0;
			int c = 0;
			int maxc = 0;
			int maxx = -1;
			int x = 0;
			for (int j = 0; j < matrixHeight; j++)
			{
				int n = buffer[j * matrixWidth + i];
				if (state == 0 && n == 1)
				{
					x = j;
					state = 1;
					c = 1;
				}
				else if (state == 1 && n == 1)
				{
					c++;
				}
				else if (state == 0 && n == 0)
				{
				}
				else if (state == 1 && n == 0)
				{
					if (c > maxc)
					{
						maxc = c;
						maxx = x;
					}
					state = 0;
				}
			}
			if (maxc > vc)
			{
				vc = maxc;
				vx = maxx;
				vcol = i;
			}
		}
		if (horSegmentCount == 0 && vc == 0)
			break;
		if (horSegmentCount > vc)
		{
			printf("    drawHorizontalLine(x+%d, y+%d, %d);\n", horSegmentOffset - firstColumnWithAtLeastOnePixel, hrow, horSegmentCount);
			for (int z = horSegmentOffset; z < horSegmentOffset + horSegmentCount; z++)
				buffer[hrow * matrixWidth + z] = 0;
		}
		else
		{
			printf("    drawVerticalLine(x+%d, y+%d, %d);\n", vcol - firstColumnWithAtLeastOnePixel, vx, vc);
			for (int z = vx; z < vx + vc; z++)
				buffer[z * matrixWidth + vcol] = 0;
		}
	}
	printf("    return %d;\n", lastColumnWithAtLeastOnePixel - firstColumnWithAtLeastOnePixel + 1);
	printf("}\n");
}

void createFont(const char *what)
{
	const char *charsInFont = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a";

	int x = 1;
	int y = 1;
	for (int i = 0; i < 300; i++)
		for (int j = 0; j < 250; j++)
			if (black(i, j))
			{
				x = i + 2;
				y = j;
				j = 300;
				i = 300;
			}

	for (int i = y; i < 250; i++)
		if (!black(x, i))
		{
			y = i;
			break;
		}

	int px = x;
	int py = y;
	int tot = 0;
	int miny = 100;
	int maxy = -1;
	int count = 0;

	int _max = 0;
	int _min = 100;
	int _maxy = 0;
	int _miny = 100;

	int *bytes = (int *)malloc(sizeof(int) * 60000);
	int *w = (int *)malloc(sizeof(int) * 60000);
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 13; c++)
		{
			int min = 0;
			int max = 29;
			for (int x = 0; x < 30; x++)
			{
				for (int y = 0; y < 20; y++)
				{
					int tx = c * 32 + px + x;
					int ty = r * 32 + py + y;
					if (black(tx, ty))
					{
						if (min == 0)
							min = x;
						max = x;
					}
				}
			}
			w[tot++] = max - min + 1;

			for (int y = 0; y < 20; y++)
			{
				for (int x = 0; x < 30; x++)
				{
					int tx = c * 32 + px + x;
					int ty = r * 32 + py + y;
					if (black(tx, ty))
					{
						if (y < miny)
							miny = y;
						if (y > maxy)
							maxy = y;
					}
				}
			}

			if (_min > min)
				_min = min;
			if (_miny > miny)
				_miny = miny;
			if (_max < max)
				_max = max;
			if (_maxy < maxy)
				_maxy = maxy;

			for (int y = 0; y <= 20; y++)
			{
				int b = 0;
				for (int x = 0; x < 30; x++)
				{
					int tx = c * 32 + px + x;
					int ty = r * 32 + py + y;
					if (x >= min && x <= max)
					{
						if (black(tx, ty))
						{
							b += pow(2, x - min);
						}
					}
				}
				bytes[count++] = b;
			}
		}
	}

	if (!strcmp(what, "fastfont"))
	{
		tot = 0;
		int h = _max - _min + 1;
		int v = _maxy - _miny + 1;
		for (int r = 0; r < 8; r++)
		{
			for (int c = 0; c < 13; c++)
			{
				int *buffer = (int *)malloc(h * v * sizeof(int));
				int z = 0;
				for (int y = 0; y < v; y++)
					for (int x = 0; x < h; x++)
					{
						int ty = r * 32 + _miny + y + py;
						int tx = c * 32 + _min + x + px;
						buffer[z] = black(tx, ty);
						z++;
					}
				generate(charsInFont[tot], buffer, h, v);
				tot++;
			}
		}
	}
	if (!strcmp(what, "font"))
	{

		printf("\n\n\n\n\n {%d,4,", maxy - miny + 1);
		for (int i = miny; i <= maxy; i++)
		{
			printf("0,");
		}
		for (int i = 1; i < 128; i++)
		{
			int pos = 0;
			for (int t = 0; t < strlen(charsInFont); t++)
				if (charsInFont[t] == i)
					pos = t;
			printf("%d,", w[pos]);
			int ini = pos * 21;
			for (int z = ini + miny; z <= ini + maxy; z++)
				printf("%d,", bytes[z]);
		}
		printf("};\n\n\n\n\n");
	}
	if (!strcmp(what, "widths"))
	{

		printf("{");
		for (int i = 1; i < 128; i++)
		{
			int pos = 0;
			for (int t = 0; t < strlen(charsInFont); t++)
				if (charsInFont[t] == i)
					pos = t;
			printf("%d,", w[pos]);
		}
		printf("};\n\n\n");
	}
}

// ic file icon/font extra
int main(int argc, const char *argv[])
{
	const char *file = argv[1];
	const char *kind = argv[2];
	const char *extra = argv[3];
	if (!strcmp(kind, "icon"))
		createIcon(file, extra);
	if (!strcmp(kind, "font"))
	{
		bmp = BMP_ReadFile(file);
		createFont(extra);
	}
	return 0;
}
