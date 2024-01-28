//
// Created by pigzhu on 2023/11/22.
//
#include "global.hpp"
#include "polygondrawer.hpp"

PolygonDrawer::PolygonDrawer(int x, int y, int w, int h, Fl_Callback* _c) : Fl_Box(x, y, w, h), graph_des(100), real_des(100), area(0), measuring(2), cb(_c) {
    pix0 = new uchar[w*h*3];
    for(int i = 0; i < w*h*3; ++i) pix0[i] = 255;
}

void PolygonDrawer::start_poly(){
    drawing = true;
    measuring = 2;
    points.clear();
}

void PolygonDrawer::star_measure(){
    measuring = 0;
    drawing = false;
    points.clear();
}

int PolygonDrawer::handle(int event) {
    if(drawing) {
        switch (event) {
            case FL_PUSH:
                if (Fl::event_button() == FL_LEFT_MOUSE) {
                    points.push_back(std::make_pair(Fl::event_x(), Fl::event_y()));
                    start = true;
                    redraw();
                    return 1;
                }
                else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                    start = false;
                    points.push_back(std::make_pair(points[0].first, points[0].second));
                    area = 0;
                    if(points.size() < 3){
                        drawing = false;
                        return 1;
                    }
                    for (size_t i = 0; i < points.size(); ++i) area += points[i].first * points[(i + 1) % points.size()].second - points[i].second * points[(i + 1) % points.size()].first;
                    area = std::fabs(area / 2);
                    redraw();
                    Fl::flush();
                    drawing = false;
                    cb(nullptr, nullptr);
                    return 1;
                }
                break;
            case FL_MOVE:
                if (start) {
                    redraw();
                    Fl::flush();
                    fl_color(FL_RED);
                    fl_line_style(FL_SOLID,2);
                    fl_circle(Fl::event_x(), Fl::event_y(), 3);
                    fl_line_style(FL_DASHDOT, 2);
                    fl_line(points.back().first, points.back().second, Fl::event_x(), Fl::event_y());
                    return 1;
                } else{
                    redraw();
                    Fl::flush();
                    fl_color(FL_RED);
                    fl_line_style(FL_SOLID,2);
                    fl_circle(Fl::event_x(), Fl::event_y(), 3);
                    return 1;
                }
        }
    }
    if(measuring < 2){
        switch (event) {
            case FL_PUSH:
                if (Fl::event_button() == FL_LEFT_MOUSE) {
                    points.push_back(std::make_pair(Fl::event_x(), Fl::event_y()));
                    redraw();
                    Fl::flush();
                    return 1;
                }
                break;
            case FL_DRAG:
                if(points.size() == 2) points.pop_back();
                points.push_back(std::make_pair(Fl::event_x(), Fl::event_y()));
                redraw();
                Fl::flush();
                fl_color(FL_RED);
                fl_line_style(FL_SOLID);
                fl_circle(points[1].first, points[1].second, 3);
                fl_line_style(FL_DASHDOT, 2);
                fl_line(points[0].first, points[0].second, points[1].first, points[1].second);
                fl_line_style(FL_DASHDOT, 1);
                fl_line(points[1].first, 25, points[1].first, h()+25);
                fl_line(0, points[1].second, w(), points[1].second);
                return 1;
            case FL_RELEASE:
                points[1] = std::make_pair(Fl::event_x(), Fl::event_y());
                measuring++;
                redraw();
                Fl::flush();
                measuring++;
                graph_des = std::sqrt((points[0].first - points[1].first)*(points[0].first - points[1].first) + (points[0].second - points[1].second)*(points[0].second - points[1].second));
//                std::cout<<graph_des<<std::endl;
                return 1;
            case FL_MOVE: {
                redraw();
                Fl::flush();
                int _x = Fl::event_x(), _y = Fl::event_y();
                fl_color(FL_RED);
                fl_line_style(FL_SOLID);
                fl_circle(_x, _y, 3);
                fl_line_style(FL_DASHDOT, 1);
                fl_line(_x, 25, _x, h() + 25);
                fl_line(0, _y, w(), _y);

                return 1;
            }
        }
    }
    return Fl_Box::handle(event);
}

void PolygonDrawer::draw() {
    Fl_Box::draw();
    if ((drawing && points.size() > 2 && !start) || kb_mode) {
        fl_color(FL_YELLOW);
        fl_begin_polygon();
        for (auto &point: points) { fl_vertex(point.first, point.second); }
        fl_end_polygon();
        Fl::flush();
        pix1 = fl_read_image(NULL, x(), y(), w(), h());
        for(int i = 0; i < h() * w() * 3; i++) pix1[i] = ((int)pix1[i] + 4 * (int)pix0[i]) / 5;
        fl_draw_image(pix1, x(), y(), w(), h());
        pix0 = pix1;
    }
    fl_color(FL_BLUE);
    fl_line_style(FL_SOLID, 2);
    if (drawing && points.size() > 1 || kb_mode) {
        for (size_t i = 1; i < points.size(); ++i) {
            fl_line(points[i - 1].first, points[i - 1].second, points[i].first, points[i].second);
        }
    }
    fl_color(FL_RED);
    if (drawing && !points.empty() || kb_mode) {
        fl_line_style(FL_SOLID, 2);
        for (int i = 0; i < points.size(); i++) fl_circle(points[i].first, points[i].second, 3);
    }
    if (measuring == 0 && !points.empty()) {
        fl_line_style(FL_SOLID, 2);
        fl_circle(points[0].first, points[0].second, 3);
        fl_line_style(FL_DASHDOT, 1);
        fl_line(points[0].first, 25, points[0].first, h() + 25);
        fl_line(0, points[0].second, w(), points[0].second);
    }
    if (measuring == 1) {
        fl_line_style(FL_SOLID, 2);
        fl_circle(points[1].first, points[1].second, 3);
        fl_line_style(FL_DASHDOT, 1);
        fl_line(points[0].first, 25, points[0].first, h() + 25);
        fl_line(0, points[0].second, w(), points[0].second);
        fl_line(points[1].first, 25, points[1].first, h() + 25);
        fl_line(0, points[1].second, w(), points[1].second);
        fl_color(FL_BLUE);
        fl_line(points[0].first, points[0].second, points[1].first, points[1].second);
    }
}

std::string PolygonDrawer::get_data() {
    std::string result;
    result += std::to_string(real_des) + " " + std::to_string(graph_des) + " " + std::to_string(area) + " ";
    for (auto &point: points) {
        result += std::to_string(point.first) + " " + std::to_string(point.second) + " ";
    }
    return result;
}

void PolygonDrawer::set_data(std::string data) {
    set_mode(true);
    std::istringstream iss(data);
    iss >> real_des >> graph_des >> area;
    int x, y;
    while (iss >> x >> y) {
        points.push_back(std::make_pair(x, y));
    }
    parent() -> redraw();
    Fl::flush();
    kb_mode = false;
}


