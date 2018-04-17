//
//  opengl_finalwork.cpp
//  XcodeGlutDemo
//
//  Created by woodenwhite on 12/01/2018.
//  Copyright © 2018 woodenwhite. All rights reserved.
//

////
////  main.cpp
////  XcodeGlutDemo
////
////  Created by woodenwhite on 12/01/2018.
////  Copyright © 2017 woodenwhite. All rights reserved.
////
//
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#include"particle.hpp"
#include<iostream>
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
const double PI = acos(-1.0);
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat posx = 0.0f;
static GLfloat posz = 0.0f;
static GLfloat dez = 1.0f;
bool lighton1 = true;
bool lighton2 = true;
bool lighton3 = true;
bool snowon = true;
//定义两个纹理对象编号
GLuint texName;
GLuint texWall;
GLuint texDoor;
GLuint texGround;
GLuint texKakaxi;
GLuint texMotto;
GLuint texWall0;
GLuint texPaper;
GLuint texKid;
double aerfa = 0;
CParticle Snow;
GLuint texSnow;
float h,m,s;
#define BMP_Header_Length 54  //图像数据在内存块中的偏移量
float x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec0;
int r,g,b; //设定粒子的属性值
bool InitSnow();

// 函数power_of_two用于判断一个整数是不是2的整数次幂
int power_of_two(int n)
{
    if( n <= 0 )
        return 0;
    return (n & (n-1)) == 0;
}

const double pi = acos(-1.0);
//绘制大长方形中的每一个小方格
void drawlittlegrid(int type,double sx, double sy, double sz, double ex, double ey, double ez) {
    glBegin(GL_POLYGON);
    
    if(type == 1) { // 确定颜色
        glColor3f(147.0 / 256, 224.0/ 256  , 255.0 / 256);
    }else{
        glColor3f(1.0, 1.0, 1.0);
    }
    //    if(sx == ex && ex > 0){
    //        glColor3f(1.0, 1.0, 1.0);
    //    }
    if(sx == ex){ //与YOZ平面平行
        if(sx > 0)
            glNormal3f(-1.0, 0.0, 0.0);
        else
            glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(sx, sy, sz);
        glVertex3f(sx, ey, sz);
        glVertex3f(sx, ey, ez);
        glVertex3f(sx, sy, ez);
    }else if(sy == ey) {//与XOZ平面平行
        if(sy > 0)
            glNormal3f(0.0, -1.0, 0.0);
        else
            glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(sx, sy, sz);
        glVertex3f(sx, sy, ez);
        glVertex3f(ex, sy, ez);
        glVertex3f(ex, sy, sz);
    }else if(sz == ez) { //与XOY平面平行
        if(sz > 0)
            glNormal3f(0.0, 0.0, -1.0);
        else
            glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(sx, sy, sz);
        glVertex3f(sx, ey, sz);
        glVertex3f(ex, ey, sz);
        glVertex3f(ex, sy, sz);
    }
    glEnd();
}

//绘制每个大长方形
void drawgrid(double sx, double sy, double sz, double ex, double ey, double  ez){
    double d = 20; //设置小方块边长
    int ty = 1;
    int TY = 1;
    
    if(sx == ex){  //与YOZ平面平行
        for(double j = sy; j < ey; j += d) {
            TY = !TY;
            ty = TY;
            if(j + d <= ey)
                for(double k = sz; k < ez; k += d) {
                    if(k + d <= ez){ //特判最后一个边长
                        drawlittlegrid(ty, sx, j, k, ex, j + d, k + d);
                    }
                    else{
                        drawlittlegrid(ty,sx,j,k,ex,j + d, ez);
                    }
                    ty = !ty;
                }
            else{
                for(double k = sz; k < ez; k += d) {
                    if(k + d <= ez){
                        drawlittlegrid(ty, sx, j, k, ex, ey, k + d);
                    }else{
                        drawlittlegrid(ty,sx,j,k,ex,ey, ez);
                    }
                    ty = !ty;
                }
            }
        }
    }else if(sz == ez){   //与XOZ平面平行
        for(double j = sx; j < ex; j += d) {
            TY = !TY;
            ty = TY;
            if(j + d <= ex){
                for(double k = sy; k < ey; k += d) {
                    if(k + d <= ey) {
                        drawlittlegrid(ty, j, k, sz, j + d, k + d, ez);
                    }else{
                        drawlittlegrid(ty, j, k, sz, j + d, ey, ez);
                    }
                    ty = !ty;
                }
            }else{
                for(double k = sy; k < ey; k += d) {
                    if(k + d <= ey) {
                        drawlittlegrid(ty, j, k, sz, ex, k + d, ez);
                    }else{
                        drawlittlegrid(ty, j, k, sz, ex, ey, ez);
                    }
                    ty = !ty;
                }
            }
        }
        
    }else if(sy == ey) {  //与XOY平面平行
        for(double j = sx; j < ex; j += d){
            TY = !TY;
            ty = TY;
            if(j + d <= ex){
                for(double k = sz; k < ez; k += d) {
                    if(k + d <= ez)
                        drawlittlegrid(ty, j, sy,k, j + d, ey, k + d);
                    else
                        drawlittlegrid(ty, j, sy,k, j + d, ey, ez);
                    ty = !ty;
                }
            }else{
                for(double k = sz; k < ez; k += d) {
                    if(k + d <= ez)
                        drawlittlegrid(ty, j, sy,k, ex, ey, k + d);
                    else
                        drawlittlegrid(ty, j, sy,k, ex , ey, ez);
                    ty = !ty;
                }
                
            }
        }
    }
}

