#include <FastLED.h>

#define x_jostik_pin A1
#define y_jostik_pin A2
#define push_jostik_pin 2

#define rgb_pin 9


enum State {UP, DOWN, LEFT, RIGHT, NONE};
// функция для джостика: loop() читает координаты джостика и функция определяет и возвращает направление джостика
// в качестве аргумента принимает считываемые значения с джостика
State click(int pinX, int pinY) {
  if ((pinX > 256 && pinX < 768) && (pinY > 1000)) {
    return UP;
  } else if ((pinX > 256 && pinX < 768) && (pinY < 100)) {
    return DOWN;
  } else if ((pinY > 256 && pinY < 768) && (pinX < 100)) {
    return RIGHT;
  } else if ((pinY > 256 && pinY < 768) && (pinX > 1000)) { 
    return LEFT;
  } else {
    return NONE;
  };
};


// класс с логикой игры 
class Field {
  public: 
  char field[3][3] = {{'v', 'v', 'v'},
                      {'v', 'v', 'v'},
                      {'v', 'v', 'v'}};
  int cursor_str = 0; // индекс символов внутри массивов (x координата)
  int cursor_column = 0; // индекс массивов (y координата)
  // функция cursor() двигает виртуальный курсор в зависимости от манипуляций джостиком
  // в качестве аргумента передается результат функции click() 
  void cursor(State command){
    if (command == UP && cursor_column != 0){
      cursor_column -= 1;
    } else if (command == DOWN && cursor_column != 2){
      cursor_column += 1;
    } else if (command == LEFT && cursor_str != 0) {
      cursor_str -= 1;
    } else if (command == RIGHT && cursor_str != 2) { 
      cursor_str += 1;
      };
  };

  char players[2] = {'X', 'O'};
  int index_player = 0;
  int index_start_player = 0;


  // функция, меняющая стартующего в следующем раунде игрока после завершения текущего 
  void swich_start_player (){
    if(index_start_player == 0){
      index_start_player += 1;
    } else if (index_start_player == 1){
      index_start_player -= 1;
    };
    index_player = index_start_player;
  };

  // функция меняет индекс игрока, swich_player() плюс simbol() влияют на раскладку field[3][3], которая влияет на отображаемую раскладку на RGB
  void swich_player(){
    if(index_player == 0){
      index_player +=1;
    } else if (index_player == 1){
      index_player -= 1;
    };
  };

  // функция меняет пустую клетку в зависимости от хода игрока и возвращает статус успеха кода если ход был - true, если нет - false
  bool simbol(){
    if (field[cursor_column][cursor_str] == 'v'){
      field[cursor_column][cursor_str] = players[index_player];
      return true;
    } else {
      return false;;
    };
  };

  int win_combination[8][3][2] = {
    {{0, 0}, {0, 1}, {0, 2}},
    {{1, 0}, {1, 1}, {1, 2}},
    {{2, 0}, {2, 1}, {2, 2}},
    {{0, 0}, {1, 0}, {2, 0}},
    {{0, 1}, {1, 1}, {2, 1}},
    {{0, 2}, {1, 2}, {2, 2}},
    {{0, 0}, {1, 1}, {2, 2}},
    {{0, 2}, {1, 1}, {2, 0}}
  };

  enum Win_states {X_WIN, O_WIN, NOBODY, NOT_END};

  int win_combination_index = 0;

  // функция возвращает статус выигрыша. от нее зависит продолжится ли игра или нет.
  Win_states win_or_not(){
    Win_states result = NOBODY;
    bool end = true;
    bool have_winner = false;
    for (int i = 0; i < 8 && have_winner == false; i++){
      int column_first_el = win_combination[i][0][0];
      int str_first_el = win_combination[i][0][1];
      char first_el = field[column_first_el][str_first_el];
      int column_second_el = win_combination[i][1][0];
      int str_second_el = win_combination[i][1][1];
      char second_el = field[column_second_el][str_second_el];
      int column_third_el = win_combination[i][2][0];
      int str_third_el = win_combination[i][2][1];
      char third_el = field[column_third_el][str_third_el];
      if (first_el == 'v' || second_el == 'v' || third_el == 'v'){
        end = false;
      } else if (first_el == second_el && second_el == third_el) {
        if (first_el == 'X'){
          result = X_WIN;
          have_winner = true;
          win_combination_index = i;
        } else if (first_el == 'O'){
          result = O_WIN;
          have_winner = true;
          win_combination_index = i;
        };
      };      
    };
    if (end == false && have_winner == false){
       result = NOT_END;
    };
    return result;
  };



 
  int current_round = 0;
  // функция меняет раунд, очищает виртуальное поле, ставит виртуальный курсор на начальную позицию
  void next_round(){
    current_round += 1;
    cursor_str = 0;
    cursor_column = 0;

    for (int i = 0; i < 9; i++){
      field[cursor_column][cursor_str] = 'v';
      cursor_str += 1;
      if (cursor_str == 3){
      cursor_str = 0;
      cursor_column += 1;
      };
    };

    cursor_str = 0;
    cursor_column = 0;
    swich_start_player();
  };
};



