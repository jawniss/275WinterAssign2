#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <SD.h>
#include "consts_and_types.h"
#include "map_drawing.h"
#include <stdio.h>
#include <string.h>

// the variables to be shared across the project, they are declared here!
shared_vars shared;

Adafruit_ILI9341 tft = Adafruit_ILI9341(clientpins::tft_cs, clientpins::tft_dc);

// max size of buffer, including null terminator
// const uint16_t buf_size = 256;
// current number of chars in buffer, not counting null terminator
// uint16_t buf_len = 0;

// input buffer
// char* buffer = (char *) malloc(buf_size);


void setup() {
  // initialize Arduino
  init();

  // initialize zoom pins
  pinMode(clientpins::zoom_in_pin, INPUT_PULLUP);
  pinMode(clientpins::zoom_out_pin, INPUT_PULLUP);

  // initialize joystick pins and calibrate centre reading
  pinMode(clientpins::joy_button_pin, INPUT_PULLUP);
  // x and y are reverse because of how our joystick is oriented
  shared.joy_centre = xy_pos(analogRead(clientpins::joy_y_pin), analogRead(clientpins::joy_x_pin));

  // initialize serial port
  Serial.begin(9600);


  // set up buffer as empty string
  // buf_len = 0;
  // buffer[buf_len] = 0;

  Serial.flush(); // get rid of any leftover bits

  // initially no path is stored
  shared.num_waypoints = 0;

  // initialize display
  shared.tft = &tft;
  shared.tft->begin();
  shared.tft->setRotation(3);
  shared.tft->fillScreen(ILI9341_BLUE); // so we know the map redraws properly

  // initialize SD card
  if (!SD.begin(clientpins::sd_cs)) {
      Serial.println("Initialization has failed. Things to check:");
      Serial.println("* Is a card inserted properly?");
      Serial.println("* Is your wiring correct?");
      Serial.println("* Is the chipSelect pin the one for your shield or module?");

      while (1) {} // nothing to do here, fix the card issue and retry
  }

  // initialize the shared variables, from map_drawing.h
  // doesn't actually draw anything, just initializes values
  initialize_display_values();

  // initial draw of the map, from map_drawing.h
  draw_map();
  draw_cursor();

  // initial status message
  status_message("FROM?");
}

void process_input() {
  // read the zoom in and out buttons
  shared.zoom_in_pushed = (digitalRead(clientpins::zoom_in_pin) == LOW);
  shared.zoom_out_pushed = (digitalRead(clientpins::zoom_out_pin) == LOW);

  // read the joystick button
  shared.joy_button_pushed = (digitalRead(clientpins::joy_button_pin) == LOW);

  // joystick speed, higher is faster
  const int16_t step = 64;

  // get the joystick movement, dividing by step discretizes it
  // currently a far joystick push will move the cursor about 5 pixels
  xy_pos delta(
    // the funny x/y swap is because of our joystick orientation
    (analogRead(clientpins::joy_y_pin)-shared.joy_centre.x)/step,
    (analogRead(clientpins::joy_x_pin)-shared.joy_centre.y)/step
  );
  delta.x = -delta.x; // horizontal axis is reversed in our orientation

  // check if there was enough movement to move the cursor
  if (delta.x != 0 || delta.y != 0) {
    // if we are here, there was noticeable movement

    // the next three functions are in map_drawing.h
    erase_cursor();       // erase the current cursor
    move_cursor(delta);   // move the cursor, and the map view if the edge was nudged
    if (shared.redraw_map == 0) {
      // it looks funny if we redraw the cursor before the map scrolls
      draw_cursor();      // draw the new cursor position
    }
  }
}

void drawingPath (){

}
//

// void process_line() {
//   // print what's in the buffer back to server
//
//   string temp = buffer[0];
//   if (temp == "N") {
//     Serial.print("A");
//     Serial.print("\n");
//
//   }
//   Serial.print("Got: ");
//   Serial.println(buffer);
//
//   // clear the buffer
//   buf_len = 0;
//   buffer[buf_len] = 0;
// }


