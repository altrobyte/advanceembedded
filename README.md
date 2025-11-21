ESP32 Setup & Programming Guide
1. Software Installation
Windows
Download:

Go to: https://dl.espressif.com/dl/esp-idf/

Download: esp-idf-tools-setup-x.x.exe

Install:

Run the installer

Click "Next" through all steps

Wait 15-20 minutes

Desktop shortcut "ESP-IDF CMD" will appear

Install USB Driver:

Download from: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

Install and restart computer

Verify:

bash
# Open ESP-IDF CMD
idf.py --version
2. Basic Commands
Create Project
bash
# Go to documents folder
cd %userprofile%\Documents

# Copy example
xcopy /E /I %IDF_PATH%\examples\get-started\blink my_project

# Go to project
cd my_project

# Set target
idf.py set-target esp32
Build & Flash
bash
# Build
idf.py build

# Flash (replace COM3 with your port)
idf.py -p COM3 flash

# Monitor
idf.py -p COM3 monitor

# Flash + Monitor together
idf.py -p COM3 flash monitor

# Exit monitor: Press Ctrl + ]
Find Your COM Port
Open Device Manager

Look under "Ports (COM & LPT)"

Find "Silicon Labs" or "CH340"

Note the COM number (COM3, COM4, etc.)

3. GPIO Pin Reference
Safe Pins (Use These)
text
GPIO2  - Onboard LED
GPIO4
GPIO16, GPIO17, GPIO18, GPIO19
GPIO21, GPIO22, GPIO23
GPIO25, GPIO26, GPIO27
GPIO32, GPIO33
DON'T Use These
text
GPIO6-11  - Flash (will damage board!)
GPIO34-39 - Input only
Special Pins
text
GPIO0  - BOOT button
GPIO2  - LED (on most boards)
4. Common Problems & Solutions
Problem: Board not detected
Solution:

Check USB cable (not all cables work for data)

Install driver from link above

Try different USB port

Restart computer

Problem: Flash failed
Solution:

Hold BOOT button while connecting

Try: idf.py -p COM3 -b 115200 flash

Press EN button after connecting

Problem: Build error
Solution:

bash
idf.py fullclean
idf.py build
Problem: Monitor shows garbage
Solution:

Press EN (reset) button on board

Check COM port number is correct

5. Quick Command Reference
bash
# Setup
idf.py set-target esp32

# Build
idf.py build

# Flash
idf.py -p COM3 flash

# Monitor
idf.py -p COM3 monitor

# Clean
idf.py fullclean

# Menu
idf.py menuconfig

# Erase
idf.py -p COM3 erase-flash
6. Project File Location
Where to put your projects:

text
C:\Users\YourName\Documents\esp32_projects\
Main code file:

text
your_project/main/main.c
Edit this file to change your code!

7. Important Notes
ESP32 uses 3.3V logic (NOT 5V!)

Always use included resistors with LEDs

USB cable must support data (not just charging)

Keep board connected while flashing

Press BOOT button if flash fails

------------------------------------------------

===============================================
COMPLETE COMMANDS - GIT + SSH
===============================================

-------------------
GIT COMMANDS
-------------------

git config --global user.name "Your Name"
git config --global user.email "your@email.com"
git init
git clone https://github.com/username/repo.git
git status
git add .
git commit -m "message"
git push
git pull
git log
git remote add origin https://github.com/username/repo.git

-------------------

-------------------

# Generate SSH key
ssh-keygen -t ed25519 -C "your@email.com"

# Start SSH agent
eval "$(ssh-agent -s)"

Get-Content C:\Users\Lenovo\.ssh\id_ed25519_altrobyte.pub | clip

# Add key to agent
ssh-add ~/.ssh/id_ed25519

# View public key (copy this)
cat ~/.ssh/id_ed25519.pub

# Copy to clipboard (Windows)
clip < ~/.ssh/id_ed25519.pub

# Copy to clipboard (Mac)
pbcopy < ~/.ssh/id_ed25519.pub

# Test SSH connection
ssh -T git@github.com

# Change repo to SSH
git remote set-url origin git@github.com:username/repo.git

# Check remote
git remote -v

-------------------
SSH TROUBLESHOOTING
-------------------

# List added keys
ssh-add -l

# Fix permissions (Linux/Mac)
chmod 600 ~/.ssh/id_ed25519
chmod 644 ~/.ssh/id_ed25519.pub

-------------------
COMPLETE SSH SETUP STEPS
-------------------

1. ssh-keygen -t ed25519 -C "your@email.com"
2. Press Enter (3 times)
3. eval "$(ssh-agent -s)"
4. ssh-add ~/.ssh/id_ed25519
5. cat ~/.ssh/id_ed25519.pub
6. Copy the output
7. GitHub.com → Settings → SSH Keys → New SSH key
8. Paste key → Add
9. ssh -T git@github.com

===============================================
