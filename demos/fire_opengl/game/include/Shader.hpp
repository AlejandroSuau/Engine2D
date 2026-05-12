#pragma once

#include <string>
#include <glad/glad.h>

class Shader {
public:
    Shader();
    ~Shader();

    void Load(const std::string& vertex_path, const std::string& fragment_path);
    void Use() const;

    GLuint GetID() const { return program_id_; }

    // Uniforms básicos
    void SetFloat(const std::string& name, float value) const;
    void SetInt(const std::string& name, int value) const;
    void SetVec2(const std::string& name, float x, float y) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;

private:
    GLuint program_id_;

    std::string ReadFile(const std::string& path);
    GLuint CompileShader(GLenum type, const std::string& source);
};