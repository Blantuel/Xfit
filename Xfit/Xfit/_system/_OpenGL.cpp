#ifdef __ANDROID__

#include "_OpenGL.h"
#include "_Renderer.h"
#include "_Android.h"


#include "../file/AssetFile.h"

#include "_XfitDataFiles.h"

using namespace std;

namespace _System::_OpenGL {

	static void GL_APIENTRY OpenglDebugCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const GLchar* _message, const void* _userParam) {
		string str = "!!XFit OpenGL DebugLog ";
        
		switch (_source) {
		case GL_DEBUG_SOURCE_API:
			str += "source=API ";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			str += "source=WINDOW SYSTEM ";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			str += "source=SHADER COMPILER ";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			str += "source=THIRD PARTY ";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			str += "source=APPLICATION ";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			str += "source=OTHER ";
			break;
		default:
			str += "source=UNKNOWN ";
			break;
		}
		switch (_type) {
		case GL_DEBUG_TYPE_ERROR:
			str += "type=ERROR id=";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			str += "type=DEPRECATED_BEHAVIOR id=";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			str += "type=UNDEFINED_BEHAVIOR id=";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			str += "type=PORTABILITY id=";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			str += "type=PERFORMANCE id=";
			break;
		case GL_DEBUG_TYPE_OTHER:
			str += "type=OTHER id=";
			break;
		default:
			str += "type=UNKNOWN id=";
			break;
		}
		char num[20];
		sprintf(num, "%d", _id);

		str += num;
		switch (_severity) {
		case GL_DEBUG_SEVERITY_LOW:
			str += " severity=LOW message=";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			str += " severity=MEDIUM message=";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			str += " severity=HIGH message=";
			break;
		default:
			str += " severity=UNKNOWN message=";
			break;
		}
		str += _message;
		str += "\n";
		
		PRINTMSG("%s", str.c_str());
	}
	//프로그램 병합 Shader
	static GLuint LoadAndCompileShader2(const char* _vertPath, const char* _fragPath) {
		const GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		const GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		AssetFile file(_vertPath);

		int shaderSize = file.GetSize();
		char* data = new char[shaderSize];
		file.ReadBytes(shaderSize, data);
		glShaderSource(vertShader, 1, &data, &shaderSize);


		glCompileShader(vertShader);

		GLint compileStatus;
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileStatus);
		if (!compileStatus) {
			GLint compileLogLen;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &compileLogLen);
			char* compileLogCharAr = new char[compileLogLen];
			glGetShaderInfoLog(vertShader, compileLogLen, nullptr, compileLogCharAr);

			PRINTMSG("!!XFit OpenGL Shader Compile Error : %s\n", compileLogCharAr);
			delete[]compileLogCharAr;
		}

		delete[] data;
		file.Close();

		file.Open(_fragPath);
		shaderSize = file.GetSize();
		data = new char[shaderSize];
		file.ReadBytes(shaderSize, data);

		glShaderSource(fragShader, 1, &data, &shaderSize);

		glCompileShader(fragShader);

		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);
		if (!compileStatus) {
			GLint compileLogLen;
			glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &compileLogLen);
			char* compileLogCharAr = new char[compileLogLen];
			glGetShaderInfoLog(fragShader, compileLogLen, nullptr, compileLogCharAr);

			PRINTMSG("!!XFit OpenGL Shader Compile Error : %s\n", compileLogCharAr);
			delete[]compileLogCharAr;
		}

		delete[] data;
		file.Close();

		const GLuint prog = glCreateProgram();

		glAttachShader(prog, vertShader);
		glAttachShader(prog, fragShader);

		glLinkProgram(prog);

		GLint linkStatus;
		glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
		if (!linkStatus) {
			GLint linkLogLen;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &linkLogLen);
			char* linkLogCharAr = new char[linkLogLen];
			glGetProgramInfoLog(prog, linkLogLen, nullptr, linkLogCharAr);

			PRINTMSG("!!XFit OpenGL Shader Link Error : %s\n", linkLogCharAr);
			delete[]linkLogCharAr;
		}

		glDetachShader(prog, vertShader);
		glDetachShader(prog, fragShader);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		return prog;
	}
	//프로그램 분리 Shader
	static GLuint LoadAndCompileShader(const char* _path,GLenum _type) {
		const GLuint shader = glCreateShader(_type);

		AssetFile file(_path);

		int shaderSize = file.GetSize();
		char* data = new char[shaderSize];
		file.ReadBytes(shaderSize, data);

		glShaderSource(shader, 1, &data, &shaderSize);

		glCompileShader(shader);

		GLint compileStatus;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
		if (!compileStatus) {
			GLint compileLogLen;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compileLogLen);
			char* compileLogCharAr = new char[compileLogLen];
			glGetShaderInfoLog(shader, compileLogLen, nullptr, compileLogCharAr);

			PRINTMSG("!!XFit OpenGL Shader Compile Error : %s\n", compileLogCharAr);
			delete[]compileLogCharAr;
		}
		const GLuint prog = glCreateProgram();

		glProgramParameteri(prog, GL_PROGRAM_SEPARABLE, GL_TRUE);

		glAttachShader(prog, shader);
		glLinkProgram(prog);

		GLint linkStatus;
		glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
		if (!linkStatus) {
			GLint linkLogLen;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &linkLogLen);
			char* linkLogCharAr = new char[linkLogLen];
			glGetProgramInfoLog(prog, linkLogLen, nullptr, linkLogCharAr);

			PRINTMSG("!!XFit OpenGL Shader Link Error : %s\n", linkLogCharAr);
			delete[]linkLogCharAr;
		}
		glDetachShader(prog, shader);
		glDeleteShader(shader);

		delete []data;
		file.Close();

		return prog;
	}
	void Init(System::CreateInfo* _info) {
		int major, minor;
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		_System::_Renderer::version.minorVersion = (unsigned)minor;
		_System::_Renderer::version.majorVersion = (unsigned)major;

		_System::_Renderer::versionNumber = _System::_Renderer::version.majorVersion * 100 + _System::_Renderer::version.minorVersion;

#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);


		PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback2 = (PFNGLDEBUGMESSAGECALLBACKPROC)eglGetProcAddress("glDebugMessageCallback");

		if(glDebugMessageCallback2)glDebugMessageCallback2(OpenglDebugCallback, nullptr);
#endif
		glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);

		/*if(_System::_Renderer::versionNumber >= 301) {
			imgVertProg = LoadAndCompileShader("shaders/imgVert2D.glsl", GL_VERTEX_SHADER);
			//imgInsVertProg = LoadAndCompileShader("shaders/imgInsVert.glsl", GL_VERTEX_SHADER);
			//shapeVertProg = LoadAndCompileShader("shaders/shapeVert.glsl", GL_VERTEX_SHADER);
			//imgMultiInsVertProg = LoadAndCompileShader("shaders/imgMultiInsVert.glsl", GL_VERTEX_SHADER);

			imgFragProg = LoadAndCompileShader("shaders/imgFrag2D.glsl", GL_FRAGMENT_SHADER);
			//shapeFragProg = LoadAndCompileShader("shaders/shapeFrag.glsl", GL_FRAGMENT_SHADER);
			//imgMultiInsFragProg = LoadAndCompileShader("shaders/imgMultiInsFrag.glsl", GL_FRAGMENT_SHADER);

			glGenProgramPipelines(1, &progPipeline);
			glBindProgramPipeline(progPipeline);

			imgVert::matUniform = glGetUniformLocation(imgVertProg, "matUniform");
            imgVert::viewMatUniform = glGetUniformLocation(imgVertProg, "viewMatUniform");
			//shapeVert::matUniform = glGetUniformLocation(shapeVertProg, "matUniform");

			imgFrag::colorMatUniform = glGetUniformLocation(imgFragProg, "colorMatUniform");
			//shapeFrag::colorUniform = glGetUniformLocation(shapeFragProg, "colorUniform");
			//imgMultiInsFrag::colorMatUniform = glGetUniformLocation(imgMultiInsFragProg, "colorMatUniform");

			imgFrag::samplerUniform = glGetUniformLocation(imgFragProg, "samplerUniform");
			//imgMultiInsFrag::samplerUniform = glGetUniformLocation(imgMultiInsFragProg, "samplerUniform");

			glProgramUniform1i(imgFragProg, imgFrag::samplerUniform, 0);
			//glProgramUniform1i(imgMultiInsFragProg, imgMultiInsFrag::samplerUniform, 0);
		} else {*/
			imgProg = LoadAndCompileShader2("shaders/imgVert2D.glsl", "shaders/imgFrag2D.glsl");
			shapeProg = LoadAndCompileShader2("shaders/shapeVert.glsl", "shaders/shapeFrag.glsl");
			imgInsProg = LoadAndCompileShader2("shaders/imgInsVert2D.glsl", "shaders/imgInsFrag2D.glsl");
			imgMultiInsProg = LoadAndCompileShader2("shaders/imgMultiInsVert2D.glsl", "shaders/imgMultiInsFrag2D.glsl");

			img::matUniform = glGetUniformLocation(imgProg, "matUniform");
            img::viewMatUniform = glGetUniformLocation(imgProg, "viewMatUniform");
			imgIns::matUniform = glGetUniformLocation(imgInsProg, "matUniform");
			imgIns::viewMatUniform = glGetUniformLocation(imgInsProg, "viewMatUniform");
            imgMultiIns::matUniform = glGetUniformLocation(imgMultiInsProg, "matUniform");
            imgMultiIns::viewMatUniform = glGetUniformLocation(imgMultiInsProg, "viewMatUniform");
			shape::matUniform = glGetUniformLocation(shapeProg, "matUniform");
			shape::viewMatUniform = glGetUniformLocation(shapeProg, "viewMatUniform");
			shape::colorUniform = glGetUniformLocation(shapeProg, "colorUniform");

			img::colorMatUniform = glGetUniformLocation(imgProg, "colorMatUniform");
			//shape::colorUniform = glGetUniformLocation(shapeProg, "colorUniform");
			//imgMultiIns::colorMatUniform = glGetUniformLocation(imgMultiInsProg, "colorMatUniform");

			img::samplerUniform = glGetUniformLocation(imgProg, "samplerUniform");
			imgIns::samplerUniform = glGetUniformLocation(imgInsProg, "samplerUniform");
            imgMultiIns::samplerUniform = glGetUniformLocation(imgMultiInsProg, "samplerUniform");
			//imgMultiIns::samplerUniform = glGetUniformLocation(imgMultiInsProg, "samplerUniform");

			glUniform1i(img::samplerUniform, 0);
			glUniform1i(imgIns::samplerUniform, 0);
			glUniform1i(imgMultiIns::samplerUniform, 0);
			//glUniform1i(imgMultiIns::samplerUniform, 0);
		//}
		_System::_Renderer::vSync = _info->vSync;
		_System::_Renderer::msaaCount = _info->msaaCount;

		_System::_Renderer::version.name = System::RendererName::OpenGL;

		glViewport(0, 0, (int)System::GetWindowWidth(), (int)System::GetWindowHeight());
	}
	void Release() {

	}

	void Resize() {
		glViewport(0, 0, (int)System::GetWindowWidth(), (int)System::GetWindowHeight());
	}
}

#endif