// как вариант в setup () изначально при включении объявлять игрока Х и int round = 0
// и при завершении раунда увеличивать переменную, продумать логику смены игороков, либо крестик и нолик поочередно начинает, либо победитель
// в rgb добавить подсветку внешнего поля цветом текущего игрока
// а сам rgb прописать не как функцию, а как класс-наследник, он будет наследовать текушее поле и курсор у класса Field
// в rgb при анимации курсора создать 2 массива, которые будут а-ля python словарем: 
  //1 массив будет хранить переменные cursor_str и cursor_column, второй массив будет хрнить значение от 1 до 9, в соответствии с пози




class RGB_matrix{
  public:

  // задает базовые настройки матрицы
  CRGB leds[256];

  void init(){
    FastLED.addLeds<WS2812, rgb_pin, GRB>(leds, 256); // rgb_pin объявлен в начале кода (номер 9)
    FastLED.setBrightness(11);
    FastLED.clear(true);
    FastLED.show();
  };
  
  // функция возвращает индекс от 0 до 8, для понимания стартовой позиции в клетке в функции start_cell()
  // в качестве аргументов передавать в функцию переменные cursor_str и cursor_column из класса Field
  int xyindex_to_index(int x, int y){
    int index = y * 3 + x;
    return index;
  };
 
  int real_rgb_index[9][2] = {{2, 2}, {7, 2}, {12, 2}, {2, 7}, {7, 7}, {12, 7}, {2, 12}, {7, 12}, {12, 12}}; // индексы X и Y первой верхней клетки в ячейках поля 3*3 (по порядку от 1ой до 9ой согласно таблице)

  // функция при вводе x и y по таблице возвращает индекс в матрице
  int XY(int x, int y) {
    int index = 0;
    if (y % 2 == 0) {  // значит, согласно таблице, строка начинается с четного
        index = 16 * (y - 1) + x - 1;
    }
    else if (y % 2 > 0) { // значит, согласно таблице, строка начинается с нечетного
        index = 16 * y - x;
    };
    return index;
};


  // функция принимает в качестве параметра результата функции xyindex_to_index() и он дальше используется для получения координат первой клетки в блоке и прорисовки рисунка в ней
  // в качестве параметра принимает еще цвет(яркий красный или нет), не имеет FastLED.show() - надо вызывать отдельно 
  void draw_x(int coordinate, CRGB color){
    int Xbase = real_rgb_index[coordinate][0];
    int Ybase = real_rgb_index[coordinate][1];
    leds[XY(Xbase, Ybase)] = color;
    leds[XY(Xbase + 3, Ybase)] = color;
    leds[XY(Xbase + 1, Ybase + 1)] = color;
    leds[XY(Xbase + 2, Ybase + 1)] = color;
    leds[XY(Xbase + 1, Ybase + 2)] = color;
    leds[XY(Xbase + 2, Ybase + 2)] = color;
    leds[XY(Xbase, Ybase + 3)] = color;
    leds[XY(Xbase + 3, Ybase+ 3)] = color;   
  };

  // функция принимает в качестве параметра результата функции xyindex_to_index() и он дальше используется для получения координат первой клетки в блоке и прорисовки рисунка в ней
  // в качестве параметра принимает еще цвет(яркий голубой или нет), не имеет FastLED.show() - надо вызывать отдельно 
  void draw_o(int coordinate, CRGB color){
    int Xbase = real_rgb_index[coordinate][0];
    int Ybase = real_rgb_index[coordinate][1];
    leds[XY(Xbase + 1, Ybase)] = color;
    leds[XY(Xbase + 2, Ybase)] = color;
    leds[XY(Xbase, Ybase + 1)] = color;
    leds[XY(Xbase + 3, Ybase + 1)] = color;
    leds[XY(Xbase, Ybase + 2)] = color;
    leds[XY(Xbase + 3, Ybase + 2)] = color;
    leds[XY(Xbase + 1, Ybase + 3)] = color;
    leds[XY(Xbase + 2, Ybase + 3)] = color;
  };

