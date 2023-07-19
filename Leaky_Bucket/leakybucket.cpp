#include<stdlib.h>
#include<iostream>
#include<glut.h>
#include<math.h>
#include<string.h>

int state =0; // Remaining < o/p rate state = 0 else state 1
int accepted[5]; //array of accepted packets
int acceptedPackets = 0; 
int current_display = 0; // front page 0 menu 1 flowchart...
int drawPacket = 0;
int size = 0,packSize=0;
int acceptSize=0,rejectSize=0;
int packets[5]; // input packet array

int bucketSize=0,numberPackets=0,oprate=0,Loprate;


void delay(){
	int i,j=0;
	for(i=0;i<=1000000000/5;i++){
		j++;
	}
}

void reshape(int w,int h)
{
	glViewport(0,0,w,h);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,w,0,h);
}


// Text Printer
void print(char *string,void *font,int x,int y)
{
	char *c;
	glRasterPos2i(x,y);
	for(c=string;*c!='\0';c++)
		glutBitmapCharacter(font,*c); // charater render
}



//To print borders
void borders(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	glBegin(GL_LINE_LOOP);
		glVertex2i(x1,y1);
		glVertex2i(x2,y2);
		glVertex2i(x3,y3);
		glVertex2i(x4,y4);
	glEnd();
}



//To generate a polygon
void square(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	glBegin(GL_POLYGON);
		glVertex2i(x1,y1);
		glVertex2i(x2,y2);
		glVertex2i(x3,y3);
		glVertex2i(x4,y4);
	glEnd();
}


// to set background
void background(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	glBegin(GL_POLYGON);
		glColor3f(0.0,0.0,1.0);
		glVertex2i(x1,y1);
		glColor3f(0.0,0.0,0.0);
		glVertex2i(x2,y2);
		glColor3f(0.0,0.0,0.0);
		glVertex2i(x3,y2);
		glColor3f(0.0,0.0,1.0);
		glVertex2i(x4,y4);
	glEnd();
}

void drawBucket()
{
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_POLYGON);
		glVertex2i(400,250);
		glVertex2i(600,250);
		glVertex2i(650,650);
		glVertex2i(350,650);
	glEnd();
	glColor3f(0.0,0.8,0.3);
	glLineWidth(7.0);
	borders(400,250,600,250,650,650,350,650);
	glFlush();
}

void drawPackets(int numberOfPackets)
{
	int i=0;
	for(i=0;i<numberOfPackets;i++)
	{
		glColor3f(1.0,1.0,0.0);
		glBegin(GL_QUADS);
			glVertex2i(250+size,800);
			glVertex2i(325+size,800);
			glVertex2i(325+size,875);
			glVertex2i(250+size,875);
		glEnd();
		
		glLineWidth(3.0);
		glColor3f(0.0,0.0,0.0);
		borders(250+size,800,325+size,800,325+size,875,250+size,875);
		size+=75;
		
		glFlush(); 
		delay();
		delay();
	}
	packSize = size;
}
void drawLine(float x1, float y1, float x2, float y2)
{
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glFlush();
}


void addLayer(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int remainingb,int remainingp)
{
	//int slope;
	//slope = (600-400)/(650-250);
	char temp[100]="";
	glBegin(GL_POLYGON);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glVertex2i(x3,y3);
	glVertex2i(x4,y4);
	glEnd();
	sprintf(temp,"%d",remainingp);
	glColor3f(0.0,1.0,1.0);
	print(temp,GLUT_BITMAP_TIMES_ROMAN_24,490,300);
	strcpy(temp,"");
	delay();
	sprintf(temp,"%d",remainingb);
	glColor3f(0.0,0.0,0.0);
	print(temp,GLUT_BITMAP_TIMES_ROMAN_24,490,300);
	strcpy(temp,"");
	glFlush();
}


//Remove layer from the bucket
void delLayer(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int remainingb,int remainingp)
{
	//int slope;
	//slope = (600-400)/(650-250);
	char temp[100]="";
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_POLYGON);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glVertex2i(x3,y3);
	glVertex2i(x4,y4);
	glEnd();
	sprintf(temp,"%d",remainingp);
	glColor3f(0.0,1.0,1.0);
	print(temp,GLUT_BITMAP_TIMES_ROMAN_24,490,300);
	strcpy(temp,"");
	delay();
	sprintf(temp,"%d",remainingb);
	glColor3f(0.0,0.0,0.0);
	print(temp,GLUT_BITMAP_TIMES_ROMAN_24,490,300);
	strcpy(temp,"");
	glFlush();
}

int dropPacketY =240;

//Draw Outgoing packet
void myPacket(){
	glBegin(GL_QUADS);
	glVertex2i(475,dropPacketY);
	glVertex2i(525,dropPacketY);
	glVertex2i(525,dropPacketY-50);
	glVertex2i(475,dropPacketY-50);
	glEnd();
	glFlush();
}

// packet output animation
void packetOut(int value){
	char temp[100];
	while(dropPacketY>0){
	glColor3f(0.0,1.0,1.0);
	myPacket();
	if(value==1){
	sprintf(temp,"%d",oprate);
	glColor3f(0.0,0.0,0.0);
	print(temp,GLUT_BITMAP_TIMES_ROMAN_24,490,dropPacketY-25);
	glFlush();
	strcpy(temp,"");
	}
	else if(value==0){
	sprintf(temp,"%d",Loprate);
	glColor3f(0.0,0.0,0.0);
	print(temp,GLUT_BITMAP_TIMES_ROMAN_24,490,dropPacketY-25);
	glFlush();
	strcpy(temp,"");
	}
	delay();
	glColor3f(0.0,0.0,1.0);
	myPacket();
	dropPacketY-=20;
	}
	dropPacketY=245;
}


