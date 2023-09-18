#include <iostream>
#include <fstream>
#include <sstream>
#include<bits/stdc++.h>
#include <math.h>
#include <limits.h>
#include <vector>
#include <algorithm>

#include </home/horej024/csci5607/assignment2/glm/glm.hpp>  //set to correct directory

class FovType {
    public:
    float h;
    float v;
};

class ImsizeType{
    public:
    int width;
    int height;
};

class ViewingwindowType{
    public:
    int height;
    int width;
    glm::vec3 ul;
    glm::vec3 ur;
    glm::vec3 ll;
    glm::vec3 lr;
};

class ColorType{
    public:
    float r;
    float g;
    float b;
};

class MaterialType{
    public:
    glm::vec3 od;
    glm::vec3 os;
    float ka;
    float kd;
    float ks;
    int n;
    bool justrgb = true;
};

class RayType{
    public:
    glm::vec3 position;
    glm::vec3 direction;
};

class SphereType {
    public:
    float radius;
    glm::vec3 position;
    MaterialType m;
    bool justrgb;
};

class LightType{
    public:
    glm::vec3 position;
    int type;
    glm::vec3 color;
};

std::vector<LightType> lights;  //global variables
std::vector<SphereType> objects;

int Shadow_status(glm::vec3 intersectionPoint, glm::vec3 L, SphereType current_sphere, int o_num){
    float min_d = FLT_MAX;
    int current_obj = -1; 
    int iter = -1;
    for(auto sphere : objects){
        iter += 1; 
        if(iter != o_num){
            float xc = intersectionPoint.x - sphere.position.x;
            float yc = intersectionPoint.y - sphere.position.y;
            float zc = intersectionPoint.z - sphere.position.z;
            float A = 1.0;
            float B = 2 * (L.x * (xc) + L.y * (yc) + L.z * (zc));
            float C = (xc) * (xc) + (yc) * (yc) + (zc) * (zc) - sphere.radius * sphere.radius;
            float disc = (B * B) - (4 * C);
            if(disc > 0){
                float t1 = (-B + sqrt(disc)) / (2 * A);
                float t2 = (-B - sqrt(disc)) / (2 * A);
                if(t1 > 0 && t1 < min_d){
                    min_d = t1;
                    current_obj = iter;
                }
                if(t2 > 0 && t2 < min_d){
                    min_d = t2;
                    current_obj = iter;
                }
            }
            else if(disc == 0){
                float t1 = (-B / 2 * A);
                if(t1 > 0 && t1 < min_d){
                    min_d = t1;
                    current_obj = iter;
                }
            }
        }
        if(current_obj != -1){
            return 0;
        }
    }
    return 1;
}

ColorType Shade_ray(SphereType sphere){
    ColorType colorToReturn;
    colorToReturn.r = sphere.m.od.x;
    colorToReturn.g = sphere.m.od.y;
    colorToReturn.b = sphere.m.od.z;
    return colorToReturn;
}

ColorType Shade_ray2(SphereType sphere, glm::vec3 intersectionPoint, RayType ray, int o_num){   //for use when there is lighting in the scene
    ColorType colorToReturn;
    glm::vec3 vectorColor;
    glm::vec3 N;
    glm::vec3 H;
    glm::vec3 L;
    glm::vec3 sum;
    sum.x = 0;
    sum.y = 0;
    sum.z = 0;
    glm::vec3 newVec;
    //glm::vec3 v = glm::normalize(ray.position - intersectionPoint);
    glm::vec3 v = glm::normalize(intersectionPoint - ray.position);
    N = (intersectionPoint - sphere.position)/ sphere.radius;
    for(int i = 0; i < lights.size(); i++){
        if(lights.at(i).type == 0){ //directional
            L = glm::normalize(float(-1) * lights.at(i).position);
        }
        else if(lights.at(i).type == 1){
            L = glm::normalize(lights.at(i).position - intersectionPoint);
        }
        H = glm::normalize(L + v);
        newVec = lights.at(i).color * ((sphere.m.kd) * sphere.m.od * (std::max(float(0), glm::dot(N, L))) + sphere.m.ks * sphere.m.os * float(pow(std::max(float(0), glm::dot(N, H)), sphere.m.n))); //calculate value in sum
        sum += newVec * float(Shadow_status(intersectionPoint, L, sphere, o_num));
    }
    vectorColor.x = sphere.m.ka * sphere.m.od.x;
    vectorColor.y = sphere.m.ka * sphere.m.od.y;
    vectorColor.z = sphere.m.ka * sphere.m.od.z;
    vectorColor += sum;
    colorToReturn.r = std::min(vectorColor.x, 1.0f);
    colorToReturn.g = std::min(vectorColor.y, 1.0f);
    colorToReturn.b = std::min(vectorColor.z, 1.0f);
    return colorToReturn;
}

