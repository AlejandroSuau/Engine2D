#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

Shader::Shader()
    : program_id_(0) {}

Shader::~Shader() {
    if (program_id_ != 0) {
        glDeleteProgram(program_id_);
    }
}

std::string Shader::ReadFile(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el fichero shader: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint Shader::CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);

    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Comprueba compilación del shader
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(log_length, '\0');
        glGetShaderInfoLog(shader, log_length, nullptr, log.data());

        glDeleteShader(shader);

        const std::string shader_type =
            (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";

        throw std::runtime_error(
            "Error compilando shader " + shader_type + ":\n" + log
        );
    }

    return shader;
}

void Shader::Load(const std::string& vertex_path, const std::string& fragment_path) {
    // Lee el código fuente de ambos shaders
    std::string vertex_code = ReadFile(vertex_path);
    std::string fragment_code = ReadFile(fragment_path);

    // Compila cada shader
    GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_code);
    GLuint fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_code);

    // Crea el programa y adjunta los shaders compilados
    program_id_ = glCreateProgram();

    glAttachShader(program_id_, vertex_shader);
    glAttachShader(program_id_, fragment_shader);

    glBindAttribLocation(program_id_, 0, "aPos");
    glBindAttribLocation(program_id_, 1, "aUV");

    glLinkProgram(program_id_);

    // Comprueba el enlace del programa
    GLint success = GL_FALSE;
    glGetProgramiv(program_id_, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        GLint log_length = 0;
        glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(log_length, '\0');
        glGetProgramInfoLog(program_id_, log_length, nullptr, log.data());

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(program_id_);
        program_id_ = 0;

        throw std::runtime_error("Error enlazando programa shader:\n" + log);
    }

    // Una vez enlazado el programa, los shaders individuales ya no hacen falta.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::Use() const {
    glUseProgram(program_id_);
}

void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(program_id_, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(program_id_, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(program_id_, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program_id_, name.c_str()), x, y, z);
}
