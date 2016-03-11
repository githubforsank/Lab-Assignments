#include <QTRSensors.h>

#define STBY             6
#define pwml_pin         3
#define pwmr_pin         9
#define l1 5
#define l2 4
#define r1 7
#define r2 8
#define gridX            6
#define gridY            6
#define EAST             0
#define NORTH            1
#define WEST             2           
#define SOUTH            3
#define fwd	         0
#define right	         1
#define back	         2
#define left	         3
#define NODE             80
#define ANTINODE         110
#define BLOCK            120
#define HOLE             140
#define LEFT_BRIDGE      220
#define RIGHT_BRIDGE     230
#define LEFT_BRIDGE_END  231
#define RIGHT_BRIDGE_END 232
#define S1               A6
#define S2               A7
#define ALL_BLACK        (s_val[0] == 0 && s_val[1] == 0 && s_val[2] == 0 && s_val[3] == 0 && s_val[4] == 0 && s_val[5] == 0 && s_val[6] == 0 && s_val[7] == 0)
#define NODE_CHECK       (s_val[0] == 1 && s_val[1] == 1) && (s_val[7] == 1 && s_val[6] == 1)
#define NODE_BRIDGE      (s_val[0] == 1 && s_val[1] == 1 && s_val[2] == 1) && (s_val[7] == 1 && s_val[6] == 1 && s_val[5] == 1)
#define BRIDGE_RIGHT     (analogRead(S1) > 600 && analogRead(S2) < 600)
#define BRIDGE_LEFT      (analogRead(S1) < 600 && analogRead(S2) > 600)
#define NODE_SEN         (analogRead(S1) < 250 && analogRead(S2) < 250)
#define TRAVERSE_END     (r_x == 4 && r_y == 1)
#define ANTINODE_CHECK   (s_val[3] == 0 || s_val[4] == 0) && (s_val[5] == 0 || s_val[2] == 0)
#define THRESHOLD        400
#define NUM_SENSORS      8     // number of sensors used
#define TIMEOUT          2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN      QTR_NO_EMITTER_PIN    
#define SetPt            4000
#define IntThresh        2500
#define ScaleFactor      1

//===============GRIFD PART DECLARATIONS==================//
//========================================================//
int c_dir;                    //direction of bot in real time, read as current dir
int dist;                     //stores the value of the current cordinate in the weighted matrix
int count = 0;                //counter for output[]
int output[gridX*gridY];      //the array that the function returns
int temp_x, temp_y;           //temp variables used in virtual path
int c_x, c_y;                 //variables for virtual path, read as current x, current y
int r_x, r_y;                 //variables for the physical bot, read as real x, real y
int node_found = 0;           //value of this becomes 1 when node is encountered, 2 when antinode, 3 when block and 4 when hole
int wgrid[gridX][gridY];      //the weighted array
int dx, dy;                   //the target cordinates, read as destination x, destination y
int dir;                      //this direction is used for virtual path tracing
int grid[gridX][gridY];       //the 2d array that stores the grid
int set_x, set_y;
int revert_x;
int revert_y;

//===========LINE FOLLOWER DECLARATIONS===================//
//========================================================//
int s_val[8];
static int max_pwm = 80;
QTRSensorsRC qtrrc((unsigned char[]) {12,11,A5,A4,A3,A2,A1,A0},NUM_SENSORS,TIMEOUT); 
unsigned int sensorValues[NUM_SENSORS];
static float kukuraP = 0.02;
static float kD = 0;
static float kI = 0;
float P,I,D,Integral=0;
float Error,Last=qtrrc.readLine(sensorValues);
float landi = 0;
float perr = 0;
int pwml=10,pwmr=10;
static int n_val = 0;
static int o_val = 0; 
static int p_val = 0; 
static int c_val = 0;
static int back_track = 0;
static int bridge_found = 0;
static int flag_found = 0;

