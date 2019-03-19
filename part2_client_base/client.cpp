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

int wavepoints;

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

// user defined function that takes in the beginning and end lon/lats to communicate with server
void communication(lon_lat_32 begin, lon_lat_32 last){
  bool pathisdone = false;
  int waveCounter;
  // TODO: communicate with the server to get the waypoints
  // while loop that continues unless we have finished all communications
  while(pathisdone == false) {
    // print statements that send the R and the lat/lons along with the newline
    Serial.flush();
    Serial.print("R");
    Serial.print(" ");
    Serial.print(begin.lat);
    Serial.print(" ");
    Serial.print(begin.lon);
    Serial.print(" ");
    Serial.print(last.lat);
    Serial.print(" ");
    Serial.println(last.lon);
    Serial.print("\n");
    Serial.flush();
    status_message("recieving path ...");
    // sets the maximum milliseconds to wait for serial data to 10 seconds when waiting for number of points
    Serial.setTimeout(10000);
    while (pathisdone == false) {
      // charecter array that will store the inputs
      char line[500];
      int words = 0;
      char *split;
      while (true) {
        while (Serial.available() == 0);
          // read in words from serial.read
          line[words] = Serial.read();
          words++;
          // if newline is found
          if (line[words-1] == '\n') {
              // process buffer
              line[words - 1] = '\0';
              words = 0;
              break;
          }
      }
      // split strings into tokens using space as delimeter
      split = strtok(line, " ");
      // if case if first value read in is a N
      if (split[0] == 'N') {
        split = strtok(NULL, " ");
        // convert the char into a string
        String waypointnumber = String(split);
        // convert the string to an integer
        wavepoints = waypointnumber.toInt();
        // send an A as acknowledgement
        Serial.flush();
        Serial.print("A\n");
        // sets the maximum milliseconds to wait for serial data for 1 second
        Serial.setTimeout(1000);
        Serial.flush();
      }
      // case if a W is recieved
      else if (split[0] == 'W'){
        split = strtok(NULL," ");
        // convert the latitude into int32_t
        int32_t lat = atol(split);
        split = strtok(NULL," ");
        // convert the longintude into int32_t
        int32_t lon = atol(split);
        // store into shared variable along with a counter that stores the lon lat sent over from server
        shared.waypoints[waveCounter] = lon_lat_32(lon,lat);
        Serial.flush();
        // send acknowledgement after you recieve lon lat
        Serial.print("A\n");
        // sets the maximum milliseconds to wait for serial data for 1 second for new point
        Serial.setTimeout(1000);
        Serial.flush();
        waveCounter++;
      }
      // case to end if an E is read
      else if (split[0] == 'E') {
        waveCounter = 0;
        pathisdone = true;
      }
      else {
        // break and restart if we don't recieve the correct letter (should never happen)
        waveCounter = 0;
        break;
      }
    }
  }
}

// function that draws the lines of the values sent to us from server
void drawLines(lon_lat_32 start,lon_lat_32 end){
  // loop that draws the whole path
  for(int i = 1; i < (wavepoints); i++){
    int32_t startingX = (longitude_to_x(shared.map_number,shared.waypoints[i].lon) - shared.map_coords.x);
    int32_t startingY = (latitude_to_y(shared.map_number,shared.waypoints[i].lat) - shared.map_coords.y);
    int32_t endingX = (longitude_to_x(shared.map_number,shared.waypoints[i+1].lon) - shared.map_coords.x);
    int32_t endingY = (latitude_to_y(shared.map_number,shared.waypoints[i+1].lat) - shared.map_coords.y);
    // case that only draws the lines between vertexes if they are within the tft display and not on the status bar
    if ((startingX < 320) && (startingX > 0) && (endingX < 320) && (endingX > 0) && (startingY < 216) && (startingY > 0) && (endingY < 216) && (endingY > 0)){
      shared.tft-> drawLine(startingX,startingY,endingX,endingY,ILI9341_BLUE);
    }
  }
}

// main function that loops and manages the button presses and calls the communication and drawing functions
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
        // needed this since sometimes button would count two clicks for just one
        delay(300);
      }
      else {
        // if we were waiting for the end point, record it
        // and then communicate with the server to get the path
        end = get_cursor_lonlat();

        // after recieving a two points we may begin communication with server
        communication(start,end);

        // now we have stored the path length in
        // waypointnumber and the waypoints themselves in
        // the shared.waypoints[] array, switch back to asking for the
        // start point of a new request
        curr_mode = WAIT_FOR_START;
        shared.redraw_map = 1;
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
      // draw the route
      drawLines(start,end);
    }
  }

  Serial.flush();
  return 0;
}