//The main animation after pressing N;
int leakyBucket(int packets[],int numberOfPackets)
{
	int i=0;
	int addedY=0;
	int remaining,finalY;
	int bucketFill = 390/bucketSize;
	remaining = 0;
	packSize=0;
	int incrementY =0;
	char temp[100];
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glLineWidth(3);
	glVertex2f(351,642);
	glVertex2f(649,642);
	glEnd();
	glFlush();
	do{
		if(packets[i]<=bucketSize)
		{
			if(remaining+packets[i]>bucketSize){
				glColor3f(1.0,1.0,0.0);
		glBegin(GL_QUADS);
			glVertex2i(250+packSize,800);
			glVertex2i(325+packSize,800);
			glVertex2i(325+packSize,875);
			glVertex2i(250+packSize,875);
		glEnd();
		glLineWidth(4.0);
		glColor3f(1.0,0.0,0.0);
		drawLine(250+packSize,800,325+packSize,875);
		drawLine(325+packSize,800,250+packSize,875);
		glColor3f(0.0,0.0,0.0);
		borders(250+packSize,800,325+packSize,800,325+packSize,875,250+packSize,875);
		packSize+=75;
		delay();
		glFlush();
		// Draw Reject packet
				glColor3f(1.0,1.0,0.0);
			print("REJECT",GLUT_BITMAP_TIMES_ROMAN_24,700,640);
			drawLine(700,625,775,625);
			printf("Packet ");
			printf("REJECTED\n");
			glColor3f(1.0,0.0,0.0);
			
		glBegin(GL_QUADS);
			glVertex2i(700,525+rejectSize);
			glVertex2i(775,525+rejectSize);
			glVertex2i(775,600+rejectSize);
			glVertex2i(700,600+rejectSize);
		glEnd();
		glColor3f(0.0,0.0,0.0);
		// Reject Size is used for drawing packets in Reject Side
		borders(700,525+rejectSize,775,525+rejectSize,775,600+rejectSize,700,600+rejectSize);
			sprintf(temp,"%d",packets[i]);
			glColor3f(0.0,0.0,0.0);
			print(temp,GLUT_BITMAP_TIMES_ROMAN_24,725,563+rejectSize);
			strcpy(temp,"");
			rejectSize-=75; //Decrement vertically(height)
		glFlush();
		delay();

		if(oprate > remaining){ //Condition 1: If Bucket has less than o/p rate value
			Loprate = remaining;
			remaining -= Loprate;
			//Del a layer from bucket showing packet output is engaged
			delLayer(405,finalY,595,finalY,595,finalY-Loprate*bucketFill,405,finalY-Loprate*bucketFill,remaining,remaining+Loprate);
			//Bucket value
			if(remaining == 0 ){
			glColor3f(1.0,1.0,1.0);
			print("00",GLUT_BITMAP_TIMES_ROMAN_24,490,300);
			glFlush();
	}
			finalY-=Loprate*bucketFill; // for layer deletion an addition
			addedY-=Loprate*bucketFill;
			packetOut(0); //Animate output packet
			delay();
		}
			else{
				remaining -= oprate;
				delLayer(405,finalY,595,finalY,595,finalY-oprate*bucketFill,405,finalY-oprate*bucketFill,remaining,remaining+oprate);
				if(remaining == 0 ){
			glColor3f(1.0,1.0,1.0);
			print("00",GLUT_BITMAP_TIMES_ROMAN_24,490,300);
			glFlush();
	}
			finalY-=oprate*bucketFill;
			addedY-=oprate*bucketFill;
			packetOut(1);
			delay();
				//Del a layer corresponding to output rate
			}
			}
			else{
			glColor3f(1.0,1.0,0.0);
		glBegin(GL_QUADS);
			glVertex2i(250+packSize,800);
			glVertex2i(325+packSize,800);
			glVertex2i(325+packSize,875);
			glVertex2i(250+packSize,875);
		glEnd();
		// TO show a packet is taken as input
		glLineWidth(4.0);
		glColor3f(1.0,0.0,0.0);
		drawLine(250+packSize,800,325+packSize,875);
		drawLine(325+packSize,800,250+packSize,875);
		glColor3f(0.0,0.0,0.0);
		borders(250+packSize,800,325+packSize,800,325+packSize,875,250+packSize,875);
		packSize+=75;
		delay(); // for kaat mark
		glFlush();

		//accepted

		glColor3f(0.0,1.0,0.0);
			print("ACCEPT",GLUT_BITMAP_TIMES_ROMAN_24,250,640);
			glColor3f(0.0,1.0,0.0);
			drawLine(250,625,325,625);
			printf("Packet bandwidth is less than bucket capacity:  ");
			printf("ACCEPTED\n");
			glColor3f(0.0,1.0,0.0);
			
		glBegin(GL_QUADS);
			glVertex2i(250,525+acceptSize);
			glVertex2i(325,525+acceptSize);
			glVertex2i(325,600+acceptSize);
			glVertex2i(250,600+acceptSize);
		glEnd();
		glColor3f(0.0,0.0,0.0);
		borders(250,525+acceptSize,325,525+acceptSize,325,600+acceptSize,250,600+acceptSize);
			sprintf(temp,"%d",packets[i]);
			glColor3f(0.0,0.0,0.0);
			print(temp,GLUT_BITMAP_TIMES_ROMAN_24,275,563+acceptSize);
			strcpy(temp,"");
			acceptSize-=75; // Height of the accept packets on left
		glFlush();
		delay();

		remaining += packets[i];
		//add a green layer on bucket showing packet is inside the bucket
		glColor3f(0.0,1.0,1.0);
		addLayer(405,255+addedY,595,255+addedY,595,255+addedY+packets[i]*bucketFill,405,255+addedY+packets[i]*bucketFill,remaining,remaining-packets[i]);
		//slope1-=8;
		//slope2+=8;
		//slopeLeft-=8;
		//slopeRight+=8;
		addedY+=packets[i]*bucketFill;
		finalY = 255+addedY;
		delay();
		
		if(oprate > remaining && remaining != 0){
			Loprate = remaining;
			remaining -= Loprate;
			//Del a layer from bucket showing packet output is engaged
			delLayer(405,finalY,595,finalY,595,finalY-Loprate*bucketFill,405,finalY-Loprate*bucketFill,remaining,remaining+Loprate);
		if(remaining == 0 ){
			glColor3f(1.0,1.0,1.0);
			print("00",GLUT_BITMAP_TIMES_ROMAN_24,490,300);
			glFlush();
	}
			finalY-=Loprate*bucketFill;
			addedY-=Loprate*bucketFill;
			packetOut(0);
			delay();
		}
			else{
				remaining -= oprate;
				delLayer(405,finalY,595,finalY,595,finalY-oprate*bucketFill,405,finalY-oprate*bucketFill,remaining,remaining+oprate);
			if(remaining == 0 ){
			glColor3f(1.0,1.0,1.0);
			print("00",GLUT_BITMAP_TIMES_ROMAN_24,490,300);
			glFlush();
	}
			finalY-=oprate*bucketFill;
			addedY-=oprate*bucketFill;
			packetOut(1);
			delay();
				//Del a layer corresponding to output rate
			}
		}
		}
		// Reject
		else{
			glColor3f(1.0,1.0,0.0);
		glBegin(GL_QUADS);
			glVertex2i(250+packSize,800);
			glVertex2i(325+packSize,800);
			glVertex2i(325+packSize,875);
			glVertex2i(250+packSize,875);
		glEnd();
		glLineWidth(4.0);
		glColor3f(1.0,0.0,0.0);
		drawLine(250+packSize,800,325+packSize,875);
		drawLine(325+packSize,800,250+packSize,875);
		glColor3f(0.0,0.0,0.0);
		borders(250+packSize,800,325+packSize,800,325+packSize,875,250+packSize,875);
		packSize+=75;
		delay();
		glFlush();
			glColor3f(1.0,1.0,0.0);
			print("REJECT",GLUT_BITMAP_TIMES_ROMAN_24,700,640);
			drawLine(700,625,775,625);
			printf("Packet ");
			printf("REJECTED\n");
			glColor3f(1.0,0.0,0.0);
			
		glBegin(GL_QUADS);
			glVertex2i(700,525+rejectSize);
			glVertex2i(775,525+rejectSize);
			glVertex2i(775,600+rejectSize);
			glVertex2i(700,600+rejectSize);
		glEnd();
		glColor3f(0.0,0.0,0.0);
		borders(700,525+rejectSize,775,525+rejectSize,775,600+rejectSize,700,600+rejectSize);
			sprintf(temp,"%d",packets[i]);
			glColor3f(0.0,0.0,0.0);
			print(temp,GLUT_BITMAP_TIMES_ROMAN_24,725,563+rejectSize);
			strcpy(temp,"");
			rejectSize-=75;
		glFlush();
		delay();

		if(oprate > remaining && remaining !=0){
			Loprate = remaining;
			remaining -= Loprate;
			delLayer(405,finalY,598,finalY,595,finalY-Loprate*bucketFill,405,finalY-Loprate*bucketFill,remaining,remaining+Loprate);
			if(remaining == 0 ){
			glColor3f(1.0,1.0,1.0);
			print("00",GLUT_BITMAP_TIMES_ROMAN_24,490,300);
			glFlush();
	}
			finalY-=Loprate*bucketFill;
			addedY = 0;
			packetOut(0);
			delay();
			//Del a layer from bucket showing packet output is engaged
		}
		else if(remaining != 0){
			remaining -= oprate;
			delLayer(405,finalY,595,finalY,595,finalY-oprate*bucketFill,405,finalY-oprate*bucketFill,remaining,remaining+oprate);
			if(remaining == 0 ){
			glColor3f(1.0,1.0,1.0);
			print("00",GLUT_BITMAP_TIMES_ROMAN_24,490,300);
			glFlush();
	}
			finalY-=oprate*bucketFill;
			addedY-=oprate*bucketFill;
			packetOut(1);
			delay();
		}
		}
		i++;
	}while(i<numberOfPackets);
	// for remaining packets inside the bucket after all packet inputs are over
	while(remaining != 0){
		if(oprate > remaining){
			Loprate = remaining;
			remaining -= Loprate;
			delLayer(405,finalY,595,finalY,595,finalY-Loprate*bucketFill,405,finalY-Loprate*bucketFill,remaining,remaining+Loprate);
			if(remaining == 0 ){
			glColor3f(1.0,1.0,1.0);
			print("00",GLUT_BITMAP_TIMES_ROMAN_24,490,300);
			glFlush();
			}
			finalY-=Loprate*bucketFill;
			addedY = 0;
			packetOut(0);
			delay();
			
			//Del a layer from bucket showing packet output is engaged
		}
		else{
			remaining -= oprate;
			delLayer(405,finalY,595,finalY,595,finalY-oprate*bucketFill,405,finalY-oprate*bucketFill,remaining,remaining+oprate);
			if(remaining == 0 ){
			glColor3f(1.0,1.0,1.0);
			print("00",GLUT_BITMAP_TIMES_ROMAN_24,490,300);
			glFlush();
			}
			finalY-=oprate*bucketFill;
			addedY-=oprate*bucketFill;
			packetOut(1);
			delay();
			//Del the layer corresponding to the outputrate
		}
	}
			glColor3f(1.0,1.0,1.0);
			print("<The Simulation has Ended",GLUT_BITMAP_TIMES_ROMAN_24,700,350);
			print("Restart the program for another Instance>",GLUT_BITMAP_TIMES_ROMAN_24,700,330);
			glFlush();
			return 9;
}

	