//==============================SETUP PART==============================================
//======================================================================================
void initGrid()
{
   for(int i=0;i<gridX;i++)
   {
      for(int j=0;j<gridY;j++)
      {
         grid[i][j] = 1;
      }
   }
   
   for(int i=0;i<gridX;i++)
   {
     grid[i][0] = 250;
     grid[i][gridY-1] = 250;
   }
 
   for(int i=0;i<gridY;i++)
   {
     grid[0][i] = 250;
     grid[gridX-1][i] = 250;
   }    
}

void setup()
{
  Serial.begin(9600);
  initGrid();
  setupLineFollower();    //inititalize line follower
  sensorInit();
  setupGridFollower();    //initiate grid solving
}

void loop()
{
 // actualRead();
 // normalizeSensor();
   //cazzo you loop
}

void buzz(int x)
{
  pinMode(10, OUTPUT);
  for(int i=0;i<x;i++)
  {
      digitalWrite(10, HIGH);
      delay(50);
      digitalWrite(10, LOW);
      delay(50);
      digitalWrite(10, HIGH);
      delay(50);
      digitalWrite(10, LOW);
      delay(50);
      if(i % 10 == 0)
      {
          digitalWrite(10, HIGH);
          delay(150);
          digitalWrite(10, LOW);
          delay(150);
      }
  }  
}



void sensorInit()
{
   update_grid(2, 3, ANTINODE);
  pinMode(46, OUTPUT); 
  followNEW();
  nostop(100);
  followNEW();
  nostop(100);
  //go to the starting point
  followBack();
  nostop(100);
  followNEW();
  nostop(100);
  followNEW();
  nostop(100);
  followNEW();
  nostop(100);
  followBack();
  nostop(100);
  goLeft();
  followNEW();
  nostop(100);
  digitalWrite(13, HIGH);
  buzz(30);
  digitalWrite(13, LOW);
  goBack();
  followNEW();
  nostop(100);
  followNEW();
  nostop(100);
  followNEW();
  nostop(100);
  goRight();
  followNEW();
  nostop(100);
  goLeft();
  followNEW();
  nostop(100);
  followNEW();
  nostop(100);
  followNEW();
  nostop(100);
  goRight();
  followNEW();
  goLeft();
  followNEW();
  nostop(100);
  goLeft();
  followNEW();
  digitalWrite(13, HIGH);
  buzz(25);
  digitalWrite(13, LOW); 

}
void calibrate()
{
  digitalWrite(13,HIGH);
  set(40, 40);
  readSensorsNormal();
  while(s_val[4]==1 || s_val[3] == 1)
  {
    digitalWrite(l1,LOW);
    digitalWrite(l2,HIGH);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
    readSensorsNormal();
    qtrrc.calibrate();
  }
  readSensorsNormal();   
  while(s_val[4]!=1)
  {
    digitalWrite(l1,LOW);
    digitalWrite(l2,HIGH);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
    readSensorsNormal();
    qtrrc.calibrate();
  }
  while(s_val[4]==1 || s_val[3] == 1)
  {
    digitalWrite(l1,LOW);
    digitalWrite(l2,HIGH);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
    readSensorsNormal();
    qtrrc.calibrate();
  }
  readSensorsNormal();   
  while(s_val[4]!=1 || s_val[3] != 1)
  {
    digitalWrite(l1,LOW);
    digitalWrite(l2,HIGH);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
    readSensorsNormal();
    qtrrc.calibrate();
  }
  brake(0);
  delay(1000);
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
}
//================LINE FOLLOWRER FUNCTIONING=============//
//=======================================================//
void setupLineFollower()
{
  pinMode(13,HIGH);
  pinMode(STBY,OUTPUT);
  digitalWrite(STBY,HIGH);
  pinMode(pwml_pin,OUTPUT);
  pinMode(pwmr_pin,OUTPUT);
  pinMode(l1,OUTPUT);
  pinMode(l2,OUTPUT);
  pinMode(r1,OUTPUT);
  pinMode(r2,OUTPUT);
  Serial.begin(9600);
  calibrate();
  
}

