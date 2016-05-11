#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <fstream>

//=============================================== аудио классы =========================================================
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
      //  sound_commander_t(int number_of_sounds, std::vector<int> input_map, std::vector<std::string> paths);
        void init_sound_commander(int number_of_sounds, std::vector<int> input_map, std::vector<std::string> paths);
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
	    std::vector<int> key_map;
    public:
       // writer_t (int number_of_keys );
	    void init_writer(int number_of_keys, std::vector<int> input_map);
        void remember ( sf::Time time, sf::Event event );
        void show ();
        void write();
}; //чукча-писатель


//======================================================================================================
class base_window;
//======================================= кнопка приложения ==========================================
class application_btn{
public:
	application_btn();
	virtual void draw(sf::RenderWindow *);
	void set_texture(sf::Texture);
	virtual void set_title(const std::wstring &, int);
	void set_size(float, float);
	void set_radius(float);
	void move(float,float);
	virtual void set_position(float, float);
	virtual bool is_pushed(sf::RenderWindow *);//&
	virtual void react(base_window *) = 0;

	static sf::Texture btn_texture;
	static sf::Font btn_font;
protected:
	sf::RectangleShape btn_shape;
	sf::CircleShape btn_circle;
	sf::Sprite btn_sprite;
	sf::Text btn_title;
};

sf::Texture application_btn :: btn_texture;
sf::Font application_btn :: btn_font;

//классы - наследники
//---------------------------------------------------------------------------
class mini_play: public application_btn{
public:
	mini_play();
	void react(base_window *);
	void set_title(const std::wstring &, int);
	void set_position(float,float);
	void draw(sf::RenderWindow *);
	bool is_pushed(sf::RenderWindow *);

	static sf::Texture circle_texture;
private:
	//звуковой файл, привязанный к кнопке
};

sf::Texture mini_play :: circle_texture;

//----------------------  конструктор mini_play ------------------------

mini_play :: mini_play()
{
	set_radius(15);
	set_position(50,100);
	circle_texture.loadFromFile("vlc.png");
	circle_texture.setSmooth(true);
	btn_circle.setTexture(&circle_texture);
	btn_font.loadFromFile("Courier New Bold Italic.ttf");
	btn_title.setFont(btn_font);
}

void mini_play :: draw(sf::RenderWindow *window)
{
	window->draw(btn_circle);
	window->draw(btn_title);
}

void mini_play :: set_position(float x, float y)
{
	btn_circle.setPosition(x,y);
}

bool mini_play :: is_pushed(sf::RenderWindow *window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
		    sf::Vector2i cursor = sf::Mouse::getPosition((*window));
		    sf::Vector2f position = btn_circle.getPosition();
		    float x = position.x;
		    float y = position.y;
		    if(((cursor.x - x)*(cursor.x - x) + (cursor.y - y)*(cursor.y - y))<= 15)
			{
				return true;
			}
		}
	return false;
}


void mini_play :: set_title(const std::wstring &str, int title_size)
{
    sf::Vector2f position = btn_circle.getPosition();
	float R = btn_circle.getRadius();

	position.y -= 70;

	sf::Rect<float> text_rect;
	btn_title.setString(str);
	btn_title.setCharacterSize(title_size);
	btn_title.setColor(sf::Color::Black);

	text_rect = btn_title.getLocalBounds();
	float x = text_rect.width;
	float y = text_rect.height;
	btn_title.setOrigin(x/2,y/2);
	btn_title.setPosition(position);
}
//-----------------------------------------------

class exit_btn: public application_btn{
public:
	exit_btn():application_btn(){};
	void react(base_window *);
};

class redraw_btn: public application_btn{
public:
	redraw_btn():application_btn(){};
	void react(base_window *);
};

class settings_btn: public application_btn{
public:
	settings_btn():application_btn(){};
	void react(base_window *);
};

class prompts_btn: public application_btn{
public:
	prompts_btn():application_btn(){};
	void react(base_window *);
};

//------------------------------------------------------

class game_btn : public application_btn{
public:
	game_btn():application_btn(){};
	void react(base_window *);
};

class game_prompt_btn : public application_btn{
public:
	game_prompt_btn():application_btn(){};
	void react(base_window *);
};

//--------------------------------------------------

class save_btn: public application_btn{
public:
	save_btn():application_btn(){};
	void react(base_window *);
};

class get_back21_btn: public application_btn{
public:
	get_back21_btn():application_btn(){};
	void react(base_window *);
};