void reshape (int w, int hh)
{
    
    //    glutPostRedisplay();
    glViewport (0, 0, (GLsizei) w, (GLsizei) hh);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) hh, 1.0, 1425.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    
}
//
//void init(){
//    glMatrixMode (GL_PROJECTION);
//    glLoadIdentity ();
//    glViewport (0, 0, 500, 500);
//    gluPerspective(45.0, 1, 1.0, 1425.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt (0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//}

void setuplight(){
    //材质反光性设置
    GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 0.0f};  // 环境强度
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  // 散射强度
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // 镜面强度
    
    // 点光源, GL_POSITION属性的最后一个参数为1
    GLfloat position[] = {0.0 , 0.0, 80.0, 1.0f};
    //    position[2] *= dez;
    //    glLightModelfv(GL_LIGHT_MODEL_AMBIENT , ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.003);
    glLightf(GL_LIGHT0,  GL_CONSTANT_ATTENUATION, 1.0);
    //    glLightf(GL_LIGHT0,  GL_QUADRATIC_ATTENUATION, 0.0005);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    
    
    
    GLfloat ambient1[] = {0.0f, 0.0f, 0.0f, 0.0f};  // 环境强度
    GLfloat diffuse1[] = {0.8f, 0.8f, 0.8f, 1.0f};  // 散射强度
    GLfloat specular1[] = {1.0f, 1.0f, 1.0f, 1.0f}; // 镜面强度
    
    // 点光源, GL_POSITION属性的最后一个参数为1
    GLfloat position1[] = {-90.0 , 40.0 , -140.0 , 1.0f};
    //    for(int i = 0; i < 3; i++) {
    //        position1[i] *= dez;
    //    }
    //    glLightModelfv(GL_LIGHT_MODEL_AMBIENT , ambient);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.02);
    glLightf(GL_LIGHT1,  GL_CONSTANT_ATTENUATION, 1.0);
    //    glLightf(GL_LIGHT0,  GL_QUADRATIC_ATTENUATION, 0.0005);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
    
    
    
    //未设置完成的LIGHT2和聚光灯光源
    
    GLfloat ambient2[] = {0.0f, 0.0f, 0.0f, 0.0f};  // 环境强度
    GLfloat diffuse2[] = {1.0f, 1.0f, 1.0f, 1.0f};  // 散射强度
    GLfloat specular2[] = {1.0f, 1.0f, 1.0f,10.0f}; // 镜面强度
    GLfloat position2[] = {90.0f, 40.0f, -140.0f, 1.0f};
    //    for(int i = 0; i < 3; i++) {
    //        position2[i] *= dez;
    //    }
    glLightfv(GL_LIGHT2, GL_POSITION, position2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.02);
    glLightf(GL_LIGHT2,  GL_CONSTANT_ATTENUATION, 1.0);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
    
    
    glEnable(GL_LIGHTING);   //开关:使用光
    if(lighton1 == true){
        glEnable(GL_LIGHT0); //开启LIGHT0
    }else{
        glDisable(GL_LIGHT0);
    }
    if (lighton2 == true) {
        glEnable(GL_LIGHT1);
    }else{
        glDisable(GL_LIGHT1);
    }
    if(lighton3 == true){
        glEnable(GL_LIGHT2);
    }else{
        glDisable(GL_LIGHT2);
    }
    glEnable( GL_COLOR_MATERIAL ); //设置物体的COLORMATERIAL模式
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    GLfloat wall_mat_ambient[] = { 1.0, 1.0, 1.0, 1.000000};
    GLfloat wall_mat_diffuse[] = { 0.5, 0.5, 0.5, 1.000000};
    GLfloat wall_mat_specular[] ={ 0.2, 0.2, 0.2, 1.000000};
    //        GLfloat wall_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat wall_mat_shininess =  9.846150;
    glMaterialfv(GL_FRONT, GL_AMBIENT, wall_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wall_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wall_mat_specular);
    //glMaterialfv(GL_FRONT, GL_EMISSION, wall_mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, wall_mat_shininess);
}