int main() {
  setup();

  // very simple finite state machine:
  // which endpoint are we waiting for?
  enum {WAIT_FOR_START, WAIT_FOR_STOP} curr_mode = WAIT_FOR_START;

  // the two points that are clicked
  lon_lat_32 start, end;

  while (true) {
    // clear entries for new state
    shared.zoom_in_pushed = 0;
    shared.zoom_out_pushed = 0;
    shared.joy_button_pushed = 0;
    shared.redraw_map = 0;

    // reads the three buttons and joystick movement
    // updates the cursor view, map display, and sets the
    // shared.redraw_map flag to 1 if we have to redraw the whole map
    // NOTE: this only updates the internal values representing
    // the cursor and map view, the redrawing occurs at the end of this loop
    process_input();

    // if a zoom button was pushed, update the map and cursor view values
    // for that button push (still need to redraw at the end of this loop)
    // function zoom_map() is from map_drawing.h
    if (shared.zoom_in_pushed) {
      zoom_map(1);
      shared.redraw_map = 1;
    }
    else if (shared.zoom_out_pushed) {
      zoom_map(-1);
      shared.redraw_map = 1;
    }

    // if the joystick button was clicked
    if (shared.joy_button_pushed) {

      if (curr_mode == WAIT_FOR_START) {
        // if we were waiting for the start point, record it
        // and indicate we are waiting for the end point
        start = get_cursor_lonlat();
        curr_mode = WAIT_FOR_STOP;
        status_message("TO?");

        // wait until the joystick button is no longer pushed
        while (digitalRead(clientpins::joy_button_pin) == LOW) {}
        delay(300);
      }
      else {
        // if we were waiting for the end point, record it
        // and then communicate with the server to get the path
        end = get_cursor_lonlat();

        bool pathisdone = false;
        int w_counter;
        // TODO: communicate with the server to get the waypoints
        while(pathisdone == false) {
        Serial.print("R");
        Serial.print(" ");
        Serial.print(start.lat);
        Serial.print(" ");
        Serial.print(start.lon);
        Serial.print(" ");
        Serial.print(end.lat);
        Serial.print(" ");
        Serial.println(end.lon);
        Serial.print("\n");
        status_message("FROM?");

        while (true) {
          char input_str[129];
          int used = 0;
          char *input_split;
          char in_char;
          while (true) {
            while (Serial.available() == 0);
              // read the incoming byte:
              input_str[used] = Serial.read();
              used++;

              // if end of line is received, waiting for line is done:
              if (input_str[used-1] == '\n') {
                  // now we process the buffer
                  input_str[used - 1] = '\0';
                  used = 0;
                  break;
              }
          }


        input_split = strtok(input_str, " ");

        if (input_split[0] == 'N') {
          int waypointnumber = input_split[1];
          input_split = strtok(NULL, " ");
          Serial.print("A");
          Serial.print("\n");
        }
        else if (input_split[0] == 'W'){
          input_split = strtok(NULL," ");
          uint32_t lat = atol(input_split);
          input_split = strtok(NULL," ");
          uint32_t lon = atol(input_split);
          // store into shared variable
          shared.waypoints[w_counter] = lon_lat_32(lon,lat);
          Serial.println('A'); // send ack
          w_counter++;
       }
       // else if (input_split[0] == 'E') {
       //    break;
       //  }
     }
   }
        // now we have stored the path length in
        // shared.num_waypoints and the waypoints themselves in
        // the shared.waypoints[] array, switch back to asking for the
        // start point of a new request
        curr_mode = WAIT_FOR_START;

        // wait until the joystick button is no longer pushed
        while (digitalRead(clientpins::joy_button_pin) == LOW) {}

      }
    }

    if (shared.redraw_map) {
      // redraw the status message
      if (curr_mode == WAIT_FOR_START) {
        status_message("FROM?");
      }
      else {
        status_message("TO?");
      }

      // redraw the map and cursor
      draw_map();
      draw_cursor();

      // TODO: draw the route if there is one
    }
  }

  Serial.flush();
  return 0;
}
/*
Skip to content

Search or jump to…

Pull requests
Issues
Marketplace
Explore
 @Ricky-Au Sign out
0
0 0 LiyaoJiang1998/Driving_Route_Finder
 Code  Issues 0  Pull requests 0  Projects 0  Wiki  Insights
Driving_Route_Finder/client/client.cpp
 cmput274 Completed Project uploaded
1c88a3b  on Mar 26, 2018
324 lines (278 sloc)  11.9 KB

// Student Name: Liyao Jiang
// Student ID: 1512445
// Section: EB1
// Student Name: Xiaolei Zhang
// Student ID: 1515335
// Section: B1
// Assignment1 Part2

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <SD.h>
#include "consts_and_types.h"
#include "map_drawing.h"

// the variables to be shared across the project, they are declared here!
shared_vars shared;

Adafruit_ILI9341 tft = Adafruit_ILI9341(clientpins::tft_cs, clientpins::tft_dc);

void setup() {
  // initialize Arduino
  init();

  // initialize zoom pins
  pinMode(clientpins::zoom_in_pin, INPUT_PULLUP);
  pinMode(clientpins::zoom_out_pin, INPUT_PULLUP);

  // initialize joystick pins and calibrate centre reading
  pinMode(clientpins::joy_button_pin, INPUT_PULLUP);
  // x and y are reverse because of how our joystick is oriented
  shared.joy_centre = xy_pos(analogRead(clientpins::joy_y_pin), analogRead(clientpins::joy_x_pin));

  // initialize serial port
  Serial.begin(9600);
  Serial.flush(); // get rid of any leftover bits

  // initially no path is stored
  shared.num_waypoints = 0;

  // initialize display
  shared.tft = &tft;
  shared.tft->begin();
  shared.tft->setRotation(3);
  shared.tft->fillScreen(ILI9341_BLUE); // so we know the map redraws properly

  // initialize SD card
  if (!SD.begin(clientpins::sd_cs)) {
      Serial.println("Initialization has failed. Things to check:");
      Serial.println("* Is a card inserted properly?");
      Serial.println("* Is your wiring correct?");
      Serial.println("* Is the chipSelect pin the one for your shield or module?");

      while (1) {} // nothing to do here, fix the card issue and retry
  }

  // initialize the shared variables, from map_drawing.h
  // doesn't actually draw anything, just initializes values
  initialize_display_values();

  // initial draw of the map, from map_drawing.h
  draw_map();
  draw_cursor();

  // initial status message
  status_message("FROM?");
}

void process_input() {
  // read the zoom in and out buttons
  shared.zoom_in_pushed = (digitalRead(clientpins::zoom_in_pin) == LOW);
  shared.zoom_out_pushed = (digitalRead(clientpins::zoom_out_pin) == LOW);

  // read the joystick button
  shared.joy_button_pushed = (digitalRead(clientpins::joy_button_pin) == LOW);

  // joystick speed, higher is faster
  const int16_t step = 64;

  // get the joystick movement, dividing by step discretizes it
  // currently a far joystick push will move the cursor about 5 pixels
  xy_pos delta(
    // the funny x/y swap is because of our joystick orientation
    (analogRead(clientpins::joy_y_pin)-shared.joy_centre.x)/step,
    (analogRead(clientpins::joy_x_pin)-shared.joy_centre.y)/step
  );
  delta.x = -delta.x; // horizontal axis is reversed in our orientation

  // check if there was enough movement to move the cursor
  if (delta.x != 0 || delta.y != 0) {
    // if we are here, there was noticeable movement

    // the next three functions are in map_drawing.h
    erase_cursor();       // erase the current cursor
    move_cursor(delta);   // move the cursor, and the map view if the edge was nudged
    if (shared.redraw_map == 0) {
      // it looks funny if we redraw the cursor before the map scrolls
      draw_cursor();      // draw the new cursor position
    }
  }
}

// Original code from drawing path
// takes the start and end point as argument
void draw_path(lon_lat_32 start,lon_lat_32 end){
  // I used longitude_to_x and longitude_to_y provided to get the coordinate in the edmonton map
  // and minus the map_coords to get the coordinate relative the left up corner of display

  //draw line from start point to first waypoint
  int32_t startx = longitude_to_x(shared.map_number,start.lon) - shared.map_coords.x;
  int32_t starty = latitude_to_y(shared.map_number,start.lat) - shared.map_coords.y;
  int32_t endx = longitude_to_x(shared.map_number,shared.waypoints[0].lon) - shared.map_coords.x;
  int32_t endy = latitude_to_y(shared.map_number,shared.waypoints[0].lat) - shared.map_coords.y;
  //we do not want it to draw the expected area on the display
  starty = constrain(starty,0,216);
  endy = constrain(endy,0,216);
  shared.tft-> drawLine(startx,starty,endx,endy,ILI9341_BLUE);
  // shared.tft-> drawLine(longitude_to_x(shared.map_number, start.lon)-shared.map_coords.x,
  // latitude_to_y(shared.map_number,start.lat)-shared.map_coords.y,
  // longitude_to_x(shared.map_number,shared.waypoints[0].lon)-shared.map_coords.x,
  // latitude_to_y(shared.map_number, shared.waypoints[0].lat)-shared.map_coords.y,ILI9341_BLUE);

  //Draw lines between all the intermediate waypoints
  for(int k = 0; k<(shared.num_waypoints-1); k++){
    //same drawing method
    int32_t startway_x = longitude_to_x(shared.map_number,shared.waypoints[k].lon) - shared.map_coords.x;
    int32_t startway_y = latitude_to_y(shared.map_number,shared.waypoints[k].lat) - shared.map_coords.y;
    int32_t endway_x = longitude_to_x(shared.map_number,shared.waypoints[k+1].lon) - shared.map_coords.x;
    int32_t endway_y = latitude_to_y(shared.map_number,shared.waypoints[k+1].lat) - shared.map_coords.y;
    startway_y = constrain(startway_y,0,216);
    endway_y = constrain(endway_y,0,216);
    shared.tft-> drawLine(startway_x,startway_y,endway_x,endway_y,ILI9341_BLUE);
  }

  //Draw last line between end and previous waypoint
  startx = longitude_to_x(shared.map_number,shared.waypoints[shared.num_waypoints-1].lon) - shared.map_coords.x;
  starty = latitude_to_y(shared.map_number,shared.waypoints[shared.num_waypoints-1].lat) - shared.map_coords.y;
  endx = longitude_to_x(shared.map_number,end.lon)-shared.map_coords.x;
  endy = latitude_to_y(shared.map_number,end.lat)-shared.map_coords.y;
  starty = constrain(starty,0,216);
  endy = constrain(endy,0,216);
  shared.tft-> drawLine(startx,starty,endx,endy,ILI9341_BLUE);
}

int main() {
  setup();

  // very simple finite state machine:
  // which endpoint are we waiting for?
  enum {WAIT_FOR_START, WAIT_FOR_STOP} curr_mode = WAIT_FOR_START;

  // the two points that are clicked
  lon_lat_32 start, end;

  while (true) {
    // clear entries for new state
    shared.zoom_in_pushed = 0;
    shared.zoom_out_pushed = 0;
    shared.joy_button_pushed = 0;
    shared.redraw_map = 0;

    // reads the three buttons and joystick movement
    // updates the cursor view, map display, and sets the
    // shared.redraw_map flag to 1 if we have to redraw the whole map
    // NOTE: this only updates the internal values representing
    // the cursor and map view, the redrawing occurs at the end of this loop
    process_input();

    // if a zoom button was pushed, update the map and cursor view values
    // for that button push (still need to redraw at the end of this loop)
    // function zoom_map() is from map_drawing.h
    if (shared.zoom_in_pushed) {
      zoom_map(1);
      shared.redraw_map = 1;
    }
    else if (shared.zoom_out_pushed) {
      zoom_map(-1);
      shared.redraw_map = 1;
    }

    // if the joystick button was clicked
    if (shared.joy_button_pushed) {

      if (curr_mode == WAIT_FOR_START) {
        // if we were waiting for the start point, record it
        // and indicate we are waiting for the end point
        start = get_cursor_lonlat();
        curr_mode = WAIT_FOR_STOP;
        status_message("TO?");

        // wait until the joystick button is no longer pushed
        while (digitalRead(clientpins::joy_button_pin) == LOW) {}
      }
      else {
        // if we were waiting for the end point, record it
        // and then communicate with the server to get the path
        end = get_cursor_lonlat();

        // TODO: communicate with the server to get the waypoints
        // Original code starts here!
        // Initialization of flags and counter
        bool success = false;
        bool receive_n = false;
        int16_t w_counter = 0;

        // loop until we get the path successfully
        while(!success){
          //send out R line with the start and end coordinates
          Serial.print("R ");
          Serial.print(start.lat);
          Serial.print(" ");
          Serial.print(start.lon);
          Serial.print(" ");
          Serial.print(end.lat);
          Serial.print(" ");
          Serial.println(end.lon);
          Serial.setTimeout(10000);

          while(true){
            //initialize read line buffer
            char input_str[129];
            int used = 0;
            char * input_split;

            // read one line
            while (true) {
              while (Serial.available() == 0);
              // in each iteration, one byte is read into buffer
              input_str[used] = Serial.read();
              ++used;
              // when end of line is read, put the null terminater
              // and break the reading loop
              if (input_str[used-1] == '\n') {
                input_str[used-1] = '\0';
                used = 0;
                break;
              }
            }

            // parse the input_str(buffer) line by space
            // using strok function (standard library)
            input_split = strtok(input_str," ");

            // if the first token is N, and the conditions checks
            // that our program is in the state expecting N
            if((input_split[0] == 'N') && !receive_n && !success){
              // get the second token as num_waypoints
              input_split = strtok(NULL," ");
              shared.num_waypoints = atoi(input_split);
              if(shared.num_waypoints == 0 || shared.num_waypoints > 500){
                success = false;
                receive_n = false;
                w_counter = 0;
                break; // if there is no path or N>500 then reset
              }
              receive_n = true;
              // send Ack
              Serial.println('A');
              Serial.setTimeout(1000);
            }
            // if the first token is W, and the conditions checks
            // that our program is in the state expecting W
            else if((input_split[0] == 'W') && receive_n && !success && w_counter < shared.num_waypoints){
              // get the second and third tokens as lat and lon
              input_split = strtok(NULL," ");
              uint32_t lat = atol(input_split);
              input_split = strtok(NULL," ");
              uint32_t lon = atol(input_split);
              // store into shared variable
              shared.waypoints[w_counter] = lon_lat_32(lon,lat);
              Serial.println('A'); // send ack
              w_counter++;
            }
            // if the first token is E, and the conditions checks
            // that our program is in the state expecting E
            // The state expecting E is when N and all W messages is received
            else if((input_split[0] == 'E') && receive_n && !success && w_counter == shared.num_waypoints) {
              success = true;
              break;
              // after we successfully read all the data, we continue to the next request
            }
            // otherwise, go back to resend request again
            else{
              success = false;
              receive_n = false;
              w_counter = 0;
              break;
            }
          }
        }

        // now we have stored the path length in
        // shared.num_waypoints and the waypoints themselves in
        // the shared.waypoints[] array, switch back to asking for the
        // start point of a new request
        curr_mode = WAIT_FOR_START;
        shared.redraw_map = 1; // we need to redraw the path

        // wait until the joystick button is no longer pushed
        while (digitalRead(clientpins::joy_button_pin) == LOW) {}
      }
    }

    if (shared.redraw_map) {
      // redraw the status message
      if (curr_mode == WAIT_FOR_START) {
        status_message("FROM?");
      }
      else {
        status_message("TO?");
      }

      // redraw the map and cursor
      draw_map();
      draw_cursor();

      // TODO: draw the route if there is one
      draw_path(start,end); // calling the function draw_path in this file
    }
  }

  Serial.flush();
  return 0;
}
© 2019 GitHub, Inc.
Terms
Privacy
Security
Status
Help
Contact GitHub
Pricing
API
Training
Blog
About
*/