  // функция обновления матрицы, стирает весь рисунок. В качестве аргументов принимает x и y, внутри преобразовывает через функцию XY, постепенно прибавляя по одному значению к X и Y. Не имеет функцию FastLED.show(), надо вызывать отдельно 
  void draw_v(int coordinate_x, int coordinate_y){
    for (int i = 0; i  < 4; i++){
      for (int o = 0; o < 4; o++){
        leds[XY(coordinate_x + o, coordinate_y + i)] = CRGB::Black;
      };
    };

  };

  // функция рисует сетку от четырех угловых точек центральной клетки
  void lines(){
    int coordinates[4][2] = {{6, 6}, {11, 6}, {6, 11}, {11, 11}};
    for(int i = 0; i < 5; i++){
      for(int z = 0; z < 4; z++){
        leds[XY(coordinates[z][0] - i, coordinates[z][1])] = CRGB::Yellow;
        leds[XY(coordinates[z][0] + i, coordinates[z][1])] = CRGB::Yellow;
        leds[XY(coordinates[z][0], coordinates[z][1] - i)] = CRGB::Yellow;
        leds[XY(coordinates[z][0], coordinates[z][1] + i)] = CRGB::Yellow;
      };
      FastLED.show();
      delay(350);
    };
  };

  // функция запускает начальную анимацию границ поля
  void circle_start_animation(int start_x = 1, int start_y = 1){
    while (start_x != 16){
      int coordinate = XY(start_x, start_y);
      leds[coordinate] = CRGB::Yellow;
      start_x++;
      FastLED.show();
      delay(500);
    };
    while (start_y != 16) {
      int coordinate = XY(start_x, start_y);
      leds[coordinate] = CRGB::Yellow;
      start_y++;
      FastLED.show();
      delay(500);
    };
    while (start_x != 1) {
      int coordinate = XY(start_x, start_y);
      leds[coordinate] = CRGB::Yellow;
      start_x--;
      FastLED.show();
      delay(500);
    };
    while (start_y != 1) {
      int coordinate = XY(start_x, start_y);
      leds[coordinate] = CRGB::Yellow;
      start_y--;
      FastLED.show();
      delay(500);
    };
  };
  
  
  // функция запускает завершающую анимацию. в качестве аргументов принимает статус завершения(тип enum Win_states в классе Field) 
  // и числовые индексы-координаты верхних левых клеток победивших клеток для подсветки их
  // если есть победитель функция подсвечивает 3мя миганиями победные клетки цветом победителя, если победителя нет - пропускает этот шаг
  // далее функция убирает сетку обратной анимацией (от края поля к четырем угловым точкам центральной клетки)
  // далее каждая клетка поочередно подсвечивается белым цветом, затирая тем самым все ходы
  // далее все, кроме внешней границы поля гаснет
  void end_animation(Field::Win_states state, int first_index, int second_index, int third_index){
    auto lines_revers = [&](){
      int coordinates[4][2] = {{6, 6}, {11, 6}, {6, 11}, {11, 11}};
      for(int i = 4; i > -1; i--){
        for(int z = 0; z < 4; z++){
          leds[XY(coordinates[z][0] - i, coordinates[z][1])] = CRGB::Black;
          leds[XY(coordinates[z][0] + i, coordinates[z][1])] = CRGB::Black;
          leds[XY(coordinates[z][0], coordinates[z][1] - i)] = CRGB::Black;
          leds[XY(coordinates[z][0], coordinates[z][1] + i)] = CRGB::Black;
        };
        FastLED.show();
        delay(350);
      };
    };
    
    auto draw = [&](CRGB color){
      int first_cells_list[3] = {first_index, second_index, third_index};
      for (int i = 0; i < 3; i++){
        for (int y = 0; y < 4; y++){
          for (int x = 0; x < 4; x++){
            leds[XY(real_rgb_index[first_cells_list[i]][0] + x, real_rgb_index[first_cells_list[i]][1] + y)] = color;
          };
        };
      };
    };
    

    CRGB win_color = CRGB::Red;
    if (state == Field::O_WIN){
      win_color = CRGB::Blue;
    };
    

    if (state != Field::NOBODY){
      for (int o = 0; o < 3; o++){
        draw(win_color);
        FastLED.show();
        delay(500);
        draw(CRGB::Black);
        FastLED.show();
        delay(500);
      };
      draw(win_color);
      FastLED.show();
      delay(1300);
      draw(CRGB::Black);
      FastLED.show();
      delay(500);
    };
    
    lines_revers();
    
    for (int i = 0; i < 9; i++){
      for (int y = 0; y < 4; y++){
        for (int x = 0; x < 4; x++){
          leds[XY(real_rgb_index[i][0] + x, real_rgb_index[i][1] + y)] = CRGB::White;
        };
      };
      FastLED.show();
      delay(200);
    };
    
    delay(1000);

    for (int y = 0; y < 14; y++){
      for (int x = 0; x < 14; x++){
        leds[XY(2 + x, 2 + y)] = CRGB::Black;
      };
    };
    FastLED.show();
    delay(500);
  };

};



