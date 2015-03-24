PImage test;
void setup() {
  size(512, 512);
  test = null;
}

void draw() {
  clear();
  if (test != null) {
    //println (test.width);
    //println (test.height);
    image(test, 0, 0);
    //blend(test, 0, 0, test.width, test.height, 0, 0, test.width, test.height, BLEND);
  }
}

void mousePressed() {
  selectInput("Select a file to process:", "fileSelected");
}

void fileSelected(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    println("User selected " + selection.getAbsolutePath());
    test = trim(loadImage(selection.getAbsolutePath()));
    size(test.width, test.height);
    frame.setSize(test.width+50, test.height+50);
  }
}


PImage trim(PImage in) {
  in.loadPixels();
  int xMin = in.width;
  int yMin = in.height;
  int yMax = 0;
  int xMax = 0;
  for (int y = 0; y < in.height; ++y) {
    for (int x = 0; x < in.width; ++x) {
      if (alpha(in.pixels[x + y*in.width]) > 0) {
        xMin = min(x, xMin);
        yMin = min(y, yMin);
        xMax = max(x, xMax);
        yMax = max(y, yMax);
      }
    }
  }
  xMin-=1;
  yMin-=1;
  xMax+=1;
  yMax+=1;

  println(xMin);
  println(yMin);
  println(xMax);
  println(yMax);
  println("uh");
  return in.get(xMin, yMin, xMax-xMin, yMax-yMin);
}

