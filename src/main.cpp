#include <stdio.h>
#include <iostream>
#include <sstream>

#include <GLFW/glfw3.h>


class Graphics
{
private:
    /* data */
    unsigned char* pixel_on_screen;

public:
    Graphics(uint width, uint height, uint pixel_width, uint pixel_height);
    ~Graphics();

    unsigned char* get_pixel_on_screen() const;
};

Graphics::Graphics(uint width, uint height, uint pixel_width, uint pixel_height){
    uint array_size = (width * pixel_width * height * pixel_height); 
    pixel_on_screen = new u_char[array_size * 3];
    uint i;
    for (i = 0; i < array_size; i++){
        pixel_on_screen[i * 3 + 0] = 0x14;
        pixel_on_screen[i * 3 + 1] = 0x88;
        pixel_on_screen[i * 3 + 2] = 0x14;
    }
}

Graphics::~Graphics(){

}

unsigned char* Graphics::get_pixel_on_screen() const{
    return this->pixel_on_screen;
}

//Updates positions and stuff
void update(double& deltaTime){

}

//all the draw calls come in here
void draw(Graphics& g){
    
}

int main(void)
{
    uint            screen_width_   = 256;
    uint            screen_height_  = 240;
    uint            pixel_width_    = 2;
    uint            pixel_height_   = 2;
    std::string     title           = "Scorpions Loop Stucker";

    GLFWwindow*     window;
    
    double          deltaTime_      = 0;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow((screen_width_ * pixel_width_), (screen_height_ * pixel_height_), title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* create graphics object */
    Graphics graphics = Graphics(screen_width_, screen_height_, pixel_width_, pixel_height_);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    uint buffer;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &buffer);
    glBindTexture(GL_TEXTURE_2D, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (screen_width_ * pixel_width_), (screen_height_ * pixel_height_), 0, GL_RGB, GL_UNSIGNED_BYTE, graphics.get_pixel_on_screen());

    static int i = 0;
    static double fps = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        const clock_t begin_time = clock();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update(deltaTime_);
        draw(graphics);

        //glDrawPixels((screen_width_ * pixel_width_), (screen_height_ * pixel_height_), GL_RGB, GL_UNSIGNED_BYTE, graphics.get_pixel_on_screen());

        /* Draw pixels on the screen */
        glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, (screen_width_ * pixel_width_), (screen_height_ * pixel_height_), GL_RGB, GL_UNSIGNED_BYTE, graphics.get_pixel_on_screen());
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        deltaTime_ = (float( clock () - begin_time ) /  CLOCKS_PER_SEC);
        
        if(i > 20){
            std::cout << fps/i << " fps" << std::endl;
            i = 0;  
            fps = 0;
        }else{
            fps = fps + (1/deltaTime_);
            i++;
        }
    }

    glfwTerminate();
    return 0;
}