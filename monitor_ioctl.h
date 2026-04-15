#ifndef MONITOR_IOCTL_H
#define MONITOR_IOCTL_H

#include <linux/ioctl.h>

#define MAGIC 'k'

#define SET_LIMIT _IOW(MAGIC, 1, int)
#define ADD_PID   _IOW(MAGIC, 2, int)

#endif
