/*
 * TargetTask.h
 *
 *  Created on: Apr 4, 2012
 *      Author: root
 */

#ifdef TARGET_TASK_H_
#define TARGET_TASK_H_

#include <TaskXn.h>
#include <znm-core_global.h>

class TargetUI;
typedef int (TargetUI::*task_callback)(void);

class TargetTask: public TaskXn
{
public:
    TargetTask( TargetUI* pDaq
              ,std::chrono::duration<double> period
              ,std::string name);

private:
    TargetUI *mDaq;
    double mDuration;
    task_callback mTask;
    void run() override;

};

#endif /* TARGET_TASK_H_ */
