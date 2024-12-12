import processing.serial.*;

Serial myPort;  // Create object from Serial class
String buffer;

PVector pos = new PVector(700, 0);
int dist = round(pos.mag());
float speed = 0;

String dogState = "";

float scale = 0.5;

void setup()
{
  size(800, 800);

  // print the list of available serial ports
  println(Serial.list());


  //String portName = Serial.list()[0];
  String portName = Serial.list()[Serial.list().length - 1];
  myPort = new Serial(this, portName, 9600);

  println(pos.x, pos.y);

  frameRate(10);
}

void draw() {

  // read from the serial port
  while (myPort.available() > 0) {
    char c = myPort.readChar();
    if (c == '\n') {
      println(buffer);
      onReceivedLine(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  translate (width / 2, height / 2);
  scale(scale);
  background(255);

  // distances
  noFill();
  stroke(200, 0, 0);
  circle(0, 0, 200 * 2);
  stroke(200);
  circle(0, 0, 350 * 2);
  stroke(0);
  circle(0, 0, 500 * 2 );

  // DOG
  fill(0);
  textAlign (CENTER, CENTER);
  textSize(40);
  text("DOG\n" + dogState, 0, 0);


  // Pos
  fill(0);
  stroke(0);
  circle(pos.x, pos.y, 20);
  noFill();
  circle(pos.x, pos.y, 25);

  text("Dist: " + round(pos.mag()) + "\n\nSpeed: " + speed, pos.x, pos.y);

  text("Press 0 to go to Analog mode, click and drag to simulate distance", 0, (height / 2 - 20) / scale);
}

void mouseDragged() {
  pos.x = (mouseX - width / 2) / scale;
  //pos.y = (mouseY - height / 2) / scale;

  dist = round(pos.mag());

  myPort.write(dist + "\n");
}


void keyPressed() {
  if (key == '0') {
    pos.x = 0;
    pos.y = 0;
    dist = 0;
    myPort.write(dist + "\n");
  }
}

void onReceivedLine(String line) {
  // Detect state change. Ex: ">>>>>>>>>>>>> StateBarking.enter()" and extract the state name (ex: "Barking")
  if (line.indexOf(".enter()") != -1) {
    int start = line.indexOf("State") + 5;
    int end = line.indexOf(".", start);
    dogState = line.substring(start, end);
  }

  // Decode distance and speed. Ex: "Distance:642	Speed:-52.37"
  if (line.indexOf("Distance:") != -1) {
    int start = line.indexOf("Distance:") + 9;
    int end = line.indexOf("\t", start);
    dist = int(line.substring(start, end));
    pos = PVector.fromAngle(0).mult(dist);

    start = line.indexOf("Speed:") + 6;
    speed = float(line.substring(start));
  }
}