//==========================================================
//реализация методов класса кнопок
application_btn :: application_btn()
{
	sf::Vector2f btn_size(300,144);
	btn_shape.setOrigin(btn_size.x/2,btn_size.y/2);
	btn_shape.setPosition(200,350);
	btn_shape.setSize(btn_size);
	btn_texture.loadFromFile("exit_btn.png");
	btn_texture.setSmooth(true);
	btn_shape.setTexture(&btn_texture);
	btn_font.loadFromFile("Courier New Bold Italic.ttf");
	btn_title.setFont(btn_font);
}

void application_btn :: set_position(float x, float y)
{
	 btn_shape.setPosition(x,y);
}

void application_btn :: set_radius(float r)
{
	btn_circle.setRadius(r);
	btn_circle.setOrigin(r,r);
}

void application_btn :: draw(sf::RenderWindow *window)
{
	window->draw(btn_shape);
	window->draw(btn_title);
}

void application_btn :: set_texture(sf::Texture new_texture)
{
	btn_texture = new_texture;
	btn_texture.setSmooth(true);
	btn_shape.setTexture(&btn_texture);
}

void application_btn :: set_title(const std::wstring &str, int title_size)
{
	sf::Vector2f position = btn_shape.getPosition();
	sf::Rect<float> text_rect;

	btn_title.setString(str);
	btn_title.setCharacterSize(title_size);
	btn_title.setColor(sf::Color::Black);

	text_rect = btn_title.getLocalBounds();
	float x = text_rect.width;
	float y = text_rect.height;
	btn_title.setOrigin(x/2,y/2);

	btn_title.setPosition(position);
	btn_title.move(0,-10); //костыль
}

void application_btn :: set_size(float x, float y)
{
	sf::Vector2f size(x,y);
	btn_shape.setSize(size);
	btn_shape.setOrigin(x/2,y/2);
}

void application_btn :: move(float dx, float dy)
{
	btn_shape.move(dx,dy);
}

bool application_btn :: is_pushed(sf::RenderWindow *window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
		    sf::Vector2i cursor = sf::Mouse::getPosition((*window));
		    sf::Vector2f position = btn_shape.getPosition();
		    bool x_flag = ((cursor.x >= position.x - 90)&&(cursor.x <= position.x + 90));
		    bool y_flag = ((cursor.y >= position.y -30)&&(cursor.y <= position.y+30));
		    if(x_flag && y_flag)
				return true;
		}
	return false;
}

//=================================================================================================================
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
        support_part_t (char key_symb, int key_number, float time_start, float time_stop, sf::Font* font);
        int draw (sf::Time time_c,  base_window& window ) ;
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
        //support_team_t(int* input_key_map, sf::Font* font);
        void init_support_team(int* input_key_map, sf::Font* font);
	    void draw ( sf::Time time_c, sf::Event event, base_window& window );
        int get_score(){
            return score;
        }
        ~support_team_t() {
        delete[] support;
        }
};

//==================================================================================================
//================================== класс базовой страницы ====================================

class base_window : public sf::RenderWindow
{
public:
	static sf::Texture win_texture;
	static sf::Font win_font;
	int status;
	std::vector<application_btn *> buttons;
	base_window();
	bool react_if_pushed();
	void draw_itself();
	void set_volume();
	void play_sound(sf::Event);
	void draw_sup(sf::Event);

	void set_parametrs(float, float, const std::wstring &, const sf::String &,
					   const std::wstring &, int, sf::Color);
	//void set_win_texture();
	sf::Sprite win_sprite;

	sf::Clock win_clock;
	sound_commander_t sound_commander;
    writer_t writer;
	listener_t listener;
	support_team_t support_team;

	sf::Text win_text;
    ~base_window();
protected:
	void OnCreate();

};

sf::Texture base_window :: win_texture;
sf::Font base_window :: win_font;


void base_window :: draw_sup(sf::Event event)
{
	support_team.draw(win_clock.getElapsedTime(), event, (*this));
}
void base_window :: set_volume()
{
	sound_commander.volume_check();
}

void base_window :: play_sound(sf::Event event)
{
	if( event.key.code >= 0 && event.key.code < 57 ) {
            if(sound_commander.check(event) != 0 && status == 1 ) {
                writer.remember(win_clock.getElapsedTime(), event);
              }
         }
}

base_window :: base_window()
{
	OnCreate();
}

