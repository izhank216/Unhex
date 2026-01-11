# Unhex
A hex viewer for Linux.

## Build/Compile

If you want to build Unhex directly on your system, follow the steps:

**1.** Install system deps:
```bash
sudo apt update
sudo apt install -y build-essential gcc make libncurses5-dev libncursesw5-dev python3 python3-dev python3-pip
```

**2.** Install Python deps:
```bash
python3 -m pip install -r requirements.txt
```

**3.** Build using Make:
```bash
make all
```

All done!

