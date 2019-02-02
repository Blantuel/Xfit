#include "Converter.h"
#include "../math/Point.h"
#include "../math/Point3Dw.h"

#include "../../Libpng/png.h"
#include "../../Libjpeg/jpeglib.h"
#include "../../Libogg/vorbis/vorbisfile.h"


void PNGConverter::DecodeToOut(const void* data, void** outData, unsigned size, Type _outputType) {
	png_image image;

	memset(&image,0, sizeof(image));
	image.version = PNG_IMAGE_VERSION;

	if (!png_image_begin_read_from_memory(&image, data, size)) {
		Free();
	}

	switch (image.format) {
	case PNG_FORMAT_ARGB: inputType = Type::ARGB; break;
	case PNG_FORMAT_ABGR: inputType = Type::ABGR; break;
	case PNG_FORMAT_RGBA: inputType = Type::RGBA; break;
	case PNG_FORMAT_BGRA: inputType = Type::BGRA; break;
	case PNG_FORMAT_RGB: inputType = Type::RGB; break;
	case PNG_FORMAT_BGR: inputType = Type::BGR; break;
	default:;
	}
	outputType = _outputType;
	switch (outputType) {
	case Type::ARGB:image.format = PNG_FORMAT_ARGB; break;
	case Type::ABGR:image.format = PNG_FORMAT_ABGR; break;
	case Type::RGBA:image.format = PNG_FORMAT_RGBA; break;
	case Type::BGRA:image.format = PNG_FORMAT_BGRA; break;
	case Type::RGB:image.format = PNG_FORMAT_RGB; break;
	case Type::BGR:image.format = PNG_FORMAT_BGR; break;
	default:;
	}
	source = data;
	sSize = size;
	dSize = PNG_IMAGE_SIZE(image);
	width = image.width;
	height = image.height;

	*outData = new char[dSize];
	if (!png_image_finish_read(&image, nullptr, *outData, 0, nullptr)) {
		delete[] * outData;
		Free();
	}
	dest = *outData;
}
void JPEGConverter::DecodeToOut(const void* data, void** outData, unsigned size, Type _outputType) {
	jpeg_decompress_struct cinfo;

	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, (unsigned char*)data, size);
	if (jpeg_read_header(&cinfo, true) != 1) {
		Free();
	}
	cinfo.dither_mode = JDITHER_NONE;

	source = data;
	sSize = size;
	inputType = Type::RGB;
	outputType = _outputType;
	
	switch (outputType) {
	case Type::ARGB:cinfo.out_color_space = JCS_ARGB; break;
	case Type::ABGR:cinfo.out_color_space = JCS_ABGR; break;
	case Type::RGBA:cinfo.out_color_space = JCS_RGBA; break;
	case Type::BGRA:cinfo.out_color_space = JCS_BGRA; break;
	case Type::RGB:cinfo.out_color_space = JCS_RGB; break;
	case Type::BGR:cinfo.out_color_space = JCS_BGR;break;
	default:;
	}

	jpeg_start_decompress(&cinfo);
	width = cinfo.output_width;
	height = cinfo.output_height;

	const unsigned calcWidth = cinfo.output_width * cinfo.output_components;
	dSize = calcWidth * cinfo.output_height;

	*outData = new unsigned char[dSize];
	unsigned char* outData2 = (unsigned char*)*outData;
	
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, &outData2, 1);
		outData2 += calcWidth;
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	dest = *outData;
}
size_t OGGConverter::oggRead(void* ptr, size_t size, size_t nmemb, void *_datasource) {
	OGGC* datasource = (OGGC*)_datasource;
	size_t read;
	int oggpos = datasource->oggpos;
	int oggsize = datasource->oggsize;
	if ((oggsize - oggpos) < (size*nmemb)) {
		read = oggsize - oggpos;
		memcpy(ptr,(unsigned char*)(datasource->data) + oggpos, read);
		datasource->oggpos = oggsize;
	} else {
		read = size*nmemb;
		memcpy(ptr, (unsigned char*)(datasource->data) + oggpos, size*nmemb);
		datasource->oggpos += (int)read;
	}
	return read;
}
int OGGConverter::oggSeek(void* _ptr, long long offset, int whence) {
	OGGC* ptr = (OGGC*)_ptr;
	switch (whence) {
	case SEEK_SET:
		ptr->oggpos = (int)offset;
		break;
	case SEEK_CUR:
		ptr->oggpos += (int)offset;
		break;
	case SEEK_END:
		ptr->oggpos = ptr->oggsize + (int)offset;
		break;
	}
	if (ptr->oggpos > ptr->oggsize)ptr->oggpos = ptr->oggsize;
	return ptr->oggpos;
}
int OGGConverter::oggClose(void *datasource) {
	return 0;
}
long OGGConverter::oggTell(void *datasource) {
	return ((OGGC*)datasource)->oggpos;
}
void OGGConverter::DecodeToOut(const void* data, void** outData, unsigned size) {
	ov_callbacks b;
	b.close_func = oggClose;
	b.read_func = oggRead;
	b.seek_func = oggSeek;
	b.tell_func = oggTell;

	g.oggpos = 0;
	g.oggsize = (int)size;
	g.data = data;

	OggVorbis_File f;
	if (ov_open_callbacks(&g, &f, nullptr, -1, b)) {
	}

	sSize = size;

	vorbis_info*info = ov_info(&f, -1);
	nChannels = info->channels;
	samplesPerSec = info->rate;
	wBitsPerSample = 16;


	size = ov_pcm_total(&f, -1) * info->channels << 1;
	dSize = size;

	int sec, r = 1;

	char* destChar = new char[size];
	*outData = destChar;


	int cnt = 0;
	while (true) {
		if ((size - cnt) < 4096) {
			r = ov_read(&f, destChar, (int)(size - cnt), 0, 2, 1, &sec);
			break;
		} else {
			r = ov_read(&f, destChar, 4096, 0, 2, 1, &sec);
		}
		if (r < 0) {
		} else {
			destChar += r;
			cnt += r;
		}
	}
	source = data;
	dSize = size;

	decoded = true;
	ov_clear(&f);
	dest = *outData;
}
void WAVConverter::DecodeToOut(const void* data, void** outData, unsigned size, bool copyNewData) {
	source = data;
	sSize = size;

	unsigned fmt = 0, datapos = 0;
	for (unsigned i = 12; i<size;) {
		if (*(unsigned*)((char*)data + i) == 544501094) {//fmt 
			fmt = i;
			if (fmt&&datapos)break;
		} else if (*(unsigned*)((char*)data + i) == 1635017060) {//data
			datapos = i;
			if (fmt&&datapos)break;
		}
		i += (*(unsigned*)((char*)data + i + 4)) + 8;
	}
	if (!(fmt&&datapos));

	nChannels = *(unsigned short*)((char*)data + fmt + 10);
	samplesPerSec = *(unsigned*)((char*)data + fmt + 12);
	wBitsPerSample = *(unsigned short*)((char*)data + fmt + 22);
	dSize = *(unsigned*)((char*)data + datapos + 4);

	dataPos = datapos + 8;
	if (copyNewData) {
		*outData = new char[dSize];
		memcpy(*outData, (char*)source + dataPos, dSize);
	} else {
		*outData = (char*)source + dataPos;
	}

	decoded = true;
	dest = *outData;
}
bool ObjConverter::Decode(const void * data, unsigned size, const void* mtldata, unsigned mtlsize)
{
	char* tt = (char*)data;
	char* tt2 = (char*)mtldata;
	unsigned i = 0,v=0,l=0,vn=0,vt=0;
	float x, y, z;
	bool ba = false,mtl=false;
	ObjData* vv = new ObjData;
	memset(vv, 0, sizeof(ObjData));
	while (i < size) {
		ba = false;
		if (i == 0) {
			ba = true;
		} else if (tt[i - 1] == '\n') {
			ba = true;
		}
		if (ba) {
			if (tt[i] == 'v'&&tt[i+1] == ' ') {
				v++;
			} else if (tt[i] == 'f') {
				l++;
			} else if (tt[i] == 'v'&&tt[i+1] == 'n') {
				vn++;
			} else if (tt[i] == 'v'&&tt[i + 1] == 't') {
				vt++;
			}
		}
		i++;
	}
	vv->verticesLen = v;
	vv->indicesLen = l*3;

	vv->posAr = new Point3DF[v];
	vv->indexAr = new unsigned[l*3];


	Point3DF* normal=0;
	PointF*uv=0;
	if (vn) {
		vv->normalAr = new Point3DF[v];
		normal = new Point3DF[vn];
	}
	if (vt) {
		vv->uvAr = new PointF[v];
		uv = new PointF[vt];
	}

	v = 0;
	l = 0;
	vn = 0;
	vt = 0;
	i = 0;

	int e, ca,n;
	unsigned a[3], b[3], c[3];
	
	while (i < size) {
		ba = false;
		if (i == 0) {
			ba = true;
		} else if (tt[i - 1] == '\n') {
			ba = true;
		}
		if (ba) {
			if (tt[i] == 'v'&&tt[i+1] == ' ') {
				e = 0;
				ca = 0;
				//"v %d %d %d\n"
				while (true) {
					if (tt[i + e] == ' ') {	
						if (ca == 0) {
							x = atof(&tt[i + e + 1]);
						} else if (ca == 1) {
							y = atof(&tt[i + e + 1]);
						} else {
							z = atof(&tt[i + e + 1]); break;
						}
						ca++;
					}
					e++;
				}
				vv->posAr[v] = Point3DF(x, y, z);
				v++;
			} else if (tt[i] == 'f') {
				//"f %d %d %d\n"

				e = 0;
				ca = 0;
				n=-1;
				
				if (vv->uvAr &&vv->normalAr) {
					while (true) {
						if (tt[i + e] == ' ') {
							n++;
							a[n] = atoi(&tt[i + e + 1]);
						} else if (tt[i + e] == '/') {
							if (ca == 0)b[n] = atoi(&tt[i + e + 1]);
							else if (ca == 1) {
								c[n] = atoi(&tt[i + e + 1]); if (n == 2)break;
							}
							ca++;
							if (ca == 2)ca = 0;
						}
						e++;
					}
					vv->uvAr[a[0]-1] = uv[b[0]-1];
					vv->normalAr[a[0]-1] = normal[c[0]-1];
					vv->indexAr[l++] = a[0]-1;

					vv->uvAr[a[1]-1] = uv[b[1]-1];
					vv->normalAr[a[1]-1] = normal[c[1]-1];
					vv->indexAr[l++] = a[1]-1;

					vv->uvAr[a[2]-1] = uv[b[2]-1];
					vv->normalAr[a[2]-1] = normal[c[2]-1];
					vv->indexAr[l++] = a[2]-1;
				} else if (vv->uvAr) {
					while (true) {
						if (tt[i + e] == ' ') {
							n++;
							a[n] = atoi(&tt[i + e + 1]);
						} else if (tt[i + e] == '/') {
							b[n] = atoi(&tt[i + e + 1]);
							if (n == 2)break;
						}
						e++;
					}
					vv->uvAr[a[0]-1] = uv[b[0]-1];
					vv->indexAr[l++] = a[0]-1;

					vv->uvAr[a[1]-1] = uv[b[1]-1];
					vv->indexAr[l++] = a[1]-1;

					vv->uvAr[a[2]-1] = uv[b[2]-1];
					vv->indexAr[l++] = a[2]-1;
				} else if (vv->normalAr) {
					while (true) {
						if (tt[i + e] == ' ') {
							n++;
							a[n] = atoi(&tt[i + e + 1]);
						} else if (tt[i + e] == '/') {
							e++;
							c[n] = atoi(&tt[i + e + 1]);
							if (n == 2)break;
						}
						e++;
					}
					vv->normalAr[a[0]-1] = normal[c[0]-1];
					vv->indexAr[l++] = a[0]-1;

					vv->normalAr[a[1]-1] = normal[c[1]-1];
					vv->indexAr[l++] = a[1] - 1;

					vv->normalAr[a[2]-1] = normal[c[2]-1];
					vv->indexAr[l++] = a[2] - 1;
				} else {
					while (true) {
						if (tt[i + e] == ' ') {
							n++;
							a[n] = atoi(&tt[i + e + 1]);
							if (n == 2)break;
						}
						e++;
					}
					vv->indexAr[l++] = a[0]-1;
					vv->indexAr[l++] = a[1]-1;
					vv->indexAr[l++] = a[2]-1;
				}
			} else if (tt[i] == 'v'&&tt[i + 1] == 'n') {
				e = 0;
				ca = 0;
				//"vn %d %d %d\n"
				while (true) {
					if (tt[i + e] == ' ') {
						if (ca == 0)x = atoi(&tt[i + e + 1]);
						else if (ca == 1)y = atoi(&tt[i + e + 1]);
						else { z = atoi(&tt[i + e + 1]); break; }
						ca++;
					}
					e++;
				}
				normal[vn++] = Point3DF(x, y, z);
			} else if (tt[i] == 'v'&&tt[i + 1] == 't') {
				e = 0;
				ca = 0;
				//"vt %d %d\n"
				while (true) {
					if (tt[i + e] == ' ') {
						if (ca == 0)x = atoi(&tt[i + e + 1]);
						else if (ca == 1) { y = atoi(&tt[i + e + 1]); break; }
						ca++;
					}
					e++;
				}
				uv[vt++] = PointF(x, y);
			} else if(!mtl) {
				if (!strncmp(tt + i, "mtllib ", 7)) {
					e = 0;
					while (tt[i + 7 + e] != '\n') e++;

					vv->mtlPath = new char[e + 1];
					memcpy(vv->mtlPath, tt + i + 7, e);
					vv->mtlPath[e] = 0;
					mtl = true;
				}
			} else if (tt[i] == 'o') {
				e = 0;
				while (tt[i + e] != '\n') e++;
				e--;
				vv->objectName= new char[e + 1];
				memcpy(vv->objectName, tt + i + 1, e);
				vv->objectName[e] = 0;
			}
		}
		i++;
	}
	source = data;
	sSize = size;

	mtl = false;

	bool tex=false, nortex = false;
	i = 0;
	while (i < mtlsize) {
		ba = false;
		if (i == 0) {
			ba = true;
		} else if (tt2[i - 1] == '\n') {
			ba = true;
		}
		if (ba) {
			if (!mtl) {
				if (!strncmp(tt2 + i, "newmtl ", 7)) {
					e = 0;
					while (tt2[i + 7 + e] != '\n') e++;

					vv->materialName = new char[e + 1];
					memcpy(vv->materialName, tt2 + i + 7, e);
					vv->materialName[e] = 0;
					mtl = true;
				}
			} else if (tt2[i] == 'N'&&tt2[i + 1] == 's') {
				vv->specularReflect = atof(tt2 + i + 3);
			} else if (!tex) {
				if (!strncmp(tt2 + i, "map_Kd ", 7)) {
					e = 0;
					ca = 0;
					while (tt2[i + 7 + e] != '\n') {
						if (tt2[i + 7 + e] == ' ')ca = e;
						e++;
					}
					e = e - ca + 1;
					vv->texturePath = new char[e + 1];
					memcpy(vv->texturePath, tt2 + i + 7 + ca + 1, e);
					vv->texturePath[e] = 0;

					tex = true;
				}
			} else if (!nortex) {
				if (!strncmp(tt2 + i, "bump ", 5)) {
					e = 0;
					ca = 0;
					while (tt2[i + 5 + e] != '\n') {
						if (tt2[i + 5 + e] == ' ')ca = e;
						e++;
					}
					e = e - ca + 1;
					vv->normalTexturePath = new char[e + 1];
					memcpy(vv->normalTexturePath, tt2 + i + 5 + ca + 1, e);
					vv->normalTexturePath[e] = 0;

					nortex = true;
				}
			}
		}
		i++;
	}

	dest = vv;
	dSize = sizeof(ObjData);
	if (uv)delete[]uv;
	if (normal)delete[]normal;
	return true;
}