void base_window :: set_parametrs(float x, float y, const std::wstring &title, const sf::String &image_path,
								  const std::wstring &text_string, int text_size, sf::Color text_color)
{
	create(sf::VideoMode(x, y), title ,sf::Style::Default);
	win_texture.loadFromFile(image_path);
    win_sprite.setTexture(win_texture);
	win_font.loadFromFile("Courier New Bold Italic.ttf");
	win_text.setFont(win_font);

	sf::Vector2f pos;
	pos.x = x/2;
	pos.y = y/2;
	sf::Rect<float> text_rect;
	win_text.setString(text_string);
	win_text.setCharacterSize(text_size);
	win_text.setColor(text_color);

	text_rect = win_text.getLocalBounds();
	float w = text_rect.width;
	float h = text_rect.height;
	win_text.setOrigin(w/2,h/2);
	win_text.setPosition(pos);
}



bool base_window :: react_if_pushed()
{
	 for(int i = 0; i < buttons.size(); ++i)
		{
			if(buttons[i]->is_pushed(this))
		    {
		    	buttons[i]->react(this);
				return true;
		    }
		}
	return false;
}


void base_window :: OnCreate()
{
	create(sf::VideoMode(1120, 700), L"Добро пожаловать!",sf::Style::Default);
	setKeyRepeatEnabled(false); //отключили залипание
	win_texture.loadFromFile("11.jpg");
    win_sprite.setTexture(win_texture);
	win_font.loadFromFile("Courier New Bold Italic.ttf");
	win_text.setFont(win_font);
	status = 0;
	buttons.resize(3);

	buttons[0] = new redraw_btn();
	buttons[1] = new settings_btn();
	buttons[2] = new exit_btn();

	for(int j = 0; j < buttons.size(); ++j)
    	buttons[j]->set_position(200,350 + j*80);

	buttons[0]->set_title(L"Игра",25);
	buttons[1]->set_title(L"Настройки",25);
	buttons[2]->set_title(L"Выход",25);

	//-------------------------------------------
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
	support_team.init_support_team(test_mask, &win_font );
    sound_commander.init_sound_commander(paths.size(), key_map, paths);
    writer.init_writer(37, key_map);
}

void base_window :: draw_itself()
{
     draw(win_sprite);
	 draw(win_text);
	 for(int i = 0; i < buttons.size(); ++i)
		buttons[i]->draw(this);
}

base_window :: ~base_window()
{
	for(int i = 0; i < buttons.size(); ++i)
		delete buttons[i];
}


//============================  реакции кнопок (должны находится после определения класса окна)=======================

void exit_btn :: react(base_window *window)
{
	window->close();
}

void mini_play :: react(base_window *window)
{
	//проиграть звуковой файл, привязанный к кнопке
}


void redraw_btn :: react(base_window *window)
{
	window->set_parametrs(1120,700,L"Игра","form2.jpg",L"",10,sf::Color::Black);
	window->buttons.resize(3);

	window->buttons[0] = new game_prompt_btn();
	window->buttons[1] = new game_btn();
	window->buttons[2] = new get_back21_btn();

	for(int j = 0; j < window->buttons.size(); ++j)
	{
		window->buttons[j]->set_size(500,200);
    	window->buttons[j]->set_position(560,300 + j*100);
	}

	window->buttons[0]->set_title(L"Игра с подсказками", 20);
	window->buttons[1]->set_title(L"Произвольная игра", 20);
	window->buttons[2]->set_title(L"Назад",20);
    window->status = 0;
	window->listener.end_recording();
	window->listener.save_file("my_record.ogg");

}

void game_btn :: react(base_window *window)
{
	window->set_parametrs(1120,700,L"Произвольная игра","basa.jpg",L"Добро пожаловать в игровой режим!",50,sf::Color::White);

	window->buttons.resize(1);
	window->buttons[0] = new redraw_btn();
	window->buttons[0]->set_position(1120/2-150,600);
	window->buttons[0]->set_title(L"Назад",25);

     //свободный игровой режим
	window->listener.start_recording();
	window->status = 1;
	window->win_clock.restart();
    std::cout<<window->win_clock.getElapsedTime().asSeconds()<<std::endl;
}

void prompts_btn :: react(base_window *window)
{
/*	window->set_parametrs(1120,700,L"Игра по подсказкам", "basa1.jpg",L"",
						 30,sf::Color::White);

	window->buttons.resize(1);
	window->buttons[0] = new redraw_btn();

	window->buttons[0]->set_position(1120/2-150,600);
	window->buttons[0]->set_title(L"Вернуться",25);

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
	window->support_team.init_support_team(test_mask, &(window->win_font) );
    window->sound_commander.init_sound_commander(paths.size(), key_map, paths);
    //window->writer.init_writer(37, key_map);
	window->listener.start_recording();
	window->status = 2;
    window->win_clock.restart();*/

}