void debugcircle(){
    glBegin(GL_POLYGON);
    for(int i = 0; i < 360; i++) {
        double tmp = 1.0 * i / 360;
        glColor3f(1.0, 0, 0);
        glVertex3f(20 * cos(tmp* pi * 2), 20 * sin(tmp * 2 * pi),135);
    }
    glEnd();
}


GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLuint last_texture_ID=0, texture_ID = 0;
    
    // 打开文件，如果失败，返回
    FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 ){
        cout << "12321" << endl;
        return 0;
    }
    
    // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);
    
    // 计算每行像素所占字节数，并根据此数据计算总像素字节数
    {
        GLint line_bytes = width * 3;
        while( line_bytes % 4 != 0 )
            ++line_bytes;
        total_bytes = line_bytes * height;
    }
    
    // 根据总像素字节数分配内存
    pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 )
    {
        fclose(pFile);
        return 0;
    }
    
    // 读取像素数据
    if( fread(pixels, total_bytes, 1, pFile) <= 0 )
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    
    // 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
    // 若图像宽高超过了OpenGL规定的最大值，也缩放
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if( !power_of_two(width)
           || !power_of_two(height)
           || width > max
           || height > max )
        {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;
            
            // 计算每行需要的字节数和总字节数
            new_line_bytes = new_width * 3;
            while( new_line_bytes % 4 != 0 )
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;
            
            // 分配内存
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if( new_pixels == 0 )
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }
            
            // 进行像素缩放
            gluScaleImage(GL_RGB,
                          width, height, GL_UNSIGNED_BYTE, pixels,
                          new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);
            
            // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }
    
    // 分配一个新的纹理编号
    glGenTextures(1, &texture_ID);
    if( texture_ID == 0 )
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    
    // 绑定新的纹理，载入纹理并设置纹理参数
    // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
    GLint lastTextureID=last_texture_ID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
    free(pixels);
    return texture_ID;
}

