#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
static const Worker_T INVALID_ID = (unsigned int)-1;

bool Schedule_Helper(const AvailabilityMatrix& avail, const size_t dailyNeed, 
                const size_t maxShifts, DailySchedule& sched, 
                int numWorkers, int daysToFill, int row, int col);
bool validSchedule(const AvailabilityMatrix& avail, DailySchedule& sched, 
                int row, int col, int worker, int numWorkers, const size_t maxShifts);

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // store total worker count
    int numWorkers = avail[0].size();
    // store days to fill 
    int daysToFill = avail.size();
    // create an empty schedule full with INVALID_ID 
    vector<vector<Worker_T> > tempSchedule(daysToFill, vector<Worker_T> (dailyNeed, INVALID_ID));
    sched = tempSchedule;
    // call thee helper function with row and col starting at 0
    return Schedule_Helper(avail, dailyNeed, maxShifts, sched, numWorkers, daysToFill, 0, 0);
}

/**
 * Recursive helper that schedules the workers based off their availability
 * @param avail : availability matrix
 * @param dailyNeed : the amount of shifts to fill per day 
 * @param maxShifts : the max amount of shifts a worker can work 
 * @param sched : a nested vector that schedules workers
 * @param numWorkers : the number of workers in the avail matrix
 * @param daysToFill : the number of days that the schedule should account for
 * @param row : the current row
 * @param col : the current col
 * @return true/false if a proper schedule that meets the constraints exists or not
 */ 
bool Schedule_Helper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, 
                    DailySchedule& sched, int numWorkers, int daysToFill, int row, int col) 
{
    // check if we have reached the end of the schedule indicating a complete/valid schedule
    if(row == daysToFill - 1 && col == (int)dailyNeed) {
        return true;
    } 
    // checking if we reached the end of a column in the schedule
    // if so, move on to the next row
    if(col == (int)dailyNeed) {
        row = row + 1;
        col = 0;
    }
    // iterate over each worker
    for(int i = 0; i < numWorkers; i++) {
        // check if placing the worker 'i' at position (row, col) maintins a valid schedule
        if(validSchedule(avail, sched, row, col, i, numWorkers, maxShifts)) { 
            // recurse on and try the next position in the col
            if(Schedule_Helper(avail, dailyNeed, maxShifts, sched, numWorkers, daysToFill, row, col+1)) {
                return true;
            }
        }
    }
    return false;
}

/**
 * Checks the validity of adding a worker to a certain slot in the given schedule
 * @param avail : availability matrix 
 * @param sched : the schedule that stores the worker's shifts
 * @param row : the current row 
 * @param col : the current col
 * @param worker : the current worker ID to insert into the schedule 
 * @param numWorkers : the max number of workers that were in the avail matrix
 * @param maxShifts : max number of shifts each worker is allowed to work
 * @return true/false whether the adding the worker to the schedule maintains a valid schedule
 * PREREQUISITE: the current schedule and everything before it is valid -- we are just testing the validity of the slot at (row,col)
 */ 
bool validSchedule(const AvailabilityMatrix& avail, DailySchedule& sched, 
    int row, int col, int worker, int numWorkers, const size_t maxShifts)
{
    // see if the worker can even work on this day 
    if(avail[row][worker] == 0) {
        return false;
    }
    // set the worker into the scheduled slot at (row, col)
    sched[row][col] = worker;
    // start by checking if the row is valid
    vector<int> workerListA(numWorkers, 0); 
    // count how often a worker appears in the schedule
    for(int i = 0; i < (int)sched[row].size(); i++) {
        if(sched[row][i] == INVALID_ID) {
            break;
        }
        workerListA[sched[row][i]]++;
    }
    // iterate through the vector, if anything is > 1, return false immediately
    for(int i = 0; i < numWorkers; i++) {
        if(workerListA[i] > 1) {
            // before returning, set back the position to INVALID_ID
            sched[row][col] = INVALID_ID;
            return false;
        }
    }
    // check if the maxShifts are all valid
    // iterate over the current schedule and count how many time 'worker' appears
    // can do this because we are assuming that the original schedule is always 
    // a valid schedule, so we just have to check the added 'worker' to see if 
    // it maintains this status
    int count = 0;
    for(int i = 0; i < (int)sched.size(); i++) {
        for(int j = 0; j < (int)sched[i].size(); j++) {
            if(sched[i][j] == INVALID_ID) {
                break;
            }
            if((int)sched[i][j] == worker) {
                count++;
            }
        }
    }
    // check if count goes beyond the maximum number of shifts allowed
    if(count > (int)maxShifts) {
        sched[row][col] = INVALID_ID;
        return false;
    }
    // at this point, the schedule is valid so return true
    return true;
}