void followNEW()
{
  max_pwm = 80;
  while(1)
  {
    flag_found = 0;
    readSensors();
          Serial.println("hgdghcfhgvhg");
      Serial.println(analogRead(S1));
      Serial.println(analogRead(S2));
    if(flag_found == LEFT_BRIDGE || flag_found == RIGHT_BRIDGE || flag_found == NODE)
    {
      Serial.println("hgdghcfhgvhg");
      Serial.println(analogRead(S1));
      Serial.println(analogRead(S2));
      
      brake(0);
      delay(100);
      nostop(150);
      return;
    }
    magic();
  }
}

//This will start the line follower until it encounters a node
  void follow() 
  {
    max_pwm = 80;
    while(1)
    {
      readSensors();
      if(flag_found == LEFT_BRIDGE)
      {
          goPastNode();
          buzz(10);
      }
      else if(flag_found == RIGHT_BRIDGE)
      {
          goPastNode();
          buzz(10);
      }
      else if(flag_found == NODE)
      {
        brake(10);
        buzz(3);
        goPastNode();
        delay(1000);
        return;
      }
      else if(flag_found == ANTINODE)
      {
        brake(10);
        buzz(2);
        brake(0);
        goPastNode();
        back_track = 1;
        //update_grid(r_x, r_y, ANTINODE);
        goBack();
        followBack();
        nostop(50);
        return;
      }
      magic();  //science bitch
    } 
  }

                                                                      void followUntillBridge() 
                                                                      {
                                                                        max_pwm = 70;
                                                                        while(1)
                                                                        {
                                                                          readSensorsNormal();
                                                                           if(NODE_CHECK == 1)
                                                                           {
                                                                             nostop(200);
                                                                             return;
                                                                           }
                                                                          magic();  //science bitch
                                                                        } 
                                                                      }
                                                                      
                                                                                                                  void followNormal() 
                                                                                                                  {
                                                                                                                    max_pwm = 50;
                                                                                                                    while(1)
                                                                                                                    {
                                                                                                                      readSensorsNormal();
                                                                                                                      if(BRIDGE_LEFT == 1 || BRIDGE_RIGHT == 1)
                                                                                                                      {
                                                                                                                          brake(0);
                                                                                                                          nostop(100);
                                                                                                                          return;
                                                                                                                      }
                                                                                                                      magic();  //science bitch
                                                                                                                    } 
                                                                                                                  }
                                                                                                                  
                                                                                                                  
                                                                                void followBack() 
                                                                                {
                                                                                  max_pwm = 50;
                                                                                  while(1)
                                                                                  {
                                                                                    readSensors();
                                                                                    if(NODE_CHECK == 1)
                                                                                    {
                                                                                        brake(0);
                                                                                        return;
                                                                                    }
                                                                                    else if(BRIDGE_LEFT == 1)
                                                                                    {
                                                                                          brake(0);
                                                                                          delay(1000);
                                                                                          bridge_found = LEFT_BRIDGE;
                                                                                          goPastNode();
                                                                                          set(40, 40);
                                                                                          nostop(500);
                                                                                          goLeftBridge();
                                                                                          followUntillBridge();
                                                                                          return;
                                                                                    }
                                                                                    else if(BRIDGE_RIGHT == 1)
                                                                                    {
                                                                                          brake(0);
                                                                                          delay(1000);
                                                                                            bridge_found = RIGHT_BRIDGE;
                                                                                            nostop(200);
                                                                                            goRight();
                                                                                            followUntillBridge();
                                                                                            return;
                                                                                    }
                                                                                    
                                                                                    magic();  //science bitch
                                                                                  } 
                                                                                }
                                                                      

                        void magic()
                        {
                           Error = SetPt - n_val;
                           landi = Error*kukuraP + (Error-perr)*kD;
                           perr = Error;                          
                           if (landi < 0)
                           {
                                pwml = max_pwm;
                                pwmr = max_pwm - abs(landi);
                            }
                           else
                           { 
                                pwmr = max_pwm;
                                pwml = max_pwm - abs(landi);
                           }
                           goForward();
                        }


void normalizeSensor()
{
  for(int i=0;i<8;i++)
  {
      if(sensorValues[7-i] > THRESHOLD)
        s_val[i] = 0;
      else
        s_val[i] = 1;
      
    //  Serial.print(s_val[i]);
    //  Serial.print("\t");
  }
  
  if(sensorValues[7-6] > (THRESHOLD+200))
    s_val[6] == 0;
  else
    s_val[6] == 1;
    
  //Serial.println();
}