void drawcar(){
    glPushMatrix();
    //车下半部
    glTranslated(posx, 0, posz);
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1.0);
    //    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glVertex3f(-40.0f,-30.0f,-50.0f);
    glVertex3f(40.0f,-30.0f,-50.0f);
    glColor3f(255.0 / 256, 185.0 / 256, 15.0 / 256);
    glColor3f(0.2, 0.5, 0.8);
    glVertex3f(40.0f,-45.0f,-50.0f);
    glVertex3f(-40.0f,-45.0f,-50.0f);
    glEnd();
    glBegin(GL_POLYGON);
    //    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glNormal3f(1.0, 0, 0);
    glVertex3f(40.0f,-30.0f,-80.0f);
    glVertex3f(40.0f,-30.0f,-50.0f);
    glColor3f(30.0 / 256, 144.0 / 256, 255.0 / 256);
    glVertex3f(40.0f,-45.0f,-50.0f);
    glVertex3f(40.0f,-45.0f,-80.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0, 0);
    //    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glVertex3f(-40.0f,-30.0f,-80.0f);
    glVertex3f(-40.0f,-30.0f,-50.0f);
    //    glColor3f(30.0 / 256, 144.0 / 256, 255.0 / 256);
    glVertex3f(-40.0f,-45.0f,-50.0f);
    glVertex3f(-40.0f,-45.0f,-80.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    //    glColor3f(0.1, 0.2, 0.8);
    //    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glVertex3f(-40.0f,-30.0f,-80.0f);
    glVertex3f(40.0f,-30.0f,-80.0f);
    //    glColor3f(1, 0.5, 0.8);
    glColor3f(30.0 / 256, 144.0 / 256, 255.0 / 256);
    glVertex3f(40.0f,-45.0f,-80.0f);
    glVertex3f(-40.0f,-45.0f,-80.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3f(0, 1.0, 0);
    //    glColor3f(0.5, 0, 1);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    //    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glVertex3f(-40.0f,-30.0f,-50.0f);
    glVertex3f(-40.0f,-30.0f,-80.0f);
    glVertex3f(40.0f,-30.0f,-80.0f);
    glVertex3f(40.0f,-30.0f,-50.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3f(0, 1.0, 0);
    //    glColor3f(0.8, 0.5, 0.2);
    //    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    //    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glColor3f(30.0 / 256, 144.0 / 256, 255.0 / 256);
    glVertex3f(-40.0f,-45.0f,-50.0f);
    glVertex3f(-40.0f,-45.0f,-80.0f);
    glVertex3f(40.0f,-45.0f,-80.0f);
    glVertex3f(40.0f,-45.0f,-50.0f);
    glEnd();
    //车上半部
    glBegin(GL_POLYGON);
    //    glColor3f(0.8, 0.5, 1);
    glNormal3f(0, 0, 1);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glVertex3f(-20.0f,-30.0f,-50.0f);
    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glVertex3f(-10.0f,-20.0f,-50.0f);
    glVertex3f(20.0f,-20.0f,-50.0f);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glVertex3f(25.0f,-30.0f,-50.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1.0);
    //    glColor3f(0.5, 0.8, 1);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glVertex3f(-20.0f,-30.0f,-80.0f);
    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glVertex3f(-10.0f,-20.0f,-80.0f);
    glVertex3f(20.0f,-20.0f,-80.0f);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glVertex3f(25.0f,-30.0f,-80.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3f(0, 1, 0);
    //    glColor3f(0.4, 0.2, 1);
    //    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glVertex3f(-10.0f,-20.0f,-50.0f);
    glVertex3f(-10.0f,-20.0f,-80.0f);
    glVertex3f(20.0f,-20.0f,-80.0f);
    glVertex3f(20.0f,-20.0f,-50.0f);
    glEnd();
    glBegin(GL_POLYGON);
    //    glColor3f(0.5, 0.8, 0);
    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glVertex3f(-10.0f,-20.0f,-50.0f);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glVertex3f(-20.0f,-30.0f,-50.0f);
    glVertex3f(-20.0f,-30.0f,-80.0f);
    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glVertex3f(-10.0f,-20.0f,-80.0f);
    glEnd();
    glBegin(GL_POLYGON);
    //    glColor3f(0.9, 0.5, 0.5);
    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    
    glVertex3f(20.0f,-20.0f,-50.0f);
    glVertex3f(20.0f,-20.0f,-80.0f);
    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    glVertex3f(25.0f,-30.0f,-80.0f);
    glVertex3f(25.0f,-30.0f,-50.0f);
    glEnd();
    glBegin(GL_POLYGON);
    //    glColor3f(0.3, 0.8, 1);
    //    glColor3f(254.0 / 256, 67.0 / 256, 101.0 / 256);
    //    glColor3f(252.0 / 256, 157.0 / 256, 154.0 / 256);
    glNormal3f(0, 1, 0);
    glVertex3f(-30.0f,-45.0f,-50.0f);
    glVertex3f(-30.0f,-45.0f,-80.0f);
    glVertex3f(30.0f,-45.0f,-80.0f);
    glVertex3f(30.0f,-45.0f,-50.0f);
    glEnd();
    //车轮
    glPushMatrix();
    glColor3f(105.0 / 256, 105.0 / 256, 105.0 / 256);
    glTranslated(-20.0f,-45.0f,-50.0f);
    glutSolidTorus(2,5,5,100);
    glTranslated(0.0f,0.0f,-30.0f);
    glutSolidTorus(2,5,5,100);
    glTranslated(45.0f,0.0f,0.0f);
    glutSolidTorus(2,5,5,100);
    glTranslated(0.0f,0.0f,30.0f);
    glutSolidTorus(2,5,5,100);
    glPopMatrix();
    glPopMatrix();
}

bool InitSnow()
{
    /** 用户自定义的初始化过程 */
    glClearDepth(1.0f);//指定深度缓冲区的清除值(即将深度缓冲区里的值设置为1)
    glDepthFunc(GL_LEQUAL);//指定用于深度缓冲比较值(即新进像素深度值与原来的1比较，<=则通过，否则丢弃)
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    glEnable(GL_TEXTURE_2D);             /**< 开启纹理映射 */
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);    /**< 设置混合因子获得半透明效果 */
    glEnable(GL_BLEND);                  /**< 启用混和 */
    
    /**  载入纹理 */
    
    /** 创建500个粒子 */
    Snow.Create(1000);
    for (int i=0; i < Snow.GetNumOfParticle(); ++i)
    {
        ///初始化颜色（白色）
        r = 255;
        g = 255;
        b = 255;
        Snow.SetColor(i,r,g,b);
        
        ///初始化坐标
        x = 0.3f * (rand() % 50) - 4.0f;
        y = 3.5 + 0.1f * (rand() % 2);
        if((int)x % 2 == 0)
            z = rand()%5;
        else
            x = -rand()%5;
        Snow.SetPosition(i,x,y,z);
        
        ///初始化速度
        vx = 0.00001 * (rand()%100);
        vy = 0.0000002 * (rand()%28000);
        vz = 0.00000002 * (rand() % 28000);
        Snow.SetVelocity(i,vx,vy,vz);
        
        ///初始化加速度
        ax = 0;
        ay = 0.000005f;
        az = 0;
        Snow.SetAcceleration(i,ax,ay,az);
        
        ///初始化生命周期
        lifetime = 100000;
        Snow.SetLifeTime(i,lifetime);
        
        ///消失速度
        dec0 = 0.000005 * (rand()%50);
        Snow.SetDec(i,dec0);
        
        ///初始化大小
        Snow.SetSize(i,0.03f);
    }
    return true;
}


/** 更新粒子 */
void UpdateSnow()
{
    /** 更新位置 */
    x += (vx * 5);
    y -= vy;
    z += vz * 2;
    
    
    /** 更新速度 */
    vy += ay;
    
    /** 更新生存时间 */
    lifetime -= dec0;
    
    if(x > 10)
        x = -10;
    
    /** 如果粒子消失或生命结束 */
    if(snowon = true){
        if (y <= -1 || lifetime <= 0)
        {
            /** 初始化位置 */
            x = 0.3f * (rand()%50) - 4.0f;
            y = 3.5 + 0.1f * (rand()%2);
            if((int)x%2 == 0)
                z = rand()%5;
            else
                z = -rand()%5;
            
            /** 初始化速度 */
            vx = (float)(0.00001 * (rand()%100));
            vy = (float)(0.0000002 * (rand()%28000));
            vz =  (float)(0.00000002 * (rand()%28000));
            
            /** 初始化加速度 */
            ax = 0;
            ay = 0.000005f;
            az = 0;
            lifetime = 150000;
            dec0 = 0.0005*(rand()%50);
        }
    }
}

/** 绘制粒子 */
void DrawParticle()
{
    glDisable(GL_LIGHTING);
    /** 绑定纹理 */
    glBindTexture(GL_TEXTURE_2D,texSnow);
    
    for(int i = 0; i<Snow.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        Snow.GetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec0);
        glLoadIdentity();
        //        glPushMatrix();
        glTranslatef(0.0f,0.0f,-6.0f);
        glColor4ub(r,g,b,255);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);  //绕x轴旋转
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);
        glNormal3f(0.0f,0.0f,1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glTranslatef(x, y, z);
        glColor3f(1.0, 1.0, 1.0);
        glutSolidSphere(size / 2.0, 20, 20);
        //        glPopMatrix();
        
        /** 更新粒子属性 */
        UpdateSnow();
        Snow.SetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec0);
    }
    glEnable(GL_LIGHTING);
    glutPostRedisplay();//发送渲染请求
}



