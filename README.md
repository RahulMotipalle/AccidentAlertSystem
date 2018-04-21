The rate at which road accidents are taking place in the world is truly an alarming matter of
concern. In an unfortunate situation when a person meets with an accident, it is of prime
importance that his loved ones get to know his location. We propose a system wherein
immediate notifications are present to the loved ones of the victim after he/she meets with
an accident.

In this project, we use a vibration sensor and an accelerometer to check the proper movement of the car. If the car rams into any object, then it vibrates which will be recorded by the vibration sensor. In case of a more severe accident in which the car's wheels move a few centimeters/inches off the ground, the accelerometer will detect the accident. 

The arduino board will keep on checking for the values of the vibration sensor and the accelerometer and the moment it gets to know that the values are more than what they should ideally be, it reads the coordinates of the accident site with the help of the GPS module and then sends an SMS message to the loved ones of the victim by using a GSM module.

We are still working on the project. In the future, the arduino will send data to a Raspberry Pi and we will use the data to keep track of some analytics.