void printPacketValues(int packetValue[],int numberOfPackets)
{
	int i=0,j=0;
	size=0;
	char temp[100];
	for(i=0;i<numberOfPackets;i++)
	{
			sprintf(temp,"%d",packetValue[i]);
			glColor3f(0.0,0.0,0.0);
			print(temp,GLUT_BITMAP_TIMES_ROMAN_24,275+size,837);
			size+=75;
			strcpy(temp,"");
	}
}

//for drawing the shapes required for flowchart
void drawRectangle(float x1, float y1, float width, float height)
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x1 + width, y1);
    glVertex2f(x1 + width, y1 + height);
    glVertex2f(x1, y1 + height);
    glEnd();
    glFlush();
}

void drawParalellogram(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
 glColor3f(1.0,1.0,1.0);
 glBegin(GL_QUADS);
        glVertex2i(x1,y1);
		glVertex2i(x2,y2);
		glVertex2i(x3,y3);
		glVertex2i(x4,y4);
    glEnd();
	glFlush();
}


void drawDiamond(float x1, float y1, float size)
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1 + size / 2.0);
    glVertex2f(x1 + size / 2.0, y1);
    glVertex2f(x1, y1 - size / 2.0);
    glVertex2f(x1 - size / 2.0, y1);
    glEnd();
    glFlush();
}

