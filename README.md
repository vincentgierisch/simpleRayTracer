# Yet another Raytracer

The raytracer loads an obj-File and (hopefully) outputs a photorealistic image.
Look at the jobs file for the configuration.

## Installation

````
git clone git@github.com:vincentgierisch/simpleRayTracer
cd simpleRayTracer
make
````

## Run

To start the script, run the following command:
````
./bin/srtRenderer -s jobs/testJob
````  
For a live preview, run:
````
./bin/srtRenderer -l -s jobs/testJob
````  