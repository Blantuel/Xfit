#include <iostream>
#include <file/File.h>
#include <encoder/WebPLosslessEncoder.h>
#include <encoder/WebPAnimateLosslessEncoder.h>
#include <decoder/PNGDecoder.h>

constexpr int Success = 0;
constexpr int NoInputFile = -1;

char path[MAX_PATH];
int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "오류 : 입력 파일이 없습니다." << endl;
		return NoInputFile;
	}

	if (argc > 2) {//입력 파일이 2개 이상 있어서 animate webp 파일로 만듬
		const unsigned len = argc - 1;//실행 파일 경로 뺀 갯수
		unsigned char** outDatas = new unsigned char*[len];//프레임 raw 데이터들 저장 
		strcpy_s(path, MAX_PATH, argv[1]);//첫번째 파일 이름으로 webp 저장

		unsigned width, height;
		for (unsigned i = 0; i < len; i++) {
			cout << i << " : " << argv[i + 1] << "파일 읽는 중.." << endl;
			File file(argv[i+1]);//첫번째는 실행 파일 경로이니 두번째부터 검색

			const unsigned fileSize = file.GetSize();
			unsigned char* inputData = new unsigned char[fileSize];
			file.ReadBytes(fileSize, inputData);

			PNGDecoder pngDecoder;

			pngDecoder.LoadHeader(inputData, fileSize);
			outDatas[i] = new unsigned char[pngDecoder.GetOutputSize()];

			cout << i << " : " << argv[i + 1] << "파일 디코딩 중.." << endl;

			pngDecoder.Decode(outDatas[i]);

			width = pngDecoder.GetWidth();
			height = pngDecoder.GetHeight();
			delete[]inputData;
		}
		cout << "파일 인코딩 중.." << endl;
		WebPAnimateLosslessEncoder encoder;
		void * outData2 = encoder.Encode((const void**)outDatas, ImageType::RGBA, len, width, height);

		size_t pathLen = strlen(path) - 1;
		while (path[pathLen] != '.')pathLen--;
		path[pathLen + 1] = 'w'; path[pathLen + 2] = 'e'; path[pathLen + 3] = 'b'; path[pathLen + 4] = 'p'; path[pathLen + 5] = 0;
		File outFile(path, File::OpenMode::ReadWriteCreate);

		cout << path << " 파일 저장 중.." << endl;

		outFile.WriteBytes(encoder.GetOutputSize(), outData2);
		
		for (unsigned i = 0; i < len; i++) delete[]outDatas[i];
		delete[]outDatas;

		free(outData2);
	} else {//입력파일이 1개라 단일 이미지로 만듬
		strcpy_s(path, MAX_PATH, argv[1]);
		File file(path);

		cout << path << " 파일 읽는 중.." << endl;

		const unsigned fileSize = file.GetSize();
		unsigned char* inputData = new unsigned char[fileSize];
		unsigned char* outData;
		file.ReadBytes(fileSize, inputData);

		PNGDecoder pngDecoder;
		WebPLosslessEncoder encoder;

		pngDecoder.LoadHeader(inputData, fileSize);
		outData = new unsigned char[pngDecoder.GetOutputSize()];

		cout << path << " 파일 디코딩 중.." << endl;

		pngDecoder.Decode(outData);

		cout << "파일 인코딩 중.." << endl;

		void* outData2 = encoder.Encode(outData, ImageType::RGBA, pngDecoder.GetWidth(), pngDecoder.GetHeight());

		size_t len = strlen(path) - 1;
		while (path[len] != '.')len--;
		path[len + 1] = 'w'; path[len + 2] = 'e'; path[len + 3] = 'b'; path[len + 4] = 'p'; path[len + 5] = 0;
		File outFile(path, File::OpenMode::ReadWriteCreate);

		cout << path << " 파일 저장 중.." << endl;

		outFile.WriteBytes(encoder.GetOutputSize(), outData2);

		delete[]inputData;
		delete[]outData;

		free(outData2);
	}
	return Success;
}