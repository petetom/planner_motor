// read the 4 swicth inputs


void read_switches() {

  if(!digitalRead(UP_SW))
  { delay(75);
    if (!digitalRead(UP_SW))
      up_sw = true;
  }else
    up_sw = false;

  if(!digitalRead(DOWN_SW))
  { delay(75);
    if (!digitalRead(DOWN_SW))
      down_sw = true;
  }else
    down_sw = false;

  if(!digitalRead(GO_SW))
  { delay(75);
    if (!digitalRead(GO_SW))
      go_sw = true;
  }else
    go_sw = false;

  if(!digitalRead(CAL_SW))
  { delay(50);
    if (!digitalRead(CAL_SW))
      cal_sw = true;
  }else
    cal_sw = false;
}
