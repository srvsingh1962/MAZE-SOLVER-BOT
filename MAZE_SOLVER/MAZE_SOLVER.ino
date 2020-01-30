#define LS1 A0
#define LS2 A1 
#define LCS A2
#define RCS A3 
#define RS1 A5
#define RS2 A4

int LCR;
int LNR;
int LFR;
int RCR;
int RNR;
int RFR;

int replay;


#define waittime 200

#define leftMotor1  3
#define leftMotor2  4
#define rightMotor1 5
#define rightMotor2 6


#define led 13

char path[50] = {};
int pathLength;
int readLength;


void setup() 
{
  pinMode(LS1,INPUT);    //IR LEFT FAR
  pinMode(LS2,INPUT);    //IR LEFT NEAR
  pinMode(LCS,INPUT);    //IR LEFT CENTER
  pinMode(RCS,INPUT);    //IR RIGHT CENTER
  pinMode(RS1,INPUT);    //IR RIGHT NEAR
  pinMode(RS2,INPUT);    //IR RIGHT FAR
  
  pinMode(leftMotor1,OUTPUT);   //MOTOR LEFT 1 forward
  pinMode(leftMotor2,OUTPUT);   //MOTOR LEFT 2 reverse
  pinMode(rightMotor1,OUTPUT);  //MOTOR RIGHT 1 forward
  pinMode(rightMotor2,OUTPUT);  //MOTOR RIGHT 2 reverse
  
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  
  analogWrite(7,LOW);
  analogWrite(9,LOW);
  analogWrite(10,150);
  analogWrite(13,LOW);

  Serial.begin(9600);
}

void loop()
{
  readsensors();
  if(LFR==1 && RFR==1 && LNR==1 && RNR==1 && (LCR==0 || RCR==0) )
  { 
    straight();                                                                                      
  }
  if(LFR==1 && RFR==1 && LNR==1 && RNR==1 && LCR==1 && RCR==1) 
  {
    backturn();
  }
  else
  {                                                                                              
      leftHandWall();                                                                                   
  }
  for(int i=0;path[i]!='\0';i++)
  {
      Serial.print(path[i]);
  }
}

void readsensors()
{
  LCR = digitalRead(LCS);
  LNR = digitalRead(LS2);
  LFR = digitalRead(LS1);
  RCR = digitalRead(RCS);
  RNR = digitalRead(RS2);
  RFR = digitalRead(RS1);
}

void straight()
{ 
  if( digitalRead(LCS)==1)
  {
    while(digitalRead(LCS)==1)
    {
      delay(100);
      analogWrite(leftMotor1, 100);
      analogWrite(leftMotor2, LOW);
      analogWrite(rightMotor1, LOW);
      analogWrite(rightMotor2, LOW);
    }
    
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
    return;
  }
  if(digitalRead(RCS)==1)
  {
    while(digitalRead(RCS)==1)
    {
      analogWrite(leftMotor1, LOW);
      analogWrite(leftMotor2, LOW);
      analogWrite(rightMotor1, 100);
      analogWrite(rightMotor2, LOW);
    }
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
    return;
  }
  
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
}

void leftHandWall()
{
  if( LNR==0 && (LCR==1 || RCR==1))
  {
    softleft();
    return;
  }
  if( RNR==0 && LFR==1 &&(LCR==1 || RCR==1))
  {
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(100);
    readsensors();
    if( LNR==0 )
    {
      softleft();
      return; 
    }
    else
    {
      softright();
      return;
    }
    
  }
  if( LFR==0 && RFR==0)
  {
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(waittime);
    readsensors();
    
    if(LFR==0 || RFR==0)
    {
      endpath();
    }
    if(LFR==1 && RFR==1)
    { 
      leftturn();
    }
    
  }
  
  if(LFR==0)
  {
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(waittime);
    readsensors();
      
      if(LFR==1 && RFR==1)
      {
        leftturn();
      }
      else
      {
        complete();
      }
  }
   
  if(RFR==0)
  { 
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(30);
    readsensors();
    
    if(LFR==0)
    {
      delay(waittime-30);
      readsensors();
      
      if(RFR==0 && LFR==0)
      {
        complete();
      }
      else
      {
        leftturn();
        return;
      }
    }
    delay(waittime-30);
    readsensors();
    if(LNR==0)
    {
      softleft();
      return;
    }
    if(LFR==1 && LNR==1 && LCR==1 && RCR==1 && RFR==1)
    {
      rightturn();
      return;
    }
    path[pathLength]='S';
    pathLength++;
    if(path[pathLength-2]=='B')
    {
    shortPath();
    }
    straight();
  }
  readsensors();
  if(LFR==1 && LCR==1 && RCR==1 && RFR==1 && LNR==1 && RNR==1)
  {
    backturn();
  }

}

void leftturn()
{ 
    Serial.println("leftturn");

  while(digitalRead(RCS)==0||digitalRead(LCS)==0)
  {
    printsensor();
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, 100);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(2);
  }
    
  while(digitalRead(RCS)==1)
  { 
      printsensor();
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, 100);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(2);
  }
 while(digitalRead(LCS)==1)
  { 
      printsensor();
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, 100);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(2);
  }
  if(replay==0)
  {
    path[pathLength]='L';
    pathLength++;
    if(path[pathLength-2]=='B')
     {
        shortPath();
      }
  }
}
void softleft()
{
  while(digitalRead(RCS)==0||digitalRead(LCS)==0)
  {  
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, 100);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(2);
     analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
    delay(1);
  }
  while(digitalRead(RCS)==1)
  {
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, 100);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(2);
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
    delay(1);
  }
 
  if(replay==0)
  {
    path[pathLength]='P';
    pathLength++;
    if(path[pathLength-2]=='B')
     {
        shortPath();
      }
  }
}