void actualRead()
{
  n_val = qtrrc.readLine(sensorValues,1,1);
  normalizeSensor();
}


void readSensors()
{
  flag_found = 0;
  n_val = qtrrc.readLine(sensorValues,1,1);
  normalizeSensor();
  //check if something is encountered is encountered. if yes then set the flag value that causes the loop to break
  
  if(NODE_CHECK == 1)
  {
      delay(50);
      actualRead();
      if(NODE_CHECK == 1 && NODE_SEN == 1)
          flag_found = NODE;
      else if(ANTINODE_CHECK == 1 && NODE_SEN == 1)
      {
        flag_found = ANTINODE;
      }
  }
  
  else if(BRIDGE_LEFT == 1)
  {

    
    if(BRIDGE_LEFT == 1)
    {
      delay(20);
      actualRead();
      if(BRIDGE_LEFT == 1)
        flag_found = LEFT_BRIDGE; 
    }
    
  }
  
  else if(BRIDGE_RIGHT == 1)
  {

    if(BRIDGE_RIGHT == 1)
    {
      delay(20);
      actualRead();
      if(BRIDGE_RIGHT == 1)
        flag_found = RIGHT_BRIDGE; 
    }
  }
  
}

                                //Specifically for when the bot is following the bridge and has to take turn at the end of the bridge
                                void readSensorsBridge()
                                {
                                  flag_found = 0;
                                  n_val = qtrrc.readLine(sensorValues,1,1);
                                  normalizeSensor();
                                  if(BRIDGE_LEFT == 1)
                                  {
                                    brake(0);
                                    delay(1000);
                                    flag_found = LEFT_BRIDGE;
                                    return;
                                  }
                                  else if(BRIDGE_RIGHT == 1)
                                  {
                                    brake(0);
                                    delay(1000);
                                    flag_found = RIGHT_BRIDGE;
                                    return;
                                  }
                                  
                                }


void readSensorsNormal()
{
  flag_found = 0;
  n_val = qtrrc.readLine(sensorValues,1,1);
  normalizeSensor();
}





//==================GRID PART FUNCTIONING=================//
//========================================================//
void output_init()
{
   for(int i=0;i<gridX*gridY;i++)
   {
     output[i] = -1;
   } 
}


void setupGridFollower()
{
  while(1){brake(0);}
  
   dir = c_dir = EAST;            //assigning current/starting direction
   r_x = c_x = 1;                  //assigning current/starting x cordinate
   r_y = c_y = 1;                  //assigning current/starting y cordinate
   output_init();
   set_x = 2;
   set_y = 7;
   go_to(set_x, set_y);
   digitalWrite(13, HIGH);
   buzz(30);
   digitalWrite(13, LOW);
   update_grid(2, 3, ANTINODE);
   set_x = 1;
   set_y = 1;
   go_to(set_x, set_y);
   goRight();
   follow();
   goLeft();
   follow();
   goRightLine();
   followNEW();
   digitalWrite(13, HIGH);
   buzz(30);
   digitalWrite(13, LOW);
   //traverse();                     //Traverse the whole arena
}


void go_to(int x, int y)
{
    c_x = r_x;
    c_y = r_y;
    dx = x;
    dy = y;
    output_init();
    find_path();
    exec_output();
    brake(0);
    r_x = set_x;
    r_y = set_y;  
}