RGB_matrix show;
Field game;



int first_winner = 0;
int second_winner = 0;
int third_winner = 0;
void revers_win_combination_to_index(){
  auto xy = [&](int x, int y){
    int result = y * 3 + x;
    return result;
  };
  first_winner = xy(game.win_combination[game.win_combination_index][0][0], game.win_combination[game.win_combination_index][0][1]);
  second_winner = xy(game.win_combination[game.win_combination_index][1][0], game.win_combination[game.win_combination_index][1][1]);
  third_winner = xy(game.win_combination[game.win_combination_index][2][0], game.win_combination[game.win_combination_index][2][1]);
};



void virtual_matrix_to_rgb(){
  int counter = 0;
  for (int i = 0; i < 3; i++){
    for (int o = 0; o < 3; o++){
      char simbol = game.field[i][o];
      bool cursor = false;
      if (i == game.cursor_column && o == game.cursor_str){
        cursor = true;
      };
      if (simbol == 'v'){
        if (cursor == true){
          if (game.index_player == 0){
            show.draw_x(counter, CRGB::DarkRed);
          } else if (game.index_player == 1){
            show.draw_o(counter, CRGB::DarkBlue);
          };
        } else { 
        show.draw_v(show.real_rgb_index[counter][0], show.real_rgb_index[counter][1]);
        };
      } else if (simbol == 'X'){
        if (cursor == true){
          show.draw_x(counter, CRGB(255, 40, 40));
        } else {
          show.draw_x(counter, CRGB::Red);
        };
      } else if (simbol == 'O'){
        if (cursor == true){
          show.draw_o(counter, CRGB(80, 200, 255));
        } else {
        show.draw_o(counter, CRGB::Blue);
        }
      };
      counter++;
    };
  };
  FastLED.show();
};



void setup () {
  pinMode(x_jostik_pin, INPUT);
  pinMode(y_jostik_pin, INPUT);
  pinMode(push_jostik_pin, INPUT);
  show.init();
  show.circle_start_animation();
  delay(1000);
  show.lines();
}



// для кнопки
int last_buttom_state = 0;
int current_buttom_state = 0;


void loop () {
  int pinX = analogRead(x_jostik_pin);
  int pinY = analogRead(y_jostik_pin);
  State cursor_move = click(pinX, pinY);
  game.cursor(cursor_move);
  virtual_matrix_to_rgb();

  //логика кнопки
  if (digitalRead(push_jostik_pin) == HIGH){
    current_buttom_state = 1;
  } else if (digitalRead(push_jostik_pin) == LOW){
    current_buttom_state = 0;
  };
  
  if (current_buttom_state == 1 and last_buttom_state == 0){
    bool move = game.simbol();
    if (move == true){
      Field::Win_states state = game.win_or_not();
      if (state != Field::NOT_END && state != Field::NOBODY){
        revers_win_combination_to_index();
        show.end_animation(state, first_winner, second_winner, third_winner);
        delay(500);
        game.next_round();
        show.lines();
      } else if (state == Field::NOBODY){
        show.end_animation(state, first_winner, second_winner, third_winner);
        delay(500);
        game.next_round();
        show.lines();
      } else {
      game.swich_player();
      };
    };
  };
  //

  last_buttom_state = current_buttom_state;



}