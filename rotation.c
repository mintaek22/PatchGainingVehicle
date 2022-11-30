void spin() {
    if(move == 2){
      int col_left = getColorName(cs_left);
      int col_middle = getColorName(cs_middle);
    int col_right = getColorName(cs_right);

    if (dir_cur == dir_dest) { // 0 rotation
      return;
    }
    else if ((dir_cur + dir_dest)%2 == 1 && (dir_cur - dir_dest == -1 || 3)) { // 90 rotation
      set_motor(10,-10);    
      sleep(1700);
      set_motor(0,0);
    }
    else if ((dir_cur + dir_dest)%2 == 1 && (dir_cur - dir_dest == 1 || -3)) { // -90 rotation
      set_motor(-10,10);    
      sleep(1700);
      set_motor(0,0);
    }
    else if ((dir_cur + dir_dest)%2 == 0) { // 180 rotation
       set_motor(10,-10);    
      sleep(3300);
      set_motor(0,0);
    }

    if(get_stat(OFFROAD)) {
      //calibration
    }
    dir_cur = dir_dest;
      set_stat(MOVE,1);      
      move = get_stat(MOVE);
    }
}