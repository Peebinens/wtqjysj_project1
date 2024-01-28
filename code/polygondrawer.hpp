//
// Created by pigzhu on 2023/11/22.
//

#ifndef PROJECT1_POLYGONDRAWER_HPP
#define PROJECT1_POLYGONDRAWER_HPP

class PolygonDrawer : public Fl_Box {
public:
    PolygonDrawer(int x, int y, int w, int h, Fl_Callback* _c);
    void start_poly();
    void star_measure();
    void set_pix(uchar* pix){pix0 = pix;}
    void set_real_des(double size){real_des = size;}
    void set_graph_des(double size){graph_des = size;}
    void set_area(double size){area = size;}
    void set_mode(bool on_off){
        kb_mode = on_off;
        measuring = 2;
        drawing = false;
        start = false;
        if(on_off) points.clear();
    }
    int handle(int event) override;
    void point_insert(int x, int y){points.push_back(std::make_pair(x, y));}
    double get_rs(){return area/graph_des*real_des*real_des/graph_des;}
    double get_gs(){return area;}
    double get_graph_des(){return graph_des;}
    double get_real_des(){return real_des;}
    std::string get_data();
    void set_data(std::string data);
    void draw() override ;

private:
    std::vector<std::pair<int, int>> points;
    double graph_des;
    double real_des;
    bool drawing = false;
    bool start = false;
    bool kb_mode = false;
    double area;
    int measuring;
    uchar* pix0 = NULL;
    uchar* pix1 = NULL;
    Fl_Callback *cb;
};

#endif //PROJECT1_POLYGONDRAWER_HPP
