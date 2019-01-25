#pragma once



class Compress {
public:
	static void* CompressZip(const void* input, unsigned len, unsigned* destLen);
	static void* UncompressZip(const void* input, unsigned len, unsigned destLen);
};