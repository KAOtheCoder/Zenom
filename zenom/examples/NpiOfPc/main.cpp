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
	double qd[2];


	// ----- Control Parameters -----
	double q[2];
	double amp[2];
	double freq[2];
	double Ke[2];
	double k1[2];
	double ki[2];
	double beta[2];


	// ----- Variables -----
	Integrator< double > mRfeIntegrator[2];   // rf+e integrator
	Integrator< double > mPdotIntegrator[2];  // p_dot integrator
	Integrator< double > mEfdotIntegrator[2]; // ef_dot integrator

	double rfeInt[2];
	double p[2];
	double ef[2];
	double rf[2];
	double deltaE[2];
	double ke[2];

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
	registerLogVariable(qd, "qd", 1, 2);

	// ----- Register the control paramateres -----
	registerControlVariable(q, "q", 1, 2);
	registerControlVariable(amp, "amp", 1, 2);
	registerControlVariable(freq, "freq", 1, 2);
	registerControlVariable(Ke, "Ke", 1, 2);
	registerControlVariable(k1, "k1", 1, 2);
	registerControlVariable(ki, "ki", 1, 2);
	registerControlVariable(beta, "beta", 1, 2);

	amp[0] = 0.7;
	amp[1] = 1.2;
	freq[0] = 3;
	freq[1] = 3;

	Ke[0] = 200;
	Ke[1] = 50;

	k1[0] = 1;
	k1[1] = 20;

	ki[0] = 20;
	ki[1] = 5;

	beta[0] = 50;
	beta[1] = 150;

	deltaE[0] = 10 * Deg2Rad;
	deltaE[1] = 10 * Deg2Rad;

	// ----- Prints message in screen -----
	std::cout
		<< "Position Constrained Ouptut Feedback Nonlinear PI for 2-link planar robot "
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
	mRfeIntegrator[0].setSamplingPeriod( period() );
	mRfeIntegrator[0].reset( 0 );

	mRfeIntegrator[1].setSamplingPeriod( period() );
	mRfeIntegrator[1].reset( 0 );

	mPdotIntegrator[0].setSamplingPeriod( period() );
	mPdotIntegrator[0].reset( 0 );

	mPdotIntegrator[1].setSamplingPeriod( period() );
	mPdotIntegrator[1].reset( 0 );

	mEfdotIntegrator[0].setSamplingPeriod( period() );
	mEfdotIntegrator[0].reset( 0 );

	mEfdotIntegrator[1].setSamplingPeriod( period() );
	mEfdotIntegrator[1].reset( 0 );

	rfeInt[0] = 0;
	rfeInt[1] = 0;

	p[0] = 0;
	p[1] = 0;

	ef[0] = 0;
	ef[1] = 0;

	q[0] = 0;
	q[1] = 0;

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
	//desired trajectory
	qd[0] = amp[0] * sin( elapsedTime() * freq[0]) * (1-exp(-0.3 * pow(elapsedTime(),3)));
	qd[1] = amp[1] * sin( elapsedTime() * freq[1]) * (1-exp(-0.3 * pow(elapsedTime(),3)));

	// error
	e[0] = qd[0] - q[0] ;
	e[1] = qd[1] - q[1] ;

	rf[0] = p[0] - (k1[0] + 1)*e[0];
	rf[1] = p[1] - (k1[1] + 1)*e[1];

	ke[0] = Ke[0] / (deltaE[0]*deltaE[0] - e[0]*e[0]);
	ke[1] = Ke[1] / (deltaE[1]*deltaE[1] - e[1]*e[1]);

	// calculate tau
	tau[0] = -(k1[0]+1)*rf[0] + (ki[0]+ke[0])*e[0] + beta[0]*tanh(e[0]+ef[0]) + ki[0]*rfeInt[0];
	tau[1] = -(k1[1]+1)*rf[1] + (ki[1]+ke[1])*e[1] + beta[1]*tanh(e[1]+ef[1]) + ki[1]*rfeInt[1];

	tau[0] = tau[0] * 10 / 287;
	tau[1] = tau[1] * -10 / 52;

	if(tau[0] > 10) tau[0] = 10;
	if(tau[0] < -10) tau[0] = -10;

	if(tau[1] > 10) tau[1] = 10;
	if(tau[1] < -10) tau[1] = -10;

	// integrate rf + e
	rfeInt[0] = mRfeIntegrator[0].integrate(rf[0]+e[0]);
	rfeInt[1] = mRfeIntegrator[1].integrate(rf[1]+e[1]);

	// integrate p
	p[0] = mPdotIntegrator[0].integrate(-p[0] + ke[0]*e[0] - ef[0] - (k1[0] + 1)*rf[0]);
	p[1] = mPdotIntegrator[1].integrate(-p[1] + ke[1]*e[1] - ef[1] - (k1[1] + 1)*rf[1]);

	// integrate ef
	ef[0] = mEfdotIntegrator[0].integrate(rf[0]-ef[0]);
	ef[1] = mEfdotIntegrator[1].integrate(rf[1]-ef[1]);

	//to see errors in degree form
	e[0] = e[0] * 180 / pi;
	e[1] = e[1] * 180 / pi;

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