void game_prompt_btn :: react(base_window *window)
{
	window->set_parametrs(1120,700,L"Игра по подсказкам", "fon2.jpg",L"Какую мелодию вы хотите научиться играть?)",
						 30,sf::Color::White);

	window->buttons.resize(1);
	window->buttons[0] = new redraw_btn();
//	window->buttons[1] = new prompts_btn();

	window->buttons[0]->set_position(1120/2-150,600);
	window->buttons[0]->set_title(L"Назад",25);
//	window->buttons[1]->set_position(1120/2 + 160,600);
//	window->buttons[1]->set_title(L"Начать игру!",25);



	int test_mask[37] = {25, 23, 3, 2, 5, 21, 1, 7, 13, 9, 12, 10, 49,
    50, 48, 52, 51, 16, 22, 29, 4, 30, 17, 31,
    19, 24, 33, 20, 34, 8, 14, 26, 15, 56, 46, 55, 47 } ;
	window->support_team.init_support_team(test_mask, &(window->win_font) );
	window->listener.start_recording();
	window->status = 2;
    window->win_clock.restart();
    std::cout<<window->win_clock.getElapsedTime().asSeconds()<<std::endl;
}

void save_btn :: react(base_window *bwindow)
{
	base_window window;
	window.set_parametrs(400,200,L"Сообщение", "mini2.jpg",L"Настройки успешно сохранены!",20,sf::Color::White);

	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

	    if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
		}
            if (event.type == sf::Event::Closed)
                window.close();

        }

        window.clear();
		window.draw_itself();
        window.display();
    }
}

void get_back21_btn :: react(base_window *window)
{
	window->set_parametrs(1120,700,L"Добро пожаловать!","11.jpg",L"",30,sf::Color::Black);
	window->buttons.resize(3);

	window->buttons[0] = new redraw_btn();
	window->buttons[1] = new settings_btn();
	window->buttons[2] = new exit_btn();

	for(int j = 0; j < window->buttons.size(); ++j)
    	window->buttons[j]->set_position(200,350 + j*80);

	window->buttons[0]->set_title(L"Игра",25);
	window->buttons[1]->set_title(L"Настройки",25);
	window->buttons[2]->set_title(L"Выход",25);
}

void settings_btn :: react(base_window * window)
{
	window->set_parametrs(1120,700,L"Настройки","fon2.jpg",L"",30,sf::Color::Black);

	window->buttons.resize(39);
	window->buttons[0] = new save_btn;
	window->buttons[1] = new get_back21_btn;

	for(int i = 2; i < 39; ++i)
	   window->buttons[i] = new mini_play;

	window->buttons[0]->set_position(400,600);
	window->buttons[1]->set_position(710,600);

	window->buttons[0]->set_title(L"Применить",25);
	window->buttons[1]->set_title(L"Назад",25);

	for(int i = 2; i < 20; ++i)
	{
	  window->buttons[i]->set_position(70 + 55*(i-2), 100);
	  window->buttons[i]->set_title(L"A ",30);
	}

	for(int i = 20; i < 39; ++i)
	{
	  window->buttons[i]->set_position(70+ 55*(i-20), 300);
	  window->buttons[i]->set_title(L"A ",30);
	}

}

//--------------------------------------------------------------------------------------------------

int main()
{
    base_window base;
	sf::RenderWindow *window = &base;


    while (window->isOpen())
    {
        sf::Event event;

		if(base.status)
		base.set_volume();

        while (window->pollEvent(event))
        {

	    if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Escape)
			{
				window->close();
			}
		}
			if(base.status)
			base.play_sound(event);

	  /*  if( event.key.code >= 0 && event.key.code < 57 ) { //
            if(sound_commander.check(event) != 0 ) {            //
                writer.remember(clock.getElapsedTime(), key_map[event.key.code]); //
              }   //
         }  //*/


         if (event.type == sf::Event::Closed)
                window->close();

        }

		base.react_if_pushed();

		window->clear();

		base.draw_itself();
		if(base.status==2)
		base.draw_sup(event);

		window->display();

    }

    return 0;
}


