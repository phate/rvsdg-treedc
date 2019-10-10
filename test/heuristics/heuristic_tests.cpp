#include <heuristic_tests.h>

void CHECK_UPPER_BOUND(int found, int bound, const char heuristic[])
{
    if (found > bound)
        printf("%-25s: upper bound too high, found %d\n", heuristic, found);
    else if (found < bound) {
        printf("%-25s: failed with too low upper bound, found %d\n", heuristic, found);
        CHECK(found > bound);
    } else
        printf("%-25s: Found expected upper bound!\n", heuristic);
}

void CHECK_LOWER_BOUND(int found, int bound, const char heuristic[])
{
    if (found < bound)
        printf("%-25s: lower bound too small, found %d\n", heuristic, found);
    else if (found > bound) {
        printf("%-25s: failed with too high lower bound, found %d\n", heuristic, found);
        CHECK(found < bound);
    } else
        printf("%-25s: Found expected lower bound!\n", heuristic);
}
