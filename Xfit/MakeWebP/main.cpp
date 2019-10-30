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
		cout << "���� : �Է� ������ �����ϴ�." << endl;
		return NoInputFile;
	}

	if (argc > 2) {//�Է� ������ 2�� �̻� �־ animate webp ���Ϸ� ����
		const unsigned len = argc - 1;//���� ���� ��� �� ����
		unsigned char** outDatas = new unsigned char*[len];//������ raw �����͵� ���� 
		strcpy_s(path, MAX_PATH, argv[1]);//ù��° ���� �̸����� webp ����

		unsigned width, height;
		for (unsigned i = 0; i < len; i++) {
			cout << i << " : " << argv[i + 1] << "���� �д� ��.." << endl;
			File file(argv[i+1]);//ù��°�� ���� ���� ����̴� �ι�°���� �˻�

			const unsigned fileSize = file.GetSize();
			unsigned char* inputData = new unsigned char[fileSize];
			file.ReadBytes(fileSize, inputData);

			PNGDecoder pngDecoder;

			pngDecoder.LoadHeader(inputData, fileSize);
			outDatas[i] = new unsigned char[pngDecoder.GetOutputSize()];

			cout << i << " : " << argv[i + 1] << "���� ���ڵ� ��.." << endl;

			pngDecoder.Decode(outDatas[i]);

			width = pngDecoder.GetWidth();
			height = pngDecoder.GetHeight();
			delete[]inputData;
		}
		cout << "���� ���ڵ� ��.." << endl;
		WebPAnimateLosslessEncoder encoder;
		void * outData2 = encoder.Encode((const void**)outDatas, ImageType::RGBA, len, width, height);

		size_t pathLen = strlen(path) - 1;
		while (path[pathLen] != '.')pathLen--;
		path[pathLen + 1] = 'w'; path[pathLen + 2] = 'e'; path[pathLen + 3] = 'b'; path[pathLen + 4] = 'p'; path[pathLen + 5] = 0;
		File outFile(path, File::OpenMode::ReadWriteCreate);

		cout << path << " ���� ���� ��.." << endl;

		outFile.WriteBytes(encoder.GetOutputSize(), outData2);
		
		for (unsigned i = 0; i < len; i++) delete[]outDatas[i];
		delete[]outDatas;

		free(outData2);
	} else {//�Է������� 1���� ���� �̹����� ����
		strcpy_s(path, MAX_PATH, argv[1]);
		File file(path);

		cout << path << " ���� �д� ��.." << endl;

		const unsigned fileSize = file.GetSize();
		unsigned char* inputData = new unsigned char[fileSize];
		unsigned char* outData;
		file.ReadBytes(fileSize, inputData);

		PNGDecoder pngDecoder;
		WebPLosslessEncoder encoder;

		pngDecoder.LoadHeader(inputData, fileSize);
		outData = new unsigned char[pngDecoder.GetOutputSize()];

		cout << path << " ���� ���ڵ� ��.." << endl;

		pngDecoder.Decode(outData);

		cout << "���� ���ڵ� ��.." << endl;

		void* outData2 = encoder.Encode(outData, ImageType::RGBA, pngDecoder.GetWidth(), pngDecoder.GetHeight());

		size_t len = strlen(path) - 1;
		while (path[len] != '.')len--;
		path[len + 1] = 'w'; path[len + 2] = 'e'; path[len + 3] = 'b'; path[len + 4] = 'p'; path[len + 5] = 0;
		File outFile(path, File::OpenMode::ReadWriteCreate);

		cout << path << " ���� ���� ��.." << endl;

		outFile.WriteBytes(encoder.GetOutputSize(), outData2);

		delete[]inputData;
		delete[]outData;

		free(outData2);
	}
	return Success;
}