void drawInvertedTriangle(float x1, float y1, float size)
{
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x1 - size / 2.0, y1 + size / 2.0);
    glVertex2f(x1 + size / 2.0, y1 + size / 2.0);
    glVertex2f(x1, y1 - size / 2.0);
    glEnd();
    glFlush();
}





//for drawing oval shape
void drawOval(float x, float y, float x_radius, float y_radius, int num_segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white
    
    // Calculate the angle between each line segment
    float angle = 2.0f * 3.14159f / num_segments;
    
    // Draw the oval
    for (int i = 0; i <= num_segments; ++i)
    {
        float theta = i * angle;
        float x_pos = x + x_radius * cos(theta);
        float y_pos = y + y_radius * sin(theta);
        glVertex2f(x_pos, y_pos);
    }
    
    glEnd();
}


int flowchart()
{

	background(0,0,0,1000,1000,1000,1000,0);
	glColor3f(0.0,0.5,0.5);
	borders(1,1,1,999,999,999,999,1);
	//heading
	glColor3f(1.0,1.0,1.0);
	print("FLOWCHART :-",GLUT_BITMAP_TIMES_ROMAN_24,50,930);
	glLineWidth(2.0);
	drawLine(50,910,145,910);

	drawOval(490,920,40,30,100);
	glColor3f(0.0,0.0,0.0);
	print("START",GLUT_BITMAP_TIMES_ROMAN_24,470,915);
	glLineWidth(5.0);
	drawLine(490,890,490,860);
	drawInvertedTriangle(490,860,10);
	

	drawParalellogram(410,770,550,770,585,855,435,855);
	glColor3f(0.0,0.0,0.0);
	print("Input bkt cap,pkt",GLUT_BITMAP_TIMES_ROMAN_24,450,820);
	glColor3f(0.0,0.0,0.0);
	print("   size & o/p rate",GLUT_BITMAP_TIMES_ROMAN_24,450,790);

	//input to comparision
	glLineWidth(5.0);
	drawLine(490,770,490,740);
	drawInvertedTriangle(490,740,10);
	drawDiamond(490,670,128);
	glColor3f(0.0,0.0,0.0);
	print("If the",GLUT_BITMAP_TIMES_ROMAN_24,475,690);
	glColor3f(0.0,0.0,0.0);
	print("bktcap > pktsize?",GLUT_BITMAP_TIMES_ROMAN_24,450,660);

	//if pkt size<bktcap
	glColor3f(1.0,1.0,1.0);
	print("YES",GLUT_BITMAP_TIMES_ROMAN_24,500,587);
	glLineWidth(5.0);
	drawLine(490,606,490,568);
	drawInvertedTriangle(490,568,10);
	drawRectangle(425,490,140,73);
	glColor3f(0.0,0.0,0.0);
	print("Add the pkt to bkt",GLUT_BITMAP_TIMES_ROMAN_24,445,518);

	//from pkt size not less than bkt cap to drop pkt
	glColor3f(1.0,1.0,1.0);
	print("NO",GLUT_BITMAP_TIMES_ROMAN_24,590,685);
	glLineWidth(5.0);
	drawLine(554,670,695,670);
	glLineWidth(5.0);
	drawLine(694,670,694,573);
	drawInvertedTriangle(694,573,11);
	drawRectangle(625,490,140,75);
	glColor3f(0.0,0.0,0.0);
	print("Drop the pkt",GLUT_BITMAP_TIMES_ROMAN_24,660,513);

	//from drop pkt to o/p 
	glLineWidth(5.0);
	drawLine(694,490,694,415);
	glLineWidth(5.0);
	drawLine(695,414,572,414);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_TRIANGLES);
	glVertex2i(572,425);
	glVertex2i(572,405);
	glVertex2i(565,415);
	glEnd();

	//from add pkt to o/p
	glLineWidth(5.0);
	drawLine(490,490,490,455);
	drawInvertedTriangle(490,455,10);
	drawRectangle(425,378,140,73);
	glColor3f(0.0,0.0,0.0);
	print("O/p the pkt based on",GLUT_BITMAP_TIMES_ROMAN_24,440,420);
	glColor3f(0.0,0.0,0.0);
	print("o/p rate",GLUT_BITMAP_TIMES_ROMAN_24,475,390);

	//o/p to bkt empty
	glLineWidth(5.0);
	drawLine(490,378,490,343);
	drawInvertedTriangle(490,343,10);
	drawDiamond(490,273,128);
	glColor3f(0.0,0.0,0.0);
	print("If the",GLUT_BITMAP_TIMES_ROMAN_24,475,288);
	glColor3f(0.0,0.0,0.0);
	print("  bkt is empty?",GLUT_BITMAP_TIMES_ROMAN_24,450,258);

	
	glColor3f(1.0,1.0,1.0);
	print("NO",GLUT_BITMAP_TIMES_ROMAN_24,390,290);
	glLineWidth(5.0);
	drawLine(426,273,375,273);
	glLineWidth(5.0);
	drawLine(376,273,376,400);
	glLineWidth(5.0);
	drawLine(376,400,418,400);
	glBegin(GL_TRIANGLES);//for triangle
	glVertex2i(418,410);
	glVertex2i(418,390);
	glVertex2i(425,400);
	glEnd();

	glColor3f(1.0,1.0,1.0);
	print("YES",GLUT_BITMAP_TIMES_ROMAN_24,500,192);
	glLineWidth(5.0);
	drawLine(490,210,490,175);
	drawInvertedTriangle(490,175,10);
	glColor3f(1.0,1.0,1.0);
	drawOval(490,140,40,30,100);
	glColor3f(0.0,0.0,0.0);
	print("STOP",GLUT_BITMAP_TIMES_ROMAN_24,475,135);



	// repeat the steps from o/p to checking pkt size with bkt cap
	glLineWidth(5.0);
	drawLine(425,440,375,440);
	glLineWidth(5.0);
	drawLine(376,440,376,670);
	glLineWidth(5.0);
	drawLine(376,670,418,670);
	glBegin(GL_TRIANGLES);//for triangle
	glVertex2i(418,678);
	glVertex2i(418,660);
	glVertex2i(425,670);
	glEnd();

	//terms used in flowchart
	glColor3f(1.0,1.0,1.0);
	print("Terms used :-",GLUT_BITMAP_TIMES_ROMAN_24,50,330);
	glLineWidth(2.0);
	drawLine(50,320,130,320);
	glColor3f(1.0,1.0,1.0);
	print("1. bkt : Bucket",GLUT_BITMAP_TIMES_ROMAN_24,50,290);
	glColor3f(1.0,1.0,1.0);
	print("2. pkt : Packet",GLUT_BITMAP_TIMES_ROMAN_24,50,260);
	glColor3f(1.0,1.0,1.0);
	print("3. bkt cap: Bucket Capacity",GLUT_BITMAP_TIMES_ROMAN_24,50,230);
	glColor3f(1.0,1.0,1.0);
	print("4. O/p : Output",GLUT_BITMAP_TIMES_ROMAN_24,50,200);
	glColor3f(1.0,1.0,1.0);
	print("<<< Press                            to go back to previous page >>>",GLUT_BITMAP_TIMES_ROMAN_24,330,70);
	glColor3f(1.0,0.0,0.0);
	print(" BACKSPACE",GLUT_BITMAP_TIMES_ROMAN_24,385,70);
	glFlush();

	return 6;

}

