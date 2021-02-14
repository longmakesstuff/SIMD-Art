Link the YouTube video demonstration: https://www.youtube.com/watch?v=Bjwml32dxhU 

The compression algorithm does not work well on this colorful video, so I can not upload the full version as GIF.

### The what

This is one of my latest projects with more of a artistic orientation, but also with a very technical nature. It helped me
to cope with university's exam stress and gives me a small mind space just for myself. 

<p align="center">
  <img src="data/small-output.gif" alt="animated" />
</p>

### The why

Currently I have been working a lots with Neon Register on ARM architecture and found myself falling in love with SIMD (Single Input Multiply Data)
and generally parallel programming. Normally, almost every modern CPU supports SIMD and a smart compiler will try its best 
to vectorize our code. But even the best compiler is not smart enough to understand the business logic, which is fundamental
for enhancing code performance. Therefore, when we find ourself need a bit more speed push, but can not afford to run software 
on a graphics card, explicitly programming in SIMD could be a way out. 

### The how

The working principle of the core algorithm is simple. We update each particle for each frame in according to its
position and distance to the mouse. 

The standard software engineering would want us to solve the problem object orientedly, with each object having its own 
attributes. But to parallelize the rendering efficiently on hardware, I had to break the objects into matrices, with each 
vector having only data of an attribute. 

With an AMD's Thread Ripper, I am able to process 8 particles on one run instead of just one particle. Combining this technique 
with thread level parallelization results a pretty neat visual software. 

### References 

- [Crunching Numbers with AVX and AVX2](https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX#_articleTop)
- [Leapfrog integration](https://en.wikipedia.org/wiki/Leapfrog_integration)