//For the function to work, set the values of dx annd dy as the destination cordinates and c_x and c_y as the present cordinates
void find_path()
{
        create_weighted();
	var_init();
	while(dist!=0)
	{
	   if(dir == SOUTH)
	   {
		if(wgrid[temp_x+1][temp_y] < dist)
		{
			dir = SOUTH;
			output[count] = fwd;
			dist = wgrid[temp_x+1][temp_y];
			temp_x += 1;
			temp_y += 0;
		}
		
		else if(wgrid[temp_x][temp_y+1] < dist)
		{
			dir = EAST;
			output[count] = left;
			dist = wgrid[temp_x][temp_y+1];
			temp_x += 0;
			temp_y += 1;
		}
		else if(wgrid[temp_x-1][temp_y] < dist)
		{
			dir = NORTH;
			output[count] = back;
			dist = wgrid[temp_x-1][temp_y];
			temp_x -= 1;
			temp_y -= 0;
		}
		else if(wgrid[temp_x][temp_y-1] < dist)
		{
			dir = WEST;
			output[count] = right;
			dist = wgrid[temp_x][temp_y-1];
			temp_x -= 0;
			temp_y -= 1;
		}
		else
		{
			wgrid[temp_x][temp_y] += 2;
			var_init();
			continue;
		}
		count++;
	   }

	   else if(dir == EAST)
           {
                if(wgrid[temp_x][temp_y+1] < dist)
                {
                        dir = EAST;
                        output[count] = fwd;
                        dist = wgrid[temp_x][temp_y+1];
			temp_x += 0;
			temp_y += 1;
             	}

                else if(wgrid[temp_x+1][temp_y] < dist)
                {
                        dir = SOUTH;
                        output[count] = right;
                        dist = wgrid[temp_x+1][temp_y];
			temp_x += 1;
			temp_y += 0;
                }
                else if(wgrid[temp_x-1][temp_y] < dist)
                {
                        dir = NORTH;
                        output[count] = left;
                        dist = wgrid[temp_x-1][temp_y];
			temp_x -= 1;
			temp_y -= 0;
                }
                else if(wgrid[temp_x][temp_y-1] < dist)
                {
                        dir = WEST;
                        output[count] = back;
                        dist = wgrid[temp_x][temp_y-1];
			temp_x -= 0;
			temp_y -= 1;
                }
                else
                {
                        wgrid[temp_x][temp_y] += 2;
                        var_init();
                        continue;
                }
                count++;
           }
	   else if(dir == WEST)
           {
                
                if(wgrid[temp_x][temp_y-1] < dist)
                {
                        dir = WEST;
                        output[count] = fwd;
                        dist = wgrid[temp_x][temp_y-1];
			temp_x -= 0;
			temp_y -= 1;
                }

                else if(wgrid[temp_x+1][temp_y] < dist)
                {
                        dir = SOUTH;
                        output[count] = left;
                        dist = wgrid[temp_x+1][temp_y];
			temp_x += 1;
			temp_y += 0;
                }
                else if(wgrid[temp_x][temp_y+1] < dist)
                {
                        dir = EAST;
                        output[count] = back;
                        dist = wgrid[temp_x][temp_y+1];
			temp_x += 0;
			temp_y += 1;
                }
                else if(wgrid[temp_x-1][temp_y] < dist)
                {
                        dir = NORTH;
                        output[count] = right;
                        dist = wgrid[temp_x-1][temp_y];
			temp_x -= 1;
			temp_y -= 0;
                }
                else
                {
                        wgrid[temp_x][temp_y] += 2;
                        var_init();
                        continue;
                }
                count++;
           }
	   else if(dir == NORTH)
           {
                if(wgrid[temp_x-1][temp_y] < dist)
                {
                        dir = NORTH;
                        output[count] = fwd;
                        dist = wgrid[temp_x-1][temp_y];
			temp_x -= 1;
			temp_y -= 0;
                }

                else if(wgrid[temp_x][temp_y+1] < dist)
                {
                        dir = EAST;
                        output[count] = right;
                        dist = wgrid[temp_x][temp_y+1];
			temp_x += 0;
			temp_y += 1;
                }
                else if(wgrid[temp_x+1][temp_y] < dist)
                {
                        dir = SOUTH;
                        output[count] = back;
                        dist = wgrid[temp_x+1][temp_y];
			temp_x += 1;
			temp_y += 0;
                }
                else if(wgrid[temp_x][temp_y-1] < dist)
                {
                        dir = WEST;
                        output[count] = left;
                        dist = wgrid[temp_x][temp_y-1];
			temp_x -= 0;
			temp_y -= 1;
                }
                else
                {
                        wgrid[temp_x][temp_y] += 2;
                        var_init();
                        continue;
                }
                count++;
           }

	}
}

