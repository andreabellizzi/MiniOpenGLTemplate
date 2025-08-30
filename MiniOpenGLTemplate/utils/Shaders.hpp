#ifndef _shaders_h_
#define _shaders_h_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct {
	const char* file;
	unsigned int shaderType;
} ShaderProgram_t;


class Shader {
	public:
		void use(void) const;
		Shader(ShaderProgram_t* shaders, unsigned int size);
		Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
		unsigned int getProgramId(void) const { return programId; };
		void setUniform(const char* uniformName, float f1) const;
		void setUniform(const char* uniformName, int i1) const;
		void setUniform4fv(const char* uniformName, float* fptr) const;
		void setUniform(const char* uniformName, float f1, float f2, float f3, float f4);

		~Shader();
	private:
		unsigned int programId;

};

unsigned int createShader(ShaderProgram_t* shaders, unsigned int size);
bool checkShader(unsigned int id, unsigned int pname);

#ifdef __cplusplus
}
#endif

#endif // _shaders_h_