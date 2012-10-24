#include <StdAfx.h>
#include <iostream>
#include "TrackerPod.h"
int main(char *argv){
static short INITIAL_GRANULARITY = 15;//initial granularity is 15, as of now this is arbitrary
static short GRANULARITY_DELTA = 15;//how much granularity changes when you move it, also arbitrary
bool done = false;
char input;
char client_id = 'a';
short gran = INITIAL_GRANULARITY; 
CTrackerPod pod;
pod = CTrackerPod.CreateTrackerPod();
CTrackerPod.initialize(*client_id, true);
cout << "Welcome to TrackerPod Control, press WASD to pan or tilt, RF to zoom or TG to increase or decrease granularity of future movements. Press X to exit"<<endl;
while(!done)
	{
		cout << "Please enter your next command" << endl;
		cin >> input;
		if(input == 'w' || input == 'W'){
			pod.move_by(0, gran);//tilt up
		}
		if(input == 's' || input == 'S'){
			pod.move_by(0, (gran * -1));//tilt down
		}
		if(input == 'a' || input == 'A'){
			pod.move_by((gran * -1), 0);//pan left
		}
		if(input == 'd' || input == 'D'){
			pod.move_by(gran, 0);//pan right
		}
		if(input == 'r' || input == 'R'){
			pod.move_by(gran, 0);//zoom in
		}
		if(input == 'f' || input == 'F'){
			pod.move_by(gran, 0);//zoom out
		}
		if(input == 't' || input == 'T'){
			gran += GRANULARITY_DELTA;//increase granularity
		}
		if(input == 'g' || input == 'G'){
			gran -= GRANULARITY_DELTA;//decrease granularity
		}
		if(input == 'x' || input == 'X'){
			done = true;//exit
		}
	}	
	pod.ReleaseTrackerPod();//get the memory back
	return 0;
}