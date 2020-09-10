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
#include <Integrator.hpp>
#include <Differentiator.hpp>

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
	double tau[2];
	double e[2];
	double ed[2];


	// ----- Control Parameters -----
	double q[2];
	double a[2];
	double freq[2];
	double alpha[2];
	double Kr[2];
	double ki[2];
	double beta[2];


	// ----- Variables -----
	Integrator< double > mrInt[2];  // p_dot integrator
	Differentiator< double > mqDiff[2]; // q differentiator

	double d[2];
	double dd[2];
	double qd[2];
	double r[2];
	double rint[2];
	double t,t2,t3;

	const double Deg2Rad = 0.017453292519943;
	const double pi = 3.14159265359;
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
	registerLogVariable(tau, "tau", 1, 2);
	registerLogVariable(e, "e", 1, 2);
	registerLogVariable(ed, "ed", 1, 2);

	// ----- Register the control paramateres -----
	registerControlVariable(q, "q", 1, 2);
	registerControlVariable(a, "a", 1, 2);
	registerControlVariable(freq, "freq", 1, 2);
	registerControlVariable(alpha, "alpha", 1, 2);
	registerControlVariable(Kr, "Kr", 1, 2);
	registerControlVariable(ki, "ki", 1, 2);
	registerControlVariable(beta, "beta", 1, 2);

	a[0] = 0.7;
	a[1] = 1.2;
	freq[0] = 3;
	freq[1] = 3;

	alpha[0] = 3;
	alpha[1] = 3;

	Kr[0] = 70;
	Kr[1] = 20;

	ki[0] = 25;
	ki[1] = 25;

	beta[0] = 8000;
	beta[1] = 3000;

	// ----- Prints message in screen -----
	std::cout
		<< "Full State Feedback Nonlinear PI for 2-link planar robot "
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
	mrInt[0].setSamplingPeriod( period() );
	mrInt[0].reset( 0 );

	mrInt[1].setSamplingPeriod( period() );
	mrInt[1].reset( 0 );

	mqDiff[0].reset();
	mqDiff[0].setSamplingPeriod( period() );
	mqDiff[0].enableFilter();
	mqDiff[0].setDampingRatio(1);
	mqDiff[0].setCutOffFrequencyHz(500);

	mqDiff[1].reset();
	mqDiff[1].setSamplingPeriod( period() );
	mqDiff[1].setDampingRatio(1);
	mqDiff[1].setCutOffFrequencyHz(500);

	q[0] = 0;
	q[1] = 0;

	qd[0] = 0;
	qd[1] = 0;

	rint[0] = 0;
	rint[1] = 0;

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
	//derivate q to get qd
	qd[0] = mqDiff[0].differentiate(q[0]);
	qd[1] = mqDiff[1].differentiate(q[1]);

	//desired trajectory
	t = elapsedTime();
	t2 = pow(t,2);
	t3 = pow(t,3);
	d[0] = a[0] * sin(t*freq[0]) * (1-exp(-0.3 * t3));
	d[1] = a[1] * sin(t*freq[1]) * (1-exp(-0.3 * t3));
	dd[0]= a[0]*(freq[0]*cos(t*freq[0])*(1-exp(-0.3*t3))+0.9*t2*exp(-0.3*t3)*sin(t*freq[0]));
	dd[1]= a[1]*(freq[1]*cos(t*freq[1])*(1-exp(-0.3*t3))+0.9*t2*exp(-0.3*t3)*sin(t*freq[1]));

	// error
	e[0] = d[0] - q[0] ;
	e[1] = d[1] - q[1] ;
	ed[0] = dd[0] - qd[0];
	ed[1] = dd[1] - qd[1];

	r[0] = ed[0]+alpha[0]*e[0];
	r[1] = ed[1]+alpha[1]*e[1];

	// calculate tau
	tau[0] = Kr[0]*r[0] + ki[0]*rint[0] + e[0] + beta[0]*tanh(e[0]);
	tau[1] = Kr[1]*r[1] + ki[1]*rint[1] + e[1] + beta[1]*tanh(e[1]);

	// integrate r
	rint[0] = mrInt[0].integrate(r[0]);
	rint[1] = mrInt[1].integrate(r[1]);

	//to see errors in degree form
	e[0] = e[0] * 180 / pi;
	e[1] = e[1] * 180 / pi;

	ed[0] = ed[0] * 180 / pi;
	ed[1] = ed[1] * 180 / pi;

	// for robot
	tau[0] = tau[0] * 10.0 / 287.0;
	tau[1] = tau[1] * -10.0 / 52.0;

	// bound tau
	if(tau[0] > 9.6) tau[0] = 9.6;
	if(tau[0] < -9.6) tau[0] = -9.6;

	if(tau[1] > 9.6) tau[1] = 9.6;
	if(tau[1] < -9.6) tau[1] = -9.6;

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
