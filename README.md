# turksat-model-satellite-apps
Turksat model satellite competiton, satellite and station apps written with QT
## Details
I have implemented a Two-way communication protocol using Xbee 802.15.4 modules. Communication model consist of three layers:
- First layer for communicating with Xbee in API mode using UART.
- Second layer defines Command, Data and ACK packages.
- Third layer is application (ground station or satellite).
### Notes
Station is a computer running GNU/Linux (Ubuntu 16.04), satellite is a `raspberry pi zero w`. In addition to raspbery pi there are an arduino nano, code for arduino written by another team member.<br/><br/>
Because it is a code for competition, it did not written well and it is hard to maintanence. It was working and I prefer not to touch :). Code needs to refactoring. For example multhreading not used, but thread objects still there, `mainobj` is actually a object for communication for `xbee`. 
