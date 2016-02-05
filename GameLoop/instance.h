#pragma once
// Named host.h in Quake source code
#ifndef _INSTANCE
#define _INSTANCE

#define MAX_UPDATE_LOOPS_BETWEEN_RENDER (10)


typedef enum QUAKE_EXIT_CODE {
	QUAKE_SUCCESS,
	QUAKE_FAILURE
} QuakeRC;

/**************
* Timer Code *
**************/
extern bool timerIsInitialized;
extern double secondsPerTick;
extern int64_t lastMeasuredTicks;
extern double gameTimePassed;

// Initialize the program timer on a specific platform
typedef void(*initProgramTimer)(
	// the time it takes for each tick to occur on the system
	double* const secondsPerTick,
	// will store the total number of ticks init
	int64_t* const lastMeasuredTicks,
	// will undicate that the timer has been initialized
	bool* const timerIsInitialized);

// Get the total elapsed time on a specific platform
typedef float(*getTotalElapsedTime)(
	__int64* const lastMeasuredTicks,
	double* const gameTimePassed);

// Will run on each itteration of the game loop to calculate
// how much real time passed in the game loop
float calculateGameLoopElapsedTime(
	const float iterationStartTime,
	float* const nextFrameStartTime,
	__int64* const lastMeasuredTicks,
	double* const gameTimePassed,
	getTotalElapsedTime getTotalElapsedTime);

/********************
* Input processing *
********************/
// Check the OS for input
typedef void (*checkInput)(void);


/*****************
* Game functions *
******************/
// Platform independent init code
void initQuake(void);

// Platform independent update code
void updateFrame(
	// update the frame based on the time passed
	float timestep);

// Platform independent render code
void renderFrame(
	// time lag remaining after the last update call
	float timeLagAfterUpdate,
	// time step for each frame
	float timestep);

// Platform independent shutdown code
void shutdownQuake(void);

// Platform independent game loop
QuakeRC runQuakeGameLoop(
	// The FPS we wanto run at
	const int targetFPS,
	// platform specific function to initialize the program timer
	initProgramTimer initProgramTimer,
	// platform specific function to calculated the elapsed time in each itteration of the game loop
	getTotalElapsedTime getTotalElapsedTime,
	// platform specific function to handle input
	checkInput checkInput);

#endif  // _INSTANCE