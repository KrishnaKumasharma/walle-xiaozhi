#include <Arduino.h>
#include <Xiaozhi.h>

// Motor pins
#define M1_A 47
#define M1_B 43
#define M2_A 44
#define M2_B 45

void motorLeft(int d)  { digitalWrite(M1_A, d > 0); digitalWrite(M1_B, d < 0); }
void motorRight(int d) { digitalWrite(M2_A, d > 0); digitalWrite(M2_B, d < 0); }
void stopAll()  { motorLeft(0); motorRight(0); }

void drive(int l, int r, int ms) {
  motorLeft(l);
  motorRight(r);
  delay(ms);
  stopAll();
  delay(80);
}

void doForward()   { drive(1, 1, 900); }
void doBackward()  { drive(-1, -1, 900); }
void doSpin()      { drive(1, -1, 1400); }

void doDance() {
  drive(-1, 1, 500);
  drive(1, -1, 500);
  drive(1, 1, 250);
  drive(-1, -1, 250);
  drive(1, 1, 250);
  drive(-1, -1, 250);
  drive(-1, 1, 700);
  drive(1, -1, 700);
}

void registerTools() {
  auto& mcp = Xiaozhi.getMcpServer();

  mcp.AddTool("self.walle.come_here",
      "Move the robot forward toward the user. Use when asked to come here.",
      PropertyList(),
      [](const PropertyList& p) -> ReturnValue { doForward(); return std::string("ok"); });

  mcp.AddTool("self.walle.move_back",
      "Move the robot backward.",
      PropertyList(),
      [](const PropertyList& p) -> ReturnValue { doBackward(); return std::string("ok"); });

  mcp.AddTool("self.walle.spin",
      "Move forward a little and spin around.",
      PropertyList(),
      [](const PropertyList& p) -> ReturnValue { doForward(); doSpin(); return std::string("ok"); });

  mcp.AddTool("self.walle.dance",
      "Do the robot dance: spin left and right and shuffle forward and back.",
      PropertyList(),
      [](const PropertyList& p) -> ReturnValue { doDance(); return std::string("ok"); });

  mcp.AddTool("self.walle.stop",
      "Stop all motors immediately.",
      PropertyList(),
      [](const PropertyList& p) -> ReturnValue { stopAll(); return std::string("ok"); });

  mcp.AddTool("self.walle.sleep",
      "Stop moving and rest. Use when asked to stay back or go to sleep.",
      PropertyList(),
      [](const PropertyList& p) -> ReturnValue { stopAll(); return std::string("ok"); });
}

void setup() {
  Serial.begin(115200);
  pinMode(M1_A, OUTPUT);
  pinMode(M1_B, OUTPUT);
  pinMode(M2_A, OUTPUT);
  pinMode(M2_B, OUTPUT);
  stopAll();

  Xiaozhi.begin();
  registerTools();
}

void loop() {
  Xiaozhi.loop();
}
