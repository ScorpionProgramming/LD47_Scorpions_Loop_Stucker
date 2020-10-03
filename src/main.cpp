#include <stdio.h>
#include <iostream>
#include <sstream>

#include <GLFW/glfw3.h>


class Color
{
private:
    /* data */
    u_char r;
    u_char g;
    u_char b;

public:
    Color();
    Color(u_char& r, u_char& g, u_char& b);
    Color(uint r, uint g, uint b);

    u_char get_r() const;
    u_char get_g() const;
    u_char get_b() const;
    
    ~Color();
};

Color::Color()
{
    this->r = 0x00;
    this->g = 0x00;
    this->b = 0x00;
}

Color::Color(u_char& r, u_char& g, u_char& b ){
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(uint r, uint g, uint b){
    this->r = (u_char)r;
    this->g = (u_char)g;
    this->b = (u_char)b;
}

u_char Color::get_r() const {
    return this->r;
}

u_char Color::get_g() const{
    return this->g;
}

u_char Color::get_b() const{
    return this->b;
}

Color::~Color()
{
}


class Graphics
{
private:
    /* data */
    u_char* pixel_on_screen;
    uint width_;
    uint height_;
    //check if x and y are in boundry
    void check_x(uint& x);
    void check_y(uint& y);

public:
    Graphics(uint width, uint height);
    Graphics(uint width, uint height, Color c);

    u_char* get_pixels_on_screen() const;
    void draw_pixel(uint x, uint y, Color c);
    void draw_line(int x1, int y1, int x2, int y2, Color c);
    void draw_quad(uint x, uint y, uint width, uint height, Color c);
    void draw_quad_filled(uint x, uint y, uint width, uint height, Color c);
    
    ~Graphics();
};

Graphics::Graphics(uint width, uint height){
    this->width_    = width;
    this->height_   = height;

    uint array_size = (width * height); 
    pixel_on_screen = new u_char[array_size * 3];
    uint i;
    for (i = 0; i < array_size; i++){
        pixel_on_screen[i * 3 + 0] = 0x00;
        pixel_on_screen[i * 3 + 1] = 0x00;
        pixel_on_screen[i * 3 + 2] = 0x00;
    }
}

Graphics::Graphics(uint width, uint height, Color c){
    this->width_    = width;
    this->height_   = height;

    uint array_size = (width * height); 
    pixel_on_screen = new u_char[array_size * 3];
    uint i;
    for (i = 0; i < array_size; i++){
        pixel_on_screen[i * 3 + 0] = c.get_r();
        pixel_on_screen[i * 3 + 1] = c.get_g();
        pixel_on_screen[i * 3 + 2] = c.get_b();
    }
}

u_char* Graphics::get_pixels_on_screen() const{
    return this->pixel_on_screen;
}

void Graphics::draw_pixel(uint x, uint y, Color c){
    if(x < width_ && y < height_){
        uint coord = (x + (y * width_));
        pixel_on_screen[coord * 3 + 0] = c.get_r();
        pixel_on_screen[coord * 3 + 1] = c.get_g();
        pixel_on_screen[coord * 3 + 2] = c.get_b();
    }
}

void Graphics::draw_line(int x1, int y1, int x2, int y2, Color c){
    /* swap - perinio */
    if(x1 >= x2){
        int tmp;
        tmp = x1; 
        x1 = x2; 
        x2 = tmp;

        tmp = y1; 
        y1 = y2; 
        y2 = tmp;
    }

    float delta_x = (x2 - x1);
    float delta_y = (y2 - y1);

    float x = x1; 
    float y = y1; 
    if(delta_x == 0){
        while(y != y2){
            this->draw_pixel((int)x, (int)y, c);
            if(y < y2){
                y++;
            }else if(y > y2) {
                y--;
            }
        }
    }else if(delta_y == 0){
        while(x != x2){
            x++;
            this->draw_pixel((int)x, (int)y, c);
        }
    }else{
        while(x != x2 && y != y2){
            float m = ((float)delta_y / (float)delta_x);
            if(m < 1){
                x += 1;
                y += m;
            }else if(m > 1){
                y += 1;
                x += 1/m;
            }else if(m == 1){
                y++;
                x++;
            }
            this->draw_pixel((int)x, (int)y, c);
        }
    }
        
}

void Graphics::draw_quad(uint x, uint y, uint width, uint height, Color c){
    draw_line(x , y, x + width, y, c);
    draw_line(x + width, y, x + width, y + height, c);
    draw_line(x + width, y + height, x, y + height, c);
    draw_line(x, y + height, x, y, c);
}

void Graphics::draw_quad_filled(uint x, uint y, uint width, uint height, Color c){
    for(uint i = 0; i < height; i++){
        draw_line(x, y + i, x + width, y + i, c);
    }
}

Graphics::~Graphics(){

}


//Updates positions and stuff
void update(double& deltaTime){

}

//all the draw calls come in here
void draw(Graphics& g){
    Color red       = Color(255,   0,   0);
    Color green     = Color(  0, 255,   0);
    Color blue      = Color(  0,   0, 255);
    Color yellow    = Color(255, 255,   0);
    
    //g.draw_pixel(0, 0, red);
    //g.draw_line(0, 0, 320, 200, red);
    
    g.draw_line(320, 20, 20, 20, yellow);

    g.draw_quad(0, 0, 50, 50, blue);

    g.draw_quad_filled(100, 100, 60, 40, Color(0, 0, 0));
}

int main(void)
{
    uint            screen_width_   = 320;
    uint            screen_height_  = 200;
    uint            pixel_width_    = 4;
    uint            pixel_height_   = 4;
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
    Graphics graphics = Graphics(screen_width_, screen_height_, Color(0x00, 0x88, 0x88));

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    uint buffer;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &buffer);
    glBindTexture(GL_TEXTURE_2D, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width_, screen_height_, 0, GL_RGB, GL_UNSIGNED_BYTE, graphics.get_pixels_on_screen());

    static int i = 0;
    static double fps = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        const clock_t begin_time = clock();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        update(deltaTime_);
        draw(graphics);

        /* For debug is quite nice because it works! */
        //glDrawPixels(screen_width_ , screen_height_, GL_RGB, GL_UNSIGNED_BYTE, graphics.get_pixels_on_screen());

        /* Draw pixels on the screen */
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screen_width_ , screen_height_, GL_RGB, GL_UNSIGNED_BYTE, graphics.get_pixels_on_screen());
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
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