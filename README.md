# Face-Swap

Openframeworks program that can track faces using the dLib library (Boost Software License 1.0) algorithm and can swap two faces in real time. 

Requires ofxOpenCv, ofxCv, and ofxDLib (MIT License)

Impotant note: in the FaceTrackerNew folder, there are two files to replace FaceTracker.h and FaceTracker.cpp in ofxDLib/src/ o as to allow scaling to increase the speed of face tracking. There is also a Tracker.h file that some forks of ofxDLib do not have. If ofxDLib/src/ does not have Tracker.h, please add the one I provided. 

Included face cloning code based on the work of Arturo Castro and Kyle McDonald (MIT License) https://github.com/arturoc/FaceSubstitution
