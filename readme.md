
# Drift a part


## Abstract

## Inspiration

## Development Steps

### PYTHON (data modification):

Validation:
Can __random acceleration__ data generate interesting shapes?

	 - Script to generate mock data with predictable/calculatable results 
	 	(i.e.: 
	 		AccelX: 10m/sˆ2
	 		deltaTime = 1 second
	 		=> 
	 			deltaSpeedX of 10m/s (from AccelX * deltaTime)
	 			=>
	 				initSpeedX = 0 m/s
					finalSpeedX = initSpeedX + deltaSpeedX = 10m/s
					averageSpeedX = deltaSpeedX/2
					deltaDistanceX = deltaSpeedX * deltaTime = 5m)
	 - Script to integrate acceleration data into Speed and Speed into positions
	 	use previous mockData to test script, 
	 	should generate hand calculated data for multiple data points)
	 - Script to generate random mock data
	 - Generate positions and view on 3D Space from of CSV (Rhino and Web)

	 - Next issue: 
	 	Would it continue to be interesting with real data?



Validation:
Can **real noise from acceleration data** generate interesting shapes?

	 - Hardware/Firmware/Mechanical development
	 - Import real data
	 - Export positions into 3D Space from CSV (Rhino)
	 - Heavy skew into Vertical Axis
	 	(I speculate that it is because its the axis that is suffering constant 1g
	 	and therefore drifting much more)
	 - Implementation of "High Pass" filter 
	 	(subtracting rolling average of current reading,
	 	previous implementation was just calculating local G from n readings during boot)

	 - Next issue: 
	 	It generates interesting shapes but unrecognizable in a linear scale.
	 	The overal shape is almost a straight line, and there are interesting
	 	movements zooming in 1000 times, 1000000 times.
	 	All cannot be seen at the same time.
		Is it possible to conciliate these different scales?


Validation:
Can **log-log graphs** make the interesting shapes in different scales all visible at the same time in 3D space?

	 - Implementation of log() conversion in Cartesian (XYZ) space.
	 	(log operation is done on each axis individually,
	 	this results in a heavy distortion of the shape, 
	 	it always crosses the quadrant planes perpendicularlly)
	 - Correction of log() implementation, now using Spherical (Radius, elevation, Azimuth) space.
	 	(log operation is done only on radius)
	
#### Python stable status:
	 - XYZ Acceleration noise data is imported into python
	 - XYZ Acceleration is integrated into XYZ speed
	 - XYZ Speed is integrated into XYZ positions
	 - XYZ positions are converted into Spherical positions
	 - Radius from spherical coordinates are changed into log scale
	 - Log Spherical positions are transformed into XYZ positions
	 - A CSV point cloud is exported


### FIRMWARE

Teensy + MPU 9250 via Interrupt Driven SPI (an interval is configured during setup), so the IMU tells the MCU when to call for data.
Data is then logged into an SD card.

	 - Testing IMU via Interrupt driven SPI to Serial Port
	 - Testing IMU via Interrupt driven SPI to Serial Port and SD Card (which also uses SPI)
	 - Inclusion of OLED screen to show # of samples taken
	 - Inclusion of capacitive button to pause sample gathering
	 	In case the datalogger is about to be moved
	 - Testing dataset
	 - Implementing gravity "High Pass" filter
	 	A rolling average is subtracted from the current sample value.
	 	Previous implementation consisted of single calibration during boot.
	


### GRASSHOPPER + PRC (parametric robot control)

	 - Importing of point cloud data into SimplePath.GH example
	 - Modification of SimplePath example to
	 -
	 -
	 -
	 - 


### PCB HOLDERS

3D printed supports for the datalogger

	 - Teensy + MPU
	 - Teensy + MPU + OLED + Capacitive button

### LED HOLDERS

3D printed + carbon fiber parts for the LED extension for the Kuka Robot

	 - STL with mock dimensions to test mesh import as tool into Grasshopper/PRC
	 	("extension" parallel to flange axis, thus wasting one axis :( )
	 - Holder with LED extension directly into X axis
	 	("extension" perpendicular to flange axis, turned out to be too agressive)
	 - Holder with LED extension 45 degress in relation to X and Z axis 
