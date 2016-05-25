//globalmem.c

#include<linux/module.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/errno.h>
#include<linux/mm.h>
#include<linux/sched.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>

#include<linux/slab.h>

#define GLOBALMEM_SIZE	0x1000		//4k
#define MEM_CLEAR		0x01
#define GLOBALMEM_MAJOR	250

static int globalmem_major = GLOBALMEM_MAJOR;

struct globalmem_dev{
	struct cdev cdev;
	unsigned char mem[GLOBALMEM_SIZE];
};

struct globalmem_dev *globalmem_devp;

// file_operation definiton
static int globalmem_open(struct inode *inode, struct file *filp)
{
	filp->private_data = globalmem_devp;
	return 0;
}

static int globalmem_release(struct inode *inode, struct file *filp)
{
	return 0;
}

//file_operation fuctions
static ssize_t globalmem_read(struct file *filp, \
							char __user *buf, \
							size_t count, \
							loff_t *ppos)
{
	unsigned long p = *ppos;
	int ret = 0;
	struct globalmem_dev *dev = filp->private_data;

	if(p >= GLOBALMEM_SIZE)
		return 0;
	if(count > (GLOBALMEM_SIZE - p))
		count = GLOBALMEM_SIZE - p;

	if(copy_to_user(buf, (void*)(dev->mem+p), count))
		ret = -EFAULT;
	else{
		*ppos += count;
		ret = count;
		printk(KERN_INFO "read %d bytes(s) from %ld.\n", count, p);
	}

	return ret;
}

static ssize_t globalmem_write(struct file *filp, \
								const char __user *buf, \
								size_t count, \
								loff_t *ppos)
{
	unsigned long p = *ppos;
	int ret = 0;
	struct globalmem_dev *dev = filp->private_data;

	if(p >= GLOBALMEM_SIZE)
		return 0;
	if(count > (GLOBALMEM_SIZE - p))
		count = GLOBALMEM_SIZE - p;

	if(copy_from_user(dev->mem + p, buf, count))
		ret = -EFAULT;
	else{
		*ppos += count;
		ret = count;
		printk(KERN_INFO "written %d bytes(s) from %ld.\n", count, p);
	}

	return ret;
}

static loff_t globalmem_llseek(struct file *filp, \
								loff_t offset, \
								int orig)
{
	loff_t ret;
	switch(orig){
	case 0://SEEK_SET
		if((unsigned int)offset > GLOBALMEM_SIZE || offset < 0){
			ret = -EINVAL;
			break;
		}
		filp->f_pos = (unsigned int)offset;
		ret = filp->f_pos;
		break;
	case 1://SEEK_CUR
		if((filp->f_pos+(unsigned int)offset) < 0 || (filp->f_pos+(unsigned int)offset) > GLOBALMEM_SIZE){
			ret = -EINVAL;
			break;
		}
		filp->f_pos += (unsigned int)offset;
		ret = filp->f_pos;
		break;
	default:
		ret = -EINVAL;
	}
	
	return ret;
}

static long globalmem_unlocked_ioctl(struct file *filp, \
							unsigned int cmd, \
							unsigned long arg)
{
	struct globalmem_dev *dev = filp->private_data;

	switch(cmd){
	case MEM_CLEAR:
		memset(dev->mem, 0, GLOBALMEM_SIZE);
		printk(KERN_INFO "globalmem is set to zero.\n");
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static const struct file_operations globalmem_fops = {
	.owner = THIS_MODULE, 
	.open = globalmem_open,
	.release = globalmem_release,
	.llseek = globalmem_llseek, 
	.read = globalmem_read, 
	.write = globalmem_write, 
	.unlocked_ioctl = globalmem_unlocked_ioctl, 
};

// module init

static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
	int err, devno = MKDEV(globalmem_major, index);

	cdev_init(&dev->cdev, &globalmem_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	if(err)
		printk(KERN_NOTICE "Error %d adding globalmem %d.\n", err, index);
}

static int __init globalmem_init(void)
{
	//分配设备号
	int result;
	dev_t devno = MKDEV(globalmem_major, 0);

	if(globalmem_major)
		result = register_chrdev_region(devno, 1, "globalmem");
	else
	{
		result = alloc_chrdev_region(&devno, 0, 1, "globalmem");
		globalmem_major = MAJOR(devno);
	}

	if(result < 0)
		return result;

	globalmem_devp = kmalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
	if(!globalmem_devp){
		result = -ENOMEM;
		goto fail_malloc;
	}
	
	memset(globalmem_devp, 0, sizeof(struct globalmem_dev));

	globalmem_setup_cdev(globalmem_devp, 0);
    return 0;

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return result;
}

static void __exit globalmem_exit(void)
{
	cdev_del(&globalmem_devp->cdev);
	kfree(globalmem_devp);
	unregister_chrdev_region(MKDEV(globalmem_major, 0), 1);
}

module_init(globalmem_init);
module_exit(globalmem_exit);

module_param(globalmem_major, int, S_IRUGO);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Liu Ke <liuke_lk@163.com>");