void drawSnowMan() {
    
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Draw Body
    glTranslatef(0.0f ,0.75f, 0.0f);
    glutSolidSphere(0.75f,20,20);
    
    glPushMatrix();
    glScaled(1.5, 1.5, 1.5);
    // Draw Head
    glTranslatef(0.0f, 2.0 / 3, 0.0f);
    glutSolidSphere(0.25f,20,20);
    
    // Draw Eyes
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f,10,10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f,10,10);
    glPopMatrix();
    
    // Draw Nose
    glColor3f(1.0f, 0.5f , 0.5f);
    glRotatef(0.0f,1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f,0.5f,10,2);
    glPopMatrix();
}

//void drawkid(){
//
//}
int num = 33;//一共多少帧
int col = 4;//一行有多少帧
static GLfloat xx1 = -20, xx2 = 20;
static GLfloat yy1 = -10, yy2 = 10;
float ii = 0, jj = 0;
void drawKid()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texKid);
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    
    const GLfloat x = 1.0 / col, y = 1.0 / 4;
    GLfloat point[4][3] = { { 99, 0,0 },{ 99,1 ,1 },{ 99,2,1},{ 99,2,2 } };
    point[0][1] = xx1; point[0][2] = yy2;
    point[1][1] = xx1; point[1][2] = yy1;
    point[2][1] = xx2; point[2][2] = yy1;
    point[3][1] = xx2; point[3][2] = yy2;
    GLfloat dir[4][2] = { { (int)jj*x,1 - ((int)ii + 1)*y },{ ((int)jj + 1)*x ,1 - ((int)ii + 1)*y },{ ((int)jj + 1)*x ,1 - (int)ii*y },{ (int)jj*x,1 - (int)ii*y } };
    glBegin(GL_QUADS);
    glColor4f(1, 1, 1, 0);
    glNormal3f(-1.0, 0, 0);
    for(int i = 0; i < 4; i++) {
        if(i == 0 || i == 3)
            dir[i][0] += 0.1;
        else
            dir[i][0] += 0.05;
    }
    for (int k = 0; k < 4; k++) {
        glTexCoord2fv(dir[k]);
        glVertex3fv(point[k]);
    }
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void kidmove(){
    if(ii == 2){
        if(yy2 - 0.5 > -130){
            yy1 -= 0.5;
            yy2 -= 0.5;
        }
    }
    else if(ii == 3){
        if(xx2 + 0.5 < 60){
            xx1 += 0.5;
            xx2 += 0.5;
        }
    }
    else if(ii == 0){
        if(xx1 - 0.5 > -50){
            xx1 -= 0.5;
            xx2 -= 0.5;
        }
    }
    else if(ii == 1){
        if(yy1 + 0.5 < 20){
            yy1 += 0.5;
            yy2 += 0.5;
        }
    }
}
void init_clock(){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    h=timeinfo->tm_hour;
    m=timeinfo->tm_min;
    s=timeinfo->tm_sec;
}
void clockcheck(int id){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    h=timeinfo->tm_hour;
    m=timeinfo->tm_min;
    s=timeinfo->tm_sec;
    
    glutTimerFunc(33,clockcheck,1);
}

