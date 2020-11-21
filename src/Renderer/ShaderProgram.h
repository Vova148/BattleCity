#include <iostream>
#include <glad/glad.h>
namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();
		bool isCompiled() const { return m_isCompiled; }
		void use()const;

		ShaderProgram() = delete; 
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&&)noexcept;
		ShaderProgram(ShaderProgram&&)noexcept;
	private:	
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID); //в параметрах код шейдера, його вид і на вихід його індефікатор
		bool m_isCompiled = false;
		GLuint m_ID = 0;

	};
}