//Start of the algorithm
//Still in working no animation has started
//Only takes input and display if maximum limit is exceeded
// before pressing n
int bucket()
{
	int i,input=0;
	int packetSize=20;
	char temp[100];
	printf("\t\t\t***LEAKY BUCKET TERMINAL***\n");
	background(0,0,0,1000,1000,1000,1000,0);
	glColor3f(0.0,0.5,0.5);
	borders(1,1,1,999,999,999,999,1);
	glColor3f(0.0,1.0,1.0);
	print("LEAKY BUCKET IMPLEMENTATION:- ",GLUT_BITMAP_TIMES_ROMAN_24,20,950);
	glColor3f(1.0,1.0,1.0);
	print("*** Give the inputs in                       ***",GLUT_BITMAP_TIMES_ROMAN_24,20,880);
	glColor3f(1.0,0.0,0.0);
	print("TERMINAL",GLUT_BITMAP_TIMES_ROMAN_24,140,880);
	glColor3f(1.0,1.0,1.0);
	print("<<< Press                            to go back to previous page >>>",GLUT_BITMAP_TIMES_ROMAN_24,20,60);
	glColor3f(1.0,0.0,0.0);
	print("  BACKSPACE",GLUT_BITMAP_TIMES_ROMAN_24,70,60);
	glColor3f(1.0,1.0,1.0);
	print("<<< Press          to Exit >>>",GLUT_BITMAP_TIMES_ROMAN_24,720,60);
	glColor3f(1.0,0.0,0.0);
	print("     ESC",GLUT_BITMAP_TIMES_ROMAN_24,760,60);
	glFlush();
	printf("Bucket capacity ranges from  20 to 50 and Max number of packets is 5\n");
	printf("Enter the capcity of the bucket: ");
	scanf("%d",&bucketSize);
	printf("Enter number of packets: ");
	scanf("%d",&numberPackets);
	if(numberPackets > 5)
	{
		glColor3f(1.0,0.0,0.0);
		print("Maximum number of packets = 5",GLUT_BITMAP_TIMES_ROMAN_24,800,900);
		glFlush();
	}
	else if(bucketSize > 50 ){
		glColor3f(1.0,0.0,0.0);
		print("Maximum Size of bucket = 50",GLUT_BITMAP_TIMES_ROMAN_24,800,800);
		glFlush();
	}
	else if(bucketSize <= 20){
		glColor3f(1.0,0.0,0.0);
		print("Minimum bucket capacity= 20",GLUT_BITMAP_TIMES_ROMAN_24,800,800);
		glFlush();
	}
	else{
	printf("Enter the output rate: ");
	scanf("%d",&oprate);
	printf("Enter values for packets: ");
	for(i=0;i<numberPackets;i++)
	{
		scanf("%d",&input);
		packets[i]=input;
	}
	drawBucket();
	glColor3f(1.0,1.0,0.0);
	print("<--- Bucket",GLUT_BITMAP_TIMES_ROMAN_24,615,320);
	glColor3f(1.0,1.0,0.0);
	print("<--- Input Packets",GLUT_BITMAP_TIMES_ROMAN_24,645,825);
	drawPackets(numberPackets);
	printPacketValues(packets,numberPackets);
	glColor3f(1.0,1.0,1.0);
	print("<<< Press               to start the simulation >>>",GLUT_BITMAP_TIMES_ROMAN_24,20,100);
	glColor3f(1.0,0.0,0.0);
	print(" 'n'|'N'",GLUT_BITMAP_TIMES_ROMAN_24,75,100);
	glFlush();
	glColor3f(1.0,1.0,0.0);
	print("<--- Output Rate",GLUT_BITMAP_TIMES_ROMAN_24,540,150);

	glColor3f(0.0,1.0,1.0);
	glBegin(GL_POLYGON);
		glVertex2f(800,80);
		glVertex2f(800,120);
		glVertex2f(840,120);
		glVertex2f(840,80);
	glEnd();

	glColor3f(1.0,1.0,1.0);
	print("<--- Packet Bandwidth",GLUT_BITMAP_TIMES_ROMAN_24,850,100);
	glColor3f(1.0,1.0,1.0);
	print("The bucket Capacity = ",GLUT_BITMAP_TIMES_ROMAN_24,400,660);
	sprintf(temp,"%d",bucketSize);
	glColor3f(1.0,0.0,0.0);
	print(temp,GLUT_BITMAP_TIMES_ROMAN_24,540,660);
	glFlush();
	strcpy(temp,"");
	glFlush();
	}
	return 4;
}