ColorType Trace_Ray(RayType ray, std::vector<SphereType> objects, ColorType backgroud){
    float min_d = FLT_MAX;
    int current_obj = -1; 
    int iter = -1;
    glm::vec3 intersectionPoint;
    for(auto sphere : objects){
        iter += 1; 
        float xc = ray.position.x - sphere.position.x;
        float yc = ray.position.y - sphere.position.y;
        float zc = ray.position.z - sphere.position.z;
        float A = 1.0;
        float B = 2 * (ray.direction.x * (xc) + ray.direction.y * (yc) + ray.direction.z * (zc));
        float C = (xc) * (xc) + (yc) * (yc) + (zc) * (zc) - sphere.radius * sphere.radius;
        float disc = (B * B) - (4 * C);
        if(disc > 0){
            float t1 = (-B + sqrt(disc)) / (2 * A);
            float t2 = (-B - sqrt(disc)) / (2 * A);
            if(t1 > 0 && t1 < min_d){
                min_d = t1;
                current_obj = iter;
            }
            if(t2 > 0 && t2 < min_d){
                min_d = t2;
                current_obj = iter;
            }
        }
        else if(disc == 0){
            float t1 = (-B / 2 * A);
            if(t1 > 0 && t1 < min_d){
                min_d = t1;
                current_obj = iter;
            }
        }
    }
    if(current_obj != -1){
        intersectionPoint = min_d * ray.direction + ray.position;
        if(objects.at(current_obj).m.justrgb == false){
            return Shade_ray2(objects.at(current_obj), intersectionPoint, ray, current_obj);
        }
        else return Shade_ray(objects.at(current_obj));
    }
    return backgroud;
}

bool is_number(const std::string& s){ //determines if a string is a number.
    for(char const &ch : s){
        if(std::isdigit(ch) == 0 && ch != '-' && ch != '.'){
            return false;
        }
    }
    return true;
}

