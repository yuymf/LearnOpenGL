#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //set opengl to 3.3;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //GLFW_CONTEXT_VERSION_MAJOR != GLFW_VERSION_MAJOR
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Rec Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // frame rate
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, // texture
             50.0f, -50.0f, 1.0f, 0.0f,
             50.0f,  50.0f, 1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f, 1.0f
        };

        // index buffer
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // R = RS * (ALPHA) + RD * (1 - ALPHA) 

        //unsigned int vao;
        //glGenVertexArrays(1, &vao);
        //glBindVertexArray(vao);
        VertexArray va;

        //unsigned int buffer;
        //glGenBuffers(1, &buffer);
        //glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        //GLCall(glEnableVertexAttribArray(0));
        //GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
        VertexBufferLayout layout;
        layout.Push<float>(2);

        layout.Push<float>(2); // texture layout, m_Stride + 4
        va.AddBuffer(vb, layout);

        //unsigned int ibo;
        //glGenBuffers(1, &ibo);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

       /* ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

        unsigned int shader = CreaterShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);

        int location = glGetUniformLocation(shader, "u_Color");
        ASSERT(location != -1);
        glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);*/
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/ChernoLogo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        //glBindVertexArray(0);
        va.Unbind();
        //glUseProgram(0);
        shader.Unbind();
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        vb.Unbind();
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        ib.Unbind();
        //texture.Unbind();

        Renderer renderer;

        // Setup Dear ImGui context
        const char* glsl_version = "#version 130";
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);
        //danamic color based on uniform u_Color£»
        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                //glUseProgram(shader);

                //glDrawArrays(GL_TRIANGLES, 0, 3);
                //glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
                //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
                shader.SetUniformMat4f("u_MVP", mvp);
                //glBindVertexArray(vao);
                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }
            //glBindBuffer(GL_ARRAY_BUFFER, buffer);
            //va.Bind(); //in this addbuffer, vb already bind;
            //vb.Bind();

            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            //ib.Bind();

            ////GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            // 2. Show a simple window that we create ourselves. 
            //We use a Begin/End pair to created a named window.
            {         
                ImGui::SliderFloat3("Tanslation A", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("Tanslation B", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}