//initializes the variables, part of find_path algo
void var_init()
{
        dir = c_dir;
        temp_x = c_x;
	temp_y = c_y;
	dist = wgrid[c_x][c_y];
	count = 0; 
}

//part of find_path algo
void create_weighted() 
{
        int i, j;
	for(i=0;i<gridX;i++)
	{
		for(j=0;j<gridY;j++)
		{
			if(grid[i][j] < 90)
			{
				wgrid[i][j] = fabs(dx - i) + fabs(dy - j);
			}
			
			else
			{
				wgrid[i][j] = grid[i][j];
			}	
		}
	}
}

//This function executes the output array that has been generated by the find_path() function
void exec_output()        
{
    
   int r_temp_x, r_temp_y, c_temp_dir, r_temp2_x, r_temp2_y;        //set of temporary varibles that are used in this scope only
   r_temp_x = r_x;                            //keeping backup of the prev set of coordinates
   r_temp_y = r_y;
   c_temp_dir = c_dir;
   int i = 0;
   
   while(output[i]!=-1)
   {
       r_temp2_x = r_x;
       r_temp2_y = r_y;
      //update the real time cordinates and the direction
      if(c_dir == SOUTH)
      {
           if(output[i] == fwd)
           {
                r_x++;
                c_dir = SOUTH;
           }
           else if(output[i] == right)
           {
                r_y--;
                c_dir = WEST;
           }
           else if(output[i] == left)
           {
                r_y++;
                c_dir = EAST;
           }
           else if(output[i] == back)
           {
                r_x--;
                c_dir = NORTH;
           }
     }
     
     else if(c_dir == NORTH)
     {
           if(output[i] == back)
           {
                r_x++;
                c_dir = SOUTH;
           }
           else if(output[i] == left)
           {
                r_y--;
                c_dir = WEST;
           }
           else if(output[i] == right)
           {
                r_y++;
                c_dir = EAST;
           }
           else if(output[i] == fwd)
           {
                r_x--;
                c_dir = NORTH;
           }
     }
     else if(c_dir == EAST)
     {
           if(output[i] == right)
           {
                r_x++;
                c_dir = SOUTH;
           }
           else if(output[i] == back)
           {
                r_y--;
                c_dir = WEST;
           }
           else if(output[i] == fwd)
           {
                r_y++;
                c_dir = EAST;
           }
           else if(output[i] == left)
           {
                r_x--;
                c_dir = NORTH;
           }
     }
     else if(c_dir == WEST)
     {
           if(output[i] == left)
           {
                r_x++;
                c_dir = SOUTH;
           }
           else if(output[i] == fwd)
           {
                r_y--;
                c_dir = WEST;
           }
           else if(output[i] == back)
           {
                r_y++;
                c_dir = EAST;
           }
           else if(output[i] == right)
           {
                r_x--;
                c_dir = NORTH;
           }
     }
     
     
     if(output[i] == fwd)                                         
     {
       follow(); 
     }
     else if(output[i] == right)                                        
     {
       goRight();
       follow();
     }
     else if(output[i] == left)                                           
     {
       goLeft();
       follow();
     }
     else if(output[i] == back)                                            
     {
       goBack();
       follow();
       
     }
     if(back_track == 1)
     {
       revert_x = r_x;
       revert_y = r_y;
       r_x = r_temp2_x;
       r_y = r_temp2_y;
       c_dir = (c_dir+2)%4;
       back_track = 0;
       if(bridge_found == 0)
       {
           go_to(set_x, set_y);
           return;
       }  
     }
     
     if(bridge_found == LEFT_BRIDGE)
     {
              r_temp2_x = revert_x;
       r_temp2_y = revert_y;
       Serial.println("pasila1");
         if(c_dir == EAST)
         {
             r_x = r_temp2_x+1;
             r_y = r_temp2_y-1;
             c_dir = NORTH;
             //update_grid(r_temp2_x, r_temp2_y++, ANTINODE);
         }
         else if(c_dir == WEST)
         {
             r_x = r_temp2_x-1;
             r_y = r_temp2_y+1;
             c_dir = SOUTH;
             //update_grid(r_temp2_x, r_temp2_y--, ANTINODE);
         }
         else if(c_dir == NORTH)
         {
             r_x = r_temp2_x-1;
             r_y = r_temp2_y-1;
             c_dir = WEST;
             //update_grid(r_temp2_x--, r_temp2_y, ANTINODE);
         }
         else if(c_dir == SOUTH)
         {
             r_x = r_temp2_x+1;
             r_y = r_temp2_y+1;
             c_dir = EAST;
             //update_grid(r_temp2_x++, r_temp2_y, ANTINODE);
         }
         go_to(set_x, set_y);
         return;
     }
     else if(bridge_found == RIGHT_BRIDGE)
     {
              r_temp2_x = revert_x;
       r_temp2_y = revert_y;
       Serial.println("pasila2");
         if(c_dir == EAST)
         {
             r_x = r_temp2_x+1;
             r_y = r_temp2_y+1;
             c_dir = SOUTH;
             //update_grid(r_temp2_x, r_temp2_y++, ANTINODE);
         }
         else if(c_dir == WEST)
         {
             r_x = r_temp2_x-1;
             r_y = r_temp2_y-1;
             c_dir = NORTH;
             //update_grid(r_temp2_x, r_temp2_y--, ANTINODE);
         }
         else if(c_dir == NORTH)
         {
             r_x = r_temp2_x-1;
             r_y = r_temp2_y+1;
             c_dir = EAST;
             //update_grid(r_temp2_x--, r_temp2_y, ANTINODE);
         }
         else if(c_dir == SOUTH)
         {
             r_x = r_temp2_x+1;
             r_y = r_temp2_y-1;
             c_dir = WEST;
             //update_grid(r_temp2_x++, r_temp2_y, ANTINODE);
         }
         Serial.print("zzz");
         go_to(set_x, set_y);
         return;
     }     
     i++;
   }
}

