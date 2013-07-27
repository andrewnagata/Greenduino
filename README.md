# Greenduino: Arduino for Greenhouse

This repo contains the files and samples necessary to use the awesomeness that is arduino with [Oblong's Greenhouse SDK](http://greenhouse.oblong.com).

There are 2 samples in this repo:
- Sample.C
- stethoscope.C and operaio.C

### Sample.C

Shows how to interfaces with your arduino directly via method calls. It also shows how to respond to event driven protein deposits

### stethoscope.C and operaio.C

This pair shows how to control an arduino entirely using proteins, allowing great flexibility in hardware setup and input devices. It also shows how to use heartbeat proteins from Greenduino for input into your greenhouse application.

# Getting Started

To get started, open up one of the samples and read the comments to get your hardware set up correctly. Then type the following in your terminal:

    make
    ./name-of-sample-you-want-to-run
