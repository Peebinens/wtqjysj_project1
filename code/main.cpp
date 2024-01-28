#include "global.hpp"
#include "map_window.hpp"
#include "polygondrawer.hpp"

void ok_button_cb(Fl_Widget* w, void* v) ;
Fl_Double_Window* main_window = new Fl_Double_Window(0,20,1700, 990, "Project 1");
Fl_Value_Output* Output1 = new Fl_Value_Output(900, 0, 100, 25, "Graph Scale");
Fl_Value_Output* Output2 = new Fl_Value_Output(1100, 0, 100, 25, "Real Scale");
Fl_Output* Output3 = new Fl_Output(1200, 0, 50, 25, nullptr);
Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,125,25);
map_window* map = new map_window(0,25,1700, 965);
PolygonDrawer* drawer = new PolygonDrawer(0,25,1700, 965, ok_button_cb);

Fl_Window *save_win = new Fl_Window(300, 180, "Save file");
Fl_Input *name_input = new Fl_Input(70, 50, 200, 30, "File name:");
Fl_Button *ok = new Fl_Button(120, 120, 70, 30, "OK");
Fl_Button *cancel = new Fl_Button(200, 120, 70, 30, "Cancel");

Fl_Window *w_cho = new Fl_Window(250, 100, "Select");
Fl_Input_Choice* cho = new Fl_Input_Choice(10,10,125,25);
Fl_Button *ok1 = new Fl_Button(195, 70, 50, 30, "OK");
Fl_Button *cancel1 = new Fl_Button(195, 40, 50, 30, "Cancel");

Fl_Window *measure_window = new Fl_Window(300, 160, "Measure");
Fl_Input* input = new Fl_Input(120, 0, 100, 50, "real distance");
Fl_Input* unit_button = new Fl_Input(120, 50, 50, 50, "unit");
Fl_Button* ok2 = new Fl_Button(110, 120, 75, 30, "OK");
Fl_Button* cancel2 = new Fl_Button(190, 120, 75, 30, "Cancel");
std::unordered_map<std::string, std::string> file_data;
void file_button_cb(Fl_Widget* w, void* v) {
    Fl_File_Chooser chooser("./", "*", Fl_File_Chooser::SINGLE, "Choose a map");
    chooser.show();
    while (chooser.shown())
        Fl::wait();
    if (chooser.value() == NULL)
        return;
    map_window *win = (map_window *) v;
    win->add_image(chooser.value());
    win->redraw();
    Fl::flush();
    drawer -> set_pix(win->get_pix());
}

void ok_button_cb(Fl_Widget* w, void* v) {
    try{ drawer->set_real_des(std::stod(input->value())); }
    catch (std::exception& e){ }
    char a[10];
    strcpy(a, unit_button->value());
    strcat(a, "^2");
    Output1->value(drawer->get_gs());
    Output2->value(drawer->get_rs());
    Output3->value(a);
}

void kb_button_cb(Fl_Widget* w, void* v) {
    std::vector<std::pair<int, int>> p;
    drawer->set_mode(true);
    double zoom = map->get_zoom(), ar = 0;
    std::cout<<"Please input the numbers of the point you want to draw:\n";
    int n, x, y, first_x, first_y;
    std::cin>>n;
    if(n < 3){
        std::cout<<"The number of the point should be more than 3!\n";
        return;
    }
    std::cout<<"Please input the x and y of the point:\n";
    std::cin >> first_x >> first_y;
    p.push_back(std::make_pair(first_x, first_y));
    drawer->point_insert(int(first_x * zoom), int(first_y * zoom + 25));
    for(int i = 1; i < n; i++){
        std::cin>>x>>y;
        p.push_back(std::make_pair(x, y));
        drawer->point_insert(int(x * zoom), int(y * zoom + 25));
    }
    p.push_back(std::make_pair(first_x, first_y));
    drawer->point_insert(int(first_x * zoom), int(first_y * zoom + 25));
    for (size_t i = 0; i < p.size(); ++i) ar += p[i].first * p[(i + 1) % p.size()].second - p[i].second * p[(i + 1) % p.size()].first;
    ar= std::fabs(ar / 2);
    drawer->set_area(ar);
    std::cout<<"Please input the scale of the graph:\n";
    std::cin>>x;
    char a;
    std::cin>>a;
    std::cin>>y;
    char b[10];
    std::cin>>b;
    drawer->set_real_des(y);
    drawer->set_graph_des(x);
    drawer->redraw();
    Fl::flush();
    Output1->value(drawer->get_gs());
    Output2->value(drawer->get_rs());
    strcat(b, "^2");
    Output3->value(b);
    drawer->set_mode(false);
}

