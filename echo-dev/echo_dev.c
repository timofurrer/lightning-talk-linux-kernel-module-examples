/**
 *
 * Kernel Module which creates an echo device.
 *
 * @copyright: GPLv2 (see LICENSE), by Timo Furrer <tuxtimo@gmail.com>
 *
 */

// use kernel module name in front of kernel log messages
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

// module header information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Timo Furrer");
MODULE_DESCRIPTION("Kernel module exposing an echo device");
MODULE_VERSION("1.0");

// device name to use
#define DEVICE_NAME "echo"

// the internal buffer to capture the echo
#define ECHO_BUFFER_MAX_SIZE 64
static char echo_buffer[ECHO_BUFFER_MAX_SIZE];


static int device_open(struct inode *inode, struct file *file) {
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    return 0;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset) {
    ssize_t bytes_read = 0;
    loff_t current_offset = *offset;
    char *current_buffer = echo_buffer;

    while(length && (ECHO_BUFFER_MAX_SIZE - current_offset) > 0) {
        put_user(*(current_buffer++), buffer++);
        length--;
        current_offset++;
        bytes_read++;
    }

    *offset = bytes_read;
    return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset) {
    ssize_t write_size;
    ssize_t bytes_written;
    ssize_t bytes_not_written;

    // clear internal echo buffer
    memset(echo_buffer, '\0', ECHO_BUFFER_MAX_SIZE);

    // copy data from user space to kernel space echo buffer
    write_size = (length > ECHO_BUFFER_MAX_SIZE) ? ECHO_BUFFER_MAX_SIZE : length;
    bytes_not_written = copy_from_user(echo_buffer, buffer, write_size);
    bytes_written = write_size - bytes_not_written;
    *offset += bytes_written;

    pr_info("captured '%s' in echo buffer\n", echo_buffer);
    return bytes_written;
}

static const struct file_operations fileops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static struct miscdevice echo_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &fileops,
    .mode = S_IRUGO,
};

static int __init echo_dev_init(void)
{
    int ret;

    ret = misc_register(&echo_dev);
    if(ret) {
        pr_err("could not register echo device as misc devie\n");
        return ret;
    }

    pr_info("Echo device at /dev/%s is ready\n", DEVICE_NAME);
    return 0;
}

static void __exit echo_dev_exit(void)
{
    misc_deregister(&echo_dev);
    pr_info("Echo device at /dev/%s can not longer be used\n", DEVICE_NAME);
}

module_init(echo_dev_init);
module_exit(echo_dev_exit);
