#ifndef ORGRAPH_H_
#define ORGRAPH_H_

#include "widget.h"
#include "pktStruct.h"
#include "globals.h"

#define GRAPH_ARRAY_SIZE 60

class orGraph{
  public:
    void update(PACKET p){
      if ( ++idx > GRAPH_ARRAY_SIZE ){
        idx = 0;
        drawGraph(_graph);
      }      
      int v1 = map ( p.voltage, 0, 100, _graph.y + _graph.h - 5, _graph.y );
      int v2 = map ( p.current, 0, 150, _graph.y + _graph.h - 5, _graph.y );
      value1 = value;
      value = { v1, v2 };
      
      updateGraph(value);
      updateStatus(p);
    };

    void init(Adafruit_GFX* tft, WIDGET w){
      _tft = tft;
//      _w = w;
      _status = { 0, 0, w.w, w.h / 4 ,RED};
      _graph = { 0, _status.h, w.w, _status.h * 3 ,RED};
      drawStatus(_status);
      drawGraph(_graph);
    };
  private:
    void updateStatus(PACKET p){
//      tft.setTextSize(3);
      _tft->setCursor(_status.x + 5 + 30, _status.h / 4);
      _tft->fillRect(_status.x + 5 + 30, _status.h / 4, _status.x + 5 + 30 + 100, _status.h / 2, BLACK);
      if ( 15 > p.voltage) {
        _tft->setTextColor(RED);
      } else {
        _tft->setTextColor(WHITE);
      };      
      _tft->print(p.voltage);
      _tft->setCursor(_status.x + 5 + _status.w / 2 + 30, _status.h / 4);      
      _tft->fillRect(_status.x + 5 + _status.w / 2 + 30, _status.h / 4, _status.x + 5 + _status.w / 4, _status.h / 2, BLACK);
      _tft->print(p.current);
    };
    void updateGraph(PACKET p){
      int t = ( _graph.w - 5 ) / GRAPH_ARRAY_SIZE;
      int x00 = ( _graph.x + 5 ) + ( ( idx - 1 ) * t );
      int y00 = value1.voltage;
      int x01 = ( _graph.x + 5 ) + idx * t;
      int y01 = p.voltage;

      int x10 = ( _graph.x + 5 ) + ( ( idx - 1 ) * t );
      int y10 = value1.current;
      int x11 = ( _graph.x + 5 ) + idx * t;
      int y11 = p.current;
      plotLine(_graph.x + 5 +x00, y00, _graph.x + 5 + x01, y01, PINK );
      plotLine(_graph.x + 5 +x10, y10, _graph.x + 5 + x11, y11, CYAN );      
    };
    void drawStatus(WIDGET w){
  //  tft.fillRect(w.x, w.y, w.w, w.h, w.color);
      _tft->drawRect(w.x, w.y, w.w, w.h, w.color); 
      _tft->setTextSize(3);
      _tft->setCursor(_status.x + 5, _status.h / 4);
      _tft->print("V: ");
      _tft->setCursor(_status.x + 5 + _status.w / 2, _status.h / 4);
      _tft->print("I: ");
    };
    void drawGraph(WIDGET _w){
      _tft->fillRect(_w.x+6, _w.y, _w.w-6, _w.h-6, BLACK);      
      _tft->drawFastVLine(_w.x + 5, _w.y, _w.y + _w.h, WHITE);
      _tft->drawFastHLine(_w.x, _w.y + _w.h - 5, _w.w, WHITE );
    };
    int idx = 1;
    Adafruit_GFX* _tft;
    WIDGET _graph, _status;
//    PACKET values[GRAPH_ARRAY_SIZE];
    PACKET value = {0,0};
    PACKET value1 = {0,0};
};

#endif  // ORGRAPH_H_
