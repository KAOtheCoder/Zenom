/**

 * Zenom - Hard Real-Time Simulation Enviroment
 * @author zenom
 *
 * TargetTest
 * A simple example of a control program.
 * Does not require any hardware, just generates a sine signal.
 */
#include <iostream>
#include <controlbase.h>
#include <math.h>

class TargetTest : public ControlBase
{
public:

    // ----- User Functions -----
    // This functions need to be implemented by the user.
    int initialize();
    int start();
    int doloop();
    int stop();
    int terminate();

private:
    // ----- Log Variables -----
    double out1;
    double out2;
    double out3;


    // ----- Control Parameters -----
    double in1;
    double in2;
    double in3;


    // ----- Variables -----
};

/**
 * This function is called when the control program is loaded to zenom.
 * Use this function to register control parameters, to register log variables
 * and to initialize control parameters.
 *
 * @return Return non-zero to indicate an error.
 */
int TargetTest::initialize()
{
    // ----- Initializes log and control variables -----
    // ----- Register the log variables -----
    registerLogVariable(&out1, "out1");
    registerLogVariable(&out2, "out2");
    registerLogVariable(&out3, "out3");

    // ----- Register the control paramateres -----
    registerControlVariable(&in1, "in1");
    registerControlVariable(&in2, "in2");
    registerControlVariable(&in3, "in3");

    // ----- Prints message in screen -----
    std::cout
        << "This is a simple test program that does outN=inN "
        << std::endl << std::endl;

    return 0;
}

/**
 * This function is called when the START button is pushed from zenom.
 *
 * @return If you return 0, the control starts and the doloop() function is
 * called periodically. If you return nonzero, the control will not start.
 */
int TargetTest::start()
{


    return 0;
}


/**
 * This function is called periodically (as specified by the control frequency).
 * The useful functions that you can call used in doloop() are listed below.
 *
 * frequency()          returns frequency of simulation.
 * period()             returns period of simulation.
 * duration()           returns duration of simulation.
 * simTicks()           returns elapsed simulation ticks.
 * simTimeInNano()      returns elapsed simulation time in nano seconds.
 * simTimeInMiliSec()   returns elapsed simulation time in miliseconds.
 * simTimeInSec()       returns elapsed simulation time in seconds.
 * overruns()           returns the count of overruns.
 *
 * @return If you return 0, the control will continue to execute. If you return
 * nonzero, the control will abort.
 */
int TargetTest::doloop()
{
	out1 = in1;
	out2 = in2;
	out3 = in3;
    return 0;
}


/**
 * Called when a timed run ends or the STOP button is pushed from zenom.
 *
 * @return Return non-zero to indicate an error.
 */
int TargetTest::stop()
{


    return 0;
}


/**
 * This function is called when the control is unloaded. It happens when
 * the user loads a new control program or exits.
 *
 * @return Return non-zero to indicate an error.
 */
int TargetTest::terminate()
{


    return 0;
}


/**
 * The main function starts the control program
 */
int main( int argc, char *argv[] )
{
    TargetTest c;
    c.run( argc, argv );

    return 0;
}
