///*
// * TargetTask.cpp
// *
// *  Created on: Apr 4, 2012
// *      Author: root
// */

//#include <iostream>
//#include "targettask.h"
//#include "targetui.h"

//TargetTask::TargetTask( TargetUI* pDaq
//                    ,std::chrono::duration<double> period
//                    ,std::string name)
//    : TaskXn(name, period,TaskXn::maxPriority())
//    , mDaq(pDaq)
//{
//}

//void TargetTask::run()
//{
//    int error = 0;
//    if( mDaq->state != STOPPED )
//    {
//        if( mDaq->state != PAUSED )
//        {
//            try
//            {
//                error = mDaq->doloop();			// User Function
//                if( error )
//                {
//                    std::cerr << "The target doloop() function returned non zero: "
//                              << error << std::endl;
//                }
//            }
//            catch( std::exception& e )
//            {
//                error = -1;
//                std::cerr << "An exception occured in the target doloop() function:"
//                          << e.what() << std::endl;
//            }
//            catch (...)
//            {
//                error = -1;
//                std::cerr << "An unknown exception occured in the target doloop()"
//                             " function." << std::endl;
//            }
//        }
//	}
//}
