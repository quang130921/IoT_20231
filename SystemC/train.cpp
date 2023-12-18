//Khai bao thu vien 
#include "systemc.h"
#include <iostream>

// Khai bao cac trang thai 
enum train_state {IDLE, SPEEDUP, SLOWDOWN, RUNNING, STOP};
enum door_state {OPEN, CLOSE /*OPENING, , CLOSING*/ };

// Khai bao module train cho khoi dieu khien
SC_MODULE(train) {
  // Khai bao cac cong vao ra cho module
  sc_in<bool> clk; // Cong vao cho xung clock
  sc_in<bool> automatic; // Cong vao cho nut khoi dong
  sc_in<bool> sen_human; // Cong vao cam bien nguoi
  sc_out<bool> speed_up; // Cong ra cho trang thai tang toc
  sc_out<bool> running; // Cong ra cho trang thai dang duy tri toc do
  sc_out<bool> slow_down; // Cong ra cho trang thai giam toc
  sc_out<bool> stop; // Cong ra cho trang thai dung do
  sc_out<bool> door_open; // Cong ra cho mo cua
  sc_out<bool> door_close; // Cong ra cho dong cua
  //sc_out<bool> door_opening; // Cong ra cho cua dang mo
  //sc_out<bool> door_closing; // Cong ra cho ca dang dong

  // Khai bao cac bien noi bo cho module
  train_state current_state; // Bien luu trang thai hien tai cua tau
  door_state current_door; // Bien luu trang thai hien tai cua cua
  int current_speed; // Bien luu toc do hien tai cua tau
  int max_speed; // Bien luu toc do nguong cua tau
  int target_station; // Bien luu tram dich cua tau
  int time_running; // Bien luu thoi gian duy tri toc do
  int time_wait; // Bien luu tru thoi gian cho tai ben
  // Khai bao ham khoi tao cho module
  SC_CTOR(train) {
    // Khoi tao cac bien noi bo va cac gia tri ban dau
    current_state = STOP; // Tau o trang thai doi
    current_door = CLOSE; // Cua thang may dong
    current_speed = 0; // Toc do hien tai cua tau
    max_speed = 6; // Toc do nguong cua tau 1 
    target_station = 1; // Luu san tram dich la 1
    time_running = 0; // Thoi gian duy tri mac dinh = 0 giây
    time_wait = 0; //Thoi gian cho tai ben
    // Dang ki ham xu ly chinh theo moi xung clock
    SC_METHOD(process);
    sensitive << clk.pos();
  }

  // Khai bao ham xu ly chinh cho module
  void process() {
    // Neu nut automatic duoc kich hoat thi xu ly cac yeu cau:
    if (automatic.read() == true) {
      // Xu ly theo trang thai hien tai cua tau
      switch (current_state) {
        case IDLE: // Neu tau dang cho
        
        if(time_wait < 5){
            time_wait++;
        }
        else{
          // Thay doi tram dich luon phien giua 1 va 2
          target_station = (target_station == 1) ? 2 : 1;
            current_state = SPEEDUP;
          	close_door();
            time_wait =0;
        }
        
          break;
        case SPEEDUP: // Neu tau dang tang toc
          // Neu toc do hien tai da bang toc do nguong, dung tang toc
          if (current_speed == max_speed) {
            speed_up.write(false); // Dung trang thai tang toc
            current_state = RUNNING; // Chuyen sang trang thai duy tri toc do
          }
          // Neu toc do hien tai chua bang, tiep tuc tang toc
          else {
            speed_up.write(true); // Tiep tuc duy tri trang thai tang toc
            current_speed++; // Tang toc do hien tai len 1
          }
          break;
        case RUNNING: // Neu tau dang trang thai duy tri toc do
        // Neu thoi gian duy tri toc do > 5 giay, dung duy tri toc do
        if(time_running < 3){
          running.write(true); // Tiep tuc duy tri trang thai duy tri toc do
          time_running++; // Tang thoi gian duy tri len 1
        }
        else {
          running.write(false); // Dung trang thai duy tri toc do
          current_state = SLOWDOWN;// Chuyen sang trang thai giam toc
          time_running = 0;
        }
        	break;
        case SLOWDOWN: // Neu tau dang giam toc
          // Neu toc do giam toi 0, dung giam toc
          if (current_speed == 0) {
            slow_down.write(false); // Dung giam toc
            current_state = STOP; // Chuyen sang trang thai dung
          }
          // Neu toc do chua giam xuong 0 thi tiep tuc giam
          else {
            slow_down.write(true); // Tiep tuc duy tri giam toc
            current_speed -= 2; // Giam toc do hien tai di 1
          }
          break;
        case STOP: // Neu tau dang o trang thai dung
        	stop.write(false); // Dung trang thai dung
        	open_door();
        	current_state = IDLE; // Chuyen sang trang thai cho
        
        break;
      }
      //Hien thi ra cac trang thai cua tau, tram dich, tram hien tai
      cout    <<"[" <<  sc_time_stamp()<<"]"<< "Current State: " << train_state_to_string(current_state)
              << ", Door state: " << door_state_to_string(current_door)
              << ", Target Station: " << target_station << endl;
    }
  }
  // Ham mo cua 
  void open_door() {
    // Neu cua dang dong
    if (current_door == CLOSE) {
      // Dat trang thai cua cua thanh OPEN
      current_door = OPEN;
      // Dat tin hieu mo cua la TRUE
      door_open.write(true);
      //Dat tin hieu dong cua la FALSE
      door_close.write(false);

    }
    // Neu cua dang mo tthi giu cua mo va ngat tin hieu mo cua
    else if (current_door == OPEN){
      current_door = OPEN;
      door_open.write(false);
    }
  }

  // Ham dong cua
  void close_door() {
    // Neu cua dang mo
    if (current_door == OPEN) {
      // Dat trang thai cua thanh CLOSE
      current_door = CLOSE;
      // Dat tin hieu dong cua thanh true
      door_close.write(true);
      door_open.write(false);

    }
    // Neu canh cua dang dong
    else if (current_door == CLOSE) {
      // Giu trang thai cua cua la CLOSE
      current_door = CLOSE;
      // Dat tin hieu mo cua thanh FALSE
      door_close.write(false);
    }
  }

  /*// Ham dang mo cua
  void opening_door(){
    // Neu cua dang dong
    if (current_door == CLOSE) {
      // Dat trang thai thanh OPENING
      current_door = OPENING;
      // Dat tin hieu OPENING thanh true
       door_opening.write(true);
      // Dat tin hieu CLOSE thanh false
      door_close.write(false);
    }
    // Neu cua dang mo, giu nguyen trang thai, tat tin hieu dang mo
    else if (current_door == OPENING) {
      current_door = OPENING;
      door_opening.write(false);
    }
    // Neu cua dang dong, giu nguyen trang thai, tat tin hieu dang mo
    else if (current_door == CLOSING) {
        current_door = CLOSING;
      door_opening.write(false);
    }
    // Neu cua mo, giu nguyen trang thai, tat tin hieu dang mo
    else if (current_door == OPEN){
        current_door = OPEN;
      door_opening.write(false);
    }
    
      
  }
  // Ham mo cua 
  void open_door() {
    // Neu cua dang mo
    if (current_door == OPENING) {
      // Dat trang thai cua cua thanh OPEN
      current_door = OPEN;
      // Dat tin hieu mo cua la TRUE
      door_open.write(true);
      //Dat tin hieu dang mo cua la FALSE
      door_opening.write(false);
    }
     // Neu cua mo, giu nguyen trang thai, tat tin hieu mo
    else if (current_door == OPEN){
      current_door = OPEN;
      door_open.write(false);
    }
    // Neu cua dong, giu nguyen trang thai, tat tin hieu mo
     else if (current_door == CLOSE){
      current_door = CLOSE;
      door_open.write(false);
    }
     // Neu cua dang dong, giu nguyen trang thai, tat tin hieu mo
     else if (current_door == CLOSING){
      current_door = CLOSING;
      door_open.write(false);
    }
  }

  // Ham dong cua
  void close_door() {
    // Neu cua dang dong
    if (current_door == CLOSING) {
      // Dat trang thai cua thanh CLOSE
      current_door = CLOSE;
      // Dat tin hieu dong cua thanh true, tat tin hieu dang dong cua
      door_close.write(true);
      door_closing.write(false);
      if(sen_human.read()==true){
        current_door = OPENING;
        door_opening.write(true);
        door_close.write(false);
      }
    }
    // Neu canh cua dang mo, giu nguyen trang thai, tat tin hieu dong
    else if (current_door == OPENING) {
      current_door = OPENING;
      door_close.write(false);
    }
    // Neu canh cua mo, giu nguyen trang thai, tat tin hieu dong
    else if (current_door == OPEN) {
      current_door = OPEN;
      door_close.write(false);
    }
    // Neu canh cua dong, giu nguyen trang thai, tat tin hieu dong
    else if (current_door == CLOSE) {
      current_door = CLOSE;
      door_close.write(false);
    }
  }
 // Ham dang dong cua
  void closing_door() {
    // Neu cua mo
    if (current_door == OPEN) {
      // Dat trang thai cua thanh CLOSING
      current_door = CLOSING;
      // Dat tin hieu dong cua thanh true, tat tin hieu dang dong cua
      door_closing.write(true);
      door_open.write(false);
      if(sen_human.read()==true){
        current_door = OPENING;
        door_opening.write(true);
        door_closing.write(false);
      }
    }
    // Neu canh cua dong, giu nguyen trang thai, tat tin hieu dang dong
    else if (current_door == CLOSE) {
      current_door = CLOSE;
      door_closing.write(false);
    }
    // Neu canh cua dang dong, giu nguyen trang thai, tat tin hieu dang dong
    else if (current_door == CLOSING) {
      current_door = CLOSING;
      door_closing.write(false);
    }
    // Neu canh cua dang mo, giu nguyen trang thai, tat tin hieu dang dong
    else if (current_door == OPENING) {
      current_door = OPENING;
      door_closing.write(false);
    }
  }
*/
  // Ham chuyen doi enum train_state thanh chuoi
  const char* train_state_to_string(train_state state) {
      switch (state) {
          case IDLE:
              return "IDLE";
          case SPEEDUP:
              return "SPEEDUP  ";
          case RUNNING:
              return "RUNNING";
          case SLOWDOWN:
              return "SLOWDOWN";
        case STOP:
        	return "STOP";
          default:
              return "UNKNOWN_STATE";
      }
  }
  // Ham chuyen doi enum door_state thanh chuoi
  const char* door_state_to_string(door_state state) {
    switch (state) {
      case OPEN:
            return "OPEN ";
        case CLOSE:
            return "CLOSE";
      
       /* case OPENING:
            return "OPENING ";
      
      case CLOSING:
            return "CLOSING ";*/
        default:
            return "UNKNOWN_STATE";
    }
  }

};