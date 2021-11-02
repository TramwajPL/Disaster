PShape s;

void setup() {
  size(640, 480, P3D);
  noStroke();
  s = loadShape("capsule.obj");
}

float time=0.01;

void draw() {
 lights();
  background(0);
  translate(width/2, height/2, -20);
  rotateX(time*PI/2.f);
  scale(100);
  shape(s, 0, 0);
  time+=0.01;
}