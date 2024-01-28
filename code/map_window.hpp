//
// Created by pigzhu on 2023/11/22.
//

#ifndef PROJECT1_MAP_WINDOW_HPP
#define PROJECT1_MAP_WINDOW_HPP

class map_window : public Fl_Widget {
private:
    Fl_Image* map;
    std::string file_name;
    double zoom;
    uchar* pix = nullptr;
public:
    void draw() ;
    double get_zoom() ;
    uchar* get_pix() ;
    std::string get_image() ;
    void add_image(const char* filename) ;
    map_window(int x, int y, int w, int h) ;
};
#endif //PROJECT1_MAP_WINDOW_HPP