void open_cb(Fl_Widget* w, void* v) {
    w_cho -> show();
    cancel1 -> callback([](Fl_Widget* v, void* w) { w_cho->hide(); });
    ok1 -> callback([](Fl_Widget* v, void* w) {
        w_cho->hide();
        std::string s = cho->value();
        std::string sss = file_data[s];
        std::string name = sss.substr(0, sss.find(' '));
        std::string data = sss.substr(sss.find(' ') + 1, sss.find('%') - sss.find(' ') - 1);
        const char* c = name.c_str();
        map->add_image(c);
        map->redraw();
        Fl::flush();
        drawer->set_pix(map->get_pix());
        drawer->set_data(data);
        size_t i = sss.find('%') + 1;
        double t;
        std::istringstream iss(data);
        iss >> t;
        input->value(std::to_string(t).c_str());
        unit_button->value(sss.substr(i, sss.find('^') - i ).c_str());
        Output1->value(drawer->get_gs());
        Output2->value(drawer->get_rs());
        Output3->value(sss.substr(i).c_str());
    });
}

void save_cb(Fl_Widget* w, void* v) {
    save_win->show();
    cancel -> callback([](Fl_Widget* w, void* v) { save_win->hide(); });
    ok -> callback([](Fl_Widget* w, void* v) {
        std::ofstream ff("save_data.txt", std::ios::out);
        std::string now = map -> get_image() + " " + drawer -> get_data() + "%";
        char a[10];
        strcpy(a, Output3->value());
        now += a;
        cho -> add(name_input->value());
        file_data.insert(std::make_pair(name_input->value(), now));
        for(auto it = file_data.begin(); it != file_data.end(); it++) ff << it->first << "@" << it->second << std::endl;
        save_win->hide();
    });
}

void file_read() {
    std::ifstream ff("save_data.txt", std::ios::in);
    std::string s;
    while(getline(ff, s)){
        std::string name, data;
        int i = 0;
        while(s[i] != '@'){
            name += s[i];
            i++;
        }
        i++;
        while(i < s.size()){
            data += s[i];
            i++;
        }
        file_data.insert(std::make_pair(name, data));
        cho -> add(name.c_str());
    }
}

int main() {
    fl_register_images();
    file_read();
    Output1->color(FL_WHITE);
    Output2->color(FL_WHITE);
    ok2->callback([](Fl_Widget*, void*) {measure_window->hide(); drawer->set_real_des(std::stod(input->value())); drawer->star_measure();});
    cancel2->callback([](Fl_Widget*, void*) {measure_window->hide();});
    menu->add("File/Load Image", "^d", file_button_cb, map);
    menu->add("File/Open", "^o", open_cb, 0);
    menu->add("File/Save", "^s", save_cb, 0, FL_MENU_DIVIDER);
    menu->add("File/Quit", "^q", [](Fl_Widget*, void*) {exit(0); });
    menu->add("Edit/Measure", "^1", [](Fl_Widget*, void*) { measure_window->show(); });
    menu->add("Edit/Draw Polygon", "^2", [](Fl_Widget*, void*) {drawer->start_poly(); });
    menu->add("Edit/Clear", "^3", [](Fl_Widget*, void*) {map->redraw(); drawer->set_mode(false); drawer->set_pix(map->get_pix());}, nullptr, FL_MENU_DIVIDER);
    menu->add("Edit/Keyboard Input", "^k", kb_button_cb, drawer);
    menu->add("Help/About", "^h", [](Fl_Widget*, void*) {Fl_Window* win = new Fl_Window(500, 300, "About"); Fl_Box* box = new Fl_Box(0, 0, 500, 300, "This is a program to measure the area of a polygon on a map.\n\n" "You can use the mouse to draw a polygon on the map,\n\n " "or you can use the keyboard to input the coordinates of the polygon,\n\n" "and then save it.\n\n" "The area of the polygon will be shown on the screen,\n\n" "and the polygon will be highlighted.\n\n" "The program is written by Jianbin Zhu.\n\n" "More information see: https://github.com/Peebinens/wtqjysj_project1"); win->end(); win->show(); });
    main_window->show();
    return Fl::run();
}