//this function traverses the whole grid. This is equivalent to dry run part.
void traverse()   
{
   int i, j, x, ci, cj;
   x = 1;
   i = 1;   //traversal starts from 1,1
   j = 1;
   while(1)    
   { 
      ci = i; cj = j;     //storing the current positions
      j+=x;               //the  j is either incremented or decrememnted according to whether the turn has been made or not
      dx = i;             //setting the destination points
      dy = j;
      c_x = ci;           //setting the current positions
      c_y = cj;
      output_init();
      find_path();
      exec_output();
      if(TRAVERSE_END)    //if the destination point has ended
      {
        return;
      }
      if(j == gridX-2)   //if condition occurs to make the turn
      {
        ci = i; cj = j;
        x = -1;
        i++;
        dx = i;  //setting the destination points
        dy = j;
        c_x = ci;  //setting the current positions
        c_y = cj;
        output_init();
        find_path();
        exec_output();
      }
      
      else if(j == 1)
      {
        ci = i; cj = j;
        x = 1;
        i++;
        dx = i;  //setting the destination points
        dy = j;
        c_x = ci;  //setting the current positions
        c_y = cj;
        output_init();
        find_path();
        exec_output();
      }
      
  }
}

void update_grid(int x, int y, int valeft) //updates the grid with the given val at given cordinate x and y
{
  grid[x][y] = valeft;
}



//=======================MOVEMENTS========================//
//========================================================//
void goForward() 
{
    analogWrite(pwml_pin, pwml);
    analogWrite(pwmr_pin, pwmr);
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
}


void nostop(int x) 
{
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
  delay(x);
  brake(0);
}

void goPastNode()
{
  set(50, 50);
  do
  {
     readSensorsNormal();
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
  }while(s_val[0] != 0 && s_val[7] != 0);
  nostop(150);
}  
  
void set(int x, int y)
{
    pwml = x; pwmr = y;
  analogWrite(pwml_pin, pwml);
  analogWrite(pwmr_pin, pwmr);
}

void brake(int x) 
{
    digitalWrite(l1,HIGH);
    digitalWrite(l2,HIGH);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,HIGH);
}