//==============================================реализация аудио-классов ====================================

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
/*sound_commander_t::sound_commander_t(int number_of_sounds, std::vector<int> input_map, std::vector<std::string> paths) { //конструктор
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
} //объект создан и готов работать*/
void sound_commander_t::init_sound_commander(int number_of_sounds, std::vector<int> input_map, std::vector<std::string> paths) { //конструктор
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
void writer_t::init_writer (int number_of_keys, std::vector<int> input_map ) { //массив с кодами кнопок из sfml
    sequence = new std::vector<float>[number_of_keys]; //это не наша нумерация кнопок для звуков
    sequence_size = number_of_keys;
	for ( int i = 0; i < input_map.size(); ++i ) {
        key_map.push_back(input_map[ i ]);
    } //я обращаюсь к 25му индексу (код Z) и вижу 0, потому что к Z привязан нулевой звук

}
void writer_t::remember ( sf::Time time, sf::Event event) {
	int key_code =key_map[event.key.code];
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
support_part_t::support_part_t (char key_symb, int key_number, float time_start, float time_stop, sf::Font* font) {
    y = 20;
    x = 30 * (key_number ) + 20;
    time_pressed = time_start; //время, когда кнопка должна будет быть нажата
    time_elapsed = time_stop;
    body.setRadius(20);
    body.setFillColor(sf::Color(100, 250, 50));
    MyFont.loadFromFile("arial.ttf");
    letter.setString(key_symb);
    letter.setFont(*font);
    letter.setCharacterSize(24);
    letter.setColor(sf::Color::Red);
    tail_length = (int ) 200 * (time_elapsed - time_pressed) ;
    tail.setSize(sf::Vector2f( tail_length , 4));
    tail.rotate(90);
    tail.setFillColor(sf::Color(100, 250, 50)); //это все движение вокруг подсказок
}
int support_part_t::draw (sf::Time time_c,  base_window& window ) {
    if ( (time_pressed >= time_c.asSeconds() + 3.175 ) || (time_elapsed < time_c.asSeconds()  ) ) { //еще слишком рано что-то рисовать
        return 0; //или уже слишком поздно
    }
    int status;
    if ( time_pressed < time_c.asSeconds() + 3.175 && time_pressed >= time_c.asSeconds() ) {//самое время
        y = 15 + (int)(200 * (time_c.asSeconds() + 3.175 - time_pressed ));
        body.setPosition(x, y);
        letter.setPosition( x + 10, y + 5 );
        tail.setPosition(x+22, y-tail_length);
        status = 0;
    }
    if (time_pressed < time_c.asSeconds() && time_elapsed > time_c.asSeconds() ) { //самое время
        body.setOutlineThickness(3);
        body.setOutlineColor(sf::Color(250, 150, 100));
        body.setPosition( x, 650 );
        letter.setPosition( x + 10, 655 );
        tail.setPosition(x+22, y - (int) (tail_length * (time_elapsed - time_c.asSeconds() ) / (time_elapsed - time_pressed) ) );
        tail.setSize(sf::Vector2f(  (int) (tail_length * (time_elapsed - time_c.asSeconds() ) / (time_elapsed - time_pressed) ), 4));
        status = 1;
    }
    window.draw(tail);
    window.draw(body);
    window.draw(letter);
    return status;
}
/*support_team_t::support_team_t(int* input_key_map, sf::Font* font) {
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
}*/
void support_team_t::init_support_team(int* input_key_map, sf::Font* font) {
    std::ifstream infile;
    infile.open("writer_test_final.txt");
    int temp;
    float f_tmp_1, f_tmp_2;
    infile >> temp;
    std::cout<<temp<<std::endl;
    support = new std::vector<support_part_t>[temp];
    support_size = temp;
    char key_symb[37] = {'Z','X','D','C','F','V','B','H','N','J','M','K',',','.',';','/','\'','Q','W','3','E','4','R','5','T','Y','7','U','8','I','O','0','P','-','[','=',']'};
    for ( int i = 0; i < support_size; ++i ) {
        infile >> temp;
        for ( int j = 0; j < temp/2; ++j ) {
            infile >> f_tmp_1;
            infile >> f_tmp_2;
            support_part_t temp_sup(key_symb[i],i,f_tmp_1, f_tmp_2, font);
            support[ i ].push_back(temp_sup);
        }
    }
    infile.close();
    score = 0;
    key_map = input_key_map;
}
void support_team_t::draw ( sf::Time time_c, sf::Event event, base_window& window ) {
    for ( int i = 0; i < support_size; ++i ) {
        bool must_be_pressed = 0;
        for ( int j = 0; j < support[ i ].size(); ++j ){
            int tmp = support[i][j].draw(time_c, window);
			must_be_pressed = must_be_pressed || tmp;
        }
        if( sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::A + key_map[i])) ) {
            if(must_be_pressed == true) {++score;}
        }
    }
}//складывается абсурдная ситуация, когда подсчетом очков должен заниматься графический класс

