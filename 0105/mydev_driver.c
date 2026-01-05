
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DEVICE_NAME "mydev"

static dev_t devno;                 // major/minor 번호
static struct cdev my_cdev;         // cdev 구조체
static struct class *my_class;      // class 구조체

// file_operations 예시 (필요하면 구현)
static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mydev: open\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mydev: release\n");
    return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    printk(KERN_INFO "mydev: read\n");
    return 0;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    printk(KERN_INFO "mydev: write\n");
    return len;
}

static struct file_operations my_fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

// -------------------------------
//   모듈 로드 함수
// -------------------------------

//권한 변경 로직 추가
static char *my_devnode(const struct device *dev, umode_t*mode)
{
    if (mode)
        *mode = 0666;   // crw-rw-rw
    return NULL;
}

static int __init my_driver_init(void)
{
    int ret;

    printk(KERN_INFO "mydev: init\n");

    // 1) 문자 디바이스 번호 할당
    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "alloc_chrdev_region failed\n");
        return ret;
    }

    // 2) cdev 초기화 및 등록
    cdev_init(&my_cdev, &my_fops);
    ret = cdev_add(&my_cdev, devno, 1);
    if (ret < 0) {
        printk(KERN_ERR "cdev_add failed\n");
        unregister_chrdev_region(devno, 1);
        return ret;
    }

    // 3) class 생성
    my_class = class_create("myclass");
    if (IS_ERR(my_class)) {
        printk(KERN_ERR "class_create failed\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(devno, 1);
        return PTR_ERR(my_class);
    }
    //권한 변경 로직 추가2
    my_class->devnode= my_devnode;

    // 4) /dev/mydev 생성
    if (device_create(my_class, NULL, devno, NULL, DEVICE_NAME) == NULL) {
        printk(KERN_ERR "device_create failed\n");
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(devno, 1);
        return -1;
    }

    printk(KERN_INFO "mydev: loaded successfully\n");
    return 0;
}

// -------------------------------
//   모듈 언로드 함수
// -------------------------------
static void __exit my_driver_exit(void)
{
    printk(KERN_INFO "mydev: exit\n");

    // 1) /dev/mydev 제거
    device_destroy(my_class, devno);

    // 2) class 제거
    class_destroy(my_class);

    // 3) cdev 제거
    cdev_del(&my_cdev);

    // 4) 문자 디바이스 번호 해제
    unregister_chrdev_region(devno, 1);

    printk(KERN_INFO "mydev: unloaded\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");

