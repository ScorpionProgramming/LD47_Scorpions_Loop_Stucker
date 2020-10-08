#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

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

Color::Color(){
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

Color::~Color(){
}

class Image
{
private:
    /* data */
    unsigned char* pixel_data_;
    uint width_;
    uint height_;

public:
    Image(std::string path);

    unsigned char* get_full_image();
    Color get_pixel(uint x, uint y);
    uint get_width() const;
    uint get_height() const;

    ~Image();
};

Image::Image(std::string path){
    std::ifstream file(path);

    std::string line;

    std::string check;
    uint width;
    uint height;
    std::string color;

    std::string word;

    std::vector<unsigned char> data;
    
    unsigned int number;

    std::istringstream iss;

    if(file.is_open()){
        int i = 0;
        while(!file.eof()){
            getline(file, line);

            if(line[0] != '#' && line.length() != 0){
                switch (i)
                {
                case 0:
                    check = line;
                    break;
                case 1:
                    /* split string later create an new method for it */ 
                    iss.str(line);

                    iss >> width;
                    iss >> height;
                    break;
                case 2:
                    color = line;
                    break;
                default:
                    if(check.compare("P6") == 0){
                        line.erase(line.length()-1);
                        for(unsigned int i = 0; i < line.length(); i++){
                            data.push_back(line.at(i));
                        }
                    }else if (check.compare("P3") == 0){
                        number = stoi(line);
                        //std::cout << number << "->";
                        data.push_back((unsigned char)number);
                        //std::cout << data.back() << " "; 
                    }
                    break;
                }
                i++;
            }
            //std::cout << line << std::endl;
        }

    }
    file.close();
    
    std::cout << "Check: " << check << std::endl;
    std::cout << width << " | " << height << std::endl;
    std::cout << "Color: " << color << std::endl;
    std::cout << "Data Size: " << data.size() << std::endl;

    //std::cout << data << std::endl;

    this->width_  = width;
    this->height_ = height; 

    if(data.size() != 0){
        pixel_data_ = new unsigned char[data.size()];
        
        for(unsigned int i = 0; i < data.size(); i++){
            //std::cout << (int)data.at(i) << " " << std::endl;
            pixel_data_[i] = data.at(i); 
        }
    }
}

u_char* Image::get_full_image(){
    return this->pixel_data_;
}

Color Image::get_pixel(uint x, uint y){
    int index = (x + (y * width_));
    u_char r = pixel_data_[index * 3 + 0];
    u_char g = pixel_data_[index * 3 + 1];
    u_char b = pixel_data_[index * 3 + 2];

    // std::cout << "Index: " << index << " x-> " << x << " y-> " << y << ": ";
    // std::cout << (int)r << ", "
    //           << (int)g << ", "
    //           << (int)b << ", "
    //           << std::endl;
    return Color(r, g, b);
}

uint Image::get_width() const{
    return this->width_;
}
uint Image::get_height() const{
    return this->height_;
}

Image::~Image(){
    delete pixel_data_;
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
    void draw_pixel(uint x, uint y, u_char r, u_char g, u_char b);
    void draw_line(int x1, int y1, int x2, int y2, Color c);
    void draw_quad(uint x, uint y, uint width, uint height, Color c);
    void draw_quad_filled(uint x, uint y, uint width, uint height, Color c);
    void draw_image(uint x, uint y, uint width, uint height, Image& img);
    
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
    // if(x < width_ && y < height_){
        uint coord = (x + (y * width_));
        pixel_on_screen[coord * 3 + 0] = c.get_r();
        pixel_on_screen[coord * 3 + 1] = c.get_g();
        pixel_on_screen[coord * 3 + 2] = c.get_b();
    // }
}

void Graphics::draw_pixel(uint x, uint y, u_char r, u_char g, u_char b){
    uint coord = (x + (y * width_));
    pixel_on_screen[coord * 3 + 0] = r;
    pixel_on_screen[coord * 3 + 1] = g;
    pixel_on_screen[coord * 3 + 2] = b;
}


void Graphics::draw_line(int x1, int y1, int x2, int y2, Color c){
    // c1 = b1 - a1;
    // c2 = b2 - a2; 

    // float length = sqrt(c1 * c1 + c2 * c2);

    // float cn1 = ((float)c1 / length);
    // float cn2 = ((float)c2 / length);

    // float drawV1 = a1;
    // float drawV2 = a2;

    // while((int)drawV1 != b1 || (int)drawV2 != b2){
    //     g.draw_pixel(drawV1, drawV2, red);
    //     drawV1 += cn1;
    //     drawV2 += cn2;
    // }

    
    //vector
    int cx = x2 - x1;
    int cy = y2 - y1; 

    float length = sqrt(cx * cx + cy * cy);
    // std::cout << "Length: " << length << std::endl;


    float c_norm_x = ((float)cx / length);
    float c_norm_y = ((float)cy / length);

    // std::cout << c_norm_x << ", " << c_norm_y << std::endl; 

    float draw_vec_x = x1;
    float draw_vec_y = y1;

    while((int)draw_vec_x != x2 || (int)draw_vec_y != y2){
        draw_pixel(draw_vec_x, draw_vec_y, c);
        //std::cout << "DRAW: " << draw_vec_x << ", " << draw_vec_y << std::endl; 
        draw_vec_x += c_norm_x;
        draw_vec_y += c_norm_y;
    }
    
    /* swap - perinio */
    // if(x1 >= x2){
    //     int tmp;
    //     tmp = x1; 
    //     x1 = x2; 
    //     x2 = tmp;

    //     tmp = y1; 
    //     y1 = y2; 
    //     y2 = tmp;
    // }

    // float delta_x = (x2 - x1);
    // float delta_y = (y2 - y1);

    // float x = x1; 
    // float y = y1; 
    // if(delta_x == 0){
    //     while(y != y2){
    //         this->draw_pixel((int)x, (int)y, c);
    //         if(y < y2){
    //             y++;
    //         }else if(y > y2) {
    //             y--;
    //         }
    //     }
    // }else if(delta_y == 0){
    //     while(x != x2){
    //         x++;
    //         this->draw_pixel((int)x, (int)y, c);
    //     }
    // }else{
    //     while(x != x2 && y != y2){
    //         float m = ((float)delta_y / (float)delta_x);
    //         if(m < 1){
    //             x += 1;
    //             y += m;
    //         }else if(m > 1){
    //             y += 1;
    //             x += 1/m;
    //         }else if(m == 1){
    //             y++;
    //             x++;
    //         }
    //         this->draw_pixel((int)x, (int)y, c);
    //     }
    // }
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

void Graphics::draw_image(uint x, uint y, uint width, uint height, Image& img){
    /* every vertical line */
    for(int u = 0; u < img.get_width(); u++){
        for(int v = 0; v < img.get_height(); v++){
            this->draw_pixel(x+u, y+v, img.get_pixel(u, v));
        }
    }
}

Graphics::~Graphics(){
    delete pixel_on_screen;
}

typedef struct {
    int index;
    int length;
    unsigned char entry_size;
}COLOR_MAP_SPECIFICATION;

typedef struct{
    short int x_origin;
    short int y_origin;
    short int width;
    short int height;
    char pixel_depth;
    char descriptor; 
}IMAGE_SPECIFICATION;

typedef struct{
    char id_length;
    char color_map_type;
    char image_type;
    COLOR_MAP_SPECIFICATION color_map_specification;
    IMAGE_SPECIFICATION image_specification;
}TGA_HEAD;

void load_tga(std::string path ){
    std::ifstream file(path, std::ios::binary | std::ios::in);
    TGA_HEAD head;

    if(file.is_open()){
        file >> head.id_length;
        file >> head.color_map_type;
        file >> head.image_type;
        file >> head.color_map_specification.index;
        file >> head.color_map_specification.length;
        file >> head.color_map_specification.entry_size;
        file >> head.image_specification.x_origin;
        file >> head.image_specification.y_origin;
        file >> head.image_specification.width;
        file >> head.image_specification.height;
        file >> head.image_specification.pixel_depth;
        file >> head.image_specification.descriptor;
    }
    file.close();

    std::cout << "Filename: " << path <<std::endl;  
    std::cout << (int)head.id_length << std::endl;
    std::cout << (int)head.color_map_type << std::endl;
    std::cout << (int)head.image_type << std::endl;
    std::cout << std::endl;
    std::cout << head.color_map_specification.index << std::endl; 
    std::cout << head.color_map_specification.length << std::endl;
    std::cout << head.color_map_specification.entry_size << std::endl;
    std::cout << std::endl;
    std::cout << head.image_specification.x_origin << std::endl;
    std::cout << head.image_specification.y_origin << std::endl;
    std::cout << head.image_specification.width << std::endl;
    std::cout << head.image_specification.height << std::endl;
    std::cout << head.image_specification.pixel_depth << std::endl;
    std::cout << head.image_specification.descriptor << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

//Updates positions and stuff
void update(double& deltaTime){

}

Color red       = Color(255,   0,   0);
Color green     = Color(  0, 255,   0);
Color blue      = Color(  0,   0, 255);
Color yellow    = Color(255, 255,   0);
// Image img1      = Image("Test1.ppm");
// Image img1      = Image("CheckThisFormat.ppm");
// Image img1      = Image("Test_ASCII.ppm");
Image img1      = Image("Test_raw.ppm");

//all the draw calls come in here
void draw(Graphics& g){

    // g.draw_pixel(0, 0, red);

    // g.draw_line(0, 0, 320, 200, red);
    
    // g.draw_line(320, 20, 20, 20, yellow);

    // g.draw_quad(0, 0, 50, 50, blue);

    // g.draw_quad_filled(100, 100, 60, 40, Color(0, 0, 0));
    
    //------------------------------------------------------------------------
    
    g.draw_line(100, 100, 100, 0, red);

    //------------------------------------------------------------------------

    int b1 = 0 , b2 = 0, a1 = 384, a2 = 216, c1 = 0, c2 = 0;

    g.draw_line(0, 0, 384,216, red);
    g.draw_line(0, 216, 384, 0, red);

    // //vector
    // c1 = b1 - a1;
    // c2 = b2 - a2; 

    // float length = sqrt(c1 * c1 + c2 * c2);

    // float cn1 = ((float)c1 / length);
    // float cn2 = ((float)c2 / length);

    // float drawV1 = a1;
    // float drawV2 = a2;

    // while((int)drawV1 != b1 || (int)drawV2 != b2){
    //     g.draw_pixel(drawV1, drawV2, red);
    //     drawV1 += cn1;
    //     drawV2 += cn2;
    // }

    //------------------------------------------------------------------------

    // for (unsigned int v = 0; v < 216; v++){
    //     for (unsigned int u = 0; u < 384; u++){
    //         g.draw_pixel(u, v, rand() % 256, rand()% 256, rand()% 256);
    //     }
    // }

    // for(int i = 0; i < 12; i++){
    //     for (int j = 0; j < 12; j++){
    //         g.draw_image(32*j, 32*i, 50, 50, img1);
            
    //     }
    // }
}

int main(void)
{
    //Image img = Image("CheckThisFormat.ppm");

    load_tga("/TestImage_BOT_NoRLE.tga");
    load_tga("/TestImage_BOT_RLE.tga");
    load_tga("/TestImage_TOP_NoRLE.tga");
    load_tga("/TestImage_TOP_RLE.tga");

    uint            screen_width_   = 384;//1920; //320;
    uint            screen_height_  = 216;//1080;//200;
    uint            pixel_width_    = 4;
    uint            pixel_height_   = 4;
    std::string     title_          = "Scorpions Loop Stucker";

    GLFWwindow*     window;
    
    double          deltaTime_      = 0;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow((screen_width_ * pixel_width_), (screen_height_ * pixel_height_), title_.c_str(), NULL, NULL);
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
        
        if(i > 50){
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