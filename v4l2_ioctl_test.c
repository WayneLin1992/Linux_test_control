#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <linux/media.h>

#include "vcam.h"

static char ctl_path[128] = "/dev/vcamctl";
static char ctl_path1[128];
int main()
{
    struct vcam_device_spec dev = {.idx = 0};
    struct v4l2_format fmt;
    struct v4l2_capability cap;
    int ret;

    int fd = open(ctl_path, O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "Failed to open %s device\n", ctl_path);
        return -1;
    }

    printf("Available virtual V4L2 compatible devices:\n");
    while (!ioctl(fd, VCAM_IOCTL_GET_DEVICE, &dev)) {
        dev.idx++;
        printf("%d. %s(%d,%d,%s) -> %s\n", dev.idx, dev.fb_node, dev.width,
               dev.height, dev.pix_fmt == VCAM_PIXFMT_RGB24 ? "rgb24" : "yuyv",
               dev.video_node);
    }
    close(fd);
    memcpy(ctl_path1, dev.video_node, sizeof(dev.video_node));
    fd = open(ctl_path1, O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "Failed to open %s device\n", ctl_path1);
    }else{
    	printf("success %s \n", ctl_path1);
    }
    ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
    if(ret < 0){
    	fprintf(stderr, "Failed get capability");
    }else{
    	printf("capability : %08X\n", cap.capabilities);
    }
    fmt.type = 1; //important
    ret = ioctl(fd, VIDIOC_TRY_FMT, &fmt);
    if(ret < 0){
        fprintf(stderr, "Failed get format");
    }else{
    	printf("type : %d\nwidth : %d", fmt.type,fmt.fmt.pix.width);
    }
    close(fd);
    return 0;
}