//About page
int about()
{
 background(0,0,0,1000,1000,1000,1000,0);
 glColor3f(0.0,1.0,0.0);
 print("****** ABOUT PAGE ******",GLUT_BITMAP_TIMES_ROMAN_24,400,900);
 glColor3f(0.0,1.0,0.0);
 glLineWidth(2);
 glBegin(GL_LINE_LOOP);
 glVertex2f(70,620);
 glVertex2f(70,800);
 glVertex2f(430,800);
 glVertex2f(430,620);
 glEnd();
  glColor3f(1.0,0.0,0.0);
 glLineWidth(2);
 glBegin(GL_LINE_LOOP);
 glVertex2f(55,590);
 glVertex2f(55,830);
 glVertex2f(445,830);
 glVertex2f(445,590);
 glEnd();
 glColor3f(1.0,1.0,1.0);
 print("Name: Shrikrishna Ganapati Hegde",GLUT_BITMAP_TIMES_ROMAN_24,100,750);
 print("USN: 4SU20CS101",GLUT_BITMAP_TIMES_ROMAN_24,100,720);
 print("3rd Year, Department of CSE",GLUT_BITMAP_TIMES_ROMAN_24,100,690);
 print("Address: Dhundasi Nagar, Sirsi ,Uttar Kannada",GLUT_BITMAP_TIMES_ROMAN_24,100,660);
 glColor3f(0.0,1.0,0.0);
 glLineWidth(2);
 glBegin(GL_LINE_LOOP);
 glVertex2f(540,620);
 glVertex2f(540,800);
 glVertex2f(930,800);
 glVertex2f(930,620);
 glEnd();
  glColor3f(1.0,0.0,0.0);
 glLineWidth(2);
 glBegin(GL_LINE_LOOP);
 glVertex2f(525,590);
 glVertex2f(525,830);
 glVertex2f(945,830);
 glVertex2f(945,590);
 glEnd();
 glColor3f(1.0,1.0,1.0);
 print("Name: Srujan P Jain",GLUT_BITMAP_TIMES_ROMAN_24,570,750);
 print("USN: 4SU20CS107",GLUT_BITMAP_TIMES_ROMAN_24,570,720);
 print("3rd Year, Department of CSE",GLUT_BITMAP_TIMES_ROMAN_24,570,690);
 print("Address: Prahalladh Nagar, Perla Road, Ujire, Dakshina Kannada",GLUT_BITMAP_TIMES_ROMAN_24,570,660);
  glColor3f(0.0,1.0,0.0);
 glLineWidth(2);
 glBegin(GL_LINE_LOOP);
 glVertex2f(70,320);
 glVertex2f(70,500);
 glVertex2f(430,500);
 glVertex2f(430,320);
 glEnd();
  glColor3f(1.0,0.0,0.0);
 glLineWidth(2);
 glBegin(GL_LINE_LOOP);
 glVertex2f(55,290);
 glVertex2f(55,530);
 glVertex2f(445,530);
 glVertex2f(445,290);
 glEnd();
 glColor3f(1.0,1.0,1.0);
 print("Name: Vaishnavi R",GLUT_BITMAP_TIMES_ROMAN_24,100,450);
 print("USN: 4SU20CS118",GLUT_BITMAP_TIMES_ROMAN_24,100,420);
 print("3rd Year, Department of CSE",GLUT_BITMAP_TIMES_ROMAN_24,100,390);
 print("Address: Bilagodi, Hebbarige, Sagar, Shivamogga",GLUT_BITMAP_TIMES_ROMAN_24,100,360);
 glColor3f(0.0,1.0,0.0);
 glLineWidth(2);
 glBegin(GL_LINE_LOOP);
 glVertex2f(540,320);
 glVertex2f(540,500);
 glVertex2f(930,500);
 glVertex2f(930,320);
 glEnd();
  glColor3f(1.0,0.0,0.0);
 glLineWidth(2);
 glBegin(GL_LINE_LOOP);
 glVertex2f(525,290);
 glVertex2f(525,530);
 glVertex2f(945,530);
 glVertex2f(945,290);
 glEnd();
 glColor3f(1.0,1.0,1.0);
 print("Name: Vaishnavi Vijaykumar Hegde",GLUT_BITMAP_TIMES_ROMAN_24,570,450);
 print("USN: 4SU20CS119",GLUT_BITMAP_TIMES_ROMAN_24,570,420);
 print("3rd Year, Department of CSE",GLUT_BITMAP_TIMES_ROMAN_24,570,390);
 print("Address: Guddemane, Bommanalli, Sirsi, Uttara Kannada",GLUT_BITMAP_TIMES_ROMAN_24,570,360);
glColor3f(1.0,1.0,1.0);
	print("<<< Press                             to go back to previous page >>>",GLUT_BITMAP_TIMES_ROMAN_24,330,150);
	glColor3f(1.0,0.0,0.0);
	print(" BACKSPACE ",GLUT_BITMAP_TIMES_ROMAN_24,385,150);
 glFlush();
 return 2;
	
}


