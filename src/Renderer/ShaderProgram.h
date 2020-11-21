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
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID); //� ���������� ��� �������, ���� ��� � �� ����� ���� �����������
		bool m_isCompiled = false;
		GLuint m_ID = 0;

	};
}