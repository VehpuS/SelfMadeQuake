// Named host.c in Quake source code
#include "common.h"
#include "instance.h"

/**************
* Timer Code *
**************/
bool timerIsInitialized = false;
double secondsPerTick = 0;
int64_t lastMeasuredTicks = 0;
double gameTimePassed = 0;

// Will run on each iteration of the game loop to calculate how much real time passed in the game loop
float calculateGameLoopElapsedTime(
	const float iterationStartTime,
	float* const nextFrameStartTime,
	__int64* const lastMeasuredTicks,
	double* const gameTimePassed,
	getTotalElapsedTime getTotalElapsedTime) {
	float totalElapsedTime = getTotalElapsedTime(lastMeasuredTicks, gameTimePassed);
	*nextFrameStartTime = totalElapsedTime;
	return (totalElapsedTime - iterationStartTime);
}

static bool isRunning = false;

// Platform independent initialization code
void initQuake(void) {
	isRunning = true;
}

// Platform independent update code
void updateFrame(float timestep) {
	//update game
}

// Platform independent render code
void renderFrame(float timeLagAfterUpdate, float timestep) {
	float updateRenderLag = timeLagAfterUpdate / timestep;
}

// Platform independent shutdown code
void shutdownQuake(void) {
	isRunning = false;
}

// Platform independent game loop
QuakeRC runQuakeGameLoop(
	const int targetFPS,
	initProgramTimer initProgramTimer,
	getTotalElapsedTime getTotalElapsedTime,
	checkInput checkInput)
	{
	// Run platform independent initialization
	initQuake();

	// Setup timing logic
	initProgramTimer(&secondsPerTick, &lastMeasuredTicks, &timerIsInitialized);
	float targetTimestep = 1.0f / (float)targetFPS;
	float gameLoopElapsedTime = 0;
	float nextFrameStartTime = 0;

	// Designed using tips from http://gameprogrammingpatterns.com/game-loop.html
	while (isRunning) {
		// Update timer
		gameLoopElapsedTime = calculateGameLoopElapsedTime(
			nextFrameStartTime,
			&nextFrameStartTime,
			&lastMeasuredTicks,
			&gameTimePassed,
			getTotalElapsedTime);

		// Check the OS for input
		checkInput();

		// Start an update loop until we return to "real time" or we've looped too much
		for (int updateLoops = 0;
			gameLoopElapsedTime > targetTimestep &&
				updateLoops < MAX_UPDATE_LOOPS_BETWEEN_RENDER;
			updateLoops += 1) {
			updateFrame(targetTimestep);
			gameLoopElapsedTime -= targetTimestep;
		}

		renderFrame(gameLoopElapsedTime, targetTimestep);
	}

	return (0);
}