//Help page
int help()
{
	background(0,0,0,1000,1000,1000,1000,0);
	glColor3f(0.0,0.5,0.5);
	borders(1,1,1,999,999,999,999,1);
	glColor3f(0.0,1.0,0.0);
	print("****** HELP PAGE ******",GLUT_BITMAP_TIMES_ROMAN_24,400,920);
	glColor3f(0.0,1.0,0.0);
	print("Leaky Bucket Concept: ",GLUT_BITMAP_TIMES_ROMAN_24,50,850);
	glColor3f(1.0,1.0,1.0);
	print("The Leaky Bucket algorithm is a popular flow control machanism used in computer networks to regulate rate of data transmission. ",GLUT_BITMAP_TIMES_ROMAN_24,80,800);
	print("It ensures that the out going data doesnot exceed a predetrmined maximum rate, preventing network congestion and maintaining ",GLUT_BITMAP_TIMES_ROMAN_24,80,750);
    print("quality of service.",GLUT_BITMAP_TIMES_ROMAN_24,80,700);
	glColor3f(0.0,1.0,0.0);
	print("Leaky Bucket Algorithm: ",GLUT_BITMAP_TIMES_ROMAN_24,50,650);
	glColor3f(1.0,1.0,1.0);
	print("Step-1 : Initialize Bucket capacity and Output rate.",GLUT_BITMAP_TIMES_ROMAN_24,80,600);
	print("Step-2 : Initialize the incoming packet size.",GLUT_BITMAP_TIMES_ROMAN_24,80,550);
	print("Step-3 : Compare the incoming packet size with bucket capacity.",GLUT_BITMAP_TIMES_ROMAN_24,80,500);
	print("*   If the incoming packet size is less than the bucket capacity,then the packet will be added to the bucket.",GLUT_BITMAP_TIMES_ROMAN_24,150,450);
	print("*   If the incoming packet size or size of the incoming packet + packet in bucket is greater than the ",GLUT_BITMAP_TIMES_ROMAN_24,150,400);
	print("bucket capacity, then the incoming packet will be dropped.",GLUT_BITMAP_TIMES_ROMAN_24,170,350);
	print("Step-4 : Output the packet based on output rate.",GLUT_BITMAP_TIMES_ROMAN_24,80,300);
	print("Step-5 : Repeat step 3 and 4 until the bucket is empty.",GLUT_BITMAP_TIMES_ROMAN_24,80,250);
	glColor3f(1.0,1.0,1.0);
	print("<<< Press                             to go back to previous page >>>",GLUT_BITMAP_TIMES_ROMAN_24,330,150);
	glColor3f(1.0,0.0,0.0);
	print(" BACKSPACE",GLUT_BITMAP_TIMES_ROMAN_24,385,150);
	glFlush();
	return 3;

}


// Menu page
int myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	background(0,0,0,1000,1000,1000,1000,0);
	glColor3f(0.0,0.5,0.5);
	glLineWidth(10.0);
	borders(1,1,1,999,999,999,999,1);
	glColor3f(1.0,0.6,0.0);
	square(430,500,430,450,530,450,530,500);
	glColor3f(0.0,0.0,0.0);
	borders(430,500,430,450,530,450,530,500);
	glColor3f(1.0,0.6,0.0);
	square(430,600,430,550,530,550,530,600);
	glColor3f(0.0,0.0,0.0);
	borders(430,600,430,550,530,550,530,600);
	glColor3f(1.0,0.6,0.0);
	square(430,700,430,650,530,650,530,700);
	glColor3f(0.0,0.0,0.0);
	borders(430,700,430,650,530,650,530,700);
	glColor3f(1.0,0.6,0.0);
	square(430,800,430,750,530,750,530,800);
	glColor3f(0.0,0.0,0.0);
	borders(430,800,430,750,530,750,530,800);
	glColor3f(1.0,0.6,0.0);
	square(430,400,430,350,530,350,530,400);
	glColor3f(0.0,0.0,0.0);
	borders(430,400,430,350,530,350,530,400);
	

	glColor3f(0.0,1.0,1.0);
	print("****** MENU PAGE ******",GLUT_BITMAP_TIMES_ROMAN_24,400,870);
	glColor3f(1.0,1.0,1.0);
	print("<<< Press                            to go back to previous page >>>",GLUT_BITMAP_TIMES_ROMAN_24,330,150);
	glColor3f(1.0,0.0,0.0);
	print(" BACKSPACE",GLUT_BITMAP_TIMES_ROMAN_24,385,150);
	glColor3f(0.0,0.0,0.0);
	print("Exit(ESC)",GLUT_BITMAP_TIMES_ROMAN_24,450,370);
	glColor3f(0.0,0.0,0.0);
	print("Help(h)",GLUT_BITMAP_TIMES_ROMAN_24,450,470);
	glColor3f(0.0,0.0,0.0);
	print("About(a)",GLUT_BITMAP_TIMES_ROMAN_24,450,570);
	glColor3f(0.0,0.0,0.0);
	print("Flowchart(f)",GLUT_BITMAP_TIMES_ROMAN_24,450,670);
	glColor3f(0.0,0.0,0.0);
	print("Start(s)",GLUT_BITMAP_TIMES_ROMAN_24,450,770);
	glFlush();
	return 1;
}



