from huskylib import HuskyLensLibrary
# hl = HuskyLensLibrary("I2C", "/dev/ttyUSB0")
hl = HuskyLensLibrary("I2C","", address=0x32, channel=0)
print(h1.knock())






