#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
struct volume_control_t {
int status;
int volume;
int time_depend_vol;
};
class my_sound_t { //класс звуков, которые не умеют ничего, кроме как проигрываться
    private:
        sf::Sound sound;
        sf::SoundBuffer buffer;
    public:
        void play();
        void stop();
        void load_buffer(std::string s);
        void set_volume(int vol);
};
class sound_commander_t { //класс, начальник над звуками
    private:
        std::vector<int> key_map; //вектор с индексами от 0 до, где-то, 55, то есть кода клавиши "="
        my_sound_t* sounds;
        volume_control_t* volume_control;
        int num_of_sounds;
    public:
        sound_commander_t(int number_of_sounds, std::vector<int> input_map, std::vector<std::string> paths);
        int play (sf::Event event);
        int check (sf::Event event);
        void volume_check ();
};
class listener_t {
    private:
        sf::SoundBufferRecorder recorder;
        sf::SoundBuffer buffer;
    public:
        void start_recording();
        void end_recording();
        void save_file(std::string file_name);
        void show_devices();
}; //чукча-слушатель
class writer_t {
    private:
        std::vector<float>* sequence;
        int sequence_size;
    public:
        writer_t (int number_of_keys );
        void remember ( sf::Time time, int key_code );
        void show ();
        void write();
}; //чукча-писатель
class support_part_t { //кусочек подсказки для одной кнопки
    private:
        sf::CircleShape body;
        sf::Text letter;
        sf::RectangleShape tail;
        int x;
        int y;
        float time_pressed;
        float time_elapsed;
        int tail_length;
        sf::Font MyFont;
    public:
        support_part_t (int key_code, int key_number, float time_start, float time_stop, sf::Font* font);
        int draw (sf::Time time_c,  sf::RenderWindow& window ) ;
        ~support_part_t() {
        }
};
class support_team_t {
    private:
        std::vector<support_part_t>* support; //массив векторов, не вектор массивов
        int support_size;
        int score;
        int* key_map;
    public:
        support_team_t(int* input_key_map, sf::Font* font);
        void draw ( sf::Time time_c, sf::Event event, sf::RenderWindow& window );
        int get_score(){
            return score;
        }
        ~support_team_t() {
        delete[] support;
        }
};
int main() {
    sf::Font MyFont;
    MyFont.loadFromFile("arial.ttf");
    std::vector<support> support_draw;//
    int support_buffer = 40;//
    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML works!");
    window.setPosition(sf::Vector2i(400, 100));
    window.setKeyRepeatEnabled(false);//
    std::vector<float>* sequence = new std::vector<float>[62];
    sf::Sound* sounds = new sf::Sound[ 62 ];
    sf::SoundBuffer* buffers = new sf::SoundBuffer[ 62 ];
    std::vector<std::string> paths;
    for( int i = 11; i < 62; ++i ) {
        std::string temp_str = "Mics2_Layer7_00";
        temp_str[ temp_str.size() - 2 ] += i / 10;
        temp_str[ temp_str.size() - 1 ] += i % 10;
        temp_str+=".wav";
        paths.push_back(temp_str);
    }
    int test_mask[37] = {25, 23, 3, 2, 5, 21, 1, 7, 13, 9, 12, 10, 49,
    50, 48, 52, 51, 16, 22, 29, 4, 30, 17, 31,
    19, 24, 33, 20, 34, 8, 14, 26, 15, 56, 46, 55, 47 } ;
    int reverse_mask[57];
    for ( int i = 0; i < 57; ++i ) {
        reverse_mask[i]=-1;
    }
    for(int i = 0; i < 37; ++i ){
        reverse_mask[test_mask[i]] = i;
    }
    std::vector<int> key_map;
    for( int i = 0; i < 57; ++i ) {
        key_map.push_back(reverse_mask[i]);
    }
    sound_commander_t sound_commander(paths.size(), key_map, paths);
    writer_t writer(37);
    sf::Clock clock;
    support_team_t support_team(test_mask, &MyFont);
    while (window.isOpen()) {
        sf::Event event;
        sound_commander.volume_check();//
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

          /*  if( event.key.code >= 0 && event.key.code < 57 ) { //
                if(sound_commander.check(event) != 0 ) {            //
                    writer.remember(clock.getElapsedTime(), key_map[event.key.code]); //
                }   //
            }  //*/
            
        }
        window.clear();
        support_team.draw(clock.getElapsedTime(), event, window);
        window.display();
    }
    int score = support_team.get_score();
    std::cout<<"your score is: "<<score<<std::endl;
    writer.write();
    return 0;
}
void my_sound_t::play() {   //проиграть звук
    sound.play();
}
void my_sound_t::stop() {
    sound.stop();
}
void my_sound_t::load_buffer(std::string s) {  //подрузить звук из файла и приготовиться его играть
    buffer.loadFromFile(s);
    sound.setBuffer(buffer);
}
void my_sound_t::set_volume(int vol) {   //установить громкость
    sound.setVolume(vol);
}
sound_commander_t::sound_commander_t(int number_of_sounds, std::vector<int> input_map, std::vector<std::string> paths) { //конструктор
    if (number_of_sounds != paths.size()){
        std::cout<<"Error, wrong number of paths/sounds";
        return;
    }
    num_of_sounds = number_of_sounds;
    sounds = new my_sound_t[number_of_sounds];
    for ( int i= 0; i < number_of_sounds; ++i ) {
        sounds[i].load_buffer(paths[ i ]);
    }
    for ( int i = 0; i < input_map.size(); ++i ) {
        key_map.push_back(input_map[ i ]);
    } //я обращаюсь к 25му индексу (код Z) и вижу 0, потому что к Z привязан нулевой звук
    volume_control = new volume_control_t[number_of_sounds];
    for ( int i = 0; i < number_of_sounds; ++i ) {
        volume_control[i].status = 1; // -1 - кнопка зажата и звук проигрывается, 0 - кнопка отпущена и звук гаснет, 1 - кнопка отпущена и звук не проигрывается
        volume_control[i].volume = 100; // полная громкость
        volume_control[i].time_depend_vol = 1000;
    }
} //объект создан и готов работать
int sound_commander_t::play (sf::Event event) {
    if ( event.type == sf::Event::KeyPressed && key_map[event.key.code] != -1 ) { //была нажата кнопка
        sounds[ key_map[ event.key.code ] ].play();
        return 1;
    }
    if ( event.type == sf::Event::KeyReleased && key_map[event.key.code] != -1) {
        return -1; //была отпущена клавиша, к которой привязан звук
    } //вообще, довольно философский вопрос, а оно нам надо, запоминать кто когда отпущен?
    return 0; //событие, пришедшее на вход к звукам не относится
}
int sound_commander_t::check (sf::Event event) {
    if(event.key.code >= 0 && event.key.code < 57 ) {
        if ( event.type == sf::Event::KeyPressed && key_map[event.key.code] != -1 ) { //была нажата кнопка
            sounds[ key_map[ event.key.code ] ].play();
            volume_control[ key_map[ event.key.code ] ].status = -1;
            volume_control[ key_map[ event.key.code ] ].volume = 100;
            sounds[ key_map[ event.key.code ] ].set_volume(volume_control[ key_map[ event.key.code ] ].volume);
            return 1;
        }
        if (  event.type == sf::Event::KeyReleased && key_map[event.key.code] != -1) {
            volume_control[ key_map [ event.key.code ] ].status = 0;
            return -1; //была отпущена клавиша, к которой привязан звук
        } //вообще, довольно философский вопрос, а оно нам надо, запоминать кто когда отпущен?
    }
    return 0; //событие, пришедшее на вход к звукам не относится
}
void sound_commander_t::volume_check () {
    for ( int i = 0; i < num_of_sounds; ++i ) {
        if(volume_control[i].status == -1 ) {
            volume_control[i].time_depend_vol = 1000;
            volume_control[i].volume = volume_control[i].time_depend_vol / 10;
            sounds[i].set_volume( volume_control[i].volume );
        }
        if(volume_control[i].status == 0 && volume_control[i].volume == 0) {
            volume_control[i].status = 1;
            volume_control[i].time_depend_vol = 1000;
            volume_control[i].volume = volume_control[i].time_depend_vol / 10;
            sounds[i].stop();
            sounds[i].set_volume( volume_control[i].volume );
        }
        if(volume_control[i].status == 0 ) {
            volume_control[i].time_depend_vol-=2;
            volume_control[i].volume = volume_control[i].time_depend_vol / 10;
            sounds[i].set_volume(volume_control[i].volume);
        }
    }
}
void listener_t::start_recording() {
    std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
    recorder.setDevice(availableDevices[ 2 ]);
    recorder.start();
}
void listener_t::end_recording() {
    recorder.stop();
}
void listener_t::save_file(std::string file_name) { //вообще говоря, возвращать надо bool на случай ошибок
    buffer = recorder.getBuffer();
    buffer.saveToFile(file_name);
}
void listener_t::show_devices() {
    std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
    for ( int i = 0; i < availableDevices.size(); ++i ) {
        std::cout << availableDevices[ i ] << std::endl;
    }
}
writer_t::writer_t (int number_of_keys ) { //массив с кодами кнопок из sfml
    sequence = new std::vector<float>[number_of_keys]; //это не наша нумерация кнопок для звуков
    sequence_size = number_of_keys;
}
void writer_t::remember ( sf::Time time, int key_code ) {
    sequence[ key_code ].push_back(time.asSeconds());
}
void writer_t::show () {
    for ( int i = 0; i < sequence_size; ++i ) {
        std::cout << "key code = " << i << " , was pressed at  ";
        for (int j = 0; j < sequence[ i ].size(); ++j ) {
            std::cout << sequence[ i ][ j ] << " ";
        }
        std::cout << std::endl;
    }
}
void writer_t::write() {
    std::ofstream outfile;
    outfile.open("writer_test.txt");
    outfile << sequence_size;
    outfile <<"\n";
    for ( int i = 0; i < sequence_size; ++i ) {
        outfile << sequence[i].size() << " ";
        for (int j = 0; j < sequence[ i ].size(); ++j ) {
            outfile << sequence[ i ][ j ] << " ";
        }
        outfile << "\n";
    }
    outfile.close();
}
support_part_t::support_part_t (int key_code, int key_number, float time_start, float time_stop, sf::Font* font) {
    y = 20;
    x = 30 * (key_number ) + 20;
    time_pressed = time_start; //время, когда кнопка должна будет быть нажата
    time_elapsed = time_stop;
    body.setRadius(20);
    body.setFillColor(sf::Color(100, 250, 50));
    MyFont.loadFromFile("arial.ttf");
    if(('A'+key_code)<='Z') {
        letter.setString((char)('A'+(key_code)));
    } else {letter.setString("["); }
    letter.setFont(*font);
    letter.setCharacterSize(24);
    letter.setColor(sf::Color::Red);
    tail_length = (int ) 200 * (time_elapsed - time_pressed) ;
    tail.setSize(sf::Vector2f( tail_length , 4));
    tail.rotate(90);
    tail.setFillColor(sf::Color(100, 250, 50)); //это все движение вокруг подсказок
}
int support_part_t::draw (sf::Time time_c,  sf::RenderWindow& window ) {
    if ( (time_pressed >= time_c.asSeconds() + 3.675 ) || (time_pressed <= time_c.asSeconds() - 0.4 ) ) { //еще слишком рано что-то рисовать
        return 0; //или уже слишком поздно
    }
    int status;
    if ( time_pressed < time_c.asSeconds() + 3.675 && time_pressed >= time_c.asSeconds() ) {//самое время
        y = 15 + (int)(200 * (time_c.asSeconds() + 3.675 - time_pressed ));
        body.setPosition(x, y);
        letter.setPosition( x + 10, y + 5 );
        tail.setPosition(x+22, y-tail_length);
        status = 0;
    }
    if (time_pressed < time_c.asSeconds() && time_elapsed > time_c.asSeconds() ) { //самое время
        body.setOutlineThickness(3);
        body.setOutlineColor(sf::Color(250, 150, 100));
        body.setPosition( x, 750 );
        letter.setPosition( x + 10, 755 );
        tail.setPosition(x+22, y - (int) (tail_length * (time_elapsed - time_c.asSeconds() ) / (time_elapsed - time_pressed) ) );
        tail.setSize(sf::Vector2f(  (int) (tail_length * (time_elapsed - time_c.asSeconds() ) / (time_elapsed - time_pressed) ), 4));
        status = 1;
    }
    window.draw(tail);
    window.draw(body);
    window.draw(letter);
    return status;
}
support_team_t::support_team_t(int* input_key_map, sf::Font* font) {
    std::ifstream infile;
    infile.open("writer_test_final.txt");
    int temp;
    float f_tmp_1, f_tmp_2;
    infile >> temp;
    std::cout<<temp<<std::endl;
    support = new std::vector<support_part_t>[temp];
    support_size = temp;
    for ( int i = 0; i < support_size; ++i ) {
        infile >> temp;
        for ( int j = 0; j < temp/2; ++j ) {
            infile >> f_tmp_1;
            infile >> f_tmp_2;
            support_part_t temp_sup(input_key_map[i],i,f_tmp_1, f_tmp_2, font);
            support[ i ].push_back(temp_sup);
        }
    }
    infile.close();
    score = 0;
    key_map = input_key_map;
}
void support_team_t::draw ( sf::Time time_c, sf::Event event, sf::RenderWindow& window ) {
    for ( int i = 0; i < support_size; ++i ) {
        bool must_be_pressed = 0;
        for ( int j = 0; j < support[ i ].size(); ++j ){
            int tmp =  support[i][j].draw(time_c, window);
            must_be_pressed = must_be_pressed || tmp;
        }
        if( sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::A + key_map[i])) ) {
            if(must_be_pressed == true) {++score;}
        }
    }
}//складывается абсурдная ситуация, когда подсчетом очков должен заниматься графический класс