//Front page
void display()
{
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	//clear color (background to white)
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,1000.0,0.0,1000.0);
	// Background function call
	background(0,0,0,1000,1000,1000,1000,0);
	glColor3f(0.0,0.5,0.5);
	borders(1,1,1,999,999,999,999,1);
	//To display Texts
	glColor3f(1.0,0.4,0.0);
	print("SDM INSTITUE OF TECHNOLOGY,UJIRE 574240",GLUT_BITMAP_TIMES_ROMAN_24,350,920);
	glColor3f(1.0,0.4,0.0);
	print("DEPARTMENT OF COMPUTER SCIENCE & ENGINEERING",GLUT_BITMAP_TIMES_ROMAN_24,315,860);
	glColor3f(1.0,0.4,0.0);
	print("COMPUTER GRAPHICS LABORATORY WITH MINI PROJECT(18CSL67)",GLUT_BITMAP_TIMES_ROMAN_24,280,800);
	glColor3f(1.0,0.8,0.0);
	print("A MINI PROJECT ON",GLUT_BITMAP_TIMES_ROMAN_24,430,720);
	glColor3f(1.0,0.0,0.0);
	print("LEAKY BUCKET ALGORITHM",GLUT_BITMAP_TIMES_ROMAN_24,400,660);
	glColor3f(1.0,0.4,0.0);
	print("SUBMITTED BY : ",GLUT_BITMAP_TIMES_ROMAN_24,100,480);
	glColor3f(1.0,0.4,0.0);
	print("GUIDED BY : ",GLUT_BITMAP_TIMES_ROMAN_24,750,480);
	glColor3f(1.0,0.4,0.0);
	print("Shrikrishna Hegde   (4SU20CS101)",GLUT_BITMAP_TIMES_ROMAN_24,100,420);
	glColor3f(1.0,0.4,0.0);
	print("Srujan P Jain   (4SU20CS107)",GLUT_BITMAP_TIMES_ROMAN_24,100,380);
	glColor3f(1.0,0.4,0.0);
	print("Vaishnavi R    (4SU20CS118)",GLUT_BITMAP_TIMES_ROMAN_24,100,340);
	glColor3f(1.0,0.4,0.0);
	print("Vaishnavi Hegde  (4SU20CS119)",GLUT_BITMAP_TIMES_ROMAN_24,100,300);
	glColor3f(1.0,0.4,0.0);
	print("Mr. Arjun K",GLUT_BITMAP_TIMES_ROMAN_24,750,420);
	glColor3f(1.0,0.4,0.0);
	print("Asst. Professor",GLUT_BITMAP_TIMES_ROMAN_24,750,390);
	glColor3f(1.0,0.4,0.0);
	print("Department of CSE",GLUT_BITMAP_TIMES_ROMAN_24,750,360);
	glColor3f(1.0,1.0,1.0);
	print("<<< Press                 for                page >>>",GLUT_BITMAP_TIMES_ROMAN_24,400,120);
	glColor3f(1.0,0.0,0.0);
	print("  ENTER         MENU",GLUT_BITMAP_TIMES_ROMAN_24,450,120);
	glColor3f(1.0,1.0,0.0);
	print("<<< Press            to Exit >>>",GLUT_BITMAP_TIMES_ROMAN_24,430,80);
	glColor3f(1.0,0.0,0.0);
	print("   ESC",GLUT_BITMAP_TIMES_ROMAN_24,480,80);
	//End of Texts
	glFlush();
}



// Keyboard Function
void keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 13:if(current_display==0 || current_display==1){ 
		current_display=myDisplay(); // Enter Key
		}
		break;
	case 8 : if(current_display == 1){ // Backspace
				display();
			 }
			 else if(current_display == 2 || current_display==3 || current_display==4 || current_display==6){
				current_display= myDisplay();
			 }
			 else if(current_display == 9){
				 packSize=0;acceptSize=0;rejectSize=0;bucketSize=0;size=0;
				 current_display = myDisplay();
			 }
		break;
	case 'A':
	case 'a':
		if(current_display==1){
		current_display=about();
		}
			break;
	case 'h':
	case 'H':
		if(current_display==1){
		current_display=help();
		}
			break;
	case 's':
	case 'S':
		if(current_display==1){
		current_display=bucket();
		}
		break;
	case 'f':
	case 'F':
		if(current_display==1){
		current_display=flowchart();
		}
		break;
	case 'N':
	case 'n':
			if(current_display==4)
			current_display = leakyBucket(packets,numberPackets);
		
			break;
	case 27: if(current_display==1 || current_display==0 || current_display == 9 ){
		exit(0);
			 }
	}
}


//MAIN FUNCTION
void main(int argc,char ** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(1920,1080);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Leaky Bucket");
	//frontpage Function
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutFullScreen();
	//Keyboard actions
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}