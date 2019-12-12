/*
 * TargetTask.h
 *
 *  Created on: Apr 4, 2012
 *      Author: root
 */

#ifndef TARGET_TASK_H_
#define TARGET_TASK_H_

#include <TaskXn.h>
#include <znm-core_global.h>

class daq;
typedef int (daq::*task_callback)(void);

class TargetTask: public TaskXn
{
public:
    TargetTask( daq* pDaq
              ,std::chrono::duration<double> period
              ,std::string name);

private:
    daq *mDaq;
    double mDuration;
    task_callback mTask;
    void run() override;

};

#endif /* TARGET_TASK_H_ */
