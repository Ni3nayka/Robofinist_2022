#include <Wire.h>               

#include <iarduino_I2C_Motor.h>   
iarduino_I2C_Motor mot_R(0x0A);     
iarduino_I2C_Motor mot_L(0x0B);   

#include <iarduino_I2C_Bumper.h>
iarduino_I2C_Bumper bum(0x0C);  

int M = 50;

#define IK_K 1
#define porog_IK 500

void setup(){       
  delay(500); 
  Serial.begin(9600);
  mot_R.begin();
  mot_L.begin();
  bum.begin();

  mot_R.setDirection(true);
  mot_L.setDirection(false);
  
  mot_R.setSpeed( 0, MOT_PWM);
  mot_L.setSpeed( 0, MOT_PWM);

  inversia();
  
}


//================================ IK ==================================================================

/*bool test_end_kvadrat(bool black_kvadrat = 0) {
  int sum = 0;
  for (int i = 1; i<=9; i++)
    sum += bum.getLineAnalog(i);
  Serial.println(sum);
  return black_kvadrat?sum/9>porog_IK:sum/9<porog_IK;
}*/

bool test_end_kvadrat(bool black_kvadrat = 0) {
  return (bum.getLineAnalog(1)>porog_IK || bum.getLineAnalog(9)>porog_IK) && !black_kvadrat || (bum.getLineAnalog(1)<porog_IK || bum.getLineAnalog(9)<porog_IK) && black_kvadrat;
}

/*bool test_not_end_black_kvadrat() {
  return bum.getLineAnalog(1)>porog_IK || bum.getLineAnalog(9)>porog_IK;
}

bool test_not_end_white_kvadrat() {
  return bum.getLineAnalog(1)<porog_IK || bum.getLineAnalog(9)<porog_IK;
}*/

int getErrorPid(bool black_kvadrat = 0) { 
  return (int(bum.getLineAnalog(1)>>4) - int(bum.getLineAnalog(9)>>4))*(int(black_kvadrat)*2-1)*IK_K;
  //return find_e()*(-50)*(int(black_kvadrat)*2-1);
}

double find_e() {
  double e_up = (getIK(1)+2*getIK(2)+3*getIK(3)+4*getIK(4)-getIK(6)-2*getIK(7)-3*getIK(8)-4*getIK(9));
  double e_down = getIK(1)+getIK(2)+getIK(3)+getIK(4)+getIK(5)+getIK(6)+getIK(7)+getIK(8)+getIK(9);
  return e_down==0?0:e_up/e_down;
}

int getIK(int a) {
  return bum.getLineAnalog(a)>>4;
}

//================================ MOTOR ==================================================================

void motor(int a, int b) {
  mot_R.setSpeed(a, MOT_PWM);
  mot_L.setSpeed(b, MOT_PWM);
}

//================================ MAIN ==================================================================

void inversia() {
  bool black_kvadrat = 0;
  for (int i = 0; i<7; i++) {
    while (test_end_kvadrat(black_kvadrat)) {
      int e = getErrorPid(black_kvadrat);
      motor(M+e,M-e);
    }
    black_kvadrat = (!black_kvadrat);
  }
  motor(0,0);
  //delay(3000);
  motor(M,-M);
  while (bum.getLineAnalog(1)<porog_IK);
  delay(50);
  for (int i = 0; i<3; i++) {
    while (test_end_kvadrat(black_kvadrat)) {
      int e = getErrorPid(black_kvadrat);
      motor(M+e,M-e);
    }
    black_kvadrat = (!black_kvadrat);
  }
  motor(0,0);
  
  motor(M,M);
  delay(1000);
  
  motor(17,M);
  delay(5100);

  for (int i = 0; i<2; i++) {
    while (test_end_kvadrat(black_kvadrat)) {
      int e = getErrorPid(black_kvadrat);
      motor(M+e,M-e);
    }
    black_kvadrat = (!black_kvadrat);
  }
  
  motor(0,0);
}

void loop() { 
  //Serial.println(getErrorPid());
  //Serial.println(test_end_kvadrat());
}
