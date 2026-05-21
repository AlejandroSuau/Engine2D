#pragma once

#include <string>
#include <glad/glad.h>

class Shader {
public:
    Shader();
    ~Shader();

    // Carga, compila y enlaza un vertex shader y un fragment shader.
    void Load(const std::string& vertex_path, const std::string& fragment_path);
    
    // Activa este programa de shader para los siguientes draw calls.
    void Use() const;

    GLuint GetID() const { return program_id_; }

    // Uniforms básicos
    void SetFloat(const std::string& name, float value) const;
    void SetInt(const std::string& name, int value) const;
    void SetVec2(const std::string& name, float x, float y) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;

private:
    GLuint program_id_;

    // Lee el contenido completo de un fichero de texto.
    std::string ReadFile(const std::string& path);
    
    // Compila un shader individual: vertex o fragment.
    GLuint CompileShader(GLenum type, const std::string& source);
};