int raycast(std::string filename){
    std::ifstream file;
    file.open(filename);
    if(file.fail()){
        std::cout << "file fail\n";
        return 0;
    }
    if(filename.substr(filename.size() - 4) != ".txt"){
        std::cout << "not a text file!\n";
        return 0;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::vector<MaterialType> object_materials;
    glm::vec3 eye;
    glm::vec3 vdir;
    glm::vec3 up;
    FovType fov;
    ImsizeType imsize;
    ColorType bkgcolor;
    ViewingwindowType window;
    RayType ray;
    bool skip = false;
    std::string subs;
    while(buffer){  // 1. read information from text file
        if(skip == false){
            buffer >> subs;
        }
        skip = false;
        if(subs == "eye"){  // get eye information.
            buffer >> subs;
            if(is_number(subs)){
                eye.x = stof(subs);
            }
            else{
                return 0;
            }
            buffer >> subs;
            if(is_number(subs)){
                eye.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                eye.z = stof(subs);
            }
            else return 0;
            //std::cout << "eye: " << eye.x << " " << eye.y << " " << eye.z << std::endl;
        }
        else if(subs == "viewdir"){  // get viewdir information.
            buffer >> subs;
            if(is_number(subs)){
                vdir.x = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                vdir.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                vdir.z = stof(subs);
            } 
            else return 0;
            //std::cout << "vdir: " << vdir.x << " " << vdir.y << " " << vdir.z << std::endl;
        }
        else if(subs == "updir"){    // get updir information.
            buffer >> subs;
            if(is_number(subs)){
                up.x = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                up.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                up.z = stof(subs);
            }
            else return 0;
            //std::cout << "up: " << up.x << " " << up.y << " " << up.z << std::endl;
        }
        else if(subs == "hfov"){ // get fov information.
            buffer >> subs;
            if(is_number(subs)){
                fov.h = stof(subs);
            }
            else return 0;
            //std::cout << "fov: " << fov.h << std::endl;
        }
        else if(subs == "imsize"){   // get size information
            buffer >> subs;
            if(is_number(subs)){
                imsize.width = stoi(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                imsize.height = stoi(subs);
            }
            else return 0;
            //std::cout << "imsize: " << imsize.width << " " << imsize.height << std::endl;
        }
        else if(subs == "bkgcolor"){   // get background color information
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                bkgcolor.r = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                bkgcolor.g = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                bkgcolor.b = stof(subs);
            }
            else return 0;
            //std::cout << "bkg: " << bkgcolor.r << " " << bkgcolor.g << " " << bkgcolor.g << std::endl;
        }
        else if(subs == "mtlcolor"){   // get background color information
            buffer >> subs;
            MaterialType newmat;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newmat.od.r = stof(subs);
                //std::cout << "od.r " << subs << std::endl;
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newmat.od.g = stof(subs);
                //std::cout << "od.g " << subs << std::endl;
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newmat.od.b = stof(subs);
                //std::cout << "od.b " << subs << std::endl;
            }
            else return 0;
            buffer >> subs; //reads in sphere
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newmat.os.r = stof(subs);
                newmat.justrgb = false;
                //std::cout << "os.r " << subs << std::endl;
            }
            else{
                skip = true;
                goto onlyRGB;   
            }
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newmat.os.g = stof(subs);
                //std::cout << "os.g " << subs << std::endl;
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newmat.os.b = stof(subs);
                //std::cout << "os.b " << subs << std::endl;
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newmat.ka = stof(subs);
                //std::cout << "ka " << subs << std::endl;
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newmat.kd = stof(subs);
                //std::cout << "kd " << subs << std::endl;
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newmat.ks = stof(subs);
                //std::cout << "ks " << subs << std::endl;
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                newmat.n = stoi(subs);
                //std::cout << "n " << subs << std::endl;
            }
            else return 0;
            onlyRGB:
            object_materials.push_back(newmat);
        }
        else if(subs == "sphere"){
            buffer >> subs;
            SphereType newsphere;
            if(is_number(subs)){
                newsphere.position.x = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                newsphere.position.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                newsphere.position.z = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                newsphere.radius = stof(subs);
            }
            else return 0;
            objects.push_back(newsphere);
            //std::cout << "sphere: " << newsphere.position.x << " " << newsphere.position.y << " " << newsphere.position.z << " " << newsphere.radius << std::endl;
        }
        else if(subs == "light"){
            buffer >> subs;
            LightType newlight;
            if(is_number(subs)){
                newlight.position.x = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                newlight.position.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs)){
                newlight.position.z = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stoi(subs) == 1 || stoi(subs) == 0){
                newlight.type = stoi(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newlight.color.x = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newlight.color.y = stof(subs);
            }
            else return 0;
            buffer >> subs;
            if(is_number(subs) && stof(subs) <= 1 && stof(subs) >= 0){
                newlight.color.z = stof(subs);
            }
            else return 0;
            lights.push_back(newlight);
        }
    }
    if(objects.size() >= object_materials.size()){
        for(int i = 0; i < objects.size(); i++){ 
            if(i > object_materials.size() -1){
                objects.at(i).m = object_materials.back();
            }
            else{
                objects.at(i).m = object_materials.at(i);
            }
        }
    }
    else return 0;
    


    ColorType arr[imsize.width * imsize.height];  // 2. Define an array of sufficent size to store the color values
    float aspect_ratio = (float)imsize.width/(float)imsize.height;  // define viewing window
    window.width = 2 * 1 * round(tan(.5 * (fov.h * M_PI / 180.0)));
    window.height = window.width / aspect_ratio;

    glm::vec3 u = glm::cross(vdir, up); // calculate u and v
    glm::vec3 v = glm::cross(u, vdir);
    u = glm::normalize(u);
    v = glm::normalize(v);
    if(u.x == -0){     //cross managed to compute -0, this is to fix incase.
        u.x = 0;
    }
    if(u.y == -0){
        u.y = 0;
    }
    if(u.z == -0){
        u.z = 0;
    }
    if(v.x == -0){
        v.x = 0;
    }
    if(v.y == -0){
        u.y = 0;
    }
    if(v.z == -0){
        v.z = 0;
    }
    glm::vec3 u2;
    u2 = u * (float)(window.width/2);
    glm::vec3 v2;
    v2 = v * (float)(window.height/2);
    window.ul = eye + vdir * (float)1 - u2 + v2;    // calculate four courners of viewing window
    window.ur = eye + vdir * (float)1 + u2 + v2;
    window.ll = eye + vdir * (float)1 - u2 - v2;
    window.lr = eye + vdir * (float)1 + u2 - v2;
    //std::cout << "ul: " << window.ul.x << " " << window.ul.y << " " << window.ul.z << std::endl;
    //std::cout << "lr: " << window.lr.x << " " << window.lr.y << " " << window.lr.z << std::endl;
    glm::vec3 deltaH;
    glm::vec3 deltaV;
    deltaH = (window.ur - window.ul) / (float)(imsize.width -1);
    deltaV = (window.ll - window.ul) / (float)(imsize.height -1);
    glm::vec3 point;
    std::string newfile = filename; // A string storing the output file's name is created and assigned with the previous file name, .txt is removed and then out.txt is appended.
    newfile.pop_back();
    newfile.pop_back();
    newfile.pop_back();
    newfile.pop_back();
    newfile.append(".ppm");
    std::ofstream outfile(newfile); // create new file with our created name.
    outfile << "P3" << "\n" << imsize.width << " " << imsize.height << "\n" << "255\n"; //write header
    ray.position = eye;
    for(int j = 0; j < imsize.height; j++){  // 4. for each pixel in the output image
        for(int i = 0; i < imsize.width; i++){
            point = window.ul + (float)i * deltaH + (float)j * deltaV;
            ray.direction = (point - eye) / sqrt(((point.x - eye.x) * (point.x - eye.x)) + ((point.y - eye.y) * (point.y - eye.y)) + ((point.z - eye.z) * (point.z - eye.z)));
            //std::cout << "rd: " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
            arr[i] = Trace_Ray(ray, objects, bkgcolor);
            outfile << int(arr[i].r * 255) << " " << int(arr[i].g * 255) << " " << int(arr[i].b * 255) << "\n";
        }
    }
    outfile.close();
    return 1;
}

int main(int argc, char **argv){
    if(argc == 2){
    raycast(argv[1]);
    }
    else{
        std::cout << "One file needed\n";
        return 0;
    }

}
