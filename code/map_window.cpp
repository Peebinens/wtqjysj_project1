//
// Created by pigzhu on 2023/11/22.
//
#include "global.hpp"
#include "map_window.hpp"
    void map_window::draw() {
        fl_color(FL_WHITE);
        fl_rectf(x(), y(), w(), h());
        if (map == nullptr) return;
        double w_bei = double(w())/map->w(), h_bei = double(h())/map->h();
        zoom = std::min(w_bei, h_bei);
        Fl_Image* temp = map->copy(int(map->w()*zoom), int(map->h()*zoom));
        temp->draw(x(), y());
        pix = fl_read_image(NULL, x(), y(), w(), h());
//        std::cout<<zoom<<std::endl;
    }
    double map_window::get_zoom() {
        if (map == nullptr) return 1;
        return zoom;
    }
    uchar* map_window::get_pix(){ return pix; }
    std::string map_window::get_image() { return file_name; }
    void map_window::add_image(const char* filename) {
        if(map != NULL)
            delete map;
        file_name = filename;
        if(filename[strlen(filename) - 2] == 'n') map = new Fl_PNG_Image(filename);
        else map = new Fl_JPEG_Image(filename);
    }
    map_window::map_window(int x, int y, int w, int h) : Fl_Widget(x,y,w, h), map(nullptr) {}
