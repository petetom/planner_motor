// update the display info here


void update_display() {

     display.clearDisplay();
     display.setTextSize(3);
     display.setCursor(0,0);
     display.print(height_in,3);

     display.println("\"");
 
     display.setTextSize(2);
     display.print(height_mm,2);
     //display.print(planner_set);
     display.println("mm");
     display.setCursor(0,57);
     display.setTextSize(1);
     display.print(hb*256+lb);
     display.print("  ");
     display.print(st,HEX);
     display.print("  ");
     display.print(agc,HEX);
     display.print("  ");
     display.print(conf_h,HEX);
     display.print("  ");
     display.println(conf_l,HEX);
 
     display.display();
}
