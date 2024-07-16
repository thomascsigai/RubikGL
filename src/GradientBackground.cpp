// background_gradients.c
// Author: Maciej Halber
// https://gist.github.com/mhalber/0a9b8a78182eb62659fc18d23fe5e94e

#include <window.hpp>

#define SHADER_HEADER "#version 330 core\n"
#define SHADER_STR(x) #x

void mygl_GradientBackground(float top_r, float top_g, float top_b, float top_a,
    float bot_r, float bot_g, float bot_b, float bot_a)
{
    glDisable(GL_DEPTH_TEST);

    static GLuint background_vao = 0;
    static GLuint background_shader = 0;

    if (background_vao == 0)
    {
        glGenVertexArrays(1, &background_vao);

        const char* vs_src = (const char*)SHADER_HEADER SHADER_STR
        (
            out vec2 v_uv;
        void main()
        {
            uint idx = uint(gl_VertexID);
            gl_Position = vec4(idx & 1U, idx >> 1U, 0.0, 0.5) * 4.0 - 1.0;
            v_uv = vec2(gl_Position.xy * 0.5 + 0.5);
        }
        );

        const char* fs_src = (const char*)SHADER_HEADER SHADER_STR
        (
            uniform vec4 top_color;
        uniform vec4 bot_color;
        in vec2 v_uv;
        out vec4 frag_color;

        void main()
        {
            frag_color = bot_color * (1 - v_uv.y) + top_color * v_uv.y;
        }
        );
        GLuint vs_id, fs_id;
        vs_id = glCreateShader(GL_VERTEX_SHADER);
        fs_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vs_id, 1, &vs_src, NULL);
        glShaderSource(fs_id, 1, &fs_src, NULL);
        glCompileShader(vs_id);
        glCompileShader(fs_id);
        background_shader = glCreateProgram();
        glAttachShader(background_shader, vs_id);
        glAttachShader(background_shader, fs_id);
        glLinkProgram(background_shader);
        glDetachShader(background_shader, fs_id);
        glDetachShader(background_shader, vs_id);
        glDeleteShader(fs_id);
        glDeleteShader(vs_id);
        glUseProgram(background_shader);
    }

    glUseProgram(background_shader);
    GLuint top_color_loc = glGetUniformLocation(background_shader, "top_color");
    GLuint bot_color_loc = glGetUniformLocation(background_shader, "bot_color");
    glUniform4f(top_color_loc, top_r, top_g, top_b, top_a);
    glUniform4f(bot_color_loc, bot_r, bot_g, bot_b, bot_a);

    glBindVertexArray(background_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}
