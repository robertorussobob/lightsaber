import serial
import pygame

class MockedSerial:

	def __init__(self):
		self.count = 0

	def readline(self):
		result = ''
		self.count = self.count + 1
		if self.count > 10000000:
			self.count = 0
			result = 'ON'
		return result
		

#ser = MockedSerial()
ser = serial.Serial('/dev/ttyUSB0', 9600)

pygame.init()
pygame.display.init()
pygame.mixer.init()
hum = pygame.mixer.Sound('hum2.wav')
on = pygame.mixer.Sound('poweron2.wav')
on.set_volume(0.5)
off = pygame.mixer.Sound('poweroff2.wav')
off.set_volume(0.5)

while 1:
	s = ser.readline().strip()
	if 'ON' == s:
		on.play()
		hum.play(loops = -1)
		print('switchOn')
	if 'OFF' == s:
		hum.stop()
		off.play()
		print('switchOff')