void rightturn()
{  
   while(digitalRead(RCS)==0)
   {
    printsensor();
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, 100);
    delay(2);
  }
   while(digitalRead(RCS)==1)
   {
    printsensor();
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, 100);
    delay(2);
  }
   while(digitalRead(LCS)==1)
   {
    printsensor();
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, 100);
    delay(2);
  }
  
  if(replay==0){
  path[pathLength]='R';
  pathLength++;
    if(path[pathLength-2]=='B'){
      shortPath();
    }
  }
}
void softright()
{   
    while(digitalRead(RCS)==0)
   {
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, 100);
    delay(2);
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
    delay(1);
  }
   while(digitalRead(RCS)==1)
   {
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, 100);
    delay(2);
     analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
    delay(1);
  }
   while(digitalRead(LCS)==1)
   {
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, 100);
    delay(2);
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
    delay(1);
  }
  
  if(replay==0)
 {
  path[pathLength]='G';
  pathLength++;
    if(path[pathLength-2]=='B'){
      shortPath();
    }
  }
}
void backturn()
{    
   while(digitalRead(RCS)==1)
   {
    Serial.print("bakchodi");
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
   }
  path[pathLength]='B';
  pathLength++;
  straight();
}
void complete()
{
  analogWrite(leftMotor1, LOW);
  analogWrite(leftMotor2, LOW);
  analogWrite(rightMotor1, LOW);
  analogWrite(rightMotor2, LOW);
  replay=1;
  path[pathLength]='D';
  pathLength++;
 while(digitalRead(11)==0)
 {
    digitalWrite(12, 100);
    
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
 }
 delay(1000);
 replaypath();
}
void shortPath(){
 int shortDone=0;
  if(path[pathLength-3]=='L' && path[pathLength-1]=='R')
  {
    pathLength-=3;
    path[pathLength]='B';
    shortDone=1;
  }
   
  if(path[pathLength-3]=='L' && path[pathLength-1]=='S' && shortDone==1)
  {
    pathLength-=3;
    path[pathLength]='R';
    shortDone=1;
  }
   
  if(path[pathLength-3]=='R' && path[pathLength-1]=='L' && shortDone==1)
  {
    pathLength-=3;
    path[pathLength]='B';
    shortDone=1;
  }
  
   
  if(path[pathLength-3]=='S' && path[pathLength-1]=='L' && shortDone==1)
  {
    pathLength-=3;
    path[pathLength]='R';
    shortDone=1;
  }
     
  if(path[pathLength-3]=='S' && path[pathLength-1]=='S' && shortDone==1)
  {
    pathLength-=3;
    path[pathLength]='B';
    shortDone=1;
  }
  if(path[pathLength-3]=='L' && path[pathLength-1]=='L' && shortDone==1)
  {
    pathLength-=3;
    path[pathLength]='S';
    shortDone=1;
  }
  if(path[pathLength-3]=='P' && path[pathLength-1]=='R' && shortDone==1)
  {
    pathLength-=3;
    path[pathLength]='G';
    shortDone=1;
  }
  if(path[pathLength-3]=='G' && path[pathLength-1]=='P' && shortDone==1)
  {
    pathLength-=3;
    path[pathLength]='B';
    shortDone=1;
  }
  if(path[pathLength-3]=='L' && path[pathLength-1]=='P' && shortDone==1)
  {
    pathLength-=3;
    path[pathLength]='G';
    shortDone=1;
  }  
  if(path[pathLength-3]=='P' && path[pathLength-1]=='P' && shortDone==1)
  {
    pathLength-=3;
    path[pathLength]='R';
    shortDone=1;
  }
  path[pathLength+1]='D';
  path[pathLength+2]='D';
  pathLength++;

}
void replaypath()
{  
  digitalWrite(12, LOW);
  readsensors();
  if(LFR==1 && RFR==1 && LNR==1 && RNR==1 && (LCR==0 || RCR==0))
    { 
      straight();                                                                                      
    }
  else
  {
    if(path[readLength]=='D'){
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(100);
    analogWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
    endpath();
    }
    if(path[readLength]=='L'){
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(waittime);
    leftturn();
    }
    if(path[readLength]=='R'){
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(waittime);
    rightturn();
    }
    if(path[readLength]=='S')
    {
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(waittime);
    straight();
    }
    if(path[readLength]=='P')
    {
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(waittime);
    softleft();
    }
    if(path[readLength]=='G')
    {
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(waittime);
    softleft();
    }
    readLength++;
  }
 replaypath();
  
}
void endpath()
{
    digitalWrite(12, 100);
    
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, 100);
    analogWrite(rightMotor2, LOW);
    delay(500);
    analogWrite(leftMotor1, 100);
    analogWrite(leftMotor2, LOW);
    analogWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, LOW);
  endpath();
}
void printsensor()
{
  readsensors();
  Serial.print(LFR);
  Serial.print(LNR);
  Serial.print(LCR);
  Serial.print(RCR);
  Serial.print(RNR);
  Serial.println(RFR);
}
