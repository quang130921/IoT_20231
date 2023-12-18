#include "systemc.h"
#include "train.cpp"

// Ham main
int sc_main(int argc, char* argv[]) {
  // Khai bao cac doi tuong
  sc_clock clk("clk", 1, SC_SEC); // Xung nhip 1 giay
  sc_signal<bool> automatic; // Tin hieu khoi dong
  sc_signal<bool> sen_human; // TIn hieu cam bien nguoi
  sc_signal<bool> speed_up; // Tin hieu tang toc
  sc_signal<bool> running; // Tin hieu duy tri toc do
  sc_signal<bool> slow_down; // Tin hieu giam toc
  sc_signal<bool> stop; // Tin hieu dung
  sc_signal<bool> door_open; // Tin hieu mo cua
  sc_signal<bool> door_close; // Tin hieu dong cua
 // sc_signal<bool> door_closing; // Tin hieu dang  mo cua
  //sc_signal<bool> door_opening; // Tin hieu dang dong cua

  // Khoi tao module tau
  train e("train");
  // Ket noi cac cong vao/ra
  e.clk(clk);
  e.automatic(automatic);
  e.sen_human(sen_human);
  e.speed_up(speed_up);
  e.running(running);
  e.slow_down(slow_down);
  e.stop(stop);
  e.door_open(door_open);
  e.door_close(door_close);
  //e.door_opening(door_opening);
  //e.door_closing(door_closing);

  // Thiet lap cac tham so mo phong
  sc_trace_file* tf = sc_create_vcd_trace_file("train"); // Tao file VCD de xem wave form
  sc_trace(tf, clk, "clk"); // Theo doi xung clk
  sc_trace(tf, automatic, "automatic"); // Theo doi tin hieu khoi dong
  sc_trace(tf, sen_human, "sen_human"); // Theo doi tin hieu cam bien nguoi
  sc_trace(tf, speed_up, "speed_up"); // Theo doi tin hieu tang toc
  sc_trace(tf, running, "running"); // Theo doi tin hieu duy tri toc do
  sc_trace(tf, slow_down, "slow_down"); // Theo doi tin hieu giam toc
  sc_trace(tf, stop, "stop"); // Theo doi tin hieu dung
  sc_trace(tf, door_open, "door_open"); // Theo doi tin hieu mo cua
  sc_trace(tf, door_close, "door_close"); // Theo doi tin hieu dong cua
  //sc_trace(tf, door_opening, "door_opening"); // Theo doi tin hieu dang mo cua
  //sc_trace(tf, door_closing, "door_closing"); // Theo doi tin hieu dang dong cua

  // Bat dau mo phong
  sc_start(1, SC_SEC);
  automatic.write(true); // Dat tin hieu khoi dong la true
  sc_start(3, SC_SEC); 
  sen_human.write(false);
  //Mo phong 
    sc_start(30, SC_SEC); // Chay he thong trong 30 giay
    sc_close_vcd_trace_file(tf); 
  //Ket thuc mo phong
  return 0;
}