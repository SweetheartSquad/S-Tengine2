PImage test;
void setup() {
  size(512, 512);
  test = null;
}

void draw() {
  background(0);
  if (test != null) {
    blend(test, 0, 0, test.width, test.height, 0, 0, test.width, test.height, BLEND);
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
    test = loadImage(selection.getAbsolutePath());
    size(test.width, test.height);
    frame.setSize(test.width, test.height);
  }
}