void goRight()
{
  //nostop(50);
  readSensorsNormal();
  pwml = 40; pwmr = 40;
  analogWrite(pwml_pin, pwml);
  analogWrite(pwmr_pin, pwmr);
  while(s_val[4]==1 || s_val[3] == 1)
  {

    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
     readSensorsNormal();
  }
  readSensorsNormal();   
  while(s_val[4]!=1 && s_val[3] != 1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
      readSensorsNormal();
  }
}
void goLeft()
{
  //nostop(50);
  readSensorsNormal();
  pwml = 40; pwmr = 40;
  analogWrite(pwml_pin, pwml);
  analogWrite(pwmr_pin, pwmr);
  while(s_val[4]==1 || s_val[3] == 1)
  {

    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
     readSensorsNormal();
  }
  readSensorsNormal();   
  while(s_val[4]!=1 && s_val[3] != 1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
      readSensorsNormal();
  }
  while(s_val[4]==1 || s_val[3] == 1)
  {

    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
     readSensorsNormal();
  }
  readSensorsNormal();   
  while(s_val[4]!=1 && s_val[3] != 1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
      readSensorsNormal();
  }
  while(s_val[4]==1 || s_val[3] == 1)
  {

    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
     readSensorsNormal();
  }
  readSensorsNormal();   
  while(s_val[4]!=1 && s_val[3] != 1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
      readSensorsNormal();
  }
  
}


void goLeftBKP()
{
  //nostop(50);
  readSensorsNormal();
  pwmr = 40; pwml = 40;
  analogWrite(pwml_pin, pwml);
  analogWrite(pwmr_pin, pwmr);
  while(s_val[4]==1 || s_val[3] == 1)
  {
    digitalWrite(l1,LOW);
    digitalWrite(l2,HIGH);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
     readSensorsNormal();
  }
  readSensorsNormal();   
  while(s_val[4]!=1 && s_val[3] != 1 && s_val[2] != 1)
  {
    digitalWrite(l1,LOW);
    digitalWrite(l2,HIGH);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
      readSensorsNormal();
  }
  nostop(100);
  
}
void goRightBridge()
{
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
    digitalWrite(l1,HIGH);
     delay(1000);
}
void goLeftBridge()
{
    readSensorsNormal();
  pwml = 40; pwmr = 40;
  analogWrite(pwml_pin, pwml);
  analogWrite(pwmr_pin, pwmr);
  while(s_val[4]==1 || s_val[3] == 1)
  {

    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
     readSensorsNormal();
  }
  readSensorsNormal();   
  while(s_val[4]!=1 && s_val[3] != 1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
      readSensorsNormal();
  }
  while(s_val[4]==1 || s_val[3] == 1)
  {

    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
     readSensorsNormal();
  }
  readSensorsNormal();   
  while(s_val[4]!=1 && s_val[3] != 1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
      readSensorsNormal();
  }
}
void goRightLine()
{
  readSensorsNormal();
  do
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
     readSensorsNormal();
  }while(s_val[3] == 0);
}
void goLeftLine()
{
  readSensorsNormal();
  do
  {
    digitalWrite(l1,LOW);
    digitalWrite(l2,HIGH);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
     readSensorsNormal();
  }while(s_val[3] == 0);
}
void goBack()
{
  nostop(50);
  readSensorsNormal();
  pwml = 70; pwmr = 70;
  analogWrite(pwml_pin, pwml);
  analogWrite(pwmr_pin, pwmr);
  while(s_val[4]==1 || s_val[3] == 1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
     readSensorsNormal();
  }
  readSensorsNormal();   
  while(s_val[4]!=1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
      readSensorsNormal();
  }
  readSensorsNormal();
  while(s_val[4]==1 || s_val[3] == 1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
     readSensorsNormal();
  }
  readSensorsNormal();   
  while(s_val[4]!=1)
  {
    digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
      readSensorsNormal();
  }
  nostop(100);
}

void calLeft()
{
      digitalWrite(l1,LOW);
    digitalWrite(l2,HIGH);
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
}

void calRight()
{
      digitalWrite(l1,HIGH);
    digitalWrite(l2,LOW);
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
}


//========================================================//
//========================================================//
