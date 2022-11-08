import tello

tello.start()
power = tello.get_battery()
print("Battery = ",power,"%")
tello.takeoff()
tello.land()
