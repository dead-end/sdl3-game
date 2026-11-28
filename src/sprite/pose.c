#include "pose.h"

/**
 * The function initializes the pose.
 */
void pose_setup(Pose *pose, const int x, const int y, const int direction)
{
    pose->field.x = x;
    pose->field.y = y;
    pose->direction = direction;
}