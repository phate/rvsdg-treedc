#include "main.h"

/*
 * Upper and lower bound checks for heuristic functions.
 * if a lower bound is to small or an upper bound is too high this is only
 * reported as a warning as it will merely not generate a tight enough bound
 * if the lower bound is too high or the upper bound is is too small however
 * this is an actual error as it will not necessarily generate a correct bound,
 * this is therefore reported as an error in the test runner
 */

void CHECK_UPPER_BOUND(int found, int bound, const char heuristic[]);
void CHECK_LOWER_BOUND(int found, int bound, const char heuristic[]);