void drawclock(){
    
    int i;
    float R,TR,h_Angle,m_Angle,s_Angle,count,h_Length,m_Length,s_Length;
    R=0.5;    TR=R-0.05;   // glClearColor(1.0,1.0,1.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);  //用白色清空颜色缓冲区
    glColor3f(0.0,0.0,0.0);
    glLineWidth(7);
    glBegin(GL_LINE_LOOP);
    for (i=0; i<100; i++)
    {
        glVertex2f(R*cos(2*PI/100*i),R*sin(2*PI/100*i));
    }
    
    glEnd();
    glLineWidth(3);
    glColor3f(0.0,0.0,0.0);      //将颜色设置为黑色
    
    for (i=0; i<12; i++)
    {
        if (i == 0)
        {
            glLineWidth(7);
            glColor3f(1.0,0.0,0.0);  //当画0点钟的刻度时，颜色设置为红色
        }
        else
        {
            glLineWidth(3);
            glColor3f(0.0,0.0,0.0);  //其他的刻度，颜色都为黑色
        }
        
        glBegin(GL_LINES);
        glVertex2f(TR*sin(2*PI/12*i),TR*cos(2*PI/12*i));
        glVertex2f(R*sin(2*PI/12*i),R*cos(2*PI/12*i));
        glEnd();
    }
    glLineWidth(1);
    
    h_Length=0.2;
    m_Length=0.35;
    s_Length=0.4;
    count=60;
    s_Angle=s/count;
    count*=60;
    m_Angle=(m*60+s)/count; count*=12;
    h_Angle=(h*60*60+m*60+s)/count;
    glLineWidth(1);
    
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(s_Length*sin(2*PI*s_Angle),s_Length*cos(2*PI*s_Angle));
    glEnd();
    
    glLineWidth(5);
    
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(h_Length*sin(2*PI*h_Angle),h_Length*cos(2*PI*h_Angle));
    glEnd();
    
    glLineWidth(3);
    
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(m_Length*sin(2*PI*m_Angle),m_Length*cos(2*PI*m_Angle));
    glEnd();
    
    glLineWidth(1);
    
    glBegin(GL_POLYGON);
    for (i=0; i<100; i++)
    {
        glVertex2f(0.03*cos(2*PI/100*i),0.03*sin(2*PI/100*i));
    }
    glEnd();
}

void Display(void)

