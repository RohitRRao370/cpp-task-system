#ifndef JOB_H
#define JOB_H
struct Job {
	virtual void execute() = 0;
    virtual ~Job() = default;
};
#endif
