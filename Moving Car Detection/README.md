# Moving Car Detection using Background Subtractor

As the title says, this model only detects moving cars, any still objects like the road and stopping cars are considered as background, therefore they won't be detected. 

For simple usecase, this type of approach may be better compared to deep learning based object detection models since this model use way less computing power.
Some of the possible usecase are:

>* Vehicle counter
>* Vehicle speed detection

This model is only half baked. For further work, I'm planning to add object tracking with labeling (labeling every object with numbers for object tracking).
This step is very crucial to get the most out of this model.
