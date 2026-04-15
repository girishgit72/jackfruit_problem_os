# 📦 Container Runtime with Kernel-Level Memory Monitoring

## 🧠 Overview
This project implements a lightweight container runtime in C with kernel-level memory monitoring using a custom Linux kernel module. It demonstrates core operating system concepts such as process isolation, namespaces, and kernel-user communication.

---

## 🎯 Objectives
- Create isolated containers using `clone()`
- Implement a Linux kernel module for monitoring memory usage
- Enable communication between user space and kernel space using `ioctl()`
- Detect and report excessive memory usage

---

## ⚙️ Features
- Lightweight container creation
- Kernel module for memory monitoring
- IOCTL-based communication
- Memory limit alerts
- Custom root filesystem for isolation

---

## 🏗️ Project Structure
os_project/
├── engine.c
├── monitor.c
├── monitor_ioctl.h
├── Makefile
├── rootfs/
└── README.md


---

## 🧪 How It Works

### Kernel Module
- Registers a character device
- Accepts memory limit via ioctl
- Monitors memory usage
- Logs warnings when exceeded

### User-Space Engine
- Creates container using clone()
- Runs command inside container
- Communicates with kernel module

---

## 🛠️ Setup & Execution

### 1. Build Kernel Module
```bash
make
2. Insert Module
sudo insmod monitor.ko
3. Get Major Number
cat /proc/devices | grep monitor
4. Create Device File
sudo mknod /dev/container_monitor c <major> 0sudo chmod 666 /dev/container_monitor

🚀 Run Container
Compile engine:
gcc engine.c -o engine
Run:
sudo ./engine run 1 /bin/ls
OR:
sudo ./engine run 1 /bin/sh

📊 Output
Check logs:
sudo dmesg | grep Monitor
Example:
Memory Monitor Loaded, major = 240Monitor: Device openedMonitor: IOCTL calledMemory exceeded by PID XXXX

🧾 Concepts Used


Linux Kernel Modules


clone() system call


Namespaces (PID, UTS)


Character device drivers


IOCTL communication



⚠️ Notes


Run on Linux (Ubuntu recommended)


Requires root privileges


Remove module before reinserting:


sudo rmmod monitor

📚 Applications


Understanding container internals


OS-level resource management


Kernel programming practice



👨‍💻 Author
Gautam Krishna

🚀 Future Improvements


Add CPU monitoring


Use cgroups for better control


Add network namespace


Improve logging