{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat buffer[16];
    glGetFloatv( GL_PROJECTION_MATRIX, buffer); // 得到投影矩阵
    jj = (jj + 0.1);
    if(jj == 4) {
        jj = 0;
    }
    //        glCullFace(GL_FRONT);
    glPushMatrix();{
        //        cout << xRot << " " << yRot << endl;
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);  //绕x轴旋转
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);  //绕y轴旋转
        //        glScaled(dez, dez, dez);
        
        glPushMatrix();
        glTranslatef(0, 25, -149);
        glScaled(60, 60, 60);
        drawclock();
        glPopMatrix();
        
        kidmove();
        setuplight();
        glPushMatrix();
        glDisable(GL_BLEND);
        glPopMatrix();
        glColor3f(1.0, 0.5, 0.5);
        glPushMatrix();
        glTranslatef(-40, -50, -60);
        glScaled(20, 20, 20);
        drawSnowMan();
        glPopMatrix() ;
        drawKid();
        //        glutSolidSphere(5, 100, 100);
        // 绘制立面
        //                glBindTexture(GL_TEXTURE_2D, texWall);
        //                glBegin(GL_QUADS);
        //                glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, 0.0f);
        //                glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, -3.0f, 5.0f);
        //                glTexCoord2f(2.0f, 1.0f); glVertex3f(6.0f, -3.0f, 5.0f);
        //                glTexCoord2f(2.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
        //                glEnd();
        ////
        //                //绘制另外一个立面
        //                glBegin(GL_QUADS);
        //                glTexCoord2f(2.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
        //                glTexCoord2f(0.0f, 0.0f); glVertex3f(6.0f, 9.0f, 0.0f);
        //                glTexCoord2f(0.0f, 1.0f); glVertex3f(6.0f, 9.0f, 5.0f);
        //                glTexCoord2f(2.0f, 1.0f); glVertex3f(6.0f, -3.0f, 5.0f);
        //                glEnd();
        glPushMatrix();
        glTranslatef(30, 0, 0);
        drawcar();
        glPopMatrix();
        
        //        drawgrid(-100, 60, -150, 100, 60, 100);
        drawgrid(-100, -50, -150, -100, 60, 100);
        //        glEnable(GL_TEXTURE_2D);    // 启用纹理
        // 绘制底面以及纹理
        //        glBlendFunc(GL_ONE, GL_ONE);
        //        glColor3f(1, 1, 1);
        //        glBindTexture(GL_TEXTURE_2D, texPaper);
        //        glColorMaterial( GL_FRONT_AND_BACK, GL_DIFFUSE );
        //        glBegin(GL_QUADS);
        //        glNormal3f(1.0, 0.0, 0.0);
        //        glTexCoord2f(0.0f, 0.0f); glVertex3f(-99.5f, -50.0f, -150.0f);
        //        glTexCoord2f(0.0f, 1.0f); glVertex3f(-99.5f, 60.0f, -150.0f);
        //        glTexCoord2f(2.0f, 1.0f); glVertex3f(-99.5f, 60.0f, 100.0f);
        //        glTexCoord2f(2.0f, 0.0f); glVertex3f(-99.5f, -50.0f, 100.0f);
        //        glEnd();
        drawgrid(-100, -50, -150, 100, -50, 100);
        drawgrid(100,-50,-150,100,60,100);
        drawgrid(-100, -50, 100, 100, 60, 100);
        drawgrid(-100, -50, -150, 100, 60, -150);
        
        
        
        // 绘制底面以及纹理
        //        glBlendFunc(GL_ONE, GL_ONE);
        glEnable(GL_TEXTURE_2D);    // 启用纹理
        //        drawkid();
        //        glColor4f(1, 1, 1, 0);
        //        glBindTexture(GL_TEXTURE_2D, texWall);
        //        glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
        //        glBegin(GL_QUADS);
        //        glNormal3f(0.0, 0.0, 1.0);
        //        glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, -0.0f, -149.0f);
        //        glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 60.0f, -149.0f);
        //        glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 60.0f, -149.0f);
        //        glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, -0.0f, -149.0f);
        //        glEnd();
        //
        //        glBindTexture(GL_TEXTURE_2D, texName);
        //        glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
        //        glBegin(GL_QUADS);
        //        glColor3f(1, 1, 1);
        //        glNormal3f(0.0, 0.0, -1.0);
        //        glTexCoord2f(0.0f, 0.0f); glVertex3f(75.0f, 0.0f, 99.0f);
        //        glTexCoord2f(0.0f, 1.0f); glVertex3f(75.0f, 50.0f, 99.0f);
        //        glTexCoord2f(1.0f, 1.0f); glVertex3f(-75.0f, 50.0f, 99.0f);
        //        glTexCoord2f(1.0f, 0.0f); glVertex3f(-75.0f, 0.0f, 99.0f);
        //        glEnd();
        
        glColor4f(1, 1, 1,0);
        glBindTexture(GL_TEXTURE_2D, texKakaxi);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
        glBegin(GL_QUADS);
        glNormal3f(-1.0, 0.0, 0.0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(99.5f , -50.0f, 40.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(99.5f , 40.0f, 40.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(99.5f, 40.0f, 80.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(99.5f, -50.0f, 80.0f);
        glEnd();
        
        glColor4f(1, 1, 1, 0);
        glBindTexture(GL_TEXTURE_2D, texDoor);
        glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
        glBegin(GL_QUADS);
        glNormal3f(-1.0, 0.0, 0.0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(99.0 - 50 * sin(aerfa), -50.0f, 80 - 40 * cos(aerfa));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(99.0 - 50 * sin(aerfa), 40.0f, 80 - 40 * cos(aerfa));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(99.0f, 40.0f, 80.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(99.0f, -50.0f, 80.0f);
        glEnd();
        
        glColor4f(1, 1, 1, 0);
        glBindTexture(GL_TEXTURE_2D, texGround);
        glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
        glBegin(GL_QUADS);
        glNormal3f(0.0, 1.0, 0.0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(99.0f, -49.0f, 40.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(99.0f, -49.0f, 80.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(65.0f, -49.0f, 80.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(65.0f, -49.0f, 40.0f);
        glEnd();
        
        //        glColor4f(1, 1, 1, 0);
        //        glBindTexture(GL_TEXTURE_2D, texMotto);
        //        glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
        //        glBegin(GL_QUADS);
        //        glNormal3f(1.0, 0.0, 0.0);
        //        glTexCoord2f(0.2f, 0.0f); glVertex3f(-99.0 , -40.0f, -30.0f);
        //        glTexCoord2f(0.2f, 1.0f); glVertex3f(-99.0 , 60.0f, -30.0f);
        //        glTexCoord2f(1.0f, 1.0f); glVertex3f(-99.0f, 60.0f, -90.0f);
        //        glTexCoord2f(1.0f, 0.0f); glVertex3f(-99.0f, -40.0f, -90.0f);
        //        glEnd();
        
        
        
        //        glColor3f(1, 1, 1);
        //        glBindTexture(GL_TEXTURE_2D, texWall0);
        //        glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
        //        glBegin(GL_QUADS);
        //        glNormal3f(-1.0, 0.0, 0.0);
        //        glTexCoord2f(0.0f, 0.0f); glVertex3f(99.9f, -50.0f, -150);
        //        glTexCoord2f(0.0f, 1.0f); glVertex3f(99.9f, 40.0f, -150);
        //        glTexCoord2f(1.0f, 1.0f); glVertex3f(99.9f, 40.0f, 100.0f);
        //        glTexCoord2f(1.0f, 0.0f); glVertex3f(99.9f, -50.0f, 100.0f);
        //        glEnd();
        
        //        glDisable(GL_BLEND);
        
        glDisable(GL_TEXTURE_2D);
        
        DrawParticle();
        
    }
    glPopMatrix();
    glutSwapBuffers();   //交换缓冲
}

void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)      xRot-= 5.0f;
    if(key == GLUT_KEY_DOWN)    xRot += 5.0f;
    if(key == GLUT_KEY_LEFT)    yRot -= 5.0f;
    if(key == GLUT_KEY_RIGHT)   yRot += 5.0f;
    glutPostRedisplay(); // 刷新窗口
}

void KeyBoards(unsigned char key,int x,int y)
{
    switch (key)
    {
        case 'e':
            if(aerfa + 0.1 < 2 * pi / 3)
                aerfa+= 0.1;
            glutPostRedisplay();
            break;
        case 'q':
            if(aerfa - 0.1  > 0)
                aerfa -= 0.1;
            else
                aerfa = 0;
            glutPostRedisplay();
            break;
        case 'a':
            if(posz < -20 || posz > 30){
                if(posx > -70) posx -= 3;
            }else{
                if(posx > -10){
                    posx -= 3;
                }
            }
            glutPostRedisplay();
            break;
        case 'd':
            if(posx < 20)
                posx += 3;
            glutPostRedisplay();
            break;
        case 'w':
            if(posz < 40 && posz > -30 && (posx < -10)) ;
            else if(posz > -50)
                posz -= 3;
            glutPostRedisplay();
            break;
        case 's':
            if(posz < 40 && posz > -30 && (posx < -10)) ;
            else if(posz < 130)
                posz += 3;
            glutPostRedisplay();
            break;
            
        case '1':
            lighton1 = !lighton1;
            glutPostRedisplay();
            break;
        case '2':
            lighton2 = !lighton2;
            glutPostRedisplay();
            break;
        case '3':
            lighton3 = !lighton3;
            glutPostRedisplay();
            break;
            //        case '4':
            //            snowon = !snowon;
            //            glutPostRedisplay();
            //            break;
            
        case 'i':
            ii = 3;
            glutPostRedisplay();
            break;
        case 'k':
            ii = 0;
            glutPostRedisplay();
            break;
        case 'l':
            ii = 1;
            glutPostRedisplay();
            break;
        case 'j':
            ii = 2;
            glutPostRedisplay();
            break;
            
            //
    }
}


int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    
    glutCreateWindow("屋漏偏逢连夜雪");
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glDepthFunc(GL_LEQUAL);//指定用于深度缓冲比较值(即新进像素深度值与原来的1比较，<=则通过，否则丢弃)
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    texName = load_texture("name.bmp");  //加载纹理
    texWall = load_texture("wall.bmp");
    texDoor = load_texture("door.bmp");
    texGround = load_texture("ground.bmp");
    texKakaxi = load_texture("kakaxi.bmp");
    texMotto = load_texture("motto.bmp");
    texPaper = load_texture("wallpaper.bmp");
    texSnow = load_texture("snowball.bmp");
    texKid = load_texture("kid.bmp");
    InitSnow();
    init_clock();
    cout << " 操作说明：\n灯光操作：“1”、“2”、“3”分别控制三个灯的开关。\n门操作：“E”控制开门，“Q”控制关门\n镜头操作：“上”“下”控制镜头上下移动，“左”“右”键控制镜头绕房间旋转\n小车操作：“W”、“A”、“S”、“D”控制小车前后左右移动\n小人操作：“I”、“K”、“J”、“L”键控制小人上下左右移动\n注：雪花需要一段时间才能蔓延到整个屋子 " << endl;
    
    //    glFrustum(-100,100,-100,100, -100,100);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(KeyBoards);
    glutDisplayFunc(Display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000, clockcheck, 1);
    glutMainLoop();
    return 0;
}













