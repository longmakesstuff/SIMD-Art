# The what

This is one of my latest projects with more of a artistic orientation, but also with a very technical nature. It helped me
to cope with university's exam stress lately and gives me a small mind space just for myself.

![](data/small-output.gif)

# The why

Currently I have been working a lots with Neon Register on ARM architecture and found myself falling in love with SIMD (Single Input Multiply Data)
and generally parallel programming. Normally, almost every modern CPU supports SIMD and a smart compiler will try its best 
to vectorize our code. But even the best compiler is not smart enough to understand the programming logic, which is fundamental
for enhancing code performance. Therefore when we find ourself need a bit more speed push but can not afford to run software 
on graphics card, explicitly programming in SIMD would be a